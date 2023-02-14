#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class pQueue;
class Queue;
class Mino;
class MinoAnchor;
class Applause;

/// Klasa reprezentuj¹ca gracza i nale¿¹ce do niego elementy gry.
class Player : public sf::Sprite
{
public:
	/// Konstruktor klasy.<br> Inicjuje obiekt przez ustawienie pocz¹tkowych wartoœci.<br>
	/// Znajduje siê tu m.in. przypisanie tekstury, wyznaczenie pozycji na ekranie, utworzenie obiektów aplauzu i statystyk.
	/// @param t Referencja na teksturê Matrixu.
	/// @param f Referencja na czcionkê tekstu.
	/// @param t2 Referencja na teksturê debugowania klawiszy.
	Player(sf::Texture &t, sf::Font &f, sf::Texture &t2);

	int id;	///< Numer ID gracza.
	std::map <std::string, sf::Keyboard::Key> ctrlKey;	///< Mapa osobistych ustawieñ klawiszy sterowania.

	bool active;	///< Stan aktywnoœci gry.
	bool gameOver;	///< Stan zakonczenia gry.

	Mino* matrix[10][40];		///< Tablica wskaŸników na Mina, bêd¹ca reprezentacj¹ Matrixu.
	MinoAnchor* activeAnchor;	///< WskaŸnik na aktywne, sterowalne Tetrimino.

	sf::Sprite holdBox[2];		///< Tablica duszków przestrzeni Schowka i obra¿eñ.
	sf::Sprite qBox;			///< Duszek przestrzeni podgl¹du kolejki.

	int countdown = -1;			///< Czasomierz odliczania do pocz¹tku gry.

	int level;					///< Poziom gry.
	int score;					///< Wynik punktowy gracza.
	sf::Time time;				///< Czas trwania gry.
	int pcsLocked = 0;			///< Licznik zalokowanych Tetrimin.

	int hold;					///< Numer reprezentuj¹cy Tetrimino w Schowku.
	bool holdActive;			///< Stan aktywnoœci Schowka.
	bool holdSwap;				///< Stan wymiany Tetrimina ze Schowkiem.

	int combo;					///< Licznik Combo.
	int back2back;				///< Licznik Back-2-Back.
	int tSpin;					///< Stan wyst¹pienia T-Spina: -1 - nie wyst¹pi³, 0 - mini, 1 - wyst¹pi³.
	bool tsMiniEligible = true;	///< Stan mo¿liwoœci wyst¹pienia T-Spina Mini.
	int toClear[4];				///< Tablica numerów linii do wyczyszczenia.
	int totalCleared;			///< Licznik usuniêtych linii.

	bool ctrlState[8];			///< Tablica stanów wciœniêcia klawiszy, kolejno: Lewo, Prawo, Obrót lewo, Obrót prawo, Soft Drop, Hard Drop, Hold, Pauza

	int sideMoveTime = 0;		///< Czasomierz ruchu w bok.
	int softDropTime = 0;		///< Czasomierz opadania/Soft Dropu.
	double softDropUpdate;		///< Czas wymagany do opuszczenia Tetrimina.
	int lockdownTime = -1;		///< Czasomierz lokowania.
	int lockdownReset = 0;		///< Licznik resetów czasomierza lokowania.
	int spawnTime = -1;			///< Czasomierz generowania nowego Tetrimina.
	int domDir = 0;				///< Stan dominuj¹cego kierunku ruchu w bok - -1 to lewo, 1 to prawo

	int pcQueue[5];				///< Tablica osobistej kolejki Tetrimin.

	sf::Text txt_stats[6];		///< Tablica tekstów statystyk, kolejno: tryb, najlepszy rezultat, wynik, czas, linie, poziom
	sf::Sprite keyDebug[7];		///< Tablica duszków do debugownia wciœniêæ klawiszy.

	Applause* appl[6];			///< Tablica obiektów aplauzu.

	/// Generowanie nowego Tetrimina.
	/// Funkcja sprawdza czy podana kolejka zawiera numer kszta³tu Mina i tworzy Tetrimino na jego podstawie, nastêpnie wywo³uje aktualizacjê kolejki.
	/// Je¿eli kolejka jest NULLem, tworzone jest Tetrimino na podstawie numeru kszta³tu ze schowka.
	/// @param q WskaŸnik na kolejkê.
	///	@param t Referencja na teksturê.
	void genPiece(pQueue* q, sf::Texture &t); // stwórz nowe tetrimino

	/// Aktualizacja obsobistej kolejki.
	///	Pobiera numer przechowywany na przodzie podanej w parametrze kolejki, odkolejkuj¹c go i umieszczaj¹c numer na koñcu w³asnej tablicy kolejki.
	/// Je¿eli ID pobranego elementu kolejki by³o podzielne przez 7 (pobrana zoosta³a ca³a torba), wywo³ywane jest dodanie nowej torby do kolejki ogólnej.
	/// @param q WskaŸnik na kolejkê.
	void updateQueue(pQueue* q);

