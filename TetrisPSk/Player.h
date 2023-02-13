#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class pQueue;
class Queue;
class Mino;
class MinoAnchor;
class Applause;

/// Klasa Player
///
/// Klasa reprezentuj�ca gracza i nale��ce do niego elementy gry.
/// Klasa zawiera 34 pola:
///		id - numer gracza
///		ctrlKey - osobiste ustawienia klawiszy sterowania
///		active - reprezentacja stanu aktywno�ci gry
///		gameOver - reprezentacja stanu ko�ca gry
///		*matrix[10][40] - tablica wska�nik�w na Mina zawarte w Matrixie
///		*activeAnchor - wska�nik na aktywne Mino-Kotwic�
///		holdBox[2] - tablica duszk�w reprezentuj�cych przestrze� na schowek i kolejk� obra�e�
///		qBox - duszek reprezentuj�cy przestrze� na kolejk�
///		countdown - czasomierz odliczania do rozpocz�cia gry
///		level - poziom gry
///		score - punkty zdobyte przez gracza
///		time - czasomierz gry
///		pcsLocked - liczba zalokowanych Tetrimin
///		hold - warto�� kszta�tu Tetrimina w schowku
///		holdActive - reprezentacja aktywno�ci schowka
///		holdSwap - reprezentacja mo�liwo�ci zamiany Tetrimin pomi�dzy sterowalnym a schowkiem
///		combo - d�ugo�� Combo
///		back2back - d�ugo�� Back-2-Back
///		tSpin - reprezentacja zaj�cia T-Spina
///		tsMiniEligible - reprezentacja czy T-Spin Mini mo�e zaj��
///		toClear[4] - tablica numer�w linii do usuni�cia
///		totalCleared - liczba usuni�tych linii
///		ctrlState[8] - tablica przechowuj�ca stany wci�ni�cia klawiszy z ostatniej klatki
///		sideMoveTime - czasomierz przyci�ni�cia klawisza ruchu w bok
///		softDropTime - czasomierz opadania Tetrimina
///		softDropUpdate - czas potrzebny na opuszczenie Tetrimina
///		lockdownTime - czasomierz lokowania Tetrimina
///		lockdownReset - licznik reset�w czasomierza lokowania
///		spawnTime - czasomierz spawnoania nowego Tetrimina
///		domDir - dominuj�cy kierunek ruchu w bok
///		pcQueue[5] - osobista kolejka Tetrimin
///		txt_stats[6] - tablica tekst�w statystyk
///		keyDebug[7] - tablica duszk�w debugowania wci�ni�� klawiszy
///		*appl[6] - tablica wska�nik�w na teksty-aplauzy
/// Klasa zawiera 14 metod:
///		genPiece() - generowanie nowego Tetrimina
///		updateQueue() - aktualizacja osobistej kolejki
///		rotate() - obr�t atywnego Tetrimina
///		ctrlDo() - aktualizacja ruchu Tetrimina wg wci�ni�� klawiszy
///		checkVisuals() - sprawdzenie poprawno�ci wy�wietlania Min w Matrixie
///		checkTSpin() - sprawdzenie zaj�cia T-Spina
///		lockIn() - zalokowanie aktywnego Tetrimina
///		checkLines() - wyszukiwanie linii do usuni�cia
///		checkPos() - sprawdzenie, czy dane miejsce w Matrixie jest wolne
///		sdropCalc() - aktualizacja grawitacji
///		drawPreview() - rysowanie kolejki
///		keyTest() - rysowanie reprezentacji wci�ni�cia klawiszy (debug)
///		drawMatrix() - rysowanie wszystkich zwi�zanych z obiektem gracza element�w
class Player : public sf::Sprite
{
public:
	Player(sf::Texture &t, sf::Font &f, sf::Texture &t2);

	int id;
	std::map <std::string, sf::Keyboard::Key> ctrlKey;

	bool active;
	bool gameOver;

	Mino* matrix[10][40];
	MinoAnchor* activeAnchor;

	sf::Sprite holdBox[2];
	sf::Sprite qBox;

	int countdown = -1;

	int level;
	int score;
	sf::Time time;
	int pcsLocked = 0;

