#pragma once

#include <SFML/Graphics.hpp>

/// Klasa reprezentuj�ca pojedyncze Mino. Bazuje na klasie sf::Sprite, by m�c przybiera� tekstur� i by� wy�wietlana.
class Mino : public sf::Sprite
{
public:
	/// Konstruktor klasy.<br> Jedyna przypisywana warto�� w konstruktorze to false w readyToShow - Mino jeszcze nie by�o lokowane, wi�c nie mo�e by� gotowe do pokazania jako cz�� Matrixa.
	Mino();

	int colour;			///< Numer wyznaczaj�cy kolor/kszta� Tetrimina.
	int posX;			///< Pozycja Mina w osi X. B�d�c w sterowalnym Tetriminie, odnosi si� do Mina-Kotwicy, w innym wypadku, odnosi si� do Matrixa.
	int posY;			///< Pozycja Mina w osi Y. B�d�c w sterowalnym Tetriminie, odnosi si� do Mina-Kotwicy, w innym wypadku, odnosi si� do Matrixa.
	bool readyToShow;	///< Gotowo�� Mina do pokazania jako cz�� Matrixa.

	/// S�u�y obr�ceniu Mina wok� Mina-Kotwicy. Polega na zamienieniu miejscami warto�ci pozycji X i Y, oraz zamieniu znaku na przeciwny w jednej z tych pozycji, zale�nie od kierunku obrotu.
	///	@param cw Bool opisuj�cy kierunek obrotu. True reprezentuje obr�t w kierunku wskaz�wek zegara, false - w przeciwnym.
	void rotate(bool cw);
};