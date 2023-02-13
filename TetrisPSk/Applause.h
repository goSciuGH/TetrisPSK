#pragma once

#include <SFML/Graphics.hpp>

class Player;

/// Klasa Applause
///
/// Jest to graficzny obiekt, który ma gratulowaæ graczowi dokonanych ruchów poprzez wyœwietlanie ich nazw na ekranie.
/// Klasa posiada 9 pól:
///		txt[11] - tablica obiektów klasy sf::Text
///		len - d³ugoœæ wyœwietlanego tekstu
///		aType - typ wyœwietlanego aplauzu
///		lifeTime - czasomierz wyœwietlania aplauzu
///		spd - prêdkoœæ ruchu pierwszego od brzegu ruchomego znaku; skalowane dla kolejnych znaków
///		acc - opóŸnienie ruchu dla znaków jak wy¿ej
///		spdV - prêdkoœæ zmiany widocznoœci
///		accV - opóŸnienie zmiany widocznoœci
///		vis - widocznoœæ znaku
/// Klasa posiada 3 metody:
///		clear() - zeruje parametry, by ukryæ aplauz
///		init() - inicjuje parametry jak w konstruktorze, ustawia typ aplauzu i jego tekst, jeœli zachodzi taka potrzeba
///		updateVis() - aktualizuje reprezentacjê graficzn¹ aplauzu

class Applause
{
public:
	/// Konstruktor Applause()
	///
	/// Ustawia parametry aplauzu na pocz¹tkowe, ustawia pusty tekst/
	Applause();

	sf::Text txt[11];
	int len;
	int aType;
	int lifeTime = 0;
	double spd = 16.0 / 3.0;	// prêdkoœæ pocz¹tkowa ruchu dla drugiego znaku od matrixa (w px/s)
	double acc = 16.0 / 9.0;	// opóŸnienie dla drugiego znaku od matrixa (w px/s^2)
	double spdV = 340.0;		// prêdkoœæ pocz¹tkowa zanikania (w b/s)
	double accV = 680.0 / 3.0;	// opóŸnienie zanikania (w b/s^2)
	double vis = 255;

	/// Metoda clear()
	///
	///	Czyœci aplauz, by przypomina³ œwie¿o utworzony obiekt.
	void clear();

	/// Metoda init()
	///
	/// Ustawia tekst zale¿ny od typu aplauzu, ustawia jego pozycjê wzglêdem obiektu gracza i zmienia wartoœci ruchu, jeœli nagradzany jest Perfect Clear.
	/// @param type Numer reprezentuj¹cy typ aplauzu.
	///	@param f Referencja na czcionkê do u¿ycia w tekœcie.
	///	@param p WskaŸnik na obiekt gracza.
	void init(int type, sf::Font &f, Player* p);

	/// Metoda updateVis
	///
	/// Je¿eli czas ¿ycia aplauzu jest mniejszy ni¿ 3 sekundy, animuje tekst poprzez animacjê znaków tekstu.
	/// Zmienia prêdkoœæ ruchu i zanikania o wartoœæ wyliczon¹ z opóŸnienia i czasu klatki.
	/// Je¿eli aplauz ¿y³ przez 3 sekundy, czyœci go przez clear();
	///	@param t Czas klatki w ms.
	void updateVis(int t);

};