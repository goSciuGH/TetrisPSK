#pragma once

#include <SFML/Graphics.hpp>

class Player;

/// Jest to graficzny obiekt, który ma gratulowaæ graczowi dokonanych ruchów poprzez wyœwietlanie ich nazw na ekranie.
class Applause
{
public:
	/// Konstruktor klasy.<br>Ustawia parametry aplauzu na pocz¹tkowe, ustawia pusty tekst.
	Applause();

	sf::Text txt[11];			///< Tablica obiektów tesktowych do zwaarcia tekstu aplauzu.
	int len;					///< D³ugoœæ tekstu.
	int aType;					///< Typ wyœwietlanego aplauzu.
	int lifeTime = 0;			///< Czasomierz trwania aplauzu.
	double spd = 16.0 / 3.0;	///< Prêdkoœæ pocz¹tkowa ruchu dla drugiego znaku od matrixa (w px/s).
	double acc = 16.0 / 9.0;	///< OpóŸnienie dla drugiego znaku od matrixa (w px/s^2).
	double spdV = 340.0;		///< Prêdkoœæ pocz¹tkowa zanikania (w b/s).
	double accV = 680.0 / 3.0;	///< OpóŸnienie zanikania (w b/s^2).
	double vis = 255;

	///	Czyœci aplauz, by przypomina³ œwie¿o utworzony obiekt.
	void clear();

	/// Ustawia tekst zale¿ny od typu aplauzu, ustawia jego pozycjê wzglêdem obiektu gracza i zmienia wartoœci ruchu, jeœli nagradzany jest Perfect Clear.
	/// @param type Numer reprezentuj¹cy typ aplauzu.
	///	@param f Referencja na czcionkê do u¿ycia w tekœcie.
	///	@param p WskaŸnik na obiekt gracza.
	void init(int type, sf::Font &f, Player* p);

	/// Je¿eli czas ¿ycia aplauzu jest mniejszy ni¿ 3 sekundy, animuje tekst poprzez animacjê znaków tekstu.<br>
	/// Zmienia prêdkoœæ ruchu i zanikania o wartoœæ wyliczon¹ z opóŸnienia i czasu klatki.<br>
	/// Je¿eli aplauz ¿y³ przez 3 sekundy, czyœci go przez clear();
	///	@param t Czas klatki w ms.
	void updateVis(int t);

};