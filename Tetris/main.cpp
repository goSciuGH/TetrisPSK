#include <SFML/Graphics.hpp>
#include <string.h>

#define FULLHD_W 1920
#define FULLHD_H 1080

#define DAS_DEFAULT 170
#define ARR_DEFAULT 50
#define SDF_DEFAULT 30

// Definicja grafiki

bool fullScreen = false;

double windowScale = 1.0;

int windowW = FULLHD_W * windowScale;
int windowH = FULLHD_H * windowScale;

// Definicja obiekt�w

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

class Mino : public sf::Sprite
{
public:
	int colour;
	sf::Vector2u pos;
};

class MinoAnchor : public Mino
{
public: 
	int shape;
	int rot; // 0 - pozycja startowa; 1 - obr�t w prawo; 2 - dwa obroty; 3 - obr�t w lewo;
	Mino children[3];

	void lockIn()
	{
		// stw�rz nowy obiekt Mino
		// przeka� nowemu Mino swoj� pozycj� i kolor;
		// zniszcz siebie
	}
};

class Matrix
{
public:

	Mino matrix[10][40];
	
	void create()
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				// spraw, �eby ka�de pole by�o puste I guess
			}
		}
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Tetris PSk", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

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
	/*
	sf::Sprite mtrx;
	mtrx.setTexture(ttr_Matrix);
	*/
	sf::View view(sf::Vector2f(FULLHD_W/2.0, FULLHD_H/2.0f), sf::Vector2f(FULLHD_W, FULLHD_H)); // center, size

	// P�tla gry

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.setView(view);

		window.draw(mtrx);

		window.display();
	}

	return 0;
}