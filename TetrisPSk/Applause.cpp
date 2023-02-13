#include "Applause.h"

#include "Player.h"

Applause::Applause()
{
	lifeTime = 0;
	aType = -1;
	len = 0;

	for (int i = 0; i < 11; i++)
		txt[i].setString("");

	double spd = 16.0 / 3.0;
	double acc = 16.0 / 9.0;
	double spdV = 340.0;
	double accV = 680.0 / 3.0;
	double vis = 255;
}

void Applause::clear()
{
	lifeTime = 0;
	aType = -1;
	len = 0;

	for (int i = 0; i < 11; i++)
		txt[i].setString("");

	spd = 16.0 / 3.0;
	acc = 16.0 / 9.0;
	spdV = 340.0;
	accV = 680.0 / 3.0;
	vis = 255;
}

void Applause::init(int type, sf::Font &f, Player* p)
{
	std::string s;
	aType = type;

	spd = 16.0 / 3.0;
	acc = 16.0 / 9.0;
	spdV = 340.0;
	accV = 680.0 / 3.0;
	vis = 255;

	if (type == 0)	// Combo
	{
		s = "COMBO " + std::to_string(p->combo);
		len = s.length();
		for (int i = 0; i < len; i++)
		{
			txt[i].setString(s[i]);
			txt[i].setPosition(sf::Vector2f(p->getPosition().x - 184.0 - (24.0 * s.length()) + (24.0 * i), p->getPosition().y));
			txt[i].setCharacterSize(32);
			txt[i].setFillColor(sf::Color(255, 255, 255, 255));
			txt[i].setFont(f);
		}
	}
	else if (type == 1)	// B2B
	{
		s = "BACK 2 BACK";
		len = s.length();
		for (int i = 0; i < len; i++)
		{
			txt[i].setString(s[i]);
			txt[i].setPosition(sf::Vector2f(p->getPosition().x - 184.0 - (24.0 * s.length()) + (24.0 * i), p->getPosition().y + 32.0));
			txt[i].setCharacterSize(32);
			txt[i].setFillColor(sf::Color(255, 255, 255, 255));
			txt[i].setFont(f);
		}
	}
	else if (type < 4)	// T-Spin & T-Spin Mini
	{
		if (type == 2)	// T-Spin
			s = "T-SPIN";
		else	// T-Spin Mini
			s = "T-SPIN MINI";

		len = s.length();

		for (int i = 0; i < s.length(); i++)
		{
			txt[i].setString(s[i]);
			txt[i].setPosition(sf::Vector2f(p->getPosition().x + 180.0 + (24.0 * i), p->getPosition().y));
			txt[i].setCharacterSize(32);
			txt[i].setFillColor(sf::Color(255, 255, 255, 255));
			txt[i].setFont(f);
		}
	}
	else if (type < 8)
	{
		if (type == 4)	// Single
			s = "SINGLE";
		else if (type == 5)	// Double
			s = "DOUBLE";
		else if (type == 6)	// Triple
			s = "TRIPLE";
		else	// Quad
			s = "QUAD";

		len = s.length();

		for (int i = 0; i < s.length(); i++)
		{
			txt[i].setString(s[i]);
			txt[i].setPosition(sf::Vector2f(p->getPosition().x + 180.0 + (24.0 * i), p->getPosition().y + 32.0));
			txt[i].setCharacterSize(32);
			txt[i].setFillColor(sf::Color(255, 255, 255, 255));
			txt[i].setFont(f);
		}
	}
	else
	{
		if (type == 8)	// Perfect
		{
			spd = 56.0 / 9.0;
			acc = 56.0 / 27.0;
			len = 7;
			txt[2].setString("P");
			txt[3].setString("E");
			txt[4].setString("R");
			txt[5].setString("F");
			txt[6].setString("E");
			txt[7].setString("C");
			txt[8].setString("T");
			for (int i = 4; i >= 0; i--)
			{
				txt[i].setPosition(sf::Vector2f(p->getPosition().x - 24.0 - (36.0 * (5 - i)), p->getPosition().y - 174.0));
				txt[i].setCharacterSize(48);
				txt[i].setFillColor(sf::Color(255, 255, 255, 255));
				txt[i].setFont(f);
			}
			txt[5].setPosition(sf::Vector2f(p->getPosition().x - 24.0, p->getPosition().y - 174.0));
			txt[5].setCharacterSize(48);
			txt[5].setFillColor(sf::Color(255, 255, 255, 255));
			txt[5].setFont(f);
			for (int i = 6; i < 11; i++)
			{
				txt[i].setPosition(sf::Vector2f(p->getPosition().x - 24.0 + (36.0 * (i - 5)), p->getPosition().y - 174.0));
				txt[i].setCharacterSize(48);
				txt[i].setFillColor(sf::Color(255, 255, 255, 255));
				txt[i].setFont(f);
			}
		}
		else if (type == 9)	// Clear
		{
			spd = 28.0 / 3.0;
			acc = 28.0 / 9.0;
			txt[3].setString("C");
			txt[4].setString("L");
			txt[5].setString("E");
			txt[6].setString("A");
			txt[7].setString("R");
			len = 5;
			for (int i = 4; i >= 0; i--)
			{
				txt[i].setPosition(sf::Vector2f(p->getPosition().x - 32.0 - (48.0 * (5 - i)), p->getPosition().y - 134.0));
				txt[i].setCharacterSize(64);
				txt[i].setFillColor(sf::Color(255, 255, 255, 255));
				txt[i].setFont(f);
			}
			txt[5].setPosition(sf::Vector2f(p->getPosition().x - 32.0, p->getPosition().y - 134.0));
			txt[5].setCharacterSize(64);
			txt[5].setFillColor(sf::Color(255, 255, 255, 255));
			txt[5].setFont(f);
			for (int i = 6; i < 11; i++)
			{
				txt[i].setPosition(sf::Vector2f(p->getPosition().x - 32.0 + (48.0 * (i - 5)), p->getPosition().y - 134.0));
				txt[i].setCharacterSize(64);
				txt[i].setFillColor(sf::Color(255, 255, 255, 255));
				txt[i].setFont(f);
			}
		}
	}

	for (int i = 0; i < len; i++)
	{
		txt[i].setFont(f);
	}
}

