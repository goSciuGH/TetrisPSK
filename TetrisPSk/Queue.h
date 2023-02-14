#pragma once

/// Klasa b�d�ca elementem kolejki wskazywanej przez obiekt klasy pQueue.
class Queue
{
public:
	/// Konstruktor klasy.<br> Nie robi nic.
	Queue();

	int id;			///< Numer ID elementu kolejki.
	int color;		///< Numer reprezentuj�cy kolor/kszta�t Tetrimina.
	Queue* next;	///< Wska�nik na kolejny element kolejki.
};

