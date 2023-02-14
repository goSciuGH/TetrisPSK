#pragma once

#include "Mino.h"

class Player;

/// Reprezentuje Mino-Kotwic� - g��wne Mino w sterowalnym Tetriminie. S�u�y za punkt obrotu i reprezentuje pozycj� Tetrimina.
class MinoAnchor : public Mino
{
public:
	/// Konstruktor klasy.<br> Ustawia pocz�tkowe warto�ci Tetrimina, takie jak pozycja, obr�t czy kszta�t. Tworzy potomne Mina i przekazuje im ich przesuni�cie.<br>
	/// Ustawia odpowiadaj�c� tekstur� sobie i potomnym Minom.<br>
	/// Ustawia swoj� pozycj� na ekranie.
	///	@param p Wska�nik na obiekt gracza.
	///	@param c Numer odpowiadaj�cy kszta�towi Tetrimina.
	///	@param t Referencja na tekstur� Tetrimina.
	MinoAnchor(Player* p, int c, sf::Texture &t);

	/// Destruktor klasy.<br> Je�eli Tetrimino jest gotowe do zalokowania, kopiuje ich pozycj�, kolor i tekstur� do nowych Min w Matrixie, nast�pnie usuwa potomne Mina.<br>
	/// Na koniec, wska�nik na sterowalne Tetrimino w obiekcie gracza jest ustawiany na NULL.
	~MinoAnchor();

	Player* matrix;			///< Wska�nik na obiekt gracza.
	int shape;				///< Numer reprezentuj�cy kolor/kszta�t Tetrimina.
	int rot;				///< Numer rerezentuj�cy kierunek Tetrimina: 0 - pozycja startowa; 1 - obr�t w prawo; 2 - dwa obroty; 3 - obr�t w lewo.
	Mino* children[3];		///< Wska�nik na Mina potomne Tetrimina.
	bool readyToLock;		///< Gotowo�� do zalokowania.

	/// Ustawia pozycj� swoj� i Min potomnych na ekranie.
	void updateScreenXY();
};