	int hold;
	bool holdActive;
	bool holdSwap;

	int combo;
	int back2back;
	int tSpin;	// -1 - nie wyst�pi�, 0 - mini, 1 - wyst�pi�
	bool tsMiniEligible = true;
	int toClear[4];
	int totalCleared;

	bool ctrlState[8]; // kolejno: Lewo, Prawo, Obr�t lewo, Obr�t prawo, Soft Drop, Hard Drop, Hold, Pauza

	int sideMoveTime = 0;
	int softDropTime = 0;
	double softDropUpdate;
	int lockdownTime = -1;
	int lockdownReset = 0;
	int spawnTime = -1;
	int domDir = 0; // dominuj�cy kierunek - -1 to lewo, 1 to prawo

	int pcQueue[5];

	sf::Text txt_stats[6]; // mode, best, score, time, lines, lvl
	sf::Sprite keyDebug[7];

	Applause* appl[6];

	/// Metoda genPiece()
	///
	/// Generowanie nowego Tetrimina.
	/// Funkcja sprawdza czy podana kolejka zawiera numer kszta�tu Mina i tworzy Tetrimino na jego podstawie, nast�pnie wywo�uje aktualizacj� kolejki.
	/// Je�eli kolejka jest NULLem, tworzone jest Tetrimino na podstawie numeru kszta�tu ze schowka.
	/// @param q Wska�nik na kolejk�.
	///	@param t Referencja na tekstur�.
	void genPiece(pQueue* q, sf::Texture &t); // stw�rz nowe tetrimino
	
	/// Metoda updateQueue()
	///
	/// Aktualizacja obsobistej kolejki.
	///	Pobiera numer przechowywany na przodzie podanej w parametrze kolejki, odkolejkuj�c go i umieszczaj�c numer na ko�cu w�asnej tablicy kolejki.
	/// Je�eli ID pobranego elementu kolejki by�o podzielne przez 7 (pobrana zoosta�a ca�a torba), wywo�ywane jest dodanie nowej torby do kolejki og�lnej.
	/// @param q Wska�nik na kolejk�.
	void updateQueue(pQueue* q);

	/// Metoda rotate()
	///
	/// Obr�cenie aktywnego Tetrimina.
	/// W ka�dym Minie nieb�d�cym Minem-Kotwic� wywo�ywany jest obr�t ich pozycji. Nast�pnie wykonywane jest do 5 test�w obrotu.
	/// Ka�dy test polega na odj�ciu warto�ci przesuni�cia kierunku docelowego od warto�ci przesuni�cia kierunku wej�ciowego.
	/// W przypadku sukcesu, przesuni�cie i obr�t zostaj� zachowane; w przeciwnym, nast�puje powr�t do pozycji wej�ciowej.
	/// @param cw Bool opisuj�cy kierunek obrotu. True reprezentuje obr�t w kierunku wskaz�wek zegara, false - w przeciwnym.
	void rotate(bool cw);

	/// Metoda ctrlDo()
	///
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

	/// Metoda checkVisuals()
	///
	/// Srawdzenie poprawno�ci wy�wietlania Min w Matrixie.
	/// Metoda przechodzi przez ka�de pole Matrixu w poszukiwaniu Min, kt�re nie s� gotowe do wy�wietlenia. Ka�demu z takich Min przypisywana jest nowa tekstura.
	/// @param t Referencja na tekstur� Min.
	void checkVisuals(sf::Texture &t);

	/// Metoda checkTSpin()
	///
	/// Rozpoznanie T-Spin�w.
	/// Sprawdzenie czy dany obr�t jest T-Spinem. W zale�no�ci od kierunku Tetrimina T, testowane s� r�ne pola Matrixu.
	/// Je�eli pola po ka�dym boku wypustki Tetrimina T i przynajmniej jedno z p�l za ko�cami p�askiej cz�ci Tetrimina T s� zaj�te, zachodzi pe�ny T-Spin (zwracana jest warto�� 1).
	/// W przeciwnym wypadku, je�eli pola za ka�dym ko�cem p�askiej cz�ci Tetrimina T (lub znajduje si� tam �ciana Matrixa) i przynajmniej jedno z p�l po bokach wypustki Tetrimina T s� zaj�te, zachodzi T-Spin Mini (zwracana jest warto�� 0).
	/// W przeciwnym wypadku, nie zachodzi T-Spin (zwracana jest warto�� -1).
	int checkTSpin();

