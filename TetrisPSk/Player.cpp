#include "Player.h"

#include "enums.h"
#include "pQueue.h"
#include "Queue.h"
#include "MinoAnchor.h"
#include "Applause.h"

#include <SFML/Graphics.hpp>

extern int hiScore;
extern int offsetChart[3][4][5][2]; // Tabela obrótów - [zestaw][kierunek][nr testu][x lub y]
extern std::map <std::string, sf::Keyboard::Key> dftKey;
extern int FULLRES_W;
extern int FULLRES_H;
extern int DAS_DEFAULT;
extern int ARR_DEFAULT;
extern int SDF_DEFAULT;
extern int LOCK_TIME;
extern int LOCK_RESET;

Player::Player(sf::Texture &t, sf::Font &f, sf::Texture &t2)
{
	id = 0;

	ctrlKey = dftKey;
	for (int i = 0; i < 8; i++)
	{
		ctrlState[i] = false;
	}

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

	active = false;
	gameOver = false;

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
	txt_stats[0].setLetterSpacing(0.5);
	bounds = txt_stats[0].getLocalBounds();
	txt_stats[0].setOrigin(sf::Vector2f(bounds.width, bounds.height));
	txt_stats[0].setPosition(sf::Vector2f(1632.0, 280.0));

	txt_stats[1].setString("");
	len = std::to_string(hiScore).length();
	for (int i = 0; i < (8 - len); i++)
		txt_stats[1].setString(txt_stats[1].getString() + "0");
	txt_stats[1].setString(txt_stats[1].getString() + std::to_string(hiScore));
	txt_stats[1].setLetterSpacing(0.5);
	bounds = txt_stats[1].getLocalBounds();
	txt_stats[1].setOrigin(sf::Vector2f(bounds.width, bounds.height));
	txt_stats[1].setPosition(sf::Vector2f(1632.0, 410.0));

	txt_stats[2].setString("");
	txt_stats[2].setLetterSpacing(0.5);
	len = std::to_string(score).length();
	for (int i = 0; i < (8 - len); i++)
		txt_stats[2].setString(txt_stats[2].getString() + "0");
	txt_stats[2].setString(txt_stats[2].getString() + std::to_string(score));
	bounds = txt_stats[2].getLocalBounds();
	txt_stats[2].setOrigin(sf::Vector2f(bounds.width, bounds.height));
	txt_stats[2].setPosition(sf::Vector2f(1632.0, 540.0));

	txt_stats[3].setString("");
	txt_stats[3].setLetterSpacing(0.5);
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
	txt_stats[4].setLetterSpacing(0.5);
	bounds = txt_stats[4].getLocalBounds();
	txt_stats[4].setOrigin(sf::Vector2f(bounds.width, bounds.height));
	txt_stats[4].setPosition(sf::Vector2f(1382.0, 800.0));

	txt_stats[5].setString(std::to_string(level + 1));
	txt_stats[5].setLetterSpacing(0.5);
	bounds = txt_stats[5].getLocalBounds();
	txt_stats[5].setOrigin(sf::Vector2f(bounds.width, bounds.height));
	txt_stats[5].setPosition(sf::Vector2f(1632.0, 800.0));

	for (int i = 0; i < 7; i++)
	{
		keyDebug[i].setTexture(t2);
		keyDebug[i].setPosition(sf::Vector2f(32.0 + (i % 3) * 36.0, 1080.0 - 136.0 + ((i - (i % 3)) / 3.0) * 36.0));
		keyDebug[i].setColor(sf::Color(85, 85, 85, 85));
	}

	for (int i = 0; i < 6; i++)
		appl[i] = new Applause();
}

void Player::genPiece(pQueue* q, sf::Texture &t) // stwórz nowe tetrimino
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

void Player::updateQueue(pQueue* q)
{
	for (int i = 0; i < 4; i++)
		pcQueue[i] = pcQueue[i + 1];
	pcQueue[4] = q->head->color;
	if ((!(q->head->id % 7)) && (q->head->next == NULL))
	{
		q->addBag();
	}

	q->dequeue();
}

void Player::rotate(bool cw)
{
	int pom;
	if (cw == true)
		pom = 1;
	else
		pom = 0;

	bool rotSuccess = false;

	activeAnchor->children[0]->rotate(pom);
	activeAnchor->children[1]->rotate(pom);
	activeAnchor->children[2]->rotate(pom);

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
			tsMiniEligible = true;
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
		activeAnchor->children[0]->rotate(1 - pom);
		activeAnchor->children[1]->rotate(1 - pom);
		activeAnchor->children[2]->rotate(1 - pom);
	}
}

