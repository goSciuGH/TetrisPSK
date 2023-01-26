/*
Wa¿ne komentarze:
	NOTE - notka, do przemyœlenia
	FIXIT - trzeba to naprawiæ, czemu??
	TODO - do zrobienia
*/

/*
	TODO ogólne
		lokowanie tetrimin (~MinoAnchor())
		obracanie
		schowek
		pauza
*/

#include <SFML/Graphics.hpp>
#include <string>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>


// Definicja grafiki

#define FULLRES_W 1920
#define FULLRES_H 1080

bool fullScreen = false;

double windowScale = 1.0;
int windowW = FULLRES_W * windowScale;
int windowH = FULLRES_H * windowScale;

std::ofstream queueFile;

// Definicja sta³ych wartoœci gry

#define DAS_DEFAULT 170	// DAS - Delayed Auto Shift - iloœæ czasu w ms, przez jak¹ trzeba trzymaæ przucisk, ¿eby tetrimino zaczê³o siê poruszaæ ze sta³¹ prêdkoœci¹
#define ARR_DEFAULT 50	// ARR - Auto Repeat Rate - iloœæ czasu w ms pomiêdzy kolejnymi krokami tetrimina w bok
#define SDF_DEFAULT 20	// SDF - Soft Drop Factor - mno¿nik standardowej prêdkoœci opadania tetrimina przy soft-dropie
#define LOCK_TIME	500 // czas w ms, jaki tetrimino musi spêdziæ po nieudanej próbie opuszczenia w dó³, by zostaæ zablokowane w miejscu
#define LOCK_RESET	15	// maksymalna iloœæ resetów czasomierza lock-in, zapobiegaj¹ca nieskoñczonemu unikaniu zalokowania
#define SPAWN_RESET	117	// czas w ms od zalokowania Tetrimina, po którym generowane jest nowe

int offsetChart[3][4][5][2]; // Tabela obrótów - [zestaw][kierunek][nr testu][x lub y]

enum ctrlMask
{
	LMV = 0x01, // Left MoVement
	RMV = 0x02, // Right MoVement
	LSP = 0x04, // Left SPin
	RSP = 0x08, // Right SPin
	SDR = 0x10, // Soft DRop
	HDR = 0x20, // Hard DRop
	HLD = 0x40, // HoLD
	PSE = 0x80  // PauSE
};

std::map <std::string, sf::Keyboard::Key> dftKey;

enum scoring
{
	SNGL = 100,		// Single
	DUBL = 300,		// Double
	TRPL = 500,		// Triple
	QUAD = 800,		// Quad
	TSMN = 100,		// T-Spin Mini (bez czyszczenia)
	TSPN = 400,		// T-Spin (bez czyszczenia)
	TSMS = 200,		// T-Spin Mini Single
	TSSN = 800,		// T-Spin Single
	TSMD = 1200,	// T-Spin Mini Double
	TSDB = 1200,	// T-Spin Double
	TSTR = 1600,	// T-Spin Triple
	CMBO = 50,		// Combo (mno¿one przez d³ugoœæ combo)
	SDRP = 1,		// Soft Drop (1 pole)
	HDRP = 2,		// Hard Drop (2 pole)
	PCLR = 3500		// Perfect Clear
};


// Zmienne pokojów gry

enum rooms
{
	NORM = 0,	// NO RooM
	TITL = 1,	// TITLe Screen
	MENU = 2,	// Main MENU
	STNG = 3,	// SeTtiNGs
	GAME = 4	// GAME
};

enum gameMode
{
	NOGM = 0,	// NO GameMode
	MRTN = 1,	// MaRaThoN
	SPRN = 2,	// SPRiNt
	ULTR = 3	// ULTRa
};

// Deklaracja obiektów i funkcji

struct Queue;
struct pQueue;

class MenuOption;

class Mino;
class MinoAnchor;
class Matrix;
class Player;

void enqueue(struct pQueue*, int);
void dequeue(struct pQueue*);
void addBag(struct pQueue*);

// Definicja obiektów

struct Queue
{
	int id;
	int color;
	struct Queue* next;
};

struct pQueue
{
	struct Queue* head = NULL;
	struct Queue* tail = NULL;
};

class MenuOption : public sf::Sprite
{
public:
	MenuOption()
	{
		upleft = NULL;
		up = NULL;
		upright = NULL;

		rightup = NULL;
		right = NULL;
		rightdown = NULL;

		downright = NULL;
		down = NULL;
		downleft = NULL;

		leftdown = NULL;
		left = NULL;
		leftup = NULL;

		highlighted = false;
		activated = false;
	}

	MenuOption* upleft;
	MenuOption* up;
	MenuOption* upright;

	MenuOption* rightup;
	MenuOption* right;
	MenuOption* rightdown;

	MenuOption* downright;
	MenuOption* down;
	MenuOption* downleft;

	MenuOption* leftdown;
	MenuOption* left;
	MenuOption* leftup;

	bool highlighted;
	bool activated;

	int menuGridPos;	// pozycja na siatce menu; przydatne, gdy opcje nie s¹ u³o¿one równolegle lub maj¹ ró¿ne rozmiary
	bool focus[2];		// okreœla, z którego kierunku nadesz³o podœwietlenie; przydatne, gdy opcje nie s¹ u³o¿one równolegle lub maj¹ ró¿ne rozmiary; [0] - okreœla X, gdzie 1 to Prawo, [1] okreœla Y, gdzie 1 to dó³
};

class Mino : public sf::Sprite
{
public:
	Mino()
	{
		readyToShow = false;
	}

	Mino* parent;
	int colour;
	int posX, posY;
	bool readyToShow;

	void rotate(bool cw)
	{
		if (cw) // ClockWise
		{
			int swap = posX;
			posX = posY;
			posY = 0 - swap;
		}
		else // Counter ClockWise
		{
			int swap = posX;
			posX = 0 - posY;
			posY = swap;
		}
	}
};

