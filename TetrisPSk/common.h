#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>

class pQueue;

int FULLRES_W = 1920;	///< Szerokoœæ widoku w px.
int FULLRES_H = 1080;	///< Wysokoœæ widoku w px.

bool fullScreen = false;	///< Opis stanu pe³nego okna.

double windowScale = 1.0 / 2.0;				///< Skala wyœwietlanego okna.
int windowW = FULLRES_W * windowScale;		///< Szerokoœæ okna w px.
int windowH = FULLRES_H * windowScale;		///< Wysokoœæ okna w px.

std::fstream hiScoF;						///< Struumieñ do pliku zapisu.

// Definicja sta³ych wartoœci gry

int DAS_DEFAULT = 170;	///< DAS - Delayed Auto Shift - iloœæ czasu w ms, przez jak¹ trzeba trzymaæ przucisk, ¿eby tetrimino zaczê³o siê poruszaæ ze sta³¹ prêdkoœci¹.
int ARR_DEFAULT = 50;	///< ARR - Auto Repeat Rate - iloœæ czasu w ms pomiêdzy kolejnymi krokami tetrimina w bok.
int SDF_DEFAULT = 20;	///< SDF - Soft Drop Factor - mno¿nik standardowej prêdkoœci opadania Tetrimina przy Foft Dropie.
int LOCK_TIME = 500;	///< Czas w ms, jaki tetrimino musi spêdziæ po nieudanej próbie opuszczenia w dó³, by zostaæ zablokowane w miejscu.
int LOCK_RESET = 15;	///< Maksymalna iloœæ resetów czasomierza lokowania, zapobiegaj¹ca nieskoñczonemu unikaniu zalokowania.
int SPAWN_RESET = 117;	///< Czas w ms od zalokowania Tetrimina, po którym generowane jest nowe.

int offsetChart[3][4][5][2]; ///< Tabela obrótów - [zestaw][kierunek][nr testu][x lub y]

std::map <std::string, sf::Keyboard::Key> dftKey; ///< Mapa domyœlnych klawiszy sterowania.

int hiScore = 0; ///< Najlepszy osi¹gniêty wynik.

// Funkcje

/// S³u¿y do zapisania w pamiêci przesuniêæ Tetrimina podczas testów obrotów.<br>
/// Kolejne liczby w nawiasach kwadratowych reprezentuj¹ kolejno:<br>
///	&emsp;Zestaw Tetrimin: 0 - odpowiada Tetriminom J, L, S, Z oraz T; 1 - odpowiada Tetriminu O; 2 - odpowiada Tetriminu I;<br>
///	&emsp;Stan obrotu: 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo;<br>
///	&emsp;Numer testu<br>
///	&emsp;Oœ: 0 - X; 1 - Y;
/// @param t Tablica przesuniêæ obrotu.
void defineOffset(int t[3][4][5][2]);