	/// Obrócenie aktywnego Tetrimina.
	/// W ka¿dym Minie niebêd¹cym Minem-Kotwic¹ wywo³ywany jest obrót ich pozycji. Nastêpnie wykonywane jest do 5 testów obrotu.
	/// Ka¿dy test polega na odjêciu wartoœci przesuniêcia kierunku docelowego od wartoœci przesuniêcia kierunku wejœciowego.
	/// W przypadku sukcesu, przesuniêcie i obrót zostaj¹ zachowane; w przeciwnym, nastêpuje powrót do pozycji wejœciowej.
	/// @param cw Bool opisuj¹cy kierunek obrotu. True reprezentuje obrót w kierunku wskazówek zegara, false - w przeciwnym.
	void rotate(bool cw);

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

	/// Srawdzenie poprawnoœci wyœwietlania Min w Matrixie.
	/// Metoda przechodzi przez ka¿de pole Matrixu w poszukiwaniu Min, które nie s¹ gotowe do wyœwietlenia. Ka¿demu z takich Min przypisywana jest nowa tekstura.
	/// @param t Referencja na teksturê Min.
	void checkVisuals(sf::Texture &t);

	/// Rozpoznanie T-Spinów.
	/// Sprawdzenie czy dany obrót jest T-Spinem. W zale¿noœci od kierunku Tetrimina T, testowane s¹ ró¿ne pola Matrixu.
	/// Je¿eli pola po ka¿dym boku wypustki Tetrimina T i przynajmniej jedno z pól za koñcami p³askiej czêœci Tetrimina T s¹ zajête, zachodzi pe³ny T-Spin (zwracana jest wartoœæ 1).
	/// W przeciwnym wypadku, je¿eli pola za ka¿dym koñcem p³askiej czêœci Tetrimina T (lub znajduje siê tam œciana Matrixa) i przynajmniej jedno z pól po bokach wypustki Tetrimina T s¹ zajête, zachodzi T-Spin Mini (zwracana jest wartoœæ 0).
	/// W przeciwnym wypadku, nie zachodzi T-Spin (zwracana jest wartoœæ -1).
	int checkTSpin();

	/// S³u¿y do przeniesienia informacji o Tetriminie do Matrixu w celu zalokowania.
	/// Zaczyna siê od zwiêkszenia liczby zalokowanych Tetrimin, ustawieniu gotowoœci Tetrimina do lokowania i sprawdzeniu czy zaszed³ T-Spin.
	/// Ostatecznie usuwa aktywne Tetrimino, sprawdza reprezentacjê graficzn¹ nowych Min w Matrixie i sprawdza, czy zachodzi potrzeba usuniêcia liñ.
	/// @param t Referencja na teksturê Min.
	void lockIn(sf::Texture &t);

	/// Sprawdza, czy wymagane jest czyszczenie linii.
	/// Funkcja przechodzi przez wszystkie wiersze Matrixa, je¿eli znajdzie wiersz, w którym nie ma pustego miejsca, zapisuje jego numer do tablicy.
	void checkLines();

	/// Usuwa zape³nione linie i przyznaje nale¿ne punkty.
	/// Funkcja podgl¹da listê linii do usuniêcia z tablicy utworzonej przez checkLines(), usuwa Mina z tych linii i przesuwa wszystkie linie powy¿ej o jedno pole w dó³.
	/// Liczba usuwanych linii jest dodawana do licznika usuniêtych linii ogó³em, a jeœli ta przekroczy wielokrotnoœæ 10, zwiêksza poziom gry.
	/// W zale¿noœci od iloœci wyliczonych linii i innych wykonanych ruchów (Combo, Back-2-Back, T-Spiny, Perfect Clear) przyznawana jest nale¿na liczba punktów.
	/// Na koniec, aktualizowane s¹ teksty statystyk do wyœwietlenia na ekranie i wype³niane s¹ obiekty aplauzu dla gracza.
	/// @param f Referencja na czcionkê tekstu.
	/// @param s Tablica kana³ów dŸwiêków.
	/// @param sb Tablica wskaŸników na efekty dŸwiêkowe
	void clearLines(sf::Font &f, sf::Sound s[], sf::SoundBuffer* sb[]);

	/// Sprawdza czy dana pozycja mo¿e byæ okupowana przez aktywne Tetrimino.
	/// Funkcja kopiuje pozycje XY Min Tetrimina do w³asnych tabel i dodaje do nich przesuniêcie z parametru.
	/// Najpierw sprawdza czy wartoœci mieszcz¹ siê w granicach Matrixa, a nastêpnie czy poszukiwane miejsca nie s¹ zajête przez istniej¹ce Mino.
	/// Zwraca true jeœli nie wystêpuje kolizja.
	/// @param xSh Przesuniêcie w osi x.
	/// @param ySh Przesuniêcie w osi y.
	bool checkPos(int xSh, int ySh);

	/// Zwraca now¹ wartoœæ prêdkoœci grawitacji.
	void sdropCalc();

	/// Rysuje podgl¹d kolejki Tetrimin.
	/// @param w Referencja na okno gry.
	/// @param t Referencja na teksturê Tetrimin.
	void drawPreview(sf::RenderWindow &w, sf::Texture &t);

	/// Funkcja debugowa. Rysuje podgl¹d tabeli wciœniêæ klawiszy w dolnym lwym rogu ekranu.
	void keyTest(sf::RenderWindow &w);

	/// Rysuje wiêkszoœæ elementów graficznych zwi¹zanych z obiektem gracza, kolejno: statystyki, ducha Tetrimina, aplauzy, aktywne Tetrimino i zawartoœæ Matrixa
	void drawMatrix(sf::RenderWindow &w, sf::Texture &t);
};