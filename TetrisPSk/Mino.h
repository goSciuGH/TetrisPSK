#pragma once

#include <SFML/Graphics.hpp>

/// Klasa Mino
///
/// Klasa reprezentuj¹ca pojedyncze Mino. Bazuje na klasie sf::Sprite, by móc przybieraæ teksturê i byæ wyœwietlana.
/// Klasa posiada 4 pola:
///		colour - numer odpowiadaj¹cy kolowoi/kszta³towi Mina
///		posX, posY - zmienne opisuj¹ce po³o¿enie Mina w stosunku do Mina-Kotwicy, je¿eli jest obecnie czêœci¹ sterowalnego Tetrimina; lub matrixu
///		readyToShow - bool informuj¹cy czy zalokowane Mino jest gotowe do pokazania na ekranie
///	Klasa posiada metodê rotate(), która s³u¿y do obrócenia Mina wokó³ Mina-Kotwicy
class Mino : public sf::Sprite
{
public:
	/// Konstruktor Mino()
	///
	/// Jedyna przypisywana wartoœæ w konstruktorze to false w readyToShow - Mino jeszcze nie by³o lokowane, wiêc nie mo¿e byæ gotowe do pokazania jako czêœæ Matrixa.
	Mino();

	int colour;
	int posX, posY;
	bool readyToShow;

	/// Metoda rotate()
	///
	/// S³u¿y obróceniu Mina wokó³ Mina-Kotwicy. Polega na zamienieniu miejscami wartoœci pozycji X i Y, oraz zamieniu znaku na przeciwny w jednej z tych pozycji, zale¿nie od kierunku obrotu.
	///	@param cw Bool opisuj¹cy kierunek obrotu. True reprezentuje obrót w kierunku wskazówek zegara, false - w przeciwnym.
	void rotate(bool cw);
};