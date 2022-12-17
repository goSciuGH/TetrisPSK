/*
Wa¿ne komentarze:
	NOTE - notka, do przemyœlenia
	FIXIT - trzeba to naprawiæ, czemu??
	TODO - do zrobienia
*/

#include <SFML/Graphics.hpp>
#include <string>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <map>

// Definicja sta³ych wartoœci gry

#define DAS_DEFAULT 170	// DAS - Delayed Auto Shift - iloœæ czasu w ms, przez jak¹ trzeba trzymaæ przucisk, ¿eby tetrimino zaczê³o siê poruszaæ ze sta³¹ prêdkoœci¹
#define ARR_DEFAULT 50	// ARR - Auto Repeat Rate - iloœæ czasu w ms pomiêdzy kolejnymi krokami tetrimina w bok
#define SDF_DEFAULT 20	// SDF - Soft Drop Factor - obecnie iloœæ czasu w ms pomiêdzy kolejnymi krokami tetrimina podczas Soft Dropu

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

double windowScale = 1.0/2.0;

int windowW = FULLRES_W * windowScale;
int windowH = FULLRES_H * windowScale;

// Deklaracja obiektów i funkcji

struct Queue;
struct pQueue;

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

class Mino : public sf::Sprite
{
public:
	Mino()
	{

	}
	
	Mino* parent;
	int colour;
	int posX, posY;

	void rotate(bool cw)
	{
		if (cw) // ClockWise
		{
			int swap = this->posX;
			this->posX = this->posY;
			this->posY = 0 - swap;
		}
		else // Counter ClockWise
		{
			int swap = this->posX;
			this->posX = 0 - this->posY;
			this->posY = swap;
		}
	}
};

