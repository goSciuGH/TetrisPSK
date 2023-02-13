#pragma once

class Queue;

/// Klasa pQueue
///
/// Zadaniem klasy jest by� odno�nikiem do element�w kolejki Tetrimin.
/// Zawiera dwa pola - head i tail, wskazuj�ce odpowiednio na prz�d i ty� kolejki; oraz trzy metody - kolejkuj�c�, odkolejkuj�c� i s�u��c� do dodania nowej torby Tetrimin do kolejki.
class pQueue
{
public:
	/// Konstruktor pQueue();
	///
	/// Ustawia wska�niki na prz�d i ty� kolejki jako NULL.
	pQueue();

	Queue* head;
	Queue* tail;

	/// Metoda enqueue()
	///
	/// Dodaje nowy element Queue do kolejki jako ostatni element w istniej�cej kolejce lub jako pierwszy w pustej.
	/// @param d Numer odpowiadaj�cy kszta�towi Tetrimina.
	void enqueue(int d);	// przyk�adowe wywo�anie funkcji: enqueue(pcQueue, 5) - dodaje Tetrimino I na koniec kolejki;

	/// Metoda dequeue()
	///
	/// Usuwa z pami�ci gry pierwszy element kolejki. Wska�nik na prz�d kolejki jest zast�piony wska�nikiem na kolejny element.
	void dequeue();

	/// Metoda addBag()
	///
	/// Tworzy now� torb� Tetrimin, losowo ustawiaj�c ich kolejno�� i dodaje je do kolejki przez metod� enqueue().
	void addBag();
};

