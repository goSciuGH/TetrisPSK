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

// Definicja sta³ych wartoœci gry

int DAS_DEFAULT = 170;	// DAS - Delayed Auto Shift - iloœæ czasu w ms, przez jak¹ trzeba trzymaæ przucisk, ¿eby tetrimino zaczê³o siê poruszaæ ze sta³¹ prêdkoœci¹
int ARR_DEFAULT = 50;	// ARR - Auto Repeat Rate - iloœæ czasu w ms pomiêdzy kolejnymi krokami tetrimina w bok
int SDF_DEFAULT = 20;	// SDF - Soft Drop Factor - mno¿nik standardowej prêdkoœci opadania tetrimina przy soft-dropie
int LOCK_TIME = 500;	// czas w ms, jaki tetrimino musi spêdziæ po nieudanej próbie opuszczenia w dó³, by zostaæ zablokowane w miejscu
int LOCK_RESET = 15;	// maksymalna iloœæ resetów czasomierza lock-in, zapobiegaj¹ca nieskoñczonemu unikaniu zalokowania
int SPAWN_RESET = 117;	// czas w ms od zalokowania Tetrimina, po którym generowane jest nowe

int offsetChart[3][4][5][2]; // Tabela obrótów - [zestaw][kierunek][nr testu][x lub y]

std::map <std::string, sf::Keyboard::Key> dftKey;

int hiScore = 0;

// Funkcje

/// Funkcja defineOffset
///
/// S³u¿y do zapisania w pamiêci przesuniêæ Tetrimina podczas testów obrotów.
/// Kolejne liczby w nawiasach kwadratowych reprezentuj¹ kolejno:
///		Zestaw Tetrimin: 0 - odpowiada Tetriminom J, L, S, Z oraz T; 1 - odpowiada Tetriminu O; 2 - odpowiada Tetriminu I;
///		Stan obrotu: 0 - pozycja startowa; 1 - obrót w prawo; 2 - dwa obroty; 3 - obrót w lewo;
///		Numer testu
///		Oœ: 0 - X; 1 - Y;
/// @param t Tablica przesuniêæ obrotu.
void defineOffset(int t[3][4][5][2]);