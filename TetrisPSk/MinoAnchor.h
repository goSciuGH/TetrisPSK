#pragma once

#include "Mino.h"

class Player;

/// Klasa MinoAnchor
///
/// Reprezentuje Mino-Kotwic� - g��wne Mino w sterowalnym Tetriminie. S�u�y za punkt obrotu i reprezentuje pozycj� Tetrimina.
/// Klasa posiada 5 p�l:
///		*matrix - wska�nik na obiekt gracza (a zatem te� na Matrix)
///		shape - numer reprezentuj�cy kszta�t Tetrimina
///		rot - obecny stan obrotu Tetrimina
///		*children[3] - tablica wska�nik�w na pozosta�e Mina w Tetriminie
///		readyToLock - bool reprezentuj�cy gotowo�� do zalokowana Min na Matrixie
///	Klasa zawiera metod� updateScreenXY(), kt�ra aktualizuje graficzn� reprezentacj� Mina.
class MinoAnchor : public Mino
{
public:
	/// Konstruktor MinoAnchor()
	///
	/// Ustawia pocz�tkowe warto�ci Tetrimina, takie jak pozycja, obr�t czy kszta�t. Tworzy potomne Mina i przekazuje im ich przesuni�cie.
	/// Ustawia odpowiadaj�c� tekstur� sobie i potomnym Minom.
	/// Ustawia swoj� pozycj� na ekranie.
	///	@param p Wska�nik na obiekt gracza.
	///	@param c Numer odpowiadaj�cy kszta�towi Tetrimina.
	///	@param t Referencja na tekstur� Tetrimina.
	MinoAnchor(Player* p, int c, sf::Texture &t);

	/// Destruktor ~MinoAnchor()
	///
	/// Je�eli Tetrimino jest gotowe do zalokowania, kopiuje ich pozycj�, kolor i tekstur� do nowych Min w Matrixie, nast�pnie usuwa potomne Mina.
	/// Na koniec, wska�nik na sterowalne Tetrimino w obiekcie gracza jest ustawiany na NULL.
	~MinoAnchor();

	Player* matrix;
	int shape;
	int rot; // 0 - pozycja startowa; 1 - obr�t w prawo; 2 - dwa obroty; 3 - obr�t w lewo
	Mino* children[3];
	bool readyToLock;

	/// Metoda updateScreenXY()
	///
	/// Ustawia pozycj� swoj� i Min potomnych na ekranie.
	void updateScreenXY();
};