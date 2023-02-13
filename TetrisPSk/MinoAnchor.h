#pragma once

#include "Mino.h"

class Player;

/// Klasa MinoAnchor
///
/// Reprezentuje Mino-Kotwicê - g³ówne Mino w sterowalnym Tetriminie. S³u¿y za punkt obrotu i reprezentuje pozycjê Tetrimina.
/// Klasa posiada 5 pól:
///		*matrix - wskaŸnik na obiekt gracza (a zatem te¿ na Matrix)
///		shape - numer reprezentuj¹cy kszta³t Tetrimina
///		rot - obecny stan obrotu Tetrimina
///		*children[3] - tablica wskaŸników na pozosta³e Mina w Tetriminie
///		readyToLock - bool reprezentuj¹cy gotowoœæ do zalokowana Min na Matrixie
///	Klasa zawiera metodê updateScreenXY(), która aktualizuje graficzn¹ reprezentacjê Mina.
class MinoAnchor : public Mino
{
public:
	/// Konstruktor MinoAnchor()
	///
	/// Ustawia pocz¹tkowe wartoœci Tetrimina, takie jak pozycja, obrót czy kszta³t. Tworzy potomne Mina i przekazuje im ich przesuniêcie.
	/// Ustawia odpowiadaj¹c¹ teksturê sobie i potomnym Minom.
	/// Ustawia swoj¹ pozycjê na ekranie.
	///	@param p WskaŸnik na obiekt gracza.
	///	@param c Numer odpowiadaj¹cy kszta³towi Tetrimina.
	///	@param t Referencja na teksturê Tetrimina.
	MinoAnchor(Player* p, int c, sf::Texture &t);

	/// Destruktor ~MinoAnchor()
	///
	/// Je¿eli Tetrimino jest gotowe do zalokowania, kopiuje ich pozycjê, kolor i teksturê do nowych Min w Matrixie, nastêpnie usuwa potomne Mina.
	/// Na koniec, wskaŸnik na sterowalne Tetrimino w obiekcie gracza jest ustawiany na NULL.
	~MinoAnchor();

	Player* matrix;
	int shape;
	int rot; // 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo
	Mino* children[3];
	bool readyToLock;

	/// Metoda updateScreenXY()
	///
	/// Ustawia pozycjê swoj¹ i Min potomnych na ekranie.
	void updateScreenXY();
};