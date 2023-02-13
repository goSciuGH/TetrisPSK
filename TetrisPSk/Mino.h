#pragma once

#include <SFML/Graphics.hpp>

/// Klasa Mino
///
/// Klasa reprezentuj�ca pojedyncze Mino. Bazuje na klasie sf::Sprite, by m�c przybiera� tekstur� i by� wy�wietlana.
/// Klasa posiada 4 pola:
///		colour - numer odpowiadaj�cy kolowoi/kszta�towi Mina
///		posX, posY - zmienne opisuj�ce po�o�enie Mina w stosunku do Mina-Kotwicy, je�eli jest obecnie cz�ci� sterowalnego Tetrimina; lub matrixu
///		readyToShow - bool informuj�cy czy zalokowane Mino jest gotowe do pokazania na ekranie
///	Klasa posiada metod� rotate(), kt�ra s�u�y do obr�cenia Mina wok� Mina-Kotwicy
class Mino : public sf::Sprite
{
public:
	/// Konstruktor Mino()
	///
	/// Jedyna przypisywana warto�� w konstruktorze to false w readyToShow - Mino jeszcze nie by�o lokowane, wi�c nie mo�e by� gotowe do pokazania jako cz�� Matrixa.
	Mino();

	int colour;
	int posX, posY;
	bool readyToShow;

	/// Metoda rotate()
	///
	/// S�u�y obr�ceniu Mina wok� Mina-Kotwicy. Polega na zamienieniu miejscami warto�ci pozycji X i Y, oraz zamieniu znaku na przeciwny w jednej z tych pozycji, zale�nie od kierunku obrotu.
	///	@param cw Bool opisuj�cy kierunek obrotu. True reprezentuje obr�t w kierunku wskaz�wek zegara, false - w przeciwnym.
	void rotate(bool cw);
};