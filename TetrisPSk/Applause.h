#pragma once

#include <SFML/Graphics.hpp>

class Player;

/// Klasa Applause
///
/// Jest to graficzny obiekt, kt�ry ma gratulowa� graczowi dokonanych ruch�w poprzez wy�wietlanie ich nazw na ekranie.
/// Klasa posiada 9 p�l:
///		txt[11] - tablica obiekt�w klasy sf::Text
///		len - d�ugo�� wy�wietlanego tekstu
///		aType - typ wy�wietlanego aplauzu
///		lifeTime - czasomierz wy�wietlania aplauzu
///		spd - pr�dko�� ruchu pierwszego od brzegu ruchomego znaku; skalowane dla kolejnych znak�w
///		acc - op�nienie ruchu dla znak�w jak wy�ej
///		spdV - pr�dko�� zmiany widoczno�ci
///		accV - op�nienie zmiany widoczno�ci
///		vis - widoczno�� znaku
/// Klasa posiada 3 metody:
///		clear() - zeruje parametry, by ukry� aplauz
///		init() - inicjuje parametry jak w konstruktorze, ustawia typ aplauzu i jego tekst, je�li zachodzi taka potrzeba
///		updateVis() - aktualizuje reprezentacj� graficzn� aplauzu

class Applause
{
public:
	/// Konstruktor Applause()
	///
	/// Ustawia parametry aplauzu na pocz�tkowe, ustawia pusty tekst/
	Applause();

	sf::Text txt[11];
	int len;
	int aType;
	int lifeTime = 0;
	double spd = 16.0 / 3.0;	// pr�dko�� pocz�tkowa ruchu dla drugiego znaku od matrixa (w px/s)
	double acc = 16.0 / 9.0;	// op�nienie dla drugiego znaku od matrixa (w px/s^2)
	double spdV = 340.0;		// pr�dko�� pocz�tkowa zanikania (w b/s)
	double accV = 680.0 / 3.0;	// op�nienie zanikania (w b/s^2)
	double vis = 255;

	/// Metoda clear()
	///
	///	Czy�ci aplauz, by przypomina� �wie�o utworzony obiekt.
	void clear();

	/// Metoda init()
	///
	/// Ustawia tekst zale�ny od typu aplauzu, ustawia jego pozycj� wzgl�dem obiektu gracza i zmienia warto�ci ruchu, je�li nagradzany jest Perfect Clear.
	/// @param type Numer reprezentuj�cy typ aplauzu.
	///	@param f Referencja na czcionk� do u�ycia w tek�cie.
	///	@param p Wska�nik na obiekt gracza.
	void init(int type, sf::Font &f, Player* p);

	/// Metoda updateVis
	///
	/// Je�eli czas �ycia aplauzu jest mniejszy ni� 3 sekundy, animuje tekst poprzez animacj� znak�w tekstu.
	/// Zmienia pr�dko�� ruchu i zanikania o warto�� wyliczon� z op�nienia i czasu klatki.
	/// Je�eli aplauz �y� przez 3 sekundy, czy�ci go przez clear();
	///	@param t Czas klatki w ms.
	void updateVis(int t);

};