class MinoAnchor : public Mino
{
public: 
	MinoAnchor(Player* p, int c, sf::Texture &t)
	{
		this->matrix = p;

		this->posX = 4;
		this->posY = 20;

		this->children[0] = new Mino();
		this->children[1] = new Mino();
		this->children[2] = new Mino();

		switch (c)
		{
			case 0: // Z
			{
				this->children[0]->posX = 0-1;
				this->children[0]->posY = 1;
				this->children[1]->posX = 0;
				this->children[1]->posY = 1;
				this->children[2]->posX = 1;
				this->children[2]->posY = 0;
				break;
			}
			case 1: // L
			{
				this->children[0]->posX = 1;
				this->children[0]->posY = 1;
				this->children[1]->posX = 0-1;
				this->children[1]->posY = 0;
				this->children[2]->posX = 1;
				this->children[2]->posY = 0;
				break;
			}
			case 2: // O
			{
				this->children[0]->posX = 0;
				this->children[0]->posY = 1;
				this->children[1]->posX = 1;
				this->children[1]->posY = 1;
				this->children[2]->posX = 1;
				this->children[2]->posY = 0;
				break;
			}
			case 3: // S
			{
				this->children[0]->posX = 0;
				this->children[0]->posY = 1;
				this->children[1]->posX = 1;
				this->children[1]->posY = 1;
				this->children[2]->posX = 0-1;
				this->children[2]->posY = 0;
				break;
			}
			case 4: // I
			{
				this->children[0]->posX = 0-1;
				this->children[0]->posY = 0;
				this->children[1]->posX = 1;
				this->children[1]->posY = 0;
				this->children[2]->posX = 2;
				this->children[2]->posY = 0;
				break;
			}
			case 5: // J
			{
				this->children[0]->posX = 0-1;
				this->children[0]->posY = 1;
				this->children[1]->posX = 0-1;
				this->children[1]->posY = 0;
				this->children[2]->posX = 1;
				this->children[2]->posY = 0;
				break;
			}
			case 6: // T
			{
				this->children[0]->posX = 0;
				this->children[0]->posY = 1;
				this->children[1]->posX = 0-1;
				this->children[1]->posY = 0;
				this->children[2]->posX = 1;
				this->children[2]->posY = 0;
				break;
			}
		}
		
		this->setTexture(t);
		this->children[0]->setTexture(t);
		this->children[1]->setTexture(t);
		this->children[2]->setTexture(t);

		this->setTextureRect(sf::IntRect((t.getSize().x) / 3 * (c % 3), (t.getSize().y) / 3 * ((c - (c % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
		this->children[0]->setTextureRect(this->getTextureRect());
		this->children[1]->setTextureRect(this->getTextureRect());
		this->children[2]->setTextureRect(this->getTextureRect());
		
		this->updateScreenXY();
	}

	~MinoAnchor()
	{
		// stwórz nowy obiekt Mino w tablicy Matrix
		// przeka¿ nowemu Mino swoj¹ pozycjê i kolor;
		// zniszcz siebie
	}
	
	Player* matrix;
	int shape;
	int rot; // 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo
	Mino* children[3];

	void updateScreenXY();
};

class Player : public sf::Sprite
{
public:
	Player(sf::Texture &t)
	{
		id = 0;
		//this->matrix = new Matrix(this);

		this->ctrlKey = dftKey;
		this->ctrlState = 0x00;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				this->matrix[i][j] = NULL;
			}
		}

		this->setTexture(t);
		this->setOrigin(sf::Vector2f((t.getSize().x) / 2.0, (t.getSize().y) / 2.0));
		//this->setPosition(sf::Vector2f(FULLRES_W / 2.0, FULLRES_H / 2.0 + 2.0));
		this->setPosition(sf::Vector2f((FULLRES_W / (5.0 + (1.0 / 3.0))) + ((t.getSize().x) / 2.0), FULLRES_H / 2.0 + 2.0));

		this->level = 0;
		this->softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;
	}

	int id;
	int piecesUsed = 0;

	//Matrix* matrix;
	Mino* matrix[10][40];

	sf::Sprite hold[2];
	sf::Sprite qBox;

	int level;

	MinoAnchor* activeAnchor;

	std::map <std::string, sf::Keyboard::Key> ctrlKey;

	uint8_t ctrlState = 0x00;

	int sideMoveTime = 0;
	int softDropTime = 0;
	double softDropUpdate;
	int domDir = 0; // dominuj¹cy kierunek - -1 to lewo, 1 to prawo

	int pcQueue[5];

	void genPiece(pQueue* q, sf::Texture &t) // stwórz nowe tetrimino
	{
		this->activeAnchor = new MinoAnchor(this, pcQueue[0], t);

		this->updateQueue(q);
	}

	void updateQueue(pQueue* q)
	{
		for (int i = 0; i < 4; i++)
			this->pcQueue[i] = pcQueue[i + 1];
		this->pcQueue[4] = q->head->color;
		if ((!(q->head->id % 7)) && (q->head->next == NULL))
		{
			addBag(q);
		}
		dequeue(q);
	}

	void ctrlDo(int t)
	{
		//std::cout << t << "\t";
		//std::cout << activeAnchor->posX << "\t" << activeAnchor->posY << "\t";
		
		// Wykonaj funkcje przycisków

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["LMV"]) || sf::Keyboard::isKeyPressed(this->ctrlKey["RMV"]))	// jest wcisniety klawisz ruchu w bok
		{
			if (sf::Keyboard::isKeyPressed(this->ctrlKey["LMV"]) && sf::Keyboard::isKeyPressed(this->ctrlKey["RMV"])) // oba przyciski wciœniête
			{
				//std::cout << "LMV\tRMV\t";
				if ((this->ctrlState & LMV) && (this->ctrlState & RMV))	// poprzednio oba
				{
					this->sideMoveTime += t;
					//std::cout << " ";

					if (this->domDir == -1) // prawy guzik zosta³ dociœniêty do lewego
					{
						if (sideMoveTime >= DAS_DEFAULT)
						{
							sideMoveTime -= ARR_DEFAULT;
							if (checkPos(1, 0))
								activeAnchor->posX += 1;
						}
					}
					else // lewy guzik dociœniêty do prawego
					{
						if (sideMoveTime >= DAS_DEFAULT)
						{
							sideMoveTime -= ARR_DEFAULT;
							if (checkPos(-1, 0))
								activeAnchor->posX -= 1;
						}
					}
				}
				else if (this->ctrlState & LMV) // poprzednio lewo (teraz +prawo)
				{
					//std::cout << "+";
					sideMoveTime = 0;
				}
				else if (this->ctrlState & RMV) // poprzednio praw (teraz +lewo)
				{
					//std::cout << "-";
					sideMoveTime = 0;
				}
				else // poprzednio ¿aden
				{
					//std::cout << "_";
					sideMoveTime = 0;
				}
			}
			else if (sf::Keyboard::isKeyPressed(this->ctrlKey["LMV"])) // tylko lewo
			{
				//std::cout << "LMV\t\t";
				this->sideMoveTime += t;

				if ((this->ctrlState & (LMV | RMV)) || (this->ctrlState & RMV)) // poprzednio oba lub tylko prawo
				{
					this->domDir = -1;
				}
				else if (this->ctrlState & LMV) // poprzednio tylko lewo
				{
					// nic siê nie dzieje
				}
				else // poprzednio ¿aden
				{
					this->domDir = -1;
					if (checkPos(-1, 0))
						activeAnchor->posX -= 1;
				}

				if (sideMoveTime >= DAS_DEFAULT)
				{
					sideMoveTime -= ARR_DEFAULT;
					if (checkPos(-1, 0))
						activeAnchor->posX -= 1;
				}
			}
			else if (sf::Keyboard::isKeyPressed(this->ctrlKey["RMV"])) // tylko prawo
			{
				//std::cout << "\tRMV\t";
				this->sideMoveTime += t;

				if ((this->ctrlState & (LMV | RMV)) || (this->ctrlState & LMV)) // poprzednio oba lub tylko lewo
				{
					this->domDir = 1;
				}
				else if (this->ctrlState & LMV) // poprzednio tylko prawo
				{
					// nic siê nie dzieje
				}
				else // poprzednio ¿aden
				{
					this->domDir = 1;
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
			//std::cout << "\t\t";
			this->sideMoveTime = 0;
			this->domDir = 0;
		}

		//std::cout << sideMoveTime << "\t";

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["LSP"]) && sf::Keyboard::isKeyPressed(this->ctrlKey["RSP"])) // oba przyciski wciœniête
		{
			//std::cout << "LSP\tRSP\t";
			// nie dzieje siê nic
		}
		else if (sf::Keyboard::isKeyPressed(this->ctrlKey["LSP"])) // lewy obrót
		{
			//std::cout << "LSP\t\t";
			if (!(this->ctrlState & LSP)) // poprzednio nie by³ trzymany lewy
			{
				// wykonaj obrót w lewo
			}
		}
		else if (sf::Keyboard::isKeyPressed(this->ctrlKey["RSP"])) // prawy obrót
		{
			//std::cout << "\tRSP\t";
			if (!(this->ctrlState & RSP)) // poprzednio nie by³ trzymany prawy
			{
				// wykonaj obrót w prawo
			}
		}
		else
		{
			//std::cout << "\t\t";
		}

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["HDR"]))
		{
			//std::cout << "HDR\t";
			if (!(this->ctrlState & HDR))
			{
				// pêtla: dopóki jest wolne miejsce pod spodem, opuszczaj tetrimino o 1
			}
		}
		//else std::cout << "\t";

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["SDR"]))
		{
			//std::cout << "SDR\t";
			if (!(this->ctrlState & SDR))
			{
				// jeœli nie ma nic pod spodem, opuœæ tetrimino o 1
			}
		}
		//else std::cout << "\t";

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["HLD"]))
		{
			//std::cout << "HLD\t";
			if (!(this->ctrlState & HLD))
			{
				// jeœli schowek aktywny, schowaj tetrimino
			}
		}
		//else std::cout << "\t";

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["PSE"]))
		{
			//std::cout << "PSE\t";
			if (!(this->ctrlState & PSE))
			{
				// zapauzuj grê
			}
		}

		this->activeAnchor->updateScreenXY();

		// Zaktualizuj grawitacjê

		softDropTime += t;

		if (softDropTime >= softDropUpdate)
		{
			softDropTime -= softDropUpdate;
			if (checkPos(0, -1))
			{
				activeAnchor->posY -= 1;
			}
		}
		//std::cout << softDropTime << "/" << softDropUpdate;

		// Zapisz obecne wciœniêcia do porównania w nastêpnej klatce

		this->ctrlState = 0x00;

		if (sf::Keyboard::isKeyPressed(this->ctrlKey["LMV"]))	// ruch w lewo
		{
			this->ctrlState |= 0x01;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["RMV"]))	// ruch w prawo
		{
			this->ctrlState |= 0x02;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["LSP"]))	// obrot w lewo
		{
			this->ctrlState |= 0x04;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["RSP"]))	// obrot w prawo
		{
			this->ctrlState |= 0x08;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["SDR"]))	// softdrop
		{
			this->ctrlState |= 0x10;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["HDR"]))	// harddrop
		{
			this->ctrlState |= 0x20;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["HLD"]))	// hold
		{
			this->ctrlState |= 0x40;
		}
		if (sf::Keyboard::isKeyPressed(this->ctrlKey["PSE"]))	// pauza
		{
			this->ctrlState |= 0x80;
		}

		//std::cout << "\n";
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
		this->softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;
	}

	void drawPreview(sf::RenderWindow &w, sf::Texture t)
	{
		sf::Sprite spr_Preview;
		spr_Preview.setTexture(t);

		spr_Preview.setTextureRect(sf::IntRect((t.getSize().x) / 3 * (pcQueue[0] % 3), (t.getSize().y) / 3 * ((pcQueue[0] - (pcQueue[0] % 3)) / 3), (t.getSize().x) / 3, (t.getSize().y) / 3));
		spr_Preview.setOrigin(sf::Vector2f((t.getSize().x) / 6.0, (t.getSize().y) / 6.0));
		spr_Preview.setPosition(sf::Vector2f(qBox.getPosition().x - 2.0, qBox.getPosition().y - 278.0));

		w.draw(spr_Preview);

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
			for (int j = 0; j < 20; j++)
			{
				if (matrix[i][j] != NULL)
					w.draw(*matrix[i][j]);
			}
		}
		
	}
};

