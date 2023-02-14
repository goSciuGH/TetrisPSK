#pragma once

#include <SFML/Graphics.hpp>

class Player;

/// Jest to graficzny obiekt, kt�ry ma gratulowa� graczowi dokonanych ruch�w poprzez wy�wietlanie ich nazw na ekranie.
class Applause
{
public:
	/// Konstruktor klasy.<br>Ustawia parametry aplauzu na pocz�tkowe, ustawia pusty tekst.
	Applause();

	sf::Text txt[11];			///< Tablica obiekt�w tesktowych do zwaarcia tekstu aplauzu.
	int len;					///< D�ugo�� tekstu.
	int aType;					///< Typ wy�wietlanego aplauzu.
	int lifeTime = 0;			///< Czasomierz trwania aplauzu.
	double spd = 16.0 / 3.0;	///< Pr�dko�� pocz�tkowa ruchu dla drugiego znaku od matrixa (w px/s).
	double acc = 16.0 / 9.0;	///< Op�nienie dla drugiego znaku od matrixa (w px/s^2).
	double spdV = 340.0;		///< Pr�dko�� pocz�tkowa zanikania (w b/s).
	double accV = 680.0 / 3.0;	///< Op�nienie zanikania (w b/s^2).
	double vis = 255;

	///	Czy�ci aplauz, by przypomina� �wie�o utworzony obiekt.
	void clear();

	/// Ustawia tekst zale�ny od typu aplauzu, ustawia jego pozycj� wzgl�dem obiektu gracza i zmienia warto�ci ruchu, je�li nagradzany jest Perfect Clear.
	/// @param type Numer reprezentuj�cy typ aplauzu.
	///	@param f Referencja na czcionk� do u�ycia w tek�cie.
	///	@param p Wska�nik na obiekt gracza.
	void init(int type, sf::Font &f, Player* p);

	/// Je�eli czas �ycia aplauzu jest mniejszy ni� 3 sekundy, animuje tekst poprzez animacj� znak�w tekstu.<br>
	/// Zmienia pr�dko�� ruchu i zanikania o warto�� wyliczon� z op�nienia i czasu klatki.<br>
	/// Je�eli aplauz �y� przez 3 sekundy, czy�ci go przez clear();
	///	@param t Czas klatki w ms.
	void updateVis(int t);

};