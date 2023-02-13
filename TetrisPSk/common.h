#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <map>

class pQueue;

int FULLRES_W = 1920;
int FULLRES_H = 1080;

bool fullScreen = false;

double windowScale = 1.0 / 2.0;
int windowW = FULLRES_W * windowScale;
int windowH = FULLRES_H * windowScale;

std::fstream hiScoF;

// Definicja sta�ych warto�ci gry

int DAS_DEFAULT = 170;	// DAS - Delayed Auto Shift - ilo�� czasu w ms, przez jak� trzeba trzyma� przucisk, �eby tetrimino zacz�o si� porusza� ze sta�� pr�dko�ci�
int ARR_DEFAULT = 50;	// ARR - Auto Repeat Rate - ilo�� czasu w ms pomi�dzy kolejnymi krokami tetrimina w bok
int SDF_DEFAULT = 20;	// SDF - Soft Drop Factor - mno�nik standardowej pr�dko�ci opadania tetrimina przy soft-dropie
int LOCK_TIME = 500;	// czas w ms, jaki tetrimino musi sp�dzi� po nieudanej pr�bie opuszczenia w d�, by zosta� zablokowane w miejscu
int LOCK_RESET = 15;	// maksymalna ilo�� reset�w czasomierza lock-in, zapobiegaj�ca niesko�czonemu unikaniu zalokowania
int SPAWN_RESET = 117;	// czas w ms od zalokowania Tetrimina, po kt�rym generowane jest nowe

int offsetChart[3][4][5][2]; // Tabela obr�t�w - [zestaw][kierunek][nr testu][x lub y]

std::map <std::string, sf::Keyboard::Key> dftKey;

int hiScore = 0;

// Funkcje

/// Funkcja defineOffset
///
/// S�u�y do zapisania w pami�ci przesuni�� Tetrimina podczas test�w obrot�w.
/// Kolejne liczby w nawiasach kwadratowych reprezentuj� kolejno:
///		Zestaw Tetrimin: 0 - odpowiada Tetriminom J, L, S, Z oraz T; 1 - odpowiada Tetriminu O; 2 - odpowiada Tetriminu I;
///		Stan obrotu: 0 - pozycja startowa; 1 - obr�t w prawo; 2 - dwa obroty; 3 - obr�t w lewo;
///		Numer testu
///		O�: 0 - X; 1 - Y;
/// @param t Tablica przesuni�� obrotu.
void defineOffset(int t[3][4][5][2]);