void Applause::updateVis(int t)
{
	if (lifeTime <= 3000)
	{
		if (aType <= 1)
		{
			for (int i = 0; i < len; i++)
			{
				txt[i].move(sf::Vector2f((0.0 - spd * (len - 1 - i)) * (t / 1000.0), 0.0));
				if (lifeTime >= 1500)
				{
					txt[i].setFillColor(sf::Color(255, 255, 255, vis));
				}
			}
		}
		else if (aType <= 7)
		{
			for (int i = 0; i < len; i++)
			{
				txt[i].move(sf::Vector2f(spd * i * (t / 1000.0), 0.0));
				if (lifeTime >= 1500)
				{
					txt[i].setFillColor(sf::Color(255, 255, 255, vis));
				}
			}
		}
		else if (aType <= 10)
		{
			for (int i = 4; i >= 0; i--)
			{
				txt[i].move(sf::Vector2f(0.0 - spd * (5 - i) * (t / 1000.0), 0.0));
				if (lifeTime >= 1500)
				{
					txt[i].setFillColor(sf::Color(255, 255, 255, vis));
				}
			}

			if (lifeTime >= 1500)
			{
				txt[5].setFillColor(sf::Color(255, 255, 255, vis));
			}

			for (int i = 6; i < 11; i++)
			{
				txt[i].move(sf::Vector2f(spd * (i - 5) * (t / 1000.0), 0.0));
				if (lifeTime >= 1500)
				{
					txt[i].setFillColor(sf::Color(255, 255, 255, vis));
				}
			}
		}

		if (lifeTime >= 1500)
		{
			double temp = vis - (spdV * (t / 1000.0));
			if (temp <= 0)
				vis = 0;
			else
				vis = temp;
			spdV -= ((t / 1000.0) * accV);
		}

		spd -= ((t / 1000.0) * acc);

		lifeTime += t;
	}
	else
		clear();
}