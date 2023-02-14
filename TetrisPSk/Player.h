#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class pQueue;
class Queue;
class Mino;
class MinoAnchor;
class Applause;

/// Klasa reprezentuj�ca gracza i nale��ce do niego elementy gry.
class Player : public sf::Sprite
{
public:
	/// Konstruktor klasy.<br> Inicjuje obiekt przez ustawienie pocz�tkowych warto�ci.<br>
	/// Znajduje si� tu m.in. przypisanie tekstury, wyznaczenie pozycji na ekranie, utworzenie obiekt�w aplauzu i statystyk.
	/// @param t Referencja na tekstur� Matrixu.
	/// @param f Referencja na czcionk� tekstu.
	/// @param t2 Referencja na tekstur� debugowania klawiszy.
	Player(sf::Texture &t, sf::Font &f, sf::Texture &t2);

	int id;	///< Numer ID gracza.
	std::map <std::string, sf::Keyboard::Key> ctrlKey;	///< Mapa osobistych ustawie� klawiszy sterowania.

	bool active;	///< Stan aktywno�ci gry.
	bool gameOver;	///< Stan zakonczenia gry.

	Mino* matrix[10][40];		///< Tablica wska�nik�w na Mina, b�d�ca reprezentacj� Matrixu.
	MinoAnchor* activeAnchor;	///< Wska�nik na aktywne, sterowalne Tetrimino.

	sf::Sprite holdBox[2];		///< Tablica duszk�w przestrzeni Schowka i obra�e�.
	sf::Sprite qBox;			///< Duszek przestrzeni podgl�du kolejki.

	int countdown = -1;			///< Czasomierz odliczania do pocz�tku gry.

	int level;					///< Poziom gry.
	int score;					///< Wynik punktowy gracza.
	sf::Time time;				///< Czas trwania gry.
	int pcsLocked = 0;			///< Licznik zalokowanych Tetrimin.

	int hold;					///< Numer reprezentuj�cy Tetrimino w Schowku.
	bool holdActive;			///< Stan aktywno�ci Schowka.
	bool holdSwap;				///< Stan wymiany Tetrimina ze Schowkiem.

	int combo;					///< Licznik Combo.
	int back2back;				///< Licznik Back-2-Back.
	int tSpin;					///< Stan wyst�pienia T-Spina: -1 - nie wyst�pi�, 0 - mini, 1 - wyst�pi�.
	bool tsMiniEligible = true;	///< Stan mo�liwo�ci wyst�pienia T-Spina Mini.
	int toClear[4];				///< Tablica numer�w linii do wyczyszczenia.
	int totalCleared;			///< Licznik usuni�tych linii.

	bool ctrlState[8];			///< Tablica stan�w wci�ni�cia klawiszy, kolejno: Lewo, Prawo, Obr�t lewo, Obr�t prawo, Soft Drop, Hard Drop, Hold, Pauza

	int sideMoveTime = 0;		///< Czasomierz ruchu w bok.
	int softDropTime = 0;		///< Czasomierz opadania/Soft Dropu.
	double softDropUpdate;		///< Czas wymagany do opuszczenia Tetrimina.
	int lockdownTime = -1;		///< Czasomierz lokowania.
	int lockdownReset = 0;		///< Licznik reset�w czasomierza lokowania.
	int spawnTime = -1;			///< Czasomierz generowania nowego Tetrimina.
	int domDir = 0;				///< Stan dominuj�cego kierunku ruchu w bok - -1 to lewo, 1 to prawo

	int pcQueue[5];				///< Tablica osobistej kolejki Tetrimin.

	sf::Text txt_stats[6];		///< Tablica tekst�w statystyk, kolejno: tryb, najlepszy rezultat, wynik, czas, linie, poziom
	sf::Sprite keyDebug[7];		///< Tablica duszk�w do debugownia wci�ni�� klawiszy.

	Applause* appl[6];			///< Tablica obiekt�w aplauzu.

	/// Generowanie nowego Tetrimina.
	/// Funkcja sprawdza czy podana kolejka zawiera numer kszta�tu Mina i tworzy Tetrimino na jego podstawie, nast�pnie wywo�uje aktualizacj� kolejki.
	/// Je�eli kolejka jest NULLem, tworzone jest Tetrimino na podstawie numeru kszta�tu ze schowka.
	/// @param q Wska�nik na kolejk�.
	///	@param t Referencja na tekstur�.
	void genPiece(pQueue* q, sf::Texture &t); // stw�rz nowe tetrimino

