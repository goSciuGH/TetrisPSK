#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>

class pQueue;

int FULLRES_W = 1920;	///< Szeroko�� widoku w px.
int FULLRES_H = 1080;	///< Wysoko�� widoku w px.

bool fullScreen = false;	///< Opis stanu pe�nego okna.

double windowScale = 1.0 / 2.0;				///< Skala wy�wietlanego okna.
int windowW = FULLRES_W * windowScale;		///< Szeroko�� okna w px.
int windowH = FULLRES_H * windowScale;		///< Wysoko�� okna w px.

std::fstream hiScoF;						///< Struumie� do pliku zapisu.

// Definicja sta�ych warto�ci gry

int DAS_DEFAULT = 170;	///< DAS - Delayed Auto Shift - ilo�� czasu w ms, przez jak� trzeba trzyma� przucisk, �eby tetrimino zacz�o si� porusza� ze sta�� pr�dko�ci�.
int ARR_DEFAULT = 50;	///< ARR - Auto Repeat Rate - ilo�� czasu w ms pomi�dzy kolejnymi krokami tetrimina w bok.
int SDF_DEFAULT = 20;	///< SDF - Soft Drop Factor - mno�nik standardowej pr�dko�ci opadania Tetrimina przy Foft Dropie.
int LOCK_TIME = 500;	///< Czas w ms, jaki tetrimino musi sp�dzi� po nieudanej pr�bie opuszczenia w d�, by zosta� zablokowane w miejscu.
int LOCK_RESET = 15;	///< Maksymalna ilo�� reset�w czasomierza lokowania, zapobiegaj�ca niesko�czonemu unikaniu zalokowania.
int SPAWN_RESET = 117;	///< Czas w ms od zalokowania Tetrimina, po kt�rym generowane jest nowe.

int offsetChart[3][4][5][2]; ///< Tabela obr�t�w - [zestaw][kierunek][nr testu][x lub y]

std::map <std::string, sf::Keyboard::Key> dftKey; ///< Mapa domy�lnych klawiszy sterowania.

int hiScore = 0; ///< Najlepszy osi�gni�ty wynik.

// Funkcje

/// S�u�y do zapisania w pami�ci przesuni�� Tetrimina podczas test�w obrot�w.<br>
/// Kolejne liczby w nawiasach kwadratowych reprezentuj� kolejno:<br>
///	&emsp;Zestaw Tetrimin: 0 - odpowiada Tetriminom J, L, S, Z oraz T; 1 - odpowiada Tetriminu O; 2 - odpowiada Tetriminu I;<br>
///	&emsp;Stan obrotu: 0 - pozycja startowa; 1 - obr�t w prawo; 2 - dwa obroty; 3 - obr�t w lewo;<br>
///	&emsp;Numer testu<br>
///	&emsp;O�: 0 - X; 1 - Y;
/// @param t Tablica przesuni�� obrotu.
void defineOffset(int t[3][4][5][2]);