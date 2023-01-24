/*
Wa�ne komentarze:
	NOTE - notka, do przemy�lenia
	FIXIT - trzeba to naprawi�, czemu??
	TODO - do zrobienia
*/

/*
	TODO og�lne
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

// Deklaracja tekstur

sf::Texture ttr_Matrix;
sf::Texture ttr_Mino;
sf::Texture ttr_HoldSolo;
sf::Texture ttr_HoldTop;
sf::Texture ttr_HoldBot;
sf::Texture ttr_Queue5;
sf::Texture ttr_Preview;

std::ofstream queueFile;

// Definicja sta�ych warto�ci gry

#define DAS_DEFAULT 170	// DAS - Delayed Auto Shift - ilo�� czasu w ms, przez jak� trzeba trzyma� przucisk, �eby tetrimino zacz�o si� porusza� ze sta�� pr�dko�ci�
#define ARR_DEFAULT 50	// ARR - Auto Repeat Rate - ilo�� czasu w ms pomi�dzy kolejnymi krokami tetrimina w bok
#define SDF_DEFAULT 20	// SDF - Soft Drop Factor - mno�nik standardowej pr�dko�ci opadania tetrimina przy soft-dropie
#define LOCK_TIME	500 // czas w ms, jaki tetrimino musi sp�dzi� po nieudanej pr�bie opuszczenia w d�, by zosta� zablokowane w miejscu
#define LOCK_RESET	15	// maksymalna ilo�� reset�w czasomierza lock-in, zapobiegaj�ca niesko�czonemu unikaniu zalokowania
#define SPAWN_RESET	117	// czas w ms od zalokowania Tetrimina, po kt�rym generowane jest nowe

int offsetChart[3][4][5][2]; // Tabela obr�t�w - [zestaw][kierunek][nr testu][x lub y]

enum ctrlMask
{
	LMV = 0x01, // Left MoVement
	RMV = 0x02, // Right MoVement
	LSP = 0x04, // Left SPin
	RSP = 0x08, // Right SPin
	SDR = 0x10, // Soft DRop
	HDR = 0x20, // Hard DRop
	HLD = 0x40, // HoLD
	PSE = 0x80, // PauSE
};

std::map <std::string, sf::Keyboard::Key> dftKey;

// Definicja grafiki

#define FULLRES_W 1920
#define FULLRES_H 1080

bool fullScreen = false;

double windowScale = 1.0 / 2.0;
int windowW = FULLRES_W * windowScale;
int windowH = FULLRES_H * windowScale;

// Deklaracja obiekt�w i funkcji

struct Queue;
struct pQueue;

class Mino;
class MinoAnchor;
class Matrix;
class Player;

void enqueue(struct pQueue*, int);
void dequeue(struct pQueue*);
void addBag(struct pQueue*);

// Definicja obiekt�w

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
				children[0]->posX = 0-1;
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
				children[1]->posX = 0-1;
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
				children[2]->posX = 0-1;
				children[2]->posY = 0;
				break;
			}
			case 4: // I
			{
				children[0]->posX = 0-1;
				children[0]->posY = 0;
				children[1]->posX = 1;
				children[1]->posY = 0;
				children[2]->posX = 2;
				children[2]->posY = 0;
				break;
			}
			case 5: // J
			{
				children[0]->posX = 0-1;
				children[0]->posY = 1;
				children[1]->posX = 0-1;
				children[1]->posY = 0;
				children[2]->posX = 1;
				children[2]->posY = 0;
				break;
			}
			case 6: // T
			{
				children[0]->posX = 0;
				children[0]->posY = 1;
				children[1]->posX = 0-1;
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
	int rot; // 0 - pozycja startowa; 1 - obr�t w prawo; 2 - dwa obroty; 3 - obr�t w lewo
	Mino* children[3];
	bool readyToLock;

	void updateScreenXY();
};

class Player : public sf::Sprite
{
public:
	Player(sf::Texture &t)
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
		posShift[0] = 0;
		posShift[1] = 0;

		level = 0;
		hold = 7;
		holdActive = true;
		holdSwap = false;

		for (int i = 0; i < 4; i++)
		{
			toClear[i] = 50;
		}

		softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;
	}

	int id;
	int piecesUsed = 0;
	int posShift[2];

	Mino* matrix[10][40];

	sf::Sprite holdBox[2];
	sf::Sprite qBox;

	int level;
	int hold;
	bool holdActive;
	bool holdSwap;
	int toClear[4];

	MinoAnchor* activeAnchor;

	std::map <std::string, sf::Keyboard::Key> ctrlKey;

	uint8_t ctrlState = 0x00;

	int sideMoveTime = 0;
	int softDropTime = 0;
	double softDropUpdate;
	int lockdownTime = -1;
	int lockdownReset = 0;
	int spawnTime = -1;
	int domDir = 0; // dominuj�cy kierunek - -1 to lewo, 1 to prawo

	int pcQueue[5];

	void genPiece(pQueue* q, sf::Texture &t) // stw�rz nowe tetrimino
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

		// Wykonaj funkcje przycisk�w
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
				}
				posShift[1] = 16;

				lockdownReset = INT_MAX - LOCK_RESET;
				
				//activeAnchor->move(sf::Vector2f(posShift[0], posShift[1]));
				activeAnchor->updateScreenXY();
				return;
			}
		}

		if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]) || sf::Keyboard::isKeyPressed(ctrlKey["RMV"]))	// jest wcisniety klawisz ruchu w bok
		{
			if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]) && sf::Keyboard::isKeyPressed(ctrlKey["RMV"])) // oba przyciski wci�ni�te
			{
				if ((ctrlState & LMV) && (ctrlState & RMV))	// poprzednio oba
				{
					sideMoveTime += t;

					if (domDir == -1) // prawy guzik zosta� doci�ni�ty do lewego
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
							else
								posShift[0] = 4;
						}
					}
					else // lewy guzik doci�ni�ty do prawego
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
							else
								posShift[0] = -4;
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
					else
						posShift[0] = 4;
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
					else
						posShift[0] = -4;
				}
				else // poprzednio �aden
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
					// nic si� nie dzieje
				}
				else // poprzednio �aden
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
					else
						posShift[0] = -4;
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
					else
						posShift[0] = -4;
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
					// nic si� nie dzieje
				}
				else // poprzednio �aden
				{
					domDir = 1;
					if (checkPos(1, 0))
						activeAnchor->posX += 1;
					else
						posShift[0] = 4;
				}

				if (sideMoveTime >= DAS_DEFAULT)
				{
					sideMoveTime -= ARR_DEFAULT;
					if (checkPos(1, 0))
						activeAnchor->posX += 1;
					else
						posShift[0] = 4;
				}
			}
		}
		else // �aden klawisz ruchu w bok nie jest wci�ni�ty
		{
			sideMoveTime = 0;
			domDir = 0;
		}

		if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"]) && sf::Keyboard::isKeyPressed(ctrlKey["RSP"])) // oba przyciski obrotu wci�ni�te - FIXIT
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
		else if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"])) // lewy obr�t
		{
			if (!(ctrlState & LSP)) // poprzednio nie by� trzymany lewy
			{
				rotate(false);
			}
		}
		else if (sf::Keyboard::isKeyPressed(ctrlKey["RSP"])) // prawy obr�t
		{
			if (!(ctrlState & RSP)) // poprzednio nie by� trzymany prawy
			{
				rotate(true);
			}
		}

		if (sf::Keyboard::isKeyPressed(ctrlKey["PSE"]))
		{
			if (!(ctrlState & PSE))
			{
				// TODO - zapauzuj gr�
			}
		}

		// Zaktualizuj grawitacj�

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
			}
			else
			{
				if (sf::Keyboard::isKeyPressed(ctrlKey["SDR"]))
				{
					posShift[1] = 4;
				}
				if (lockdownTime == -1)
				{
					lockdownTime = 0;
				}
				else
				{
					lockdownTime += t;

					if (lockdownTime >= LOCK_TIME)
						lockdownReset = LOCK_RESET + 1;
					// TODO - je�eli wi�ksze ni� czas na lockdown, zalokuj
				}
			}
		}

		activeAnchor->updateScreenXY();
		activeAnchor->move(sf::Vector2f(posShift[0], posShift[1]));
		activeAnchor->children[0]->move(sf::Vector2f(posShift[0], posShift[1]));
		activeAnchor->children[1]->move(sf::Vector2f(posShift[0], posShift[1]));
		activeAnchor->children[2]->move(sf::Vector2f(posShift[0], posShift[1]));

		// Zapisz obecne wci�ni�cia do por�wnania w nast�pnej klatce

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
		for (int i = 0; i < 20; i++)
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
		for (int i = 0; i < 4; i++)
		{
			if (toClear[i] < 40)
			{
				for (int j = 0; j < 10; j++)
				{
					for (int k = toClear[i]; k < 39; k++)
					{
						matrix[j][k] = matrix[j][k+1];
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

	void shift(bool b, bool c)
	{
		if (b == true)
		{
			move(sf::Vector2f(posShift[0], posShift[1]));
			holdBox[0].move(sf::Vector2f(posShift[0], posShift[1]));
			qBox.move(sf::Vector2f(posShift[0], posShift[1]));
			for (int i = 0; i < 21; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (matrix[j][i] != NULL)
					{
						matrix[j][i]->move(sf::Vector2f(posShift[0], posShift[1]));
					}
				}
			}
		}	
		else
		{
			move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
			holdBox[0].move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
			qBox.move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
			for (int i = 0; i < 21; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (matrix[j][i] != NULL)
					{
						matrix[j][i]->move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
					}
				}
			}
			
			if (((!sf::Keyboard::isKeyPressed(ctrlKey["LMV"])) && (posShift[0] < 0)) || ((!sf::Keyboard::isKeyPressed(ctrlKey["RMV"])) && (posShift[0] > 0)))
			{
				if (abs(posShift[0]) > 1)
					posShift[0] /= 2.0;
				else
					posShift[0] = 0;
			}
			if ((activeAnchor == NULL) || ((activeAnchor != NULL) && (!sf::Keyboard::isKeyPressed(ctrlKey["LSP"]))))
			{
				if (posShift[1] > 1)
					posShift[1] /= 2.0;
				else
					posShift[1] = 0;
			}
		}
		
	}

	void drawPreview(sf::RenderWindow &w)
	{
		sf::Sprite spr_Preview;
		spr_Preview.setTexture(ttr_Preview);

		spr_Preview.setTextureRect(sf::IntRect((ttr_Preview.getSize().x) / 3 * (pcQueue[0] % 3), (ttr_Preview.getSize().y) / 3 * ((pcQueue[0] - (pcQueue[0] % 3)) / 3), (ttr_Preview.getSize().x) / 3, (ttr_Preview.getSize().y) / 3));
		spr_Preview.setOrigin(sf::Vector2f((ttr_Preview.getSize().x) / 6.0, (ttr_Preview.getSize().y) / 6.0));
		spr_Preview.setPosition(sf::Vector2f(qBox.getPosition().x - 2.0, qBox.getPosition().y - 278.0));

		w.draw(spr_Preview);

		if (hold != 7)
		{
			if (holdBox[1].getTexture() == NULL)
			{
				spr_Preview.setTextureRect(sf::IntRect((ttr_Preview.getSize().x) / 3 * (hold % 3), (ttr_Preview.getSize().y) / 3 * ((hold - (hold % 3)) / 3), (ttr_Preview.getSize().x) / 3, (ttr_Preview.getSize().y) / 3));
				spr_Preview.setOrigin(sf::Vector2f((ttr_Preview.getSize().x) / 6.0, (ttr_Preview.getSize().y) / 6.0));
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

	void drawMatrix(sf::RenderWindow &w)
	{	
		// rysuj ducha
		
		if (activeAnchor != NULL)
		{
			sf::Sprite spr_Ghost;
			spr_Ghost.setTexture(ttr_Preview);

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

			spr_Ghost.setTextureRect(sf::IntRect((ttr_Preview.getSize().x) / 3 * (activeAnchor->colour % 3), (ttr_Preview.getSize().y) / 3 * ((activeAnchor->colour - (activeAnchor->colour % 3)) / 3), (ttr_Preview.getSize().x) / 3, (ttr_Preview.getSize().y) / 3));
			spr_Ghost.setOrigin(sf::Vector2f((ttr_Preview.getSize().x) / 6.0, (ttr_Preview.getSize().y) / 6.0));
			spr_Ghost.setPosition(sf::Vector2f((xMin + xMax) / 2.0 + 16.0, ((yMin + yMax) / 2.0) + (howMuch * 32.0) + 16.0));
			spr_Ghost.setRotation(activeAnchor->rot * 90.0);
			spr_Ghost.setColor(sf::Color(255, 255, 255, 85));

			w.draw(spr_Ghost);
		}

		// rysuj aktywne Tetrimino
		if (activeAnchor != NULL)
		{
			w.draw(*activeAnchor);
			w.draw(*activeAnchor->children[0]);
			w.draw(*activeAnchor->children[1]);
			w.draw(*activeAnchor->children[2]);
			/*
			activeAnchor->move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
			activeAnchor->children[0]->move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
			activeAnchor->children[1]->move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));
			activeAnchor->children[2]->move(sf::Vector2f(0 - posShift[0], 0 - posShift[1]));*/
		}

		// rysuj zawartosc Matrixa
		
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (matrix[i][j] != NULL)
				{
					w.draw(*matrix[i][j]);
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
		//move(sf::Vector2f(0 - matrix->posShift[0], 0 - matrix->posShift[1]));

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

		// ustawienie kolor�w i duszk�w min
		matrix->matrix[posX][posY]->colour = colour;
		matrix->matrix[posX + children[0]->posX][posY + children[0]->posY]->colour = colour;
		matrix->matrix[posX + children[1]->posX][posY + children[1]->posY]->colour = colour;
		matrix->matrix[posX + children[2]->posX][posY + children[2]->posY]->colour = colour;

		// usuni�cie min sterowalnych
		
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
		std::cout << "Nowy element nie zostal utworzony!\n" ;
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
	kolory i kszta�ty Mino:
		0 - Z - czerwony
		1 - L - pomara�czowy
		2 - O - ��ty
		3 - S - zielony
		4 - I - b��kitny
		5 - J - niebieski
		6 - T - fioletowy
		7 - �mieciomino (obra�enia) - szary
		8 - �mieciomino (limit czasu) - czarny
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

// P�tla g��wna

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

	// �adowanie tekstur
	
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
	if (!ttr_Preview.loadFromFile("sprites/spr_preview.png"))
	{
		// nyeh
	}
	ttr_Preview.setSmooth(true);

	sf::View view(sf::Vector2f(FULLRES_W/2.0, FULLRES_H/2.0f), sf::Vector2f(FULLRES_W, FULLRES_H)); // center, size

	pQueue pcQueue;
	addBag(&pcQueue);

	Player* player[2];

	player[0] = new Player(ttr_Matrix);

	player[0]->holdBox[0].setTexture(ttr_HoldSolo);
	player[0]->holdBox[0].setOrigin(sf::Vector2f((player[0]->holdBox[0].getTexture()->getSize().x) / 2.0, (player[0]->holdBox[0].getTexture()->getSize().y) / 2.0));
	player[0]->holdBox[0].setPosition(sf::Vector2f(player[0]->getPosition().x - ((player[0]->getTexture()->getSize().x) / 2.0) - ((player[0]->holdBox[0].getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));

	player[0]->qBox.setTexture(ttr_Queue5);
	player[0]->qBox.setOrigin(sf::Vector2f((player[0]->qBox.getTexture()->getSize().x) / 2.0, (player[0]->qBox.getTexture()->getSize().y) / 2.0));
	player[0]->qBox.setPosition(sf::Vector2f(player[0]->getPosition().x + ((player[0]->getTexture()->getSize().x) / 2.0) + ((player[0]->qBox.getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));

	for (int i = 0; i < 5; i++)
		player[0]->updateQueue(&pcQueue);

	player[1] = NULL;

	player[0]->genPiece(&pcQueue, ttr_Mino);
	if (player[0]->checkPos(0, -1))
		player[0]->activeAnchor->posY -= 1;

	// P�tla gry

	while (window.isOpen())
	{
		

		sf::Time elapsed1 = frameTimer.getElapsedTime();
		if (elapsed1.asMicroseconds() != 0)
			fpsCount = 1000000 / elapsed1.asMicroseconds();
		else
			fpsCount = INT_MAX;
		sf::String titleText = "Tetris PSk - "+ std::to_string(fpsCount) + " FPS";
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
				//player[0]->shift(true, false);
				player[0]->lockIn(ttr_Mino);
				//player[0]->shift(false, false);
			}
		}

		player[0]->shift(true, false);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			
		}

		// Rysowanie

		window.setView(view);

		window.clear();

		window.draw(*player[0]);
		window.draw(player[0]->holdBox[0]);
		window.draw(player[0]->qBox);
		player[0]->drawPreview(window);
		player[0]->drawMatrix(window);

		window.display();

		player[0]->shift(false, true);
	}

	return 0;
}