	/// Metoda lockIn()
	///
	/// S�u�y do przeniesienia informacji o Tetriminie do Matrixu w celu zalokowania.
	/// Zaczyna si� od zwi�kszenia liczby zalokowanych Tetrimin, ustawieniu gotowo�ci Tetrimina do lokowania i sprawdzeniu czy zaszed� T-Spin.
	/// Ostatecznie usuwa aktywne Tetrimino, sprawdza reprezentacj� graficzn� nowych Min w Matrixie i sprawdza, czy zachodzi potrzeba usuni�cia li�.
	/// @param t Referencja na tekstur� Min.
	void lockIn(sf::Texture &t);

	/// Metoda checkLines()
	///
	/// Sprawdza, czy wymagane jest czyszczenie linii.
	/// Funkcja przechodzi przez wszystkie wiersze Matrixa, je�eli znajdzie wiersz, w kt�rym nie ma pustego miejsca, zapisuje jego numer do tablicy.
	void checkLines();

	/// Metoda clearLines()
	///
	/// Usuwa zape�nione linie i przyznaje nale�ne punkty.
	/// Funkcja podgl�da list� linii do usuni�cia z tablicy utworzonej przez checkLines(), usuwa Mina z tych linii i przesuwa wszystkie linie powy�ej o jedno pole w d�.
	/// Liczba usuwanych linii jest dodawana do licznika usuni�tych linii og�em, a je�li ta przekroczy wielokrotno�� 10, zwi�ksza poziom gry.
	/// W zale�no�ci od ilo�ci wyliczonych linii i innych wykonanych ruch�w (Combo, Back-2-Back, T-Spiny, Perfect Clear) przyznawana jest nale�na liczba punkt�w.
	/// Na koniec, aktualizowane s� teksty statystyk do wy�wietlenia na ekranie i wype�niane s� obiekty aplauzu dla gracza.
	/// @param f Referencja na czcionk� tekstu.
	/// @param s Tablica kana��w d�wi�k�w.
	/// @param sb Tablica wska�nik�w na efekty d�wi�kowe
	void clearLines(sf::Font &f, sf::Sound s[], sf::SoundBuffer* sb[]);

	/// Metoda checkPos()
	///
	/// Sprawdza czy dana pozycja mo�e by� okupowana przez aktywne Tetrimino.
	/// Funkcja kopiuje pozycje XY Min Tetrimina do w�asnych tabel i dodaje do nich przesuni�cie z parametru.
	/// Najpierw sprawdza czy warto�ci mieszcz� si� w granicach Matrixa, a nast�pnie czy poszukiwane miejsca nie s� zaj�te przez istniej�ce Mino.
	/// Zwraca true je�li nie wyst�puje kolizja.
	/// @param xSh Przesuni�cie w osi x.
	/// @param ySh Przesuni�cie w osi y.
	bool checkPos(int xSh, int ySh);

	/// Metoda sDropCalc()
	///
	/// Zwraca now� warto�� pr�dko�ci grawitacji.
	void sdropCalc();

	/// Metoda drawPreview()
	///
	/// Rysuje podgl�d kolejki Tetrimin.
	/// @param w Referencja na okno gry.
	/// @param t Referencja na tekstur� Tetrimin.
	void drawPreview(sf::RenderWindow &w, sf::Texture &t);

	/// Metoda keyTest()
	///
	/// Funkcja debugowa. Rysuje podgl�d tabeli wci�ni�� klawiszy w dolnym lwym rogu ekranu.
	void keyTest(sf::RenderWindow &w);

	/// Metoda drawMatrix()
	///
	/// Rysuje wi�kszo�� element�w graficznych zwi�zanych z obiektem gracza, kolejno: statystyki, ducha Tetrimina, aplauzy, aktywne Tetrimino i zawarto�� Matrixa
	void drawMatrix(sf::RenderWindow &w, sf::Texture &t);
};