void Player::ctrlDo(int t, sf::Texture &ttr, sf::Sound s[], sf::SoundBuffer* sb[])
{
	bool lockReset = false;

	// Wykonaj funkcje przycisków
	if (sf::Keyboard::isKeyPressed(ctrlKey["HLD"]))
	{
		if (!ctrlState[6])
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
					for (int i = 0; i < 8; i++)
					{
						ctrlState[i] = false;
					}
					ctrlState[5] = true;
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
					for (int i = 0; i < 8; i++)
					{
						ctrlState[i] = false;
					}
					ctrlState[5] = true;
					genPiece(NULL, ttr);
					if (checkPos(0, -1))
						activeAnchor->posY -= 1;

					hold = swap;
				}

				holdActive = false;
				s[2].setBuffer(*sb[4]);
				s[2].play();
				return;
			}
		}

		ctrlState[6] = true;
	}
	else
		ctrlState[6] = false;

	if (sf::Keyboard::isKeyPressed(ctrlKey["HDR"]))
	{
		if (!ctrlState[5])
		{
			int hdrDrop = 0;
			while (checkPos(0, -1))
			{
				activeAnchor->posY -= 1;
				hdrDrop++;
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

			if (hdrDrop > 0)
				tsMiniEligible = false;

			lockdownReset = INT_MAX - LOCK_RESET;
			s[3].setBuffer(*sb[3]);
			s[3].play();
			activeAnchor->updateScreenXY();
			return;
		}

		ctrlState[5] = true;
	}
	else
		ctrlState[5] = false;

	if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]) || sf::Keyboard::isKeyPressed(ctrlKey["RMV"]))	// jest wcisniety klawisz ruchu w bok
	{
		if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"]) && sf::Keyboard::isKeyPressed(ctrlKey["RMV"])) // oba przyciski wciœniête
		{
			if (ctrlState[0] && ctrlState[1])	// poprzednio oba
			{
				sideMoveTime += t;

				if (domDir == -1) // prawy guzik zosta³ dociœniêty do lewego
				{
					if (sideMoveTime >= DAS_DEFAULT)
					{
						sideMoveTime -= ARR_DEFAULT;
						if (checkPos(1, 0))
						{
							tsMiniEligible = false;
							activeAnchor->posX += 1;
							if (lockdownTime != -1) //resetowanie lock-ina
							{
								softDropTime = 0;
								lockdownReset++;
								lockdownTime = -1;
							}
							s[0].setBuffer(*sb[8]);
							s[0].play();
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
							tsMiniEligible = false;
							activeAnchor->posX -= 1;
							if (lockdownTime != -1) //resetowanie lock-ina
							{
								softDropTime = 0;
								lockdownReset++;
								lockdownTime = -1;
							}
							s[0].setBuffer(*sb[8]);
							s[0].play();
						}
					}
				}
			}
			else if (ctrlState[0]) // poprzednio lewo (teraz +prawo)
			{
				sideMoveTime = 0;
				if (checkPos(1, 0))
				{
					tsMiniEligible = false;
					activeAnchor->posX += 1;
					if (lockdownTime != -1) //resetowanie lock-ina
					{
						softDropTime = 0;
						lockdownReset++;
						lockdownTime = -1;
					}
					s[0].setBuffer(*sb[8]);
					s[0].play();
				}
			}
			else if (ctrlState[1]) // poprzednio praw (teraz +lewo)
			{
				sideMoveTime = 0;
				if (checkPos(-1, 0))
				{
					tsMiniEligible = false;
					activeAnchor->posX -= 1;
					if (lockdownTime != -1) //resetowanie lock-ina
					{
						softDropTime = 0;
						lockdownReset++;
						lockdownTime = -1;
					}
					s[0].setBuffer(*sb[8]);
					s[0].play();
				}
			}
			else // poprzednio ¿aden
			{
				sideMoveTime = 0;
			}

			ctrlState[0] = true;
			ctrlState[1] = true;
		}
		else if (sf::Keyboard::isKeyPressed(ctrlKey["LMV"])) // tylko lewo
		{
			sideMoveTime += t;

			if ((ctrlState[0] && ctrlState[1]) || ctrlState[1]) // poprzednio oba lub tylko prawo
			{
				domDir = -1;
			}
			else if (ctrlState[0])
			{
				// nic siê nie dzieje
			}
			else // poprzednio ¿aden
			{
				domDir = -1;
				if (checkPos(-1, 0))
				{
					tsMiniEligible = false;
					activeAnchor->posX -= 1;
					if (lockdownTime != -1) //resetowanie lock-ina
					{
						softDropTime = 0;
						lockdownReset++;
						lockdownTime = -1;
					}
					s[0].setBuffer(*sb[8]);
					s[0].play();
				}
			}

			if (sideMoveTime >= DAS_DEFAULT)
			{
				sideMoveTime -= ARR_DEFAULT;
				if (checkPos(-1, 0))
				{
					tsMiniEligible = false;
					activeAnchor->posX -= 1;
					if (lockdownTime != -1) //resetowanie lock-ina
					{
						softDropTime = 0;
						lockdownReset++;
						lockdownTime = -1;
					}
					s[0].setBuffer(*sb[8]);
					s[0].play();
				}
			}

			ctrlState[0] = true;
			ctrlState[1] = false;
		}
		else if (sf::Keyboard::isKeyPressed(ctrlKey["RMV"])) // tylko prawo
		{
			sideMoveTime += t;

			if ((ctrlState[0] && ctrlState[1]) || ctrlState[0]) // poprzednio oba lub tylko lewo
			{
				domDir = 1;
			}
			else if (ctrlState[1]) // poprzednio tylko prawo
			{
				// nic siê nie dzieje
			}
			else // poprzednio ¿aden
			{
				domDir = 1;
				if (checkPos(1, 0))
				{
					tsMiniEligible = false;
					activeAnchor->posX += 1;
					if (lockdownTime != -1) //resetowanie lock-ina
					{
						softDropTime = 0;
						lockdownReset++;
						lockdownTime = -1;
					}
					s[0].setBuffer(*sb[8]);
					s[0].play();
				}
			}

			if (sideMoveTime >= DAS_DEFAULT)
			{
				sideMoveTime -= ARR_DEFAULT;
				if (checkPos(1, 0))
				{
					tsMiniEligible = false;
					activeAnchor->posX += 1;
					if (lockdownTime != -1) //resetowanie lock-ina
					{
						softDropTime = 0;
						lockdownReset++;
						lockdownTime = -1;
					}
					s[0].setBuffer(*sb[8]);
					s[0].play();
				}
			}

			ctrlState[0] = false;
			ctrlState[1] = true;
		}
	}
	else // ¿aden klawisz ruchu w bok nie jest wciœniêty
	{
		sideMoveTime = 0;
		domDir = 0;

		ctrlState[0] = false;
		ctrlState[1] = false;
	}

	if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"]) && sf::Keyboard::isKeyPressed(ctrlKey["RSP"])) // oba przyciski obrotu wciœniête - FIXIT
	{
		if (ctrlState[2] && ctrlState[3]) // poprzednio oba
		{
			// nie dzieje sie nic
		}
		else if (ctrlState[2])	// poprzednio tylko lewo (teraz +prawo)
		{
			rotate(true);
			if (activeAnchor->colour == 6)
			{
				if (checkTSpin() >= 0)
				{
					s[1].setBuffer(*sb[10]);
					s[1].play();
				}
				else
				{
					s[1].setBuffer(*sb[9]);
					s[1].play();
				}
			}
			else
			{
				s[1].setBuffer(*sb[9]);
				s[1].play();
			}
		}
		else if (ctrlState[3])	// poprzednio tylko prawo (teraz +lewo)
		{
			rotate(false);
			if (activeAnchor->colour == 6)
			{
				if (checkTSpin() >= 0)
				{
					s[1].setBuffer(*sb[10]);
					s[1].play();
				}
				else
				{
					s[1].setBuffer(*sb[9]);
					s[1].play();
				}
			}
			else
			{
				s[1].setBuffer(*sb[9]);
				s[1].play();
			}
		}

		ctrlState[2] = true;
		ctrlState[3] = true;
	}
	else if (sf::Keyboard::isKeyPressed(ctrlKey["LSP"])) // lewy obrót
	{
		if (!ctrlState[2]) // poprzednio nie by³ trzymany lewy
		{
			rotate(false);
			if (activeAnchor->colour == 6)
			{
				if (checkTSpin() >= 0)
				{
					s[1].setBuffer(*sb[10]);
					s[1].play();
				}
				else
				{
					s[1].setBuffer(*sb[9]);
					s[1].play();
				}
			}
			else
			{
				s[1].setBuffer(*sb[9]);
				s[1].play();
			}
		}

		ctrlState[2] = true;
		ctrlState[3] = false;
	}
	else if (sf::Keyboard::isKeyPressed(ctrlKey["RSP"])) // prawy obrót
	{
		if (!ctrlState[3]) // poprzednio nie by³ trzymany prawy
		{
			rotate(true);
			if (activeAnchor->colour == 6)
			{
				if (checkTSpin() >= 0)
				{
					s[1].setBuffer(*sb[10]);
					s[1].play();
				}
				else
				{
					s[1].setBuffer(*sb[9]);
					s[1].play();
				}
			}
			else
			{
				s[1].setBuffer(*sb[9]);
				s[1].play();
			}
		}

		ctrlState[2] = false;
		ctrlState[3] = true;
	}
	else // ¿aden
	{
		ctrlState[2] = false;
		ctrlState[3] = false;
	}

	//if (sf::Keyboard::isKeyPressed(ctrlKey["PSE"]))
	//{
	//	if (!ctrlState[7])
	//	{
	//		// TODO - zapauzuj grê
	//	}

	//	ctrlState[7] = true;
	//}
	//else
	//	ctrlState[7] = false;

	// Zaktualizuj grawitacjê

	if (sf::Keyboard::isKeyPressed(ctrlKey["SDR"]))
	{
		softDropTime += (SDF_DEFAULT * t);
		ctrlState[4] = true;
	}
	else
	{
		softDropTime += t;
		ctrlState[4] = false;
	}

	if (softDropTime >= softDropUpdate)
	{
		if (checkPos(0, -1))
		{
			tsMiniEligible = false;
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

			if (softDropTime >= softDropUpdate)
			{
				ctrlDo(0, ttr, s, sb);
				return;
			}

			s[3].setBuffer(*sb[2]);
			s[3].play();
		}
		else
		{
			softDropTime = softDropUpdate - 1;
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
}

void Player::checkVisuals(sf::Texture &t)
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

int Player::checkTSpin()
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
			{
				return 1;
			}
		}
		else if (tY == 0) // check for T-Spin Mini with wall
		{
			if ((matrix[tX - 1][tY + 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
			{
				return 0;
			}
		}
		else if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX + 1][tY - 1] != NULL)) // check for T-Spin Mini with minos
		{
			if ((matrix[tX - 1][tY + 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
			{
				return 0;
			}
		}
		break;
	}
	case 1:
	{
		if ((matrix[tX + 1][tY - 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL))	// check for T-Spin
		{
			if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX - 1][tY + 1] != NULL))
			{
				return 1;
			}
		}
		else if (tX == 0) // check for T-Spin Mini with wall
		{
			if ((matrix[tX + 1][tY - 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
			{
				return 0;
			}
		}
		else if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX - 1][tY + 1] != NULL)) // check for T-Spin Mini with minos
		{
			if ((matrix[tX + 1][tY - 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
			{
				return 0;
			}
		}
		break;
	}
	case 2:
	{
		if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX + 1][tY - 1] != NULL))	// check for T-Spin
		{
			if ((matrix[tX - 1][tY + 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
			{
				return 1;
			}
		}
		else if (tY == 39) // check for T-Spin Mini with wall
		{
			if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX + 1][tY - 1] != NULL))
			{
				return 0;
			}
		}
		else if ((matrix[tX - 1][tY + 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL)) // check for T-Spin Mini with minos
		{
			if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX + 1][tY - 1] != NULL))
			{
				return 0;
			}
		}
		break;
	}
	case 3:
	{
		if ((matrix[tX - 1][tY - 1] != NULL) && (matrix[tX - 1][tY + 1] != NULL))	// check for T-Spin
		{
			if ((matrix[tX + 1][tY - 1] != NULL) || (matrix[tX + 1][tY + 1] != NULL))
			{
				return 1;
			}
		}
		else if (tX == 9) // check for T-Spin Mini with wall
		{
			if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX - 1][tY + 1] != NULL))
			{
				return 0;
			}
		}
		else if ((matrix[tX + 1][tY - 1] != NULL) && (matrix[tX + 1][tY + 1] != NULL)) // check for T-Spin Mini with minos
		{
			if ((matrix[tX - 1][tY - 1] != NULL) || (matrix[tX - 1][tY + 1] != NULL))
			{
				return 0;
			}
		}
		break;
	}
	}

	return -1;
}

