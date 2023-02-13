#pragma once

/// Klasa Queue
///
/// Klasa b�d�ca elementem kolejki wskazywanej przez obiekt klasy pQueue.
/// Posiada trzy pola: ID (numer w kolejce), color (numer odpowiadaj�cy kolorowi/kszta�towi Tetrimina) i *next (wska�nik na nast�pny element).
class Queue
{
public:
	Queue();

	int id;
	int color;
	Queue* next;
};

