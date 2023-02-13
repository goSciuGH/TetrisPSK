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
/// Klasa reprezentuj¹ca gracza i nale¿¹ce do niego elementy gry.
/// Klasa zawiera 34 pola:
///		id - numer gracza
///		ctrlKey - osobiste ustawienia klawiszy sterowania
///		active - reprezentacja stanu aktywnoœci gry
///		gameOver - reprezentacja stanu koñca gry
///		*matrix[10][40] - tablica wskaŸników na Mina zawarte w Matrixie
///		*activeAnchor - wskaŸnik na aktywne Mino-Kotwicê
///		holdBox[2] - tablica duszków reprezentuj¹cych przestrzeñ na schowek i kolejkê obra¿eñ
///		qBox - duszek reprezentuj¹cy przestrzeñ na kolejkê
///		countdown - czasomierz odliczania do rozpoczêcia gry
///		level - poziom gry
///		score - punkty zdobyte przez gracza
///		time - czasomierz gry
///		pcsLocked - liczba zalokowanych Tetrimin
///		hold - wartoœæ kszta³tu Tetrimina w schowku
///		holdActive - reprezentacja aktywnoœci schowka
///		holdSwap - reprezentacja mo¿liwoœci zamiany Tetrimin pomiêdzy sterowalnym a schowkiem
///		combo - d³ugoœæ Combo
///		back2back - d³ugoœæ Back-2-Back
///		tSpin - reprezentacja zajœcia T-Spina
///		tsMiniEligible - reprezentacja czy T-Spin Mini mo¿e zajœæ
///		toClear[4] - tablica numerów linii do usuniêcia
///		totalCleared - liczba usuniêtych linii
///		ctrlState[8] - tablica przechowuj¹ca stany wciœniêcia klawiszy z ostatniej klatki
///		sideMoveTime - czasomierz przyciœniêcia klawisza ruchu w bok
///		softDropTime - czasomierz opadania Tetrimina
///		softDropUpdate - czas potrzebny na opuszczenie Tetrimina
///		lockdownTime - czasomierz lokowania Tetrimina
///		lockdownReset - licznik resetów czasomierza lokowania
///		spawnTime - czasomierz spawnoania nowego Tetrimina
///		domDir - dominuj¹cy kierunek ruchu w bok
///		pcQueue[5] - osobista kolejka Tetrimin
///		txt_stats[6] - tablica tekstów statystyk
///		keyDebug[7] - tablica duszków debugowania wciœniêæ klawiszy
///		*appl[6] - tablica wskaŸników na teksty-aplauzy
/// Klasa zawiera 14 metod:
///		genPiece() - generowanie nowego Tetrimina
///		updateQueue() - aktualizacja osobistej kolejki
///		rotate() - obrót atywnego Tetrimina
///		ctrlDo() - aktualizacja ruchu Tetrimina wg wciœniêæ klawiszy
///		checkVisuals() - sprawdzenie poprawnoœci wyœwietlania Min w Matrixie
///		checkTSpin() - sprawdzenie zajœcia T-Spina
///		lockIn() - zalokowanie aktywnego Tetrimina
///		checkLines() - wyszukiwanie linii do usuniêcia
///		checkPos() - sprawdzenie, czy dane miejsce w Matrixie jest wolne
///		sdropCalc() - aktualizacja grawitacji
///		drawPreview() - rysowanie kolejki
///		keyTest() - rysowanie reprezentacji wciœniêcia klawiszy (debug)
///		drawMatrix() - rysowanie wszystkich zwi¹zanych z obiektem gracza elementów
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
	int tSpin;	// -1 - nie wyst¹pi³, 0 - mini, 1 - wyst¹pi³
	bool tsMiniEligible = true;
	int toClear[4];
	int totalCleared;

	bool ctrlState[8]; // kolejno: Lewo, Prawo, Obrót lewo, Obrót prawo, Soft Drop, Hard Drop, Hold, Pauza

	int sideMoveTime = 0;
	int softDropTime = 0;
	double softDropUpdate;
	int lockdownTime = -1;
	int lockdownReset = 0;
	int spawnTime = -1;
	int domDir = 0; // dominuj¹cy kierunek - -1 to lewo, 1 to prawo

	int pcQueue[5];

	sf::Text txt_stats[6]; // mode, best, score, time, lines, lvl
	sf::Sprite keyDebug[7];

	Applause* appl[6];

	/// Metoda genPiece()
	///
	/// Generowanie nowego Tetrimina.
	/// Funkcja sprawdza czy podana kolejka zawiera numer kszta³tu Mina i tworzy Tetrimino na jego podstawie, nastêpnie wywo³uje aktualizacjê kolejki.
	/// Je¿eli kolejka jest NULLem, tworzone jest Tetrimino na podstawie numeru kszta³tu ze schowka.
	/// @param q WskaŸnik na kolejkê.
	///	@param t Referencja na teksturê.
	void genPiece(pQueue* q, sf::Texture &t); // stwórz nowe tetrimino
	
	/// Metoda updateQueue()
	///
	/// Aktualizacja obsobistej kolejki.
	///	Pobiera numer przechowywany na przodzie podanej w parametrze kolejki, odkolejkuj¹c go i umieszczaj¹c numer na koñcu w³asnej tablicy kolejki.
	/// Je¿eli ID pobranego elementu kolejki by³o podzielne przez 7 (pobrana zoosta³a ca³a torba), wywo³ywane jest dodanie nowej torby do kolejki ogólnej.
	/// @param q WskaŸnik na kolejkê.
	void updateQueue(pQueue* q);

	/// Metoda rotate()
	///
	/// Obrócenie aktywnego Tetrimina.
	/// W ka¿dym Minie niebêd¹cym Minem-Kotwic¹ wywo³ywany jest obrót ich pozycji. Nastêpnie wykonywane jest do 5 testów obrotu.
	/// Ka¿dy test polega na odjêciu wartoœci przesuniêcia kierunku docelowego od wartoœci przesuniêcia kierunku wejœciowego.
	/// W przypadku sukcesu, przesuniêcie i obrót zostaj¹ zachowane; w przeciwnym, nastêpuje powrót do pozycji wejœciowej.
	/// @param cw Bool opisuj¹cy kierunek obrotu. True reprezentuje obrót w kierunku wskazówek zegara, false - w przeciwnym.
	void rotate(bool cw);

	/// Metoda ctrlDo()
	///
	/// Aktualizacja ruchu Tetrimina na podstawie wciœniêæ klawiszy.
	/// Jako pierwsze testowane jest wciœniêcie klawisza Schowka. Je¿eli klatkê wczeœniej nie by³ on wciœniêty, dokonuje siê zapisania wartoœci kszta³tu do Schowka i usuniêcie obecnego Tetrimina. Je¿eli jakas wartoœæ by³a wczeœniej, nowe Tetrimino jest tworzone z tej wartoœci. Metoda nie jest dalej wykonywana.
	/// Nastêpnie testowane jest wciœniêcie klawisza Hard Dropu. Je¿eli klatkê wczeœniej nie by³ on wciœniêty, Tetrimino jest przesuwane o 1 pole w dó³ w pêtli do napotkania przeszkody. Za ka¿de pole przyznawane s¹ punkty. Metoda nie jest dalej wykonywana.
	/// Nastêpnie testowane s¹ wciœniêcia klawiszy ruchu w bok. Je¿eli klatkê wczeœniej dany przycisk nie by³ wciœniêty, nastêpuje natychmiastowe przesuniêcie Tetrimina w danym kierunku. Je¿eli klawisz by³ trzymany klatkê wczeœniej, dodawany jest do czasomierza ruchu czas klatki, a gdy czasomierz przekroczy wymagan¹ wartoœæ, Tetrimino jest przesuwane ponownie. Dodanie wciœniêcia przeciwnego klawisza ruchu zmienia tymczasowo kierunke ruchu.
	/// Nastêpnie testowane s¹ wciœniêcia klawiszy obrotu. Je¿eli klatkê wczeœniej, dany klawisz nie by³ wciœniêty, wywo³ywana jest metoda obrotu.
	/// Ostatecznie testowane jest wciœniêcie klawisza Soft Dropu. Je¿eli nie jest wciœniêty, do czasomierza opadania dodawany jest czas klatki, je¿eli jest wciœniêty, dodawana jest wielokrotnoœæ czasu klatki.
	/// Ka¿dy z powy¿szych testów koñczy siê zapisaniem stanu wciœniêcia klawisza do tabeli.
	/// Nastêpnym krokiem jest opuszczanie Tetrimina. Je¿eli czasomierz przekroczy³ wymagany okres, wartoœæ tego kresu jest odejmowana, a Tetrimino jest opusczane w dó³ o 1 pole, lub dodany zostaje czas lokowania, jeœli nie mo¿na opuœciæ Tetrimina. Za ka¿de pole, dodawane s¹ punkty do wyniku. Je¿eli czasomierz nadal przekracza granicê, metoda ctrlDo jest ponownie wywo³ywana, z parametrem czasu klatki równym zero.
	/// Na koniec, aktualizowana jest reprezentacja graficzna aktywnego Tetrimina.
	/// @param t Czas klatki w ms.
	/// @param ttr Referencja na teksturê Min.
	/// @param s Tablica kana³ów dŸwiêku, do odgrywania efektów dŸwiêkowych.
	/// @param sb Tablica wskaŸników na efekty dŸwiêkowe.
	void ctrlDo(int t, sf::Texture &ttr, sf::Sound s[], sf::SoundBuffer* sb[]);

	/// Metoda checkVisuals()
	///
	/// Srawdzenie poprawnoœci wyœwietlania Min w Matrixie.
	/// Metoda przechodzi przez ka¿de pole Matrixu w poszukiwaniu Min, które nie s¹ gotowe do wyœwietlenia. Ka¿demu z takich Min przypisywana jest nowa tekstura.
	/// @param t Referencja na teksturê Min.
	void checkVisuals(sf::Texture &t);

	/// Metoda checkTSpin()
	///
	/// Rozpoznanie T-Spinów.
	/// Sprawdzenie czy dany obrót jest T-Spinem. W zale¿noœci od kierunku Tetrimina T, testowane s¹ ró¿ne pola Matrixu.
	/// Je¿eli pola po ka¿dym boku wypustki Tetrimina T i przynajmniej jedno z pól za koñcami p³askiej czêœci Tetrimina T s¹ zajête, zachodzi pe³ny T-Spin (zwracana jest wartoœæ 1).
	/// W przeciwnym wypadku, je¿eli pola za ka¿dym koñcem p³askiej czêœci Tetrimina T (lub znajduje siê tam œciana Matrixa) i przynajmniej jedno z pól po bokach wypustki Tetrimina T s¹ zajête, zachodzi T-Spin Mini (zwracana jest wartoœæ 0).
	/// W przeciwnym wypadku, nie zachodzi T-Spin (zwracana jest wartoœæ -1).
	int checkTSpin();

	/// Metoda lockIn()
	///
	/// S³u¿y do przeniesienia informacji o Tetriminie do Matrixu w celu zalokowania.
	/// Zaczyna siê od zwiêkszenia liczby zalokowanych Tetrimin, ustawieniu gotowoœci Tetrimina do lokowania i sprawdzeniu czy zaszed³ T-Spin.
	/// Ostatecznie usuwa aktywne Tetrimino, sprawdza reprezentacjê graficzn¹ nowych Min w Matrixie i sprawdza, czy zachodzi potrzeba usuniêcia liñ.
	/// @param t Referencja na teksturê Min.
	void lockIn(sf::Texture &t);

	/// Metoda checkLines()
	///
	/// Sprawdza, czy wymagane jest czyszczenie linii.
	/// Funkcja przechodzi przez wszystkie wiersze Matrixa, je¿eli znajdzie wiersz, w którym nie ma pustego miejsca, zapisuje jego numer do tablicy.
	void checkLines();

	/// Metoda clearLines()
	///
	/// Usuwa zape³nione linie i przyznaje nale¿ne punkty.
	/// Funkcja podgl¹da listê linii do usuniêcia z tablicy utworzonej przez checkLines(), usuwa Mina z tych linii i przesuwa wszystkie linie powy¿ej o jedno pole w dó³.
	/// Liczba usuwanych linii jest dodawana do licznika usuniêtych linii ogó³em, a jeœli ta przekroczy wielokrotnoœæ 10, zwiêksza poziom gry.
	/// W zale¿noœci od iloœci wyliczonych linii i innych wykonanych ruchów (Combo, Back-2-Back, T-Spiny, Perfect Clear) przyznawana jest nale¿na liczba punktów.
	/// Na koniec, aktualizowane s¹ teksty statystyk do wyœwietlenia na ekranie i wype³niane s¹ obiekty aplauzu dla gracza.
	/// @param f Referencja na czcionkê tekstu.
	/// @param s Tablica kana³ów dŸwiêków.
	/// @param sb Tablica wskaŸników na efekty dŸwiêkowe
	void clearLines(sf::Font &f, sf::Sound s[], sf::SoundBuffer* sb[]);

	/// Metoda checkPos()
	///
	/// Sprawdza czy dana pozycja mo¿e byæ okupowana przez aktywne Tetrimino.
	/// Funkcja kopiuje pozycje XY Min Tetrimina do w³asnych tabel i dodaje do nich przesuniêcie z parametru.
	/// Najpierw sprawdza czy wartoœci mieszcz¹ siê w granicach Matrixa, a nastêpnie czy poszukiwane miejsca nie s¹ zajête przez istniej¹ce Mino.
	/// Zwraca true jeœli nie wystêpuje kolizja.
	/// @param xSh Przesuniêcie w osi x.
	/// @param ySh Przesuniêcie w osi y.
	bool checkPos(int xSh, int ySh);

	/// Metoda sDropCalc()
	///
	/// Zwraca now¹ wartoœæ prêdkoœci grawitacji.
	void sdropCalc();

	/// Metoda drawPreview()
	///
	/// Rysuje podgl¹d kolejki Tetrimin.
	/// @param w Referencja na okno gry.
	/// @param t Referencja na teksturê Tetrimin.
	void drawPreview(sf::RenderWindow &w, sf::Texture &t);

	/// Metoda keyTest()
	///
	/// Funkcja debugowa. Rysuje podgl¹d tabeli wciœniêæ klawiszy w dolnym lwym rogu ekranu.
	void keyTest(sf::RenderWindow &w);

	/// Metoda drawMatrix()
	///
	/// Rysuje wiêkszoœæ elementów graficznych zwi¹zanych z obiektem gracza, kolejno: statystyki, ducha Tetrimina, aplauzy, aktywne Tetrimino i zawartoœæ Matrixa
	void drawMatrix(sf::RenderWindow &w, sf::Texture &t);
};