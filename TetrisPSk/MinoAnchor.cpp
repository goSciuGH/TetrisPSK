//#include <SFML/Graphics.hpp>

#include "Mino.h"
#include "MinoAnchor.h"
#include "Player.h"

MinoAnchor::MinoAnchor(Player* p, int c, sf::Texture &t)
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