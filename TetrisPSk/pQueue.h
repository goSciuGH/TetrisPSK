#pragma once

class Queue;

/// Klasa pQueue
///
/// Zadaniem klasy jest byæ odnoœnikiem do elementów kolejki Tetrimin.
/// Zawiera dwa pola - head i tail, wskazuj¹ce odpowiednio na przód i ty³ kolejki; oraz trzy metody - kolejkuj¹c¹, odkolejkuj¹c¹ i s³u¿¹c¹ do dodania nowej torby Tetrimin do kolejki.
class pQueue
{
public:
	/// Konstruktor pQueue();
	///
	/// Ustawia wskaŸniki na przód i ty³ kolejki jako NULL.
	pQueue();

	Queue* head;
	Queue* tail;

	/// Metoda enqueue()
	///
	/// Dodaje nowy element Queue do kolejki jako ostatni element w istniej¹cej kolejce lub jako pierwszy w pustej.
	/// @param d Numer odpowiadaj¹cy kszta³towi Tetrimina.
	void enqueue(int d);	// przyk³adowe wywo³anie funkcji: enqueue(pcQueue, 5) - dodaje Tetrimino I na koniec kolejki;

	/// Metoda dequeue()
	///
	/// Usuwa z pamiêci gry pierwszy element kolejki. WskaŸnik na przód kolejki jest zast¹piony wskaŸnikiem na kolejny element.
	void dequeue();

	/// Metoda addBag()
	///
	/// Tworzy now¹ torbê Tetrimin, losowo ustawiaj¹c ich kolejnoœæ i dodaje je do kolejki przez metodê enqueue().
	void addBag();
};

