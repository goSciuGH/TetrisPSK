#pragma once

/// Klasa Queue
///
/// Klasa bêd¹ca elementem kolejki wskazywanej przez obiekt klasy pQueue.
/// Posiada trzy pola: ID (numer w kolejce), color (numer odpowiadaj¹cy kolorowi/kszta³towi Tetrimina) i *next (wskaŸnik na nastêpny element).
class Queue
{
public:
	Queue();

	int id;
	int color;
	Queue* next;
};

