#pragma once

#include <SFML/Graphics.hpp>

/// Klasa reprezentuj¹ca pojedyncze Mino. Bazuje na klasie sf::Sprite, by móc przybieraæ teksturê i byæ wyœwietlana.
class Mino : public sf::Sprite
{
public:
	/// Konstruktor klasy.<br> Jedyna przypisywana wartoœæ w konstruktorze to false w readyToShow - Mino jeszcze nie by³o lokowane, wiêc nie mo¿e byæ gotowe do pokazania jako czêœæ Matrixa.
	Mino();

	int colour;			///< Numer wyznaczaj¹cy kolor/kszta³ Tetrimina.
	int posX;			///< Pozycja Mina w osi X. Bêd¹c w sterowalnym Tetriminie, odnosi siê do Mina-Kotwicy, w innym wypadku, odnosi siê do Matrixa.
	int posY;			///< Pozycja Mina w osi Y. Bêd¹c w sterowalnym Tetriminie, odnosi siê do Mina-Kotwicy, w innym wypadku, odnosi siê do Matrixa.
	bool readyToShow;	///< Gotowoœæ Mina do pokazania jako czêœæ Matrixa.

	/// S³u¿y obróceniu Mina wokó³ Mina-Kotwicy. Polega na zamienieniu miejscami wartoœci pozycji X i Y, oraz zamieniu znaku na przeciwny w jednej z tych pozycji, zale¿nie od kierunku obrotu.
	///	@param cw Bool opisuj¹cy kierunek obrotu. True reprezentuje obrót w kierunku wskazówek zegara, false - w przeciwnym.
	void rotate(bool cw);
};