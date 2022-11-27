#include <SFML/Graphics.hpp>
#include <string>
#include <time.h>
#include <cstdlib>
#include <iostream>

#define DAS_DEFAULT 170
#define ARR_DEFAULT 50
#define SDF_DEFAULT 30

// Definicja grafiki

#define FULLHD_W 1920
#define FULLHD_H 1080

bool fullScreen = false;

double windowScale = 1.0;

int windowW = FULLHD_W * windowScale;
int windowH = FULLHD_H * windowScale;

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
	struct Queue* head;
	struct Queue* tail;
};



class Mino : public sf::Sprite
{
public:
	Mino()
	{

	}
	
	int colour;
	int posX, posY;
};

class MinoAnchor : public Mino
{
public: 
	MinoAnchor()
	{

	}

	~MinoAnchor()
	{
		// stwórz nowy obiekt Mino w tablicy Matrix
		// przeka¿ nowemu Mino swoj¹ pozycjê i kolor;
		// zniszcz siebie
	}
	
	int shape;
	int rot; // 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo
	Mino* children[3];
};

class Matrix : public sf::Sprite
{
public:
	Matrix(Player* p)
	{
		this->parent = p;
		
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				this->matrix[i][j] = NULL;
			}
		}
	}

	Player* parent;
	Mino* matrix[10][40];
};

class Player
{
public:
	Player()
	{
		matrix = new Matrix(this);
	}

	int id;
	int piecesUsed = 0;

	Matrix* matrix;
	sf::Sprite hold[2];
	sf::Sprite qBox;

	MinoAnchor* activeAnchor;

	int sideMoveTime = 0;
	int softDropTime = 0;

	int pcQueue[5];

	void genPiece() // stwórz nowe tetrimino
	{
		activeAnchor = new MinoAnchor();
		activeAnchor->children[0] = new Mino();
		activeAnchor->children[1] = new Mino();
		activeAnchor->children[2] = new Mino();
	}

	void updateQueue(pQueue* q)
	{
		for (int i = 0; i < 4; i++)
			pcQueue[i] = pcQueue[i + 1];
		pcQueue[4] = q->head->color;
		if ((!(q->head->id % 7)) && (q->head->next == NULL))
		{
			// dodaj nowe 7 tetrimin do kolejki ogólnej
		}
		dequeue(q);
	}
};

// Definicja funkcji

void enqueue(struct pQueue* q, int d)
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
			q->tail->next = newNode;
			newNode->id = q->tail->id + 1;
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
	//std::cout << "xd\n";

	sf::Clock frameTimer;
	int fpsCount;

	sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Tetris PSk", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

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

	//

	sf::View view(sf::Vector2f(FULLHD_W/2.0, FULLHD_H/2.0f), sf::Vector2f(FULLHD_W, FULLHD_H)); // center, size

	//

	Player* player[2];

	player[0] = new Player();
	player[0]->matrix->setTexture(ttr_Matrix);
	player[0]->matrix->setOrigin(sf::Vector2f((player[0]->matrix->getTexture()->getSize().x) / 2.0, (player[0]->matrix->getTexture()->getSize().y) / 2.0));
	//player[0]->matrix->setPosition(sf::Vector2f(FULLHD_W / 2.0, FULLHD_H / 2.0 + 2.0));
	player[0]->matrix->setPosition(sf::Vector2f((FULLHD_W / (5.0 + (1.0 / 3.0))) + ((player[0]->matrix->getTexture()->getSize().x) / 2.0), FULLHD_H / 2.0 + 2.0));
	player[0]->hold[0].setTexture(ttr_HoldSolo);
	player[0]->hold[0].setOrigin(sf::Vector2f((player[0]->hold[0].getTexture()->getSize().x) / 2.0, (player[0]->hold[0].getTexture()->getSize().y) / 2.0));
	player[0]->hold[0].setPosition(sf::Vector2f(player[0]->matrix->getPosition().x - ((player[0]->matrix->getTexture()->getSize().x) / 2.0) - ((player[0]->hold[0].getTexture()->getSize().x) / 2.0), player[0]->matrix->getPosition().y));
	player[0]->qBox.setTexture(ttr_Queue5);
	player[0]->qBox.setOrigin(sf::Vector2f((player[0]->qBox.getTexture()->getSize().x) / 2.0, (player[0]->qBox.getTexture()->getSize().y) / 2.0));
	player[0]->qBox.setPosition(sf::Vector2f(player[0]->matrix->getPosition().x + ((player[0]->matrix->getTexture()->getSize().x) / 2.0) + ((player[0]->qBox.getTexture()->getSize().x) / 2.0), player[0]->matrix->getPosition().y));

	/*
	sf::Sprite holdTest;
	holdTest.setTexture(ttr_HoldSolo);
	holdTest.setOrigin(sf::Vector2f((holdTest.getTexture()->getSize().x) / 2.0, (holdTest.getTexture()->getSize().y) / 2.0));
	holdTest.setPosition(sf::Vector2f(FULLHD_W / 2.0, FULLHD_H / 2.0));
	*/
	player[1] = NULL;

	/*
	sf::Sprite minoTest;
	minoTest.setTexture(ttr_Mino);
	minoTest.setTextureRect(sf::IntRect(0, 0, 32, 32));
	minoTest.setOrigin(16.0f, 16.0f);
	minoTest.setPosition(sf::Vector2f(FULLHD_W / 2.0 - 512.0, FULLHD_H / 2.0 - 16.0));
	*/

	// Pêtla gry

	srand(time(NULL));

	while (window.isOpen())
	{
		sf::Time elapsed1 = frameTimer.getElapsedTime();
		//std::cout << elapsed1.asMilliseconds() << std::endl;
		if (elapsed1.asMilliseconds() != 0)
			fpsCount = 1000 / elapsed1.asMilliseconds();
		else
			fpsCount = INT_MAX;
		sf::String titleText = "Tetris PSk - "+ std::to_string(fpsCount) + " FPS";
		window.setTitle(titleText);
		frameTimer.restart();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.setView(view);

		window.clear();

		window.draw(*player[0]->matrix);
		window.draw(player[0]->hold[0]);
		window.draw(player[0]->qBox);

		int randMino = rand() % 7;

		//minoTest.setTextureRect(sf::IntRect((randMino%3)*32, (randMino-randMino%3)/3*32, 32, 32));
		//window.draw(minoTest);

		window.display();
	}

	return 0;
}