	/// Aktualizacja obsobistej kolejki.
	///	Pobiera numer przechowywany na przodzie podanej w parametrze kolejki, odkolejkuj�c go i umieszczaj�c numer na ko�cu w�asnej tablicy kolejki.
	/// Je�eli ID pobranego elementu kolejki by�o podzielne przez 7 (pobrana zoosta�a ca�a torba), wywo�ywane jest dodanie nowej torby do kolejki og�lnej.
	/// @param q Wska�nik na kolejk�.
	void updateQueue(pQueue* q);

	/// Obr�cenie aktywnego Tetrimina.
	/// W ka�dym Minie nieb�d�cym Minem-Kotwic� wywo�ywany jest obr�t ich pozycji. Nast�pnie wykonywane jest do 5 test�w obrotu.
	/// Ka�dy test polega na odj�ciu warto�ci przesuni�cia kierunku docelowego od warto�ci przesuni�cia kierunku wej�ciowego.
	/// W przypadku sukcesu, przesuni�cie i obr�t zostaj� zachowane; w przeciwnym, nast�puje powr�t do pozycji wej�ciowej.
	/// @param cw Bool opisuj�cy kierunek obrotu. True reprezentuje obr�t w kierunku wskaz�wek zegara, false - w przeciwnym.
	void rotate(bool cw);

	/// Aktualizacja ruchu Tetrimina na podstawie wci�ni�� klawiszy.
	/// Jako pierwsze testowane jest wci�ni�cie klawisza Schowka. Je�eli klatk� wcze�niej nie by� on wci�ni�ty, dokonuje si� zapisania warto�ci kszta�tu do Schowka i usuni�cie obecnego Tetrimina. Je�eli jakas warto�� by�a wcze�niej, nowe Tetrimino jest tworzone z tej warto�ci. Metoda nie jest dalej wykonywana.
	/// Nast�pnie testowane jest wci�ni�cie klawisza Hard Dropu. Je�eli klatk� wcze�niej nie by� on wci�ni�ty, Tetrimino jest przesuwane o 1 pole w d� w p�tli do napotkania przeszkody. Za ka�de pole przyznawane s� punkty. Metoda nie jest dalej wykonywana.
	/// Nast�pnie testowane s� wci�ni�cia klawiszy ruchu w bok. Je�eli klatk� wcze�niej dany przycisk nie by� wci�ni�ty, nast�puje natychmiastowe przesuni�cie Tetrimina w danym kierunku. Je�eli klawisz by� trzymany klatk� wcze�niej, dodawany jest do czasomierza ruchu czas klatki, a gdy czasomierz przekroczy wymagan� warto��, Tetrimino jest przesuwane ponownie. Dodanie wci�ni�cia przeciwnego klawisza ruchu zmienia tymczasowo kierunke ruchu.
	/// Nast�pnie testowane s� wci�ni�cia klawiszy obrotu. Je�eli klatk� wcze�niej, dany klawisz nie by� wci�ni�ty, wywo�ywana jest metoda obrotu.
	/// Ostatecznie testowane jest wci�ni�cie klawisza Soft Dropu. Je�eli nie jest wci�ni�ty, do czasomierza opadania dodawany jest czas klatki, je�eli jest wci�ni�ty, dodawana jest wielokrotno�� czasu klatki.
	/// Ka�dy z powy�szych test�w ko�czy si� zapisaniem stanu wci�ni�cia klawisza do tabeli.
	/// Nast�pnym krokiem jest opuszczanie Tetrimina. Je�eli czasomierz przekroczy� wymagany okres, warto�� tego kresu jest odejmowana, a Tetrimino jest opusczane w d� o 1 pole, lub dodany zostaje czas lokowania, je�li nie mo�na opu�ci� Tetrimina. Za ka�de pole, dodawane s� punkty do wyniku. Je�eli czasomierz nadal przekracza granic�, metoda ctrlDo jest ponownie wywo�ywana, z parametrem czasu klatki r�wnym zero.
	/// Na koniec, aktualizowana jest reprezentacja graficzna aktywnego Tetrimina.
	/// @param t Czas klatki w ms.
	/// @param ttr Referencja na tekstur� Min.
	/// @param s Tablica kana��w d�wi�ku, do odgrywania efekt�w d�wi�kowych.
	/// @param sb Tablica wska�nik�w na efekty d�wi�kowe.
	void ctrlDo(int t, sf::Texture &ttr, sf::Sound s[], sf::SoundBuffer* sb[]);

	/// Srawdzenie poprawno�ci wy�wietlania Min w Matrixie.
	/// Metoda przechodzi przez ka�de pole Matrixu w poszukiwaniu Min, kt�re nie s� gotowe do wy�wietlenia. Ka�demu z takich Min przypisywana jest nowa tekstura.
	/// @param t Referencja na tekstur� Min.
	void checkVisuals(sf::Texture &t);

