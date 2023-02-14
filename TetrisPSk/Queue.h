#pragma once

/// Klasa bêd¹ca elementem kolejki wskazywanej przez obiekt klasy pQueue.
class Queue
{
public:
	/// Konstruktor klasy.<br> Nie robi nic.
	Queue();

	int id;			///< Numer ID elementu kolejki.
	int color;		///< Numer reprezentuj¹cy kolor/kszta³t Tetrimina.
	Queue* next;	///< WskaŸnik na kolejny element kolejki.
};

