#pragma once

class Queue;

/// Zadaniem klasy jest byæ odnoœnikiem do elementów kolejki Tetrimin.<br>
class pQueue
{
public:
	/// Konstruktor klasy.<br> Ustawia wskaŸniki na przód i ty³ kolejki jako NULL.
	pQueue();

	Queue* head;	///< WskaŸnik na przód kolejki.
	Queue* tail;	///< WskaŸnik na koniec kolejki.

	/// Dodaje nowy element Queue do kolejki jako ostatni element w istniej¹cej kolejce lub jako pierwszy w pustej.
	/// @param d Numer odpowiadaj¹cy kszta³towi Tetrimina.
	void enqueue(int d);	// przyk³adowe wywo³anie funkcji: enqueue(pcQueue, 5) - dodaje Tetrimino I na koniec kolejki;

	/// Usuwa z pamiêci gry pierwszy element kolejki. WskaŸnik na przód kolejki jest zast¹piony wskaŸnikiem na kolejny element.
	void dequeue();

	/// Tworzy now¹ torbê Tetrimin, losowo ustawiaj¹c ich kolejnoœæ i dodaje je do kolejki przez metodê enqueue().
	void addBag();
};