	/// Rozpoznanie T-Spin�w.
	/// Sprawdzenie czy dany obr�t jest T-Spinem. W zale�no�ci od kierunku Tetrimina T, testowane s� r�ne pola Matrixu.
	/// Je�eli pola po ka�dym boku wypustki Tetrimina T i przynajmniej jedno z p�l za ko�cami p�askiej cz�ci Tetrimina T s� zaj�te, zachodzi pe�ny T-Spin (zwracana jest warto�� 1).
	/// W przeciwnym wypadku, je�eli pola za ka�dym ko�cem p�askiej cz�ci Tetrimina T (lub znajduje si� tam �ciana Matrixa) i przynajmniej jedno z p�l po bokach wypustki Tetrimina T s� zaj�te, zachodzi T-Spin Mini (zwracana jest warto�� 0).
	/// W przeciwnym wypadku, nie zachodzi T-Spin (zwracana jest warto�� -1).
	int checkTSpin();

	/// S�u�y do przeniesienia informacji o Tetriminie do Matrixu w celu zalokowania.
	/// Zaczyna si� od zwi�kszenia liczby zalokowanych Tetrimin, ustawieniu gotowo�ci Tetrimina do lokowania i sprawdzeniu czy zaszed� T-Spin.
	/// Ostatecznie usuwa aktywne Tetrimino, sprawdza reprezentacj� graficzn� nowych Min w Matrixie i sprawdza, czy zachodzi potrzeba usuni�cia li�.
	/// @param t Referencja na tekstur� Min.
	void lockIn(sf::Texture &t);

	/// Sprawdza, czy wymagane jest czyszczenie linii.
	/// Funkcja przechodzi przez wszystkie wiersze Matrixa, je�eli znajdzie wiersz, w kt�rym nie ma pustego miejsca, zapisuje jego numer do tablicy.
	void checkLines();

	/// Usuwa zape�nione linie i przyznaje nale�ne punkty.
	/// Funkcja podgl�da list� linii do usuni�cia z tablicy utworzonej przez checkLines(), usuwa Mina z tych linii i przesuwa wszystkie linie powy�ej o jedno pole w d�.
	/// Liczba usuwanych linii jest dodawana do licznika usuni�tych linii og�em, a je�li ta przekroczy wielokrotno�� 10, zwi�ksza poziom gry.
	/// W zale�no�ci od ilo�ci wyliczonych linii i innych wykonanych ruch�w (Combo, Back-2-Back, T-Spiny, Perfect Clear) przyznawana jest nale�na liczba punkt�w.
	/// Na koniec, aktualizowane s� teksty statystyk do wy�wietlenia na ekranie i wype�niane s� obiekty aplauzu dla gracza.
	/// @param f Referencja na czcionk� tekstu.
	/// @param s Tablica kana��w d�wi�k�w.
	/// @param sb Tablica wska�nik�w na efekty d�wi�kowe
	void clearLines(sf::Font &f, sf::Sound s[], sf::SoundBuffer* sb[]);

	/// Sprawdza czy dana pozycja mo�e by� okupowana przez aktywne Tetrimino.
	/// Funkcja kopiuje pozycje XY Min Tetrimina do w�asnych tabel i dodaje do nich przesuni�cie z parametru.
	/// Najpierw sprawdza czy warto�ci mieszcz� si� w granicach Matrixa, a nast�pnie czy poszukiwane miejsca nie s� zaj�te przez istniej�ce Mino.
	/// Zwraca true je�li nie wyst�puje kolizja.
	/// @param xSh Przesuni�cie w osi x.
	/// @param ySh Przesuni�cie w osi y.
	bool checkPos(int xSh, int ySh);

	/// Zwraca now� warto�� pr�dko�ci grawitacji.
	void sdropCalc();

	/// Rysuje podgl�d kolejki Tetrimin.
	/// @param w Referencja na okno gry.
	/// @param t Referencja na tekstur� Tetrimin.
	void drawPreview(sf::RenderWindow &w, sf::Texture &t);

	/// Funkcja debugowa. Rysuje podgl�d tabeli wci�ni�� klawiszy w dolnym lwym rogu ekranu.
	void keyTest(sf::RenderWindow &w);

	/// Rysuje wi�kszo�� element�w graficznych zwi�zanych z obiektem gracza, kolejno: statystyki, ducha Tetrimina, aplauzy, aktywne Tetrimino i zawarto�� Matrixa
	void drawMatrix(sf::RenderWindow &w, sf::Texture &t);
};