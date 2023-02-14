#pragma once

class Queue;

/// Zadaniem klasy jest by� odno�nikiem do element�w kolejki Tetrimin.<br>
class pQueue
{
public:
	/// Konstruktor klasy.<br> Ustawia wska�niki na prz�d i ty� kolejki jako NULL.
	pQueue();

	Queue* head;	///< Wska�nik na prz�d kolejki.
	Queue* tail;	///< Wska�nik na koniec kolejki.

	/// Dodaje nowy element Queue do kolejki jako ostatni element w istniej�cej kolejce lub jako pierwszy w pustej.
	/// @param d Numer odpowiadaj�cy kszta�towi Tetrimina.
	void enqueue(int d);	// przyk�adowe wywo�anie funkcji: enqueue(pcQueue, 5) - dodaje Tetrimino I na koniec kolejki;

	/// Usuwa z pami�ci gry pierwszy element kolejki. Wska�nik na prz�d kolejki jest zast�piony wska�nikiem na kolejny element.
	void dequeue();

	/// Tworzy now� torb� Tetrimin, losowo ustawiaj�c ich kolejno�� i dodaje je do kolejki przez metod� enqueue().
	void addBag();
};