// Definicja metod

void MinoAnchor::updateScreenXY()
{
	this->setPosition(sf::Vector2f(matrix->getPosition().x - (this->getTexture()->getSize().x) / 3.0 - 128.0 + (this->posX * 32.0), matrix->getPosition().y + 286.0 - (this->posY * 32.0)));
	this->children[0]->setPosition(sf::Vector2f(this->getPosition().x + (this->children[0]->posX * ((this->children[0]->getTexture()->getSize().x) / 3.0)), this->getPosition().y - (this->children[0]->posY * ((this->children[0]->getTexture()->getSize().y) / 3.0))));
	this->children[1]->setPosition(sf::Vector2f(this->getPosition().x + (this->children[1]->posX * ((this->children[1]->getTexture()->getSize().x) / 3.0)), this->getPosition().y - (this->children[1]->posY * ((this->children[1]->getTexture()->getSize().y) / 3.0))));
	this->children[2]->setPosition(sf::Vector2f(this->getPosition().x + (this->children[2]->posX * ((this->children[2]->getTexture()->getSize().x) / 3.0)), this->getPosition().y - (this->children[2]->posY * ((this->children[2]->getTexture()->getSize().y) / 3.0))));
}

// Definicja funkcji

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
	float frameHistory[1920];
	float frameAvgH[1920];
	float maxFps = 0;
	float minFps = INT_MAX;
	
	srand(time(NULL));

	dftKey["LMV"] = sf::Keyboard::Left;		// Left MoVement
	dftKey["RMV"] = sf::Keyboard::Right;	// Right MoVement
	dftKey["LSP"] = sf::Keyboard::Z;		// Left SPin
	dftKey["RSP"] = sf::Keyboard::X;		// Right SPin
	dftKey["SDR"] = sf::Keyboard::Down;		// Soft DRop
	dftKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
	dftKey["HLD"] = sf::Keyboard::C;		// HoLD
	dftKey["PSE"] = sf::Keyboard::Escape;	// PauSE
	
	//std::cout << "xd\n";

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
	
	//

	sf::View view(sf::Vector2f(FULLRES_W/2.0, FULLRES_H/2.0f), sf::Vector2f(FULLRES_W, FULLRES_H)); // center, size

	//

	pQueue pcQueue;
	addBag(&pcQueue);

	Player* player[2];

	player[0] = new Player(ttr_Matrix);
	/*
	player[0]->matrix->setTexture(ttr_Matrix);
	player[0]->matrix->setOrigin(sf::Vector2f((player[0]->matrix->getTexture()->getSize().x) / 2.0, (player[0]->matrix->getTexture()->getSize().y) / 2.0));
	//player[0]->matrix->setPosition(sf::Vector2f(FULLRES_W / 2.0, FULLRES_H / 2.0 + 2.0));
	player[0]->matrix->setPosition(sf::Vector2f((FULLRES_W / (5.0 + (1.0 / 3.0))) + ((player[0]->matrix->getTexture()->getSize().x) / 2.0), FULLRES_H / 2.0 + 2.0));
	*/
	player[0]->hold[0].setTexture(ttr_HoldSolo);
	player[0]->hold[0].setOrigin(sf::Vector2f((player[0]->hold[0].getTexture()->getSize().x) / 2.0, (player[0]->hold[0].getTexture()->getSize().y) / 2.0));
	player[0]->hold[0].setPosition(sf::Vector2f(player[0]->getPosition().x - ((player[0]->getTexture()->getSize().x) / 2.0) - ((player[0]->hold[0].getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));
	
	player[0]->qBox.setTexture(ttr_Queue5);
	player[0]->qBox.setOrigin(sf::Vector2f((player[0]->qBox.getTexture()->getSize().x) / 2.0, (player[0]->qBox.getTexture()->getSize().y) / 2.0));
	player[0]->qBox.setPosition(sf::Vector2f(player[0]->getPosition().x + ((player[0]->getTexture()->getSize().x) / 2.0) + ((player[0]->qBox.getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));

	for (int i = 0; i < 5; i++)
		player[0]->updateQueue(&pcQueue);

	player[1] = NULL;

	// TODO - wydrukuj kolejkê

	player[0]->genPiece(&pcQueue, ttr_Mino);

	// Pêtla gry

	while (window.isOpen())
	{
		sf::Time elapsed1 = frameTimer.getElapsedTime();
		//std::cout << elapsed1.asMicroseconds() << "\t";
		if (elapsed1.asMicroseconds() != 0)
			fpsCount = 1000000 / elapsed1.asMicroseconds();
		else
			fpsCount = INT_MAX;
		sf::String titleText = "Tetris PSk - "+ std::to_string(fpsCount) + " FPS";
		window.setTitle(titleText);
		frameTimer.restart();
		/*for (int i = 0; i < 1919; i++)
		{
			frameAvgH[i] = frameAvgH[i + 1];
		}
		frameAvgH[1919] = 0.0;
		for (int i = 0; i < 1919; i++)
		{
			frameAvgH[1919] += (frameHistory[i] / 1920.0);
			frameHistory[i] = frameHistory[i + 1];
		}
		frameAvgH[1919] += (frameHistory[1919]/1920.0);
		frameHistory[1919] = fpsCount;
		
		if (fpsCount > maxFps)
		{
			maxFps = fpsCount;
		}
		if (fpsCount < minFps)
		{
			minFps = fpsCount;
		}*/

		// Kontrola

		player[0]->ctrlDo(elapsed1.asMilliseconds());

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			
		}

		// Rysowanie

		window.setView(view);

		window.clear();
		/*
		for (int i = 0; i < 1919; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(i, frameHistory[i] / maxFps * FULLRES_H), sf::Color(255, 0, 0, 64)),
				sf::Vertex(sf::Vector2f(i + 1, frameHistory[i + 1] / maxFps * FULLRES_H), sf::Color(255, 0, 0, 64))
			};

			window.draw(line, 2, sf::Lines);
		}
		for (int i = 0; i < 1919; i++)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(i, frameAvgH[i] / maxFps * FULLRES_H), sf::Color(255, 255, 255, 64)),
				sf::Vertex(sf::Vector2f(i + 1, frameAvgH[i + 1] / maxFps * FULLRES_H), sf::Color(255, 255, 255, 64))
			};

			window.draw(line, 2, sf::Lines);
		}
		std::cout << minFps << "\t" << frameAvgH[1919] << "\t" << maxFps << "\n";
		*/
		window.draw(*player[0]);
		window.draw(player[0]->hold[0]);
		window.draw(player[0]->qBox);
		player[0]->drawPreview(window, ttr_Preview);
		player[0]->drawMatrix(window);

		

		window.display();
		
	}

	return 0;
}