void Player::lockIn(sf::Texture &t)
{
	pcsLocked++;
	activeAnchor->readyToLock = true;
	spawnTime = 0;
	lockdownTime = -1;
	if (activeAnchor->colour == 6)
	{
		tSpin = checkTSpin();
		if ((tSpin == 0) && (!tsMiniEligible))
			tSpin = -1;
	}
	delete activeAnchor;
	checkVisuals(t);
	checkLines();
}

void Player::checkLines()
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

void Player::clearLines(sf::Font &f, sf::Sound s[], sf::SoundBuffer* sb[])
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
				s[6].setBuffer(*sb[5]);
				s[6].play();
			}
		}
	}

	totalCleared += cleared;

	switch (cleared)
	{
	case 0:
	{
		combo = -1;

		if (tSpin == 0)
		{
			score += (TSMN * (level + 1));
		}
		else if (tSpin == 1)
		{
			score += (TSPN * (level + 1));
		}

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
	if (score != 0)
	{
		for (int i = 0; i < 10; i++)
		{
			if (matrix[i][0] != NULL)
			{
				pClear = false;
				break;
			}
		}
	}
	else
		pClear = false;
	if (pClear)
	{
		score += (PCLR * (level + 1));
		appl[4]->clear();
		appl[4]->init(8, f, this);
		appl[5]->clear();
		appl[5]->init(9, f, this);
	}

	txt_stats[2].setString("");
	int len = std::to_string(score).length();
	for (int i = 0; i < (8 - len); i++)
		txt_stats[2].setString(txt_stats[2].getString() + "0");
	txt_stats[2].setString(txt_stats[2].getString() + std::to_string(score));
	sf::FloatRect bounds = txt_stats[2].getLocalBounds();
	txt_stats[2].setOrigin(sf::Vector2f(bounds.width, bounds.height));
	txt_stats[2].setPosition(sf::Vector2f(1632.0, 540.0));

	// applauses

	if (combo > 0)
	{
		appl[0]->clear();
		appl[0]->init(0, f, this);
	}

	if ((back2back > 0) && ((cleared == 4) || ((tSpin >= 0) && (cleared >= 1))))
	{
		appl[1]->clear();
		appl[1]->init(1, f, this);
	}

	if (cleared == 1)
	{
		appl[2]->clear();
		appl[3]->clear();
		appl[3]->init(4, f, this);
	}
	else if (cleared == 2)
	{
		appl[2]->clear();
		appl[3]->clear();
		appl[3]->init(5, f, this);
	}
	else if (cleared == 3)
	{
		appl[2]->clear();
		appl[3]->clear();
		appl[3]->init(6, f, this);
	}
	else if (cleared == 4)
	{
		appl[2]->clear();
		appl[3]->clear();
		appl[3]->init(7, f, this);
	}

	if (tSpin == 0)
	{
		appl[2]->clear();
		appl[2]->init(3, f, this);
	}
	else if (tSpin == 1)
	{
		appl[2]->clear();
		appl[2]->init(2, f, this);
	}

	if ((cleared == 4) || (tSpin >= 0) || (pClear))
	{
		s[5].setBuffer(*sb[0]);
		s[5].play();
	}
	else if (cleared > 0)
	{
		s[4].setBuffer(*sb[1]);
		s[4].play();
	}
}

bool Player::checkPos(int xSh, int ySh)
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

void Player::sdropCalc()
{
	softDropUpdate = pow(0.8 - (level * 0.007), level) * 1000;
}

void Player::drawPreview(sf::RenderWindow &w, sf::Texture &t)
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

void Player::keyTest(sf::RenderWindow &w)
{

	if (ctrlState[0])	// ruch w lewo
		keyDebug[3].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[3].setColor(sf::Color(85, 85, 85, 85));
	if (ctrlState[1])	// ruch w prawo
		keyDebug[5].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[5].setColor(sf::Color(85, 85, 85, 85));
	if (ctrlState[2])	// obrot w lewo
		keyDebug[0].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[0].setColor(sf::Color(85, 85, 85, 85));
	if (ctrlState[3])	// obrot w prawo
		keyDebug[2].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[2].setColor(sf::Color(85, 85, 85, 85));
	if (ctrlState[4])	// softdrop
		keyDebug[4].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[4].setColor(sf::Color(85, 85, 85, 85));
	if (ctrlState[5])	// harddrop
		keyDebug[6].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[6].setColor(sf::Color(85, 85, 85, 85));
	if (ctrlState[6])	// hold
		keyDebug[1].setColor(sf::Color(255, 255, 255, 255));
	else
		keyDebug[1].setColor(sf::Color(85, 85, 85, 85));

	for (int i = 0; i < 7; i++)
	{
		w.draw(keyDebug[i]);
	}
}

void Player::drawMatrix(sf::RenderWindow &w, sf::Texture &t)
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

	// Rysuj aplauz

	for (int i = 0; i < 6; i++)
	{
		if (appl[i] != NULL)
		{
			for (int j = 0; j < 11; j++)
			{
				w.draw(appl[i]->txt[j]);
			}
		}
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