class MinoAnchor : public Mino
{
public:
	MinoAnchor(Player* p, int c, sf::Texture &t)
	{
		matrix = p;

		posX = 4;
		posY = 20;
		rot = 0;
		colour = c;
		readyToLock = false;

		children[0] = new Mino();
		children[1] = new Mino();
		children[2] = new Mino();

		switch (c)
		{
		case 0: // Z
		{
			children[0]->posX = 0 - 1;
			children[0]->posY = 1;
			children[1]->posX = 0;
			children[1]->posY = 1;
			children[2]->posX = 1;
			children[2]->posY = 0;
			break;
		}
		case 1: // L
		{
			children[0]->posX = 1;
			children[0]->posY = 1;
			children[1]->posX = 0 - 1;
			children[1]->posY = 0;
			children[2]->posX = 1;
			children[2]->posY = 0;
			break;
		}
		case 2: // O
		{
			children[0]->posX = 0;
			children[0]->posY = 1;
			children[1]->posX = 1;
			children[1]->posY = 1;
			children[2]->posX = 1;
			children[2]->posY = 0;
			break;
		}
		case 3: // S
		{
			children[0]->posX = 0;
			children[0]->posY = 1;
			children[1]->posX = 1;
			children[1]->posY = 1;
			children[2]->posX = 0 - 1;
			children[2]->posY = 0;
			break;
		}
		case 4: // I
		{
			children[0]->posX = 0 - 1;
			children[0]->posY = 0;
			children[1]->posX = 1;
			children[1]->posY = 0;
			children[2]->posX = 2;
			children[2]->posY = 0;
			break;
		}
		case 5: // J
		{
			children[0]->posX = 0 - 1;
			children[0]->posY = 1;
			children[1]->posX = 0 - 1;
			children[1]->posY = 0;
			children[2]->posX = 1;
			children[2]->posY = 0;
			break;
		}
		case 6: // T
		{
			children[0]->posX = 0;
			children[0]->posY = 1;
			children[1]->posX = 0 - 1;
			children[1]->posY = 0;
			children[2]->posX = 1;
			children[2]->posY = 0;
			break;
		}
		}

		setTexture(t);
		children[0]->setTexture(t);
		children[1]->setTexture(t);
		children[2]->setTexture(t);

		setTextureRect(sf::IntRect((t.getSize().x) / 3 * (c % 3), (t.getSize().y) / 3 * ((c - (c % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
		children[0]->setTextureRect(getTextureRect());
		children[1]->setTextureRect(getTextureRect());
		children[2]->setTextureRect(getTextureRect());

		updateScreenXY();
	}

	~MinoAnchor();

	Player* matrix;
	int shape;
	int rot; // 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo
	Mino* children[3];
	bool readyToLock;

	void updateScreenXY();
};

class Player : public sf::Sprite
{
public:
	Player(sf::Texture &t, sf::Font &f)
	{
		id = 0;

		ctrlKey = dftKey;
		ctrlState = 0x00;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				matrix[i][j] = NULL;
			}
		}

		setTexture(t);
		setOrigin(sf::Vector2f((t.getSize().x) / 2.0, (t.getSize().y) / 2.0));
		setPosition(sf::Vector2f((FULLRES_W / (5.0 + (1.0 / 3.0))) + ((t.getSize().x) / 2.0), FULLRES_H / 2.0 + 2.0));

		level = 0;
		hold = 7;
		holdActive = true;
		holdSwap = false;

		for (int i = 0; i < 4; i++)
		{
			toClear[i] = 50;
		}

		softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;

		score = 0;
		time = sf::Time::Zero;
		combo = -1;
		back2back = -1;
		tSpin = -1;
		int totalCleared = 0;

		for (int i = 0; i < 6; i++)
		{
			txt_stats[i].setFont(f);
			txt_stats[i].setCharacterSize(48);
			txt_stats[i].setFillColor(sf::Color::Black);
		}

		int len;
		sf::FloatRect bounds;

		txt_stats[0].setString("Marathon");
		bounds = txt_stats[0].getLocalBounds();
		txt_stats[0].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[0].setPosition(sf::Vector2f(1632.0, 280.0));

		txt_stats[1].setString("00000000");
		bounds = txt_stats[1].getLocalBounds();
		txt_stats[1].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[1].setPosition(sf::Vector2f(1632.0, 410.0));

		txt_stats[2].setString("");
		len = std::to_string(score).length();
		for (int i = 0; i < (8 - len); i++)
			txt_stats[2].setString(txt_stats[2].getString() + "0");
		txt_stats[2].setString(txt_stats[2].getString() + std::to_string(score));
		bounds = txt_stats[2].getLocalBounds();
		txt_stats[2].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[2].setPosition(sf::Vector2f(1632.0, 540.0));

		txt_stats[3].setString("");
		txt_stats[3].setString(txt_stats[3].getString() + std::to_string((int)floor(time.asSeconds() / 60.0)) + ":");
		if ((floor((int)(time.asSeconds()) % 60)) < 10)
			txt_stats[3].setString(txt_stats[3].getString() + "0" + std::to_string((int)floor((int)(time.asSeconds()) % 60)) + ".");
		else
			txt_stats[3].setString(txt_stats[3].getString() + std::to_string((int)floor((int)(time.asSeconds()) % 60)) + ".");
		if ((time.asMilliseconds() % 1000) < 10)
			txt_stats[3].setString(txt_stats[3].getString() + "00" + std::to_string(time.asMilliseconds() % 1000));
		else if ((time.asMilliseconds() % 1000) < 100)
			txt_stats[3].setString(txt_stats[3].getString() + "0" + std::to_string(time.asMilliseconds() % 1000));
		else
			txt_stats[3].setString(txt_stats[3].getString() + std::to_string(time.asMilliseconds() % 1000));
		bounds = txt_stats[3].getLocalBounds();
		txt_stats[3].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[3].setPosition(sf::Vector2f(1632.0, 670.0));

		txt_stats[4].setString(std::to_string(totalCleared));
		bounds = txt_stats[4].getLocalBounds();
		txt_stats[4].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[4].setPosition(sf::Vector2f(1382.0, 800.0));

		txt_stats[5].setString(std::to_string(level + 1));
		bounds = txt_stats[5].getLocalBounds();
		txt_stats[5].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[5].setPosition(sf::Vector2f(1632.0, 800.0));
	}

	int id;
	int piecesUsed = 0;

	Mino* matrix[10][40];

	sf::Sprite holdBox[2];
	sf::Sprite qBox;

	int level;
	int hold;
	bool holdActive;
	bool holdSwap;
	int toClear[4];

	int score;
	sf::Time time;
	int combo;
	int back2back;
	int tSpin;	// -1 - nie wyst¹pi³, 0 - mini, 1 - wyst¹pi³
	int totalCleared;

	MinoAnchor* activeAnchor;

	std::map <std::string, sf::Keyboard::Key> ctrlKey;

	uint8_t ctrlState = 0x00;

	int sideMoveTime = 0;
	int softDropTime = 0;
	double softDropUpdate;
	int lockdownTime = -1;
	int lockdownReset = 0;
	int spawnTime = -1;
	int domDir = 0; // dominuj¹cy kierunek - -1 to lewo, 1 to prawo

	int pcQueue[5];

	sf::Text txt_stats[6]; // mode, best, score, time, lines, lvl
	sf::Text acclaim[3];

	void genPiece(pQueue* q, sf::Texture &t) // stwórz nowe tetrimino
	{
		if (q != NULL)
		{
			activeAnchor = new MinoAnchor(this, pcQueue[0], t);
			updateQueue(q);
		}
		else
		{
			activeAnchor = new MinoAnchor(this, hold, t);
		}
	}

	void updateQueue(pQueue* q)
	{
		for (int i = 0; i < 4; i++)
			pcQueue[i] = pcQueue[i + 1];
		pcQueue[4] = q->head->color;
		if ((!(q->head->id % 7)) && (q->head->next == NULL))
		{
			addBag(q);
		}

		queueFile << q->head->color;
		dequeue(q);
	}

	void rotate(bool cw)
	{
		int pom;
		if (cw == true)
			pom = 1;
		else
			pom = 0;

		bool rotSuccess = false;

		activeAnchor->children[0]->rotate(abs(0 - pom));
		activeAnchor->children[1]->rotate(abs(0 - pom));
		activeAnchor->children[2]->rotate(abs(0 - pom));

		int xShift;
		int yShift;

		for (int i = 0; i < 5; i++)
		{

			if (activeAnchor->colour == 2)
			{
				xShift = offsetChart[2][activeAnchor->rot][i][0] - offsetChart[2][(activeAnchor->rot + 3 - (pom * 2)) % 4][i][0];
				yShift = offsetChart[2][activeAnchor->rot][i][1] - offsetChart[2][(activeAnchor->rot + 3 - (pom * 2)) % 4][i][1];
			}
			else if (activeAnchor->colour == 4)
			{
				xShift = offsetChart[1][activeAnchor->rot][i][0] - offsetChart[1][(activeAnchor->rot + 3 - (pom * 2)) % 4][i][0];
				yShift = offsetChart[1][activeAnchor->rot][i][1] - offsetChart[1][(activeAnchor->rot + 3 - (pom * 2)) % 4][i][1];
			}
			else
			{
				xShift = offsetChart[0][activeAnchor->rot][i][0] - offsetChart[0][(activeAnchor->rot + 3 - (pom * 2)) % 4][i][0];
				yShift = offsetChart[0][activeAnchor->rot][i][1] - offsetChart[0][(activeAnchor->rot + 3 - (pom * 2)) % 4][i][1];
			}

			if (checkPos(xShift, yShift))
			{
				activeAnchor->posX += xShift;
				activeAnchor->posY += yShift;
				activeAnchor->rot = (activeAnchor->rot + 3 - (pom * 2)) % 4;
				rotSuccess = true;
				if (lockdownTime != -1) //resetowanie lock-ina
				{
					softDropTime = 0;
					lockdownReset++;
					lockdownTime = -1;
				}
				break;
			}
		}

		if (!(rotSuccess))
		{
			activeAnchor->children[0]->rotate(pom);
			activeAnchor->children[1]->rotate(pom);
			activeAnchor->children[2]->rotate(pom);
		}
	}

	void ctrlDo(int t, sf::Texture &ttr)
	{
		bool lockReset = false;

		// Wykonaj funkcje przycisków
		if (sf::Keyboard::isKeyPressed(ctrlKey["HLD"]))
		{
			//std::cout << "\tHLD";
			if (!(ctrlState & HLD))
			{
				if (holdActive)
				{
					if (hold == 7)
					{
						hold = activeAnchor->colour;
						delete activeAnchor;

						spawnTime = -1;
						lockdownReset = 0;
						lockdownReset = -1;
						softDropTime = 0;
						ctrlState |= 0x20;
						holdSwap = true;
					}
					else
					{
						int swap = activeAnchor->colour;
						delete activeAnchor;

						spawnTime = -1;
						lockdownReset = 0;
						lockdownReset = -1;
						softDropTime = 0;
						ctrlState |= 0x20;
						genPiece(NULL, ttr);
						if (checkPos(0, -1))
							activeAnchor->posY -= 1;

						hold = swap;
					}

					holdActive = false;
					return;
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["HDR"]))
		{
			//std::cout << "\tHDR\t";
			if (!(ctrlState & HDR))
			{
				while (checkPos(0, -1))
				{
					activeAnchor->posY -= 1;
					score += HDRP;

					txt_stats[2].setString("");
					int len = std::to_string(score).length();
					for (int i = 0; i < (8 - len); i++)
						txt_stats[2].setString(txt_stats[2].getString() + "0");
					txt_stats[2].setString(txt_stats[2].getString() + std::to_string(score));
					sf::FloatRect bounds = txt_stats[2].getLocalBounds();
					txt_stats[2].setOrigin(sf::Vector2f(bounds.width, bounds.height));
					txt_stats[2].setPosition(sf::Vector2f(1632.0, 540.0));
				}

				lockdownReset = INT_MAX - LOCK_RESET;

				activeAnchor->updateScreenXY();
				return;
			}
		}

		if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]) || sf::Keyboard::isKeyPressed(ctrlKey["RMV"]))	// jest wcisniety klawisz ruchu w bok
		{
			if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]) && sf::Keyboard::isKeyPressed(ctrlKey["RMV"])) // oba przyciski wciœniête
			{
				if ((ctrlState & LMV) && (ctrlState & RMV))	// poprzednio oba
				{
					sideMoveTime += t;

					if (domDir == -1) // prawy guzik zosta³ dociœniêty do lewego
					{
						if (sideMoveTime >= DAS_DEFAULT)
						{
							sideMoveTime -= ARR_DEFAULT;
							if (checkPos(1, 0))
							{
								activeAnchor->posX += 1;
								if (lockdownTime != -1) //resetowanie lock-ina
								{
									softDropTime = 0;
									lockdownReset++;
									lockdownTime = -1;
								}
							}
						}
					}
					else // lewy guzik dociœniêty do prawego
					{

						if (sideMoveTime >= DAS_DEFAULT)
						{
							sideMoveTime -= ARR_DEFAULT;
							if (checkPos(-1, 0))
							{
								activeAnchor->posX -= 1;
								if (lockdownTime != -1) //resetowanie lock-ina
								{
									softDropTime = 0;
									lockdownReset++;
									lockdownTime = -1;
								}
							}
						}
					}
				}
				else if (ctrlState & LMV) // poprzednio lewo (teraz +prawo)
				{
					sideMoveTime = 0;
					if (checkPos(1, 0))
					{
						activeAnchor->posX += 1;
						if (lockdownTime != -1) //resetowanie lock-ina
						{
							softDropTime = 0;
							lockdownReset++;
							lockdownTime = -1;
						}
					}
				}
				else if (ctrlState & RMV) // poprzednio praw (teraz +lewo)
				{
					sideMoveTime = 0;
					if (checkPos(-1, 0))
					{
						activeAnchor->posX -= 1;
						if (lockdownTime != -1) //resetowanie lock-ina
						{
							softDropTime = 0;
							lockdownReset++;
							lockdownTime = -1;
						}
					}
				}
				else // poprzednio ¿aden
				{
					sideMoveTime = 0;
				}
			}
			else if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"])) // tylko lewo
			{
				sideMoveTime += t;

				if ((ctrlState & (LMV | RMV)) || (ctrlState & RMV)) // poprzednio oba lub tylko prawo
				{
					domDir = -1;
				}
				else if (ctrlState & LMV) // poprzednio tylko lewo
				{
					// nic siê nie dzieje
				}
				else // poprzednio ¿aden
				{
					domDir = -1;
					if (checkPos(-1, 0))
					{
						activeAnchor->posX -= 1;
						if (lockdownTime != -1) //resetowanie lock-ina
						{
							softDropTime = 0;
							lockdownReset++;
							lockdownTime = -1;
						}
					}
				}

				if (sideMoveTime >= DAS_DEFAULT)
				{
					sideMoveTime -= ARR_DEFAULT;
					if (checkPos(-1, 0))
					{
						activeAnchor->posX -= 1;
						if (lockdownTime != -1) //resetowanie lock-ina
						{
							softDropTime = 0;
							lockdownReset++;
							lockdownTime = -1;
						}
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(ctrlKey["RMV"])) // tylko prawo
			{
				//std::cout << "\tRMV\t";
				sideMoveTime += t;

				if ((ctrlState & (LMV | RMV)) || (ctrlState & LMV)) // poprzednio oba lub tylko lewo
				{
					domDir = 1;
				}
				else if (ctrlState & LMV) // poprzednio tylko prawo
				{
					// nic siê nie dzieje
				}
				else // poprzednio ¿aden
				{
					domDir = 1;
					if (checkPos(1, 0))
						activeAnchor->posX += 1;
				}

				if (sideMoveTime >= DAS_DEFAULT)
				{
					sideMoveTime -= ARR_DEFAULT;
					if (checkPos(1, 0))
						activeAnchor->posX += 1;
				}
			}
		}
		else // ¿aden klawisz ruchu w bok nie jest wciœniêty
		{
			sideMoveTime = 0;
			domDir = 0;
		}

		if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"]) && sf::Keyboard::isKeyPressed(ctrlKey["RSP"])) // oba przyciski obrotu wciœniête - FIXIT
		{
			if ((ctrlState & LSP) && (ctrlState & RSP)) // poprzednio oba
			{
				// nie dzieje sie nic
			}
			else if (ctrlState & LSP)	// poprzednio tylko lewo (teraz +prawo)
			{
				rotate(true);
			}
			else if (ctrlState & RSP)	// poprzednio tylko prawo (teraz +lewo)
			{
				rotate(false);
			}
		}
		else if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"])) // lewy obrót
		{
			if (!(ctrlState & LSP)) // poprzednio nie by³ trzymany lewy
			{
				rotate(false);
			}
		}
		else if (sf::Keyboard::isKeyPressed(ctrlKey["RSP"])) // prawy obrót
		{
			if (!(ctrlState & RSP)) // poprzednio nie by³ trzymany prawy
			{
				rotate(true);
			}
		}

		if (sf::Keyboard::isKeyPressed(ctrlKey["PSE"]))
		{
			if (!(ctrlState & PSE))
			{
				// TODO - zapauzuj grê
			}
		}

		// Zaktualizuj grawitacjê

		if (sf::Keyboard::isKeyPressed(ctrlKey["SDR"]))
		{
			softDropTime += (SDF_DEFAULT * t);
		}
		else
		{
			softDropTime += t;
		}

		if (softDropTime >= softDropUpdate)
		{
			if (checkPos(0, -1))
			{
				softDropTime -= softDropUpdate;
				activeAnchor->posY -= 1;
				if (sf::Keyboard::isKeyPressed(ctrlKey["SDR"]))
				{
					score += SDRP;

					txt_stats[2].setString("");
					int len = std::to_string(score).length();
					for (int i = 0; i < (8 - len); i++)
						txt_stats[2].setString(txt_stats[2].getString() + "0");
					txt_stats[2].setString(txt_stats[2].getString() + std::to_string(score));
					sf::FloatRect bounds = txt_stats[2].getLocalBounds();
					txt_stats[2].setOrigin(sf::Vector2f(bounds.width, bounds.height));
					txt_stats[2].setPosition(sf::Vector2f(1632.0, 540.0));
				}
			}
			else
			{
				if (lockdownTime == -1)
				{
					lockdownTime = 0;
				}
				else
				{
					lockdownTime += t;

					if (lockdownTime >= LOCK_TIME)
						lockdownReset = LOCK_RESET + 1;
					// TODO - je¿eli wiêksze ni¿ czas na lockdown, zalokuj
				}
			}
		}

		activeAnchor->updateScreenXY();

		// Zapisz obecne wciœniêcia do porównania w nastêpnej klatce

		ctrlState = 0x00;

		if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]))	// ruch w lewo
		{
			ctrlState |= 0x01;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["RMV"]))	// ruch w prawo
		{
			ctrlState |= 0x02;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"]))	// obrot w lewo
		{
			ctrlState |= 0x04;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["RSP"]))	// obrot w prawo
		{
			ctrlState |= 0x08;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["SDR"]))	// softdrop
		{
			ctrlState |= 0x10;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["HDR"]))	// harddrop
		{
			ctrlState |= 0x20;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["HLD"]))	// hold
		{
			ctrlState |= 0x40;
		}
		if (sf::Keyboard::isKeyPressed(ctrlKey["PSE"]))	// pauza
		{
			ctrlState |= 0x80;
		}

		//std::cout << "\n";
	}

	void checkVisuals(sf::Texture &t)
	{
		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (matrix[j][i] != NULL)
				{
					if (matrix[j][i]->readyToShow == false)
					{
						matrix[j][i]->setTexture(t);
						int c = matrix[j][i]->colour;
						matrix[j][i]->setTextureRect(sf::IntRect((t.getSize().x) / 3 * (c % 3), (t.getSize().y) / 3 * ((c - (c % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
						matrix[j][i]->readyToShow = true;
					}
				}
			}
		}
	}

	void lockIn(sf::Texture &t)
	{
		activeAnchor->readyToLock = true;
		spawnTime = 0;
		lockdownTime = -1;
		if (activeAnchor->colour == 6)
		{
			int tX = activeAnchor->posX;
			int tY = activeAnchor->posY;

			switch (activeAnchor->rot)
			{
				case 0:
				{
					if ((matrix[tX - 1][tY + 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL))	// check for T-Spin
					{
						if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX + 1][tY - 1] != NULL))
							tSpin = 1;
					}
					else if (tY == 0) // check for T-Spin Mini with wall
					{
						if ((matrix[tX - 1][tY + 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
							tSpin = 0;
					}
					else if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX + 1][tY - 1] != NULL)) // check for T-Spin Mini with minos
					{
						if ((matrix[tX - 1][tY + 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
							tSpin = 0;
					}
					break;
				}
				case 1:
				{
					if ((matrix[tX + 1][tY - 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL))	// check for T-Spin
					{
						if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX - 1][tY + 1] != NULL))
							tSpin = 1;
					}
					else if (tX == 0) // check for T-Spin Mini with wall
					{
						if ((matrix[tX + 1][tY - 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
							tSpin = 0;
					}
					else if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX - 1][tY + 1] != NULL)) // check for T-Spin Mini with minos
					{
						if ((matrix[tX + 1][tY - 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
							tSpin = 0;
					}
					break;
				}
				case 2:
				{
					if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX + 1][tY - 1] != NULL))	// check for T-Spin
					{
						if ((matrix[tX - 1][tY + 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
							tSpin = 1;
					}
					else if (tY == 39) // check for T-Spin Mini with wall
					{
						if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX + 1][tY - 1] != NULL))
							tSpin = 0;
					}
					else if ((matrix[tX - 1][tY + 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL)) // check for T-Spin Mini with minos
					{
						if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX + 1][tY - 1] != NULL))
							tSpin = 0;
					}
					break;
				}
				case 3:
				{
					if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX - 1][tY + 1] != NULL))	// check for T-Spin
					{
						if ((matrix[tX + 1][tY - 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
							tSpin = 1;
					}
					else if (tX == 9) // check for T-Spin Mini with wall
					{
						if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX - 1][tY + 1] != NULL))
							tSpin = 0;
					}
					else if ((matrix[tX + 1][tY - 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL)) // check for T-Spin Mini with minos
					{
						if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX - 1][tY + 1] != NULL))
							tSpin = 0;
					}
					break;
				}
			}
		}
		delete activeAnchor;
		checkVisuals(t);
		checkLines();
	}

	void checkLines()
	{
		int x = 0;

		for (int i = 0; i < 40; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (matrix[j][i] == NULL)
					j = 10;

				if (j == 9)
				{
					toClear[x] = i;
					x++;
				}
			}
		}
	}

	void clearLines()
	{
		int cleared = 0;

		for (int i = 0; i < 4; i++)
		{
			if (toClear[i] < 40)
			{
				cleared++;
				for (int j = 0; j < 10; j++)
				{
					for (int k = toClear[i]; k < 39; k++)
					{
						matrix[j][k] = matrix[j][k + 1];
						if (matrix[j][k] != NULL)
						{
							matrix[j][k]->readyToShow = false;
							matrix[j][k]->move(sf::Vector2f(0.f, 32.f));
						}
					}

					matrix[j][39] = NULL;
				}

				for (int j = i; j < 4; j++)
				{
					toClear[j]--;
				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			toClear[i] = 50;
		}

		if (((totalCleared + cleared) % 10) < (totalCleared % 10))
		{
			if (level < 14)
			{
				if (((totalCleared + cleared - ((totalCleared + cleared) % 10)) / 10) > level)
				{
					sf::FloatRect bounds;
					level = (totalCleared + cleared - ((totalCleared + cleared) % 10)) / 10;
					softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;
					txt_stats[5].setString(std::to_string(level + 1));
					bounds = txt_stats[5].getLocalBounds();
					txt_stats[5].setOrigin(sf::Vector2f(bounds.width, bounds.height));
				}
			}
		}

		totalCleared += cleared;

		switch (cleared)
		{
			case 0:
			{
				combo = -1;
				back2back = -1;

				if (tSpin == 0)
					score += (TSMN * (level + 1));
				else if (tSpin == 1)
					score += (TSPN * (level + 1));

				break;
			}
			case 1:
			{
				combo++;

				if (tSpin == 0)
				{
					if (back2back > -1)
						score += ((TSMS * 1.5 + combo * CMBO) * (level + 1));
					else
						score += ((TSMS + combo * CMBO) * (level + 1));

					back2back++;
				}
				else if (tSpin == 1)
				{
					if (back2back > -1)
						score += ((TSSN * 1.5 + combo * CMBO) * (level + 1));
					else
						score += ((TSSN + combo * CMBO) * (level + 1));

					back2back++;
				}
				else
				{
					score += ((SNGL + combo * CMBO) * (level + 1));
					back2back = -1;
				}

				break;
			}
			case 2:
			{
				combo++;

				if (tSpin == 0)
				{
					if (back2back > -1)
						score += ((TSMD * 1.5 + combo * CMBO) * (level + 1));
					else
						score += ((TSMD + combo * CMBO) * (level + 1));

					back2back++;
				}
				else if (tSpin == 1)
				{
					if (back2back > -1)
						score += ((TSDB * 1.5 + combo * CMBO) * (level + 1));
					else
						score += ((TSDB + combo * CMBO) * (level + 1));

					back2back++;
				}
				else
				{
					score += ((DUBL + combo * CMBO) * (level + 1));
					back2back = -1;
				}

				break;
			}
			case 3:
			{
				combo++;

				if (tSpin == 1)
				{
					if (back2back > -1)
						score += ((TSTR * 1.5 + combo * CMBO) * (level + 1));
					else
						score += ((TSTR + combo * CMBO) * (level + 1));

					back2back++;
				}
				else
				{
					score += ((TRPL + combo * CMBO) * (level + 1));
					back2back = -1;
				}

				break;
			}
			case 4:	// mo¿liwe tylko z I-minem
			{
				combo++;

				if (back2back > -1)
					score += ((QUAD + combo * CMBO) * (level + 1));
				else
					score += ((QUAD + combo * CMBO) * (level + 1));

				back2back++;

				break;
			}
		}

		if (cleared > 0)
		{
			sf::FloatRect bounds;
			txt_stats[4].setString(std::to_string(totalCleared));
			bounds = txt_stats[4].getLocalBounds();
			txt_stats[4].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		}

		bool pClear = true;
		for (int i = 0; i < 10; i++)
		{
			if (matrix[i][0] != NULL)
			{
				pClear = false;
				break;
			}
		}
		if (pClear)
		{
			score += (PCLR * (level + 1));
		}

		txt_stats[2].setString("");
		int len = std::to_string(score).length();
		for (int i = 0; i < (8 - len); i++)
			txt_stats[2].setString(txt_stats[2].getString() + "0");
		txt_stats[2].setString(txt_stats[2].getString() + std::to_string(score));
		sf::FloatRect bounds = txt_stats[2].getLocalBounds();
		txt_stats[2].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		txt_stats[2].setPosition(sf::Vector2f(1632.0, 540.0));
	}

	bool checkPos(int xSh, int ySh)
	{
		int checkX[4];
		int checkY[4];

		checkX[3] = activeAnchor->posX + xSh;
		checkY[3] = activeAnchor->posY + ySh;

		for (int i = 0; i < 3; i++)
		{
			checkX[i] = checkX[3] + activeAnchor->children[i]->posX;
			checkY[i] = checkY[3] + activeAnchor->children[i]->posY;
		}

		for (int i = 0; i < 4; i++)
		{
			if ((checkX[i] < 0) || (checkX[i] > 9))
				return false;
			if ((checkY[i] < 0) || (checkY[i] > 39))
				return false;
		}

		for (int i = 0; i < 4; i++)
		{
			if (matrix[checkX[i]][checkY[i]] != NULL)
				return false;
		}

		return true;
	}

	void sdropCalc()
	{
		softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;
	}

	void drawPreview(sf::RenderWindow &w, sf::Texture &t)
	{
		sf::Sprite spr_Preview;
		spr_Preview.setTexture(t);

		spr_Preview.setTextureRect(sf::IntRect((t.getSize().x) / 3 * (pcQueue[0] % 3), (t.getSize().y) / 3 * ((pcQueue[0] - (pcQueue[0] % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
		spr_Preview.setOrigin(sf::Vector2f((t.getSize().x) / 6.0, (t.getSize().y) / 6.0));
		spr_Preview.setPosition(sf::Vector2f(qBox.getPosition().x - 2.0, qBox.getPosition().y - 278.0));

		w.draw(spr_Preview);

		if (hold != 7)
		{
			if (holdBox[1].getTexture() == NULL)
			{
				spr_Preview.setTextureRect(sf::IntRect((t.getSize().x) / 3 * (hold % 3), (t.getSize().y) / 3 * ((hold - (hold % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
				spr_Preview.setOrigin(sf::Vector2f((t.getSize().x) / 6.0, (t.getSize().y) / 6.0));
				spr_Preview.setPosition(sf::Vector2f(holdBox[0].getPosition().x + 12.0, qBox.getPosition().y - 278.0));

				if (!holdActive)
				{
					spr_Preview.setColor(sf::Color(255, 255, 255, 128));
				}
			}

			w.draw(spr_Preview);
			spr_Preview.setColor(sf::Color(255, 255, 255, 255));
		}

		spr_Preview.setScale(sf::Vector2f(2.0 / 3.0, 2.0 / 3.0));
		for (int i = 1; i < 5; i++)
		{
			spr_Preview.setTextureRect(sf::IntRect(128 * (pcQueue[i] % 3), 64 * ((pcQueue[i] - (pcQueue[i] % 3)) / 3), 128, 64));
			spr_Preview.setPosition(sf::Vector2f(qBox.getPosition().x - 27.0, qBox.getPosition().y - 209.0 + 54.0 * (i - 1)));

			w.draw(spr_Preview);
		}
	}

	void drawMatrix(sf::RenderWindow &w, sf::Texture &t)
	{
		// zaktualizuj wyœwietlany czas

		sf::FloatRect bounds;
		txt_stats[3].setString("");
		txt_stats[3].setString(txt_stats[3].getString() + std::to_string((int)floor(time.asSeconds() / 60.0)) + ":");
		if ((floor((int)(time.asSeconds()) % 60)) < 10)
			txt_stats[3].setString(txt_stats[3].getString() + "0" + std::to_string((int)floor((int)(time.asSeconds()) % 60)) + ".");
		else
			txt_stats[3].setString(txt_stats[3].getString() + std::to_string((int)floor((int)(time.asSeconds()) % 60)) + ".");
		if ((time.asMilliseconds() % 1000) < 10)
			txt_stats[3].setString(txt_stats[3].getString() + "00" + std::to_string(time.asMilliseconds() % 1000));
		else if ((time.asMilliseconds() % 1000) < 100)
			txt_stats[3].setString(txt_stats[3].getString() + "0" + std::to_string(time.asMilliseconds() % 1000));
		else
			txt_stats[3].setString(txt_stats[3].getString() + std::to_string(time.asMilliseconds() % 1000));
		bounds = txt_stats[3].getLocalBounds();
		txt_stats[3].setOrigin(sf::Vector2f(bounds.width, bounds.height));
		
		w.draw(txt_stats[0]);
		w.draw(txt_stats[1]);
		w.draw(txt_stats[2]);
		w.draw(txt_stats[3]);
		w.draw(txt_stats[4]);
		w.draw(txt_stats[5]);

		// rysuj ducha

		if (activeAnchor != NULL)
		{
			sf::Sprite spr_Ghost;
			spr_Ghost.setTexture(t);

			int xMax, yMax, xMin, yMin;
			int howMuch = 0;

			xMin = activeAnchor->getPosition().x;
			yMin = activeAnchor->getPosition().y;
			xMax = activeAnchor->getPosition().x;
			yMax = activeAnchor->getPosition().y;

			for (int i = 0; i < 3; i++)
			{
				if (activeAnchor->children[i]->getPosition().x > xMin)
					xMin = activeAnchor->children[i]->getPosition().x;
				if (activeAnchor->children[i]->getPosition().y > yMin)
					yMin = activeAnchor->children[i]->getPosition().y;
				if (activeAnchor->children[i]->getPosition().x < xMin)
					xMax = activeAnchor->children[i]->getPosition().x;
				if (activeAnchor->children[i]->getPosition().y < yMin)
					yMax = activeAnchor->children[i]->getPosition().y;
			}

			while (checkPos(0, 0 - (1 + howMuch)))
				howMuch++;

			spr_Ghost.setTextureRect(sf::IntRect((t.getSize().x) / 3 * (activeAnchor->colour % 3), (t.getSize().y) / 3 * ((activeAnchor->colour - (activeAnchor->colour % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
			spr_Ghost.setOrigin(sf::Vector2f((t.getSize().x) / 6.0, (t.getSize().y) / 6.0));
			spr_Ghost.setPosition(sf::Vector2f((xMin + xMax) / 2.0 + 16.0, ((yMin + yMax) / 2.0) + (howMuch * 32.0) + 16.0));
			spr_Ghost.setRotation(activeAnchor->rot * 90.0);
			spr_Ghost.setColor(sf::Color(255, 255, 255, 128));

			w.draw(spr_Ghost);
		}

		// rysuj aktywne Tetrimino
		if (activeAnchor != NULL)
		{
			w.draw(*activeAnchor);
			w.draw(*activeAnchor->children[0]);
			w.draw(*activeAnchor->children[1]);
			w.draw(*activeAnchor->children[2]);
		}

		// rysuj zawartosc Matrixa

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (matrix[i][j] != NULL)
				{
					if (j >= 20)
					{
						matrix[i][j]->setColor(sf::Color(255, 255, 255, (int)(255.0 / pow((j - 18.0), 2.0))));
					}
					w.draw(*matrix[i][j]);
					if (j >= 20)
					{
						matrix[i][j]->setColor(sf::Color(255, 255, 255, 255));
					}
				}
			}
		}

	}
};

// Definicja metod

MinoAnchor::~MinoAnchor()
{
	if (readyToLock)
	{

		// utworzenie min w matrixie
		matrix->matrix[posX][posY] = new Mino();
		matrix->matrix[posX + children[0]->posX][posY + children[0]->posY] = new Mino();
		matrix->matrix[posX + children[1]->posX][posY + children[1]->posY] = new Mino();
		matrix->matrix[posX + children[2]->posX][posY + children[2]->posY] = new Mino();

		// zastosowanie pozycji min na ekranie
		matrix->matrix[posX][posY]->setPosition(getPosition());
		matrix->matrix[posX + children[0]->posX][posY + children[0]->posY]->setPosition(children[0]->getPosition());
		matrix->matrix[posX + children[1]->posX][posY + children[1]->posY]->setPosition(children[1]->getPosition());
		matrix->matrix[posX + children[2]->posX][posY + children[2]->posY]->setPosition(children[2]->getPosition());

		// ustawienie kolorów i duszków min
		matrix->matrix[posX][posY]->colour = colour;
		matrix->matrix[posX + children[0]->posX][posY + children[0]->posY]->colour = colour;
		matrix->matrix[posX + children[1]->posX][posY + children[1]->posY]->colour = colour;
		matrix->matrix[posX + children[2]->posX][posY + children[2]->posY]->colour = colour;

		// usuniêcie min sterowalnych

	}
	delete children[0];
	delete children[1];
	delete children[2];
	matrix->activeAnchor = NULL;
}

void MinoAnchor::updateScreenXY()
{
	setPosition(sf::Vector2f(matrix->getPosition().x - (getTexture()->getSize().x) / 3.0 - 128.0 + (posX * 32.0), matrix->getPosition().y + 286.0 - (posY * 32.0)));
	children[0]->setPosition(sf::Vector2f(getPosition().x + (children[0]->posX * ((children[0]->getTexture()->getSize().x) / 3.0)), getPosition().y - (children[0]->posY * ((children[0]->getTexture()->getSize().y) / 3.0))));
	children[1]->setPosition(sf::Vector2f(getPosition().x + (children[1]->posX * ((children[1]->getTexture()->getSize().x) / 3.0)), getPosition().y - (children[1]->posY * ((children[1]->getTexture()->getSize().y) / 3.0))));
	children[2]->setPosition(sf::Vector2f(getPosition().x + (children[2]->posX * ((children[2]->getTexture()->getSize().x) / 3.0)), getPosition().y - (children[2]->posY * ((children[2]->getTexture()->getSize().y) / 3.0))));
}

// Definicja funkcji

void defineOffset(int t[3][4][5][2])
{
	// J L S T Z
		// 0
	t[0][0][0][0] = 0;
	t[0][0][0][1] = 0;
	t[0][0][1][0] = 0;
	t[0][0][1][1] = 0;
	t[0][0][2][0] = 0;
	t[0][0][2][1] = 0;
	t[0][0][3][0] = 0;
	t[0][0][3][1] = 0;
	t[0][0][4][0] = 0;
	t[0][0][4][1] = 0;
	// R
	t[0][1][0][0] = 0;
	t[0][1][0][1] = 0;
	t[0][1][1][0] = 1;
	t[0][1][1][1] = 0;
	t[0][1][2][0] = 1;
	t[0][1][2][1] = -1;
	t[0][1][3][0] = 0;
	t[0][1][3][1] = 2;
	t[0][1][4][0] = 1;
	t[0][1][4][1] = 2;
	// 2
	t[0][2][0][0] = 0;
	t[0][2][0][1] = 0;
	t[0][2][1][0] = 0;
	t[0][2][1][1] = 0;
	t[0][2][2][0] = 0;
	t[0][2][2][1] = 0;
	t[0][2][3][0] = 0;
	t[0][2][3][1] = 0;
	t[0][2][4][0] = 0;
	t[0][2][4][1] = 0;
	// L
	t[0][3][0][0] = 0;
	t[0][3][0][1] = 0;
	t[0][3][1][0] = -1;
	t[0][3][1][1] = 0;
	t[0][3][2][0] = -1;
	t[0][3][2][1] = -1;
	t[0][3][3][0] = 0;
	t[0][3][3][1] = 2;
	t[0][3][4][0] = -1;
	t[0][3][4][1] = 2;

	// I
		// 0
	t[1][0][0][0] = 0;
	t[1][0][0][1] = 0;
	t[1][0][1][0] = -1;
	t[1][0][1][1] = 0;
	t[1][0][2][0] = 2;
	t[1][0][2][1] = 0;
	t[1][0][3][0] = -1;
	t[1][0][3][1] = 0;
	t[1][0][4][0] = 2;
	t[1][0][4][1] = 0;
	// R
	t[1][1][0][0] = -1;
	t[1][1][0][1] = 0;
	t[1][1][1][0] = 0;
	t[1][1][1][1] = 0;
	t[1][1][2][0] = 0;
	t[1][1][2][1] = 0;
	t[1][1][3][0] = 0;
	t[1][1][3][1] = 1;
	t[1][1][4][0] = 0;
	t[1][1][4][1] = -2;
	// 2
	t[1][2][0][0] = -1;
	t[1][2][0][1] = 1;
	t[1][2][1][0] = 1;
	t[1][2][1][1] = 1;
	t[1][2][2][0] = -2;
	t[1][2][2][1] = 1;
	t[1][2][3][0] = 1;
	t[1][2][3][1] = 0;
	t[1][2][4][0] = -2;
	t[1][2][4][1] = 0;
	// L
	t[1][3][0][0] = 0;
	t[1][3][0][1] = 1;
	t[1][3][1][0] = 0;
	t[1][3][1][1] = 1;
	t[1][3][2][0] = 0;
	t[1][3][2][1] = 1;
	t[1][3][3][0] = 0;
	t[1][3][3][1] = -1;
	t[1][3][4][0] = 0;
	t[1][3][4][1] = 2;

	// O
		// 0
	t[2][0][0][0] = 0;
	t[2][0][0][1] = 0;
	t[2][0][1][0] = 0;
	t[2][0][1][1] = 0;
	t[2][0][2][0] = 0;
	t[2][0][2][1] = 0;
	t[2][0][3][0] = 0;
	t[2][0][3][1] = 0;
	t[2][0][4][0] = 0;
	t[2][0][4][1] = 0;
	// R
	t[2][1][0][0] = 0;
	t[2][1][0][1] = -1;
	t[2][1][1][0] = 0;
	t[2][1][1][1] = -1;
	t[2][1][2][0] = 0;
	t[2][1][2][1] = -1;
	t[2][1][3][0] = 0;
	t[2][1][3][1] = -1;
	t[2][1][4][0] = 0;
	t[2][1][4][1] = -1;
	// 2
	t[2][2][0][0] = -1;
	t[2][2][0][1] = -1;
	t[2][2][1][0] = -1;
	t[2][2][1][1] = -1;
	t[2][2][2][0] = -1;
	t[2][2][2][1] = -1;
	t[2][2][3][0] = -1;
	t[2][2][3][1] = -1;
	t[2][2][4][0] = -1;
	t[2][2][4][1] = -1;
	// L
	t[2][3][0][0] = -1;
	t[2][3][0][1] = 0;
	t[2][3][1][0] = -1;
	t[2][3][1][1] = 0;
	t[2][3][2][0] = -1;
	t[2][3][2][1] = 0;
	t[2][3][3][0] = -1;
	t[2][3][3][1] = 0;
	t[2][3][4][0] = -1;
	t[2][3][4][1] = 0;
}

void enqueue(struct pQueue* q, int d)	// przykladowe wywolanie funkcji: enqueue(pcQueue, 5);
{
	struct Queue* newNode = (struct Queue*)malloc(sizeof(struct Queue));

	if (newNode)
	{
		newNode->color = d;
		newNode->next = NULL;

		if (q->head == NULL)
		{
			q->head = newNode;
			q->tail = newNode;
			newNode->id = 1;
		}
		else
		{
			newNode->id = q->tail->id + 1;
			q->tail->next = newNode;
			q->tail = newNode;
		}
	}
	else
	{
		std::cout << "Nowy element nie zostal utworzony!\n";
	}
}

void dequeue(struct pQueue* q)
{
	if (q->head)
	{
		struct Queue* tmp = q->head->next;
		free(q->head);
		q->head = tmp;

		if (tmp == NULL)
		{
			q->tail = NULL;
		}
	}
}

void addBag(struct pQueue* q)
{
	/*
	kolory i kszta³ty Mino:
		0 - Z - czerwony
		1 - L - pomarañczowy
		2 - O - ¿ó³ty
		3 - S - zielony
		4 - I - b³êkitny
		5 - J - niebieski
		6 - T - fioletowy
		7 - œmieciomino (obra¿enia) - szary
		8 - œmieciomino (limit czasu) - czarny
	*/

	int bag[7] = { 0, 1, 2, 3, 4, 5, 6 };

	for (int i = 0; i < 7; i++)
	{
		int rndPc = rand() % 7;
		int swap = bag[rndPc];
		bag[rndPc] = bag[i];
		bag[i] = swap;
	}

	for (int i = 0; i < 7; i++)
		enqueue(q, bag[i]);
}

// Pêtla g³ówna

int main()
{

	queueFile.open("queueFile.txt");

	defineOffset(offsetChart);

	srand(time(NULL));

	dftKey["LMV"] = sf::Keyboard::A;		// Left MoVement
	dftKey["RMV"] = sf::Keyboard::D;	// Right MoVement
	dftKey["LSP"] = sf::Keyboard::Numpad4;		// Left SPin
	dftKey["RSP"] = sf::Keyboard::Numpad6;		// Right SPin
	dftKey["SDR"] = sf::Keyboard::S;		// Soft DRop
	dftKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
	dftKey["HLD"] = sf::Keyboard::Numpad5;		// HoLD
	dftKey["PSE"] = sf::Keyboard::Escape;	// PauSE

	/*
	dftKey["LMV"] = sf::Keyboard::Left;		// Left MoVement
	dftKey["RMV"] = sf::Keyboard::Right;	// Right MoVement
	dftKey["LSP"] = sf::Keyboard::Z;		// Left SPin
	dftKey["RSP"] = sf::Keyboard::X;		// Right SPin
	dftKey["SDR"] = sf::Keyboard::Down;		// Soft DRop
	dftKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
	dftKey["HLD"] = sf::Keyboard::C;		// HoLD
	dftKey["PSE"] = sf::Keyboard::Escape;	// PauSE
	*/

	sf::Clock frameTimer;
	int fpsCount;

	sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Tetris PSk", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	// £adowanie tekstur

	sf::Texture ttr_Matrix;
	if (!ttr_Matrix.loadFromFile("sprites/spr_matrix.png"))
	{
		// nyeh
	}
	ttr_Matrix.setSmooth(true);

	sf::Texture ttr_Mino;
	if (!ttr_Mino.loadFromFile("sprites/spr_mino.png"))
	{
		// nyeh
	}
	ttr_Mino.setSmooth(true);

	sf::Texture ttr_HoldSolo;
	if (!ttr_HoldSolo.loadFromFile("sprites/spr_holdSolo.png"))
	{
		// nyeh
	}
	ttr_HoldSolo.setSmooth(true);

	sf::Texture ttr_HoldTop;
	if (!ttr_HoldTop.loadFromFile("sprites/spr_holdTop.png"))
	{
		// nyeh
	}
	ttr_HoldTop.setSmooth(true);

	sf::Texture ttr_HoldBot;
	if (!ttr_HoldBot.loadFromFile("sprites/spr_holdBot.png"))
	{
		// nyeh
	}
	ttr_HoldBot.setSmooth(true);

	sf::Texture ttr_Queue5;
	if (!ttr_Queue5.loadFromFile("sprites/spr_queue5.png"))
	{
		// nyeh
	}
	ttr_Queue5.setSmooth(true);

	sf::Texture ttr_Preview;
	if (!ttr_Preview.loadFromFile("sprites/spr_preview.png"))
	{
		// nyeh
	}
	ttr_Preview.setSmooth(true);

	sf::Texture ttr_soloStats;
	if (!ttr_soloStats.loadFromFile("sprites/spr_soloStats.png"))
	{
		// nyeh
	}
	ttr_soloStats.setSmooth(true);

	sf::Texture ttr_bg;
	if (!ttr_bg.loadFromFile("sprites/spr_bgBlurC.png"))
	{
		// nyeh
	}
	ttr_bg.setSmooth(true);

	sf::Texture ttr_matrixBgSolo;
	if (!ttr_matrixBgSolo.loadFromFile("sprites/spr_matrixBgSolo.png"))
	{
		// nyeh
	}
	ttr_matrixBgSolo.setSmooth(true);

	// £adowanie czcionek

	sf::Font fnt_MobyR;
	if (!fnt_MobyR.loadFromFile("fonts/Moby-Regular.otf"))
	{
		// nyeh
	}

	sf::Font fnt_MobyB;
	if (!fnt_MobyB.loadFromFile("fonts/Moby-Bold.otf"))
	{
		// nyeh
	}

	sf::Font fnt_MobyM;
	if (!fnt_MobyM.loadFromFile("fonts/Moby-Monospace.otf"))
	{
		// nyeh
	}

	//

	sf::View view(sf::Vector2f(FULLRES_W / 2.0, FULLRES_H / 2.0f), sf::Vector2f(FULLRES_W, FULLRES_H)); // center, size

	pQueue pcQueue;
	addBag(&pcQueue);

	sf::Sprite bg;
	bg.setTexture(ttr_bg);
	int randNum = rand() % 9;
	bg.setTextureRect(sf::IntRect((ttr_bg.getSize().x) / 3 * (randNum % 3), (ttr_bg.getSize().y) / 3 * ((randNum - (randNum % 3)) / 3), (ttr_bg.getSize().x) / 3, (ttr_bg.getSize().y) / 3));

	Player* player[2];

	player[0] = new Player(ttr_Matrix, fnt_MobyM);

	player[0]->holdBox[0].setTexture(ttr_HoldSolo);
	player[0]->holdBox[0].setOrigin(sf::Vector2f((player[0]->holdBox[0].getTexture()->getSize().x) / 2.0, (player[0]->holdBox[0].getTexture()->getSize().y) / 2.0));
	player[0]->holdBox[0].setPosition(sf::Vector2f(player[0]->getPosition().x - ((player[0]->getTexture()->getSize().x) / 2.0) - ((player[0]->holdBox[0].getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));

	player[0]->qBox.setTexture(ttr_Queue5);
	player[0]->qBox.setOrigin(sf::Vector2f((player[0]->qBox.getTexture()->getSize().x) / 2.0, (player[0]->qBox.getTexture()->getSize().y) / 2.0));
	player[0]->qBox.setPosition(sf::Vector2f(player[0]->getPosition().x + ((player[0]->getTexture()->getSize().x) / 2.0) + ((player[0]->qBox.getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));
	
	sf::Sprite soloStats;
	soloStats.setTexture(ttr_soloStats);
	soloStats.setOrigin(sf::Vector2f((ttr_soloStats.getSize().x) / 2.0, (ttr_soloStats.getSize().y) / 2.0));
	soloStats.setPosition(sf::Vector2f(FULLRES_W - 564, FULLRES_H / 2.0 + 2.0));

	sf::Sprite matrixBg;
	matrixBg.setTexture(ttr_matrixBgSolo);
	matrixBg.setOrigin(sf::Vector2f(ttr_matrixBgSolo.getSize().x / 2.0, ttr_matrixBgSolo.getSize().y / 2.0));
	matrixBg.setPosition(sf::Vector2f(player[0]->getPosition().x - 10.0, player[0]->getPosition().y));
	matrixBg.setColor(sf::Color(255, 255, 255, 128));

	for (int i = 0; i < 5; i++)
		player[0]->updateQueue(&pcQueue);

	player[1] = NULL;

	player[0]->genPiece(&pcQueue, ttr_Mino);
	if (player[0]->checkPos(0, -1))
		player[0]->activeAnchor->posY -= 1;

	// Pêtla gry

	while (window.isOpen())
	{
		sf::Time elapsed1 = frameTimer.getElapsedTime();

		player[0]->time += elapsed1;

		if (elapsed1.asMicroseconds() != 0)
			fpsCount = 1000000 / elapsed1.asMicroseconds();
		else
			fpsCount = INT_MAX;
		sf::String titleText = "Tetris PSk - " + std::to_string(fpsCount) + " FPS";
		window.setTitle(titleText);


		frameTimer.restart();

		// Kontrola
		//std::cout << player[0]->softDropTime << ", " << player[0]->lockdownReset << ", " << player[0]->spawnTime << "\t\t";
		if (player[0]->activeAnchor == NULL)
		{
			if (player[0]->holdSwap)
			{
				player[0]->genPiece(&pcQueue, ttr_Mino);
				if (player[0]->checkPos(0, -1))
					player[0]->activeAnchor->posY -= 1;
				player[0]->holdSwap = false;
			}
			else
			{
				if (player[0]->spawnTime == 0)
				{
					player[0]->checkVisuals(ttr_Mino);
				}

				if (player[0]->spawnTime >= SPAWN_RESET)
				{
					player[0]->clearLines();
					player[0]->checkVisuals(ttr_Mino);

					player[0]->spawnTime = -1;
					player[0]->lockdownReset = 0;
					player[0]->lockdownReset = -1;
					player[0]->softDropTime = 0;
					player[0]->genPiece(&pcQueue, ttr_Mino);
					player[0]->ctrlState |= 0x20;
					player[0]->holdActive = true;
					player[0]->tSpin = -1;
					if (player[0]->checkPos(0, -1))
						player[0]->activeAnchor->posY -= 1;
				}
				else
				{
					player[0]->spawnTime += elapsed1.asMilliseconds();
				}
			}
		}
		else if (player[0]->activeAnchor != NULL)
		{
			player[0]->ctrlDo(elapsed1.asMilliseconds(), ttr_Mino);
			if ((player[0]->lockdownReset > LOCK_RESET) && (player[0]->spawnTime == -1))
			{
				player[0]->lockIn(ttr_Mino);
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Rysowanie

		window.setView(view);

		window.clear();

		window.draw(bg);
		window.draw(matrixBg);
		window.draw(soloStats);

		window.draw(*player[0]);
		window.draw(player[0]->holdBox[0]);
		window.draw(player[0]->qBox);
		player[0]->drawPreview(window, ttr_Preview);
		player[0]->drawMatrix(window, ttr_Preview);
		
		

		window.display();

	}

	return 0;
}