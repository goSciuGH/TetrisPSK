#pragma once

#include "Mino.h"

class Player;

/// Reprezentuje Mino-Kotwicê - g³ówne Mino w sterowalnym Tetriminie. S³u¿y za punkt obrotu i reprezentuje pozycjê Tetrimina.
class MinoAnchor : public Mino
{
public:
	/// Konstruktor klasy.<br> Ustawia pocz¹tkowe wartoœci Tetrimina, takie jak pozycja, obrót czy kszta³t. Tworzy potomne Mina i przekazuje im ich przesuniêcie.<br>
	/// Ustawia odpowiadaj¹c¹ teksturê sobie i potomnym Minom.<br>
	/// Ustawia swoj¹ pozycjê na ekranie.
	///	@param p WskaŸnik na obiekt gracza.
	///	@param c Numer odpowiadaj¹cy kszta³towi Tetrimina.
	///	@param t Referencja na teksturê Tetrimina.
	MinoAnchor(Player* p, int c, sf::Texture &t);

	/// Destruktor klasy.<br> Je¿eli Tetrimino jest gotowe do zalokowania, kopiuje ich pozycjê, kolor i teksturê do nowych Min w Matrixie, nastêpnie usuwa potomne Mina.<br>
	/// Na koniec, wskaŸnik na sterowalne Tetrimino w obiekcie gracza jest ustawiany na NULL.
	~MinoAnchor();

	Player* matrix;			///< WskaŸnik na obiekt gracza.
	int shape;				///< Numer reprezentuj¹cy kolor/kszta³t Tetrimina.
	int rot;				///< Numer rerezentuj¹cy kierunek Tetrimina: 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo.
	Mino* children[3];		///< WskaŸnik na Mina potomne Tetrimina.
	bool readyToLock;		///< Gotowoœæ do zalokowania.

	/// Ustawia pozycjê swoj¹ i Min potomnych na ekranie.
	void updateScreenXY();
};