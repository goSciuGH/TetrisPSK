/*
Wa¿ne komentarze:
	NOTE - notka, do przemyœlenia
	FIXIT - trzeba to naprawiæ, czemu??
	TODO - do zrobienia
*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <time.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "pQueue.h"
#include "Queue.h"
#include "Mino.h"
#include "MinoAnchor.h"
#include "Player.h"
#include "Applause.h"

// Deklaracja extern

extern void defineOffset(int t[3][4][5][2]);

extern int hiScore;
extern std::fstream hiScoF;
extern int offsetChart[3][4][5][2];
extern std::map <std::string, sf::Keyboard::Key> dftKey;
extern int windowW;
extern int windowH;
extern int FULLRES_W;
extern int FULLRES_H;
extern int DAS_DEFAULT;
extern int ARR_DEFAULT;
extern int SDF_DEFAULT;
extern int LOCK_TIME;
extern int LOCK_RESET;
extern int SPAWN_RESET;

// Pêtla g³ówna

/// Funkcja g³ówna main()
///
///	Funkcja dzieli siê na dwie czêœci - inicjalizuj¹c¹ i g³ówn¹ pêtlê programu.<br>
/// W pierwszej czêœci najpierw wczytywany jest najlepszy wynik z pliku zapisu hiScore.sav, nastêpuje inicjalizacja randomizera, ustawienie tabeli podgl¹dowej obrotów Tetrimin i klawiszy sterowania.<br>
/// Inicjalizowane s¹ te¿ zegar, którego zadaniem bêdzie mierzenie czasu klatki, okno, tekstury, efekty dŸwiêkowe, muzyka, czcionka, widok, obiekt gracza i tablica do odtwarzania dŸwiêków.<br>
/// Za³adowywana jest kolejka Tetrimin.<br>
/// Po tych krokach, rozpoczyna siê pêtla programu. Na pocz¹tku odmierzany jest czas klatki, który pos³u¿y do obliczeñ zwi¹zanych z ruchem Tetrimin.<br>
/// W zale¿noœci od stanów active i gameOver z obiektu gracza wykonywany jest jeden z zestawów poleceñ:<br>
///	&emsp;Je¿eli obiekt gracza jest nieaktywny, ale rozpoczête zosta³o odliczanie, jest ono kontynuowane.<br>
///	&emsp;Je¿eli obiekt jest aktywny nastêpuje aktualizacja przebiegu gry.<br>
///	&emsp;&emsp;Lista poleceñ zaczyna siê od aktualizji grafik obiektów Applause. Do czasomierza gry dodawany jest ostatni czas klatki.<br>
///	&emsp;&emsp;Je¿eli nie istnieje sterowalne Tetrimino, sprawdzane s¹ po kolei parametry:<br>
///	&emsp;&emsp;&emsp;Je¿eli w ostatniej klatce zosta³o dokonane schowkowanie Tetrimina, natychmiast tworzone jest nowe Tetrimino, bêd¹ce poprzednim schowkowanym Tetriminem lub (je¿eli takie nie istnieje) z kolejki.<br>
///	&emsp;&emsp;&emsp;W przeciwnym wypadku, je¿eli czasomierz respawnu ma wystarczaj¹co du¿¹ wartoœæ, dokonywane jest czyszczenie linii i tworzone jest nowe Tetrimino z kolejki.<br>
///	&emsp;&emsp;&emsp;&emsp;W tym miejscu jest te¿ sprawdzany pierwszy warunek koñca gry - Block Out - zablokowanie pozycji spawnowania nowego Tetrimina przez istniej¹ce na Matrixie mino.<br>
///	&emsp;&emsp;&emsp;W przeciwnym wypadku, doliczany jest czas klatki.<br>
///	&emsp;&emsp;Je¿eli sterowalne Tetrimino istnieje, najpierw nastêpuje aktualizacja kontroli Tetriminem przez funkcjê ctrlDo(). Nastêpnie sprawdzany jest czas lokowania Tetrimina.<br>
///	&emsp;&emsp;&emsp;Je¿eli czas na zalokowanie Tetrmina up³yn¹³, przeprowadzany jest test na drugi warunek koñca gry - Lock Out - umieszczone Tetrimino znajduje siê ca³kowicie poza widoczn¹ czêœci¹ Matrixu.<br>
///	&emsp;&emsp;&emsp;&emsp;Ostatecznie Tetrimino zostaje zalokowane - to jest jego Mina sk³adowe zostaj¹ przeniesione do Matrixu.<br>
///	Kolejnym krokiem jest wykonanie poleceñ zwi¹zanych ze zdarzeniami SFML: je¿eli obiekt gracza jest niekatywny, a nie nast¹pi³ jeszcze konieæ gry, u¿ytkownik mo¿e wcisn¹æ Enter, by rozpocz¹æ grê.<br>
/// W ka¿dym momencie gracz mo¿e te¿ zamkn¹æ okno gry, klikaj¹c na krzy¿yk w rogu okna.<br>
/// Na koniec pêtli, rysowane s¹ elementy gry w widoku okna.
int main()
{
	hiScoF.open("hiScore.sav", std::ios::in | std::ios::out);
	hiScoF >> hiScore;
	hiScoF.close();
	hiScoF.open("hiScore.sav", std::ios::out | std::ios::trunc);

	srand(time(NULL));

	defineOffset(offsetChart);

	int ctrlSet = 0;
	bool ctrlSwitch = 0;
	/*
	// Osobiste sterowanie do testów
	dftKey["LMV"] = sf::Keyboard::A;		// Left MoVement
	dftKey["RMV"] = sf::Keyboard::D;	// Right MoVement
	dftKey["LSP"] = sf::Keyboard::Numpad4;		// Left SPin
	dftKey["RSP"] = sf::Keyboard::Numpad6;		// Right SPin
	dftKey["SDR"] = sf::Keyboard::S;		// Soft DRop
	dftKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
	dftKey["HLD"] = sf::Keyboard::Numpad5;		// HoLD
	dftKey["PSE"] = sf::Keyboard::Escape;	// PauSE
	*/
	
	dftKey["LMV"] = sf::Keyboard::Left;		// Left MoVement
	dftKey["RMV"] = sf::Keyboard::Right;	// Right MoVement
	dftKey["LSP"] = sf::Keyboard::Z;		// Left SPin
	dftKey["RSP"] = sf::Keyboard::X;		// Right SPin
	dftKey["SDR"] = sf::Keyboard::Down;		// Soft DRop
	dftKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
	dftKey["HLD"] = sf::Keyboard::C;		// HoLD
	dftKey["PSE"] = sf::Keyboard::Escape;	// PauSE
	
	sf::Clock frameTimer;
	int fpsCount;

	sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Tetris PSk", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);

	// £adowanie tekstur

	sf::Texture ttr_Matrix;
	if (!ttr_Matrix.loadFromFile("sprites/spr_matrix.png"))
	{
		std::cout << "Blad wczytywania duszka spr_matrix\n";
	}
	ttr_Matrix.setSmooth(true);

	sf::Texture ttr_Mino;
	if (!ttr_Mino.loadFromFile("sprites/spr_mino.png"))
	{
		std::cout << "Blad wczytywania duszka spr_mino\n";
	}
	ttr_Mino.setSmooth(true);

	sf::Texture ttr_HoldSolo;
	if (!ttr_HoldSolo.loadFromFile("sprites/spr_holdSolo.png"))
	{
		std::cout << "Blad wczytywania duszka spr_holdSolo\n";
	}
	ttr_HoldSolo.setSmooth(true);

	sf::Texture ttr_HoldTop;
	if (!ttr_HoldTop.loadFromFile("sprites/spr_holdTop.png"))
	{
		std::cout << "Blad wczytywania duszka spr_holdTop\n";
	}
	ttr_HoldTop.setSmooth(true);

	sf::Texture ttr_HoldBot;
	if (!ttr_HoldBot.loadFromFile("sprites/spr_holdBot.png"))
	{
		std::cout << "Blad wczytywania duszka spr_holdBot\n";
	}
	ttr_HoldBot.setSmooth(true);

	sf::Texture ttr_Queue5;
	if (!ttr_Queue5.loadFromFile("sprites/spr_queue5.png"))
	{
		std::cout << "Blad wczytywania duszka spr_queue5\n";
	}
	ttr_Queue5.setSmooth(true);

	sf::Texture ttr_Preview;
	if (!ttr_Preview.loadFromFile("sprites/spr_preview.png"))
	{
		std::cout << "Blad wczytywania duszka spr_preview\n";
	}
	ttr_Preview.setSmooth(true);

	sf::Texture ttr_soloStats;
	if (!ttr_soloStats.loadFromFile("sprites/spr_soloStats.png"))
	{
		std::cout << "Blad wczytywania duszka spr_soloStats\n";
	}
	ttr_soloStats.setSmooth(true);

	int randNum = rand() % 9;
	sf::Texture ttr_bg;
	if (!ttr_bg.loadFromFile("sprites/spr_bgBlur_" + std::to_string(randNum) + ".png"))
	{
		std::cout << "Blad wczytywania duszka spr_bgBlur_" << std::to_string(randNum) << "\n";
	}
	ttr_bg.setSmooth(true);

	sf::Texture ttr_matrixBgSolo;
	if (!ttr_matrixBgSolo.loadFromFile("sprites/spr_matrixBgSolo.png"))
	{
		std::cout << "Blad wczytywania duszka spr_matrixBgSolo\n";
	}
	ttr_matrixBgSolo.setSmooth(true);

	sf::Texture ttr_keyDebug;
	if (!ttr_keyDebug.loadFromFile("sprites/spr_keyDebug.png"))
	{
		std::cout << "Blad wczytywania duszka spr_keyDebug\n";
	}
	ttr_keyDebug.setSmooth(true);

	// £adowanie efektów dŸwiêkowych

	sf::SoundBuffer sfx_clearSpecial;
	if (!sfx_clearSpecial.loadFromFile("sfx/sfx_clearSpecial.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_clearSpecial\n";
	}

	sf::SoundBuffer sfx_clears;
	if (!sfx_clears.loadFromFile("sfx/sfx_clears.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_clears\n";
	}

	sf::SoundBuffer sfx_countdown;
	if (!sfx_countdown.loadFromFile("sfx/sfx_countdown.wav"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_countdown\n";
	}

	sf::SoundBuffer sfx_gravity;
	if (!sfx_gravity.loadFromFile("sfx/sfx_gravity.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_gravity\n";
	}

	sf::SoundBuffer sfx_hardDrop;
	if (!sfx_hardDrop.loadFromFile("sfx/sfx_hardDrop.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_hardDrop\n";
	}

	sf::SoundBuffer sfx_hold;
	if (!sfx_hold.loadFromFile("sfx/sfx_hold.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_hold\n";
	}

	sf::SoundBuffer sfx_levelUp;
	if (!sfx_levelUp.loadFromFile("sfx/sfx_levelUp.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_levelUp\n";
	}

	sf::SoundBuffer sfx_lockDown;
	if (!sfx_lockDown.loadFromFile("sfx/sfx_lockDown.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_lockDown\n";
	}

	sf::SoundBuffer sfx_lockOut;
	if (!sfx_lockOut.loadFromFile("sfx/sfx_lockOut.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_lockOut\n";
	}

	sf::SoundBuffer sfx_shift;
	if (!sfx_shift.loadFromFile("sfx/sfx_shift.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_shift\n";
	}

	sf::SoundBuffer sfx_spin;
	if (!sfx_spin.loadFromFile("sfx/sfx_spin.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_shift\n";
	}

	sf::SoundBuffer sfx_tSpin;
	if (!sfx_tSpin.loadFromFile("sfx/sfx_tSpin.ogg"))
	{
		std::cout << "Blad wczytywania efektu dzwiekowego sfx_tSpin\n";
	}
	
	sf::SoundBuffer* soundCollection[11];	// tablica wskaŸników na dŸwiêki dla ³atwiejszego dostêpu w funkcjach
	soundCollection[0] = &sfx_clearSpecial;
	soundCollection[1] = &sfx_clears;
	soundCollection[2] = &sfx_gravity;
	soundCollection[3] = &sfx_hardDrop;
	soundCollection[4] = &sfx_hold;
	soundCollection[5] = &sfx_levelUp;
	soundCollection[6] = &sfx_lockDown;
	soundCollection[7] = &sfx_lockOut;
	soundCollection[8] = &sfx_shift;
	soundCollection[9] = &sfx_spin;
	soundCollection[10] = &sfx_tSpin;

	// Definicja muzyki

	sf::Music mus_main;
	if (!mus_main.openFromFile("music/mus_gameplay.ogg"))
	{
		std::cout << "Blad wczytywania muzyki mus_gameplay\n";
	}
	mus_main.setLoop(true);
	mus_main.setVolume(67);

	// £adowanie czcionek

	sf::Font fnt_MobyM;
	if (!fnt_MobyM.loadFromFile("fonts/Moby-Monospace.otf"))
	{
		std::cout << "Blad wczytywania czcionki fnt_MobyM\n";
	}

	//

	sf::View view(sf::Vector2f(FULLRES_W / 2.0, FULLRES_H / 2.0f), sf::Vector2f(FULLRES_W, FULLRES_H)); // center, size

	pQueue* pcQueue = new pQueue();
	pcQueue->addBag();

	sf::Sprite bg;
	bg.setTexture(ttr_bg);

	Player* player[2];

	player[0] = new Player(ttr_Matrix, fnt_MobyM, ttr_keyDebug);

	player[0]->holdBox[0].setTexture(ttr_HoldSolo);
	player[0]->holdBox[0].setOrigin(sf::Vector2f((player[0]->holdBox[0].getTexture()->getSize().x) / 2.0, (player[0]->holdBox[0].getTexture()->getSize().y) / 2.0));
	player[0]->holdBox[0].setPosition(sf::Vector2f(player[0]->getPosition().x - ((player[0]->getTexture()->getSize().x) / 2.0) - ((player[0]->holdBox[0].getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));

	player[0]->qBox.setTexture(ttr_Queue5);
	player[0]->qBox.setOrigin(sf::Vector2f((player[0]->qBox.getTexture()->getSize().x) / 2.0, (player[0]->qBox.getTexture()->getSize().y) / 2.0));
	player[0]->qBox.setPosition(sf::Vector2f(player[0]->getPosition().x + ((player[0]->getTexture()->getSize().x) / 2.0) + ((player[0]->qBox.getTexture()->getSize().x) / 2.0), player[0]->getPosition().y));

	sf::Sprite soloStats;
	soloStats.setTexture(ttr_soloStats);
	soloStats.setOrigin(sf::Vector2f((ttr_soloStats.getSize().x) / 2.0, (ttr_soloStats.getSize().y) / 2.0));
	soloStats.setPosition(sf::Vector2f(FULLRES_W - 564, FULLRES_H / 2.0 + 2.0));

	sf::Sprite matrixBg;
	matrixBg.setTexture(ttr_matrixBgSolo);
	matrixBg.setOrigin(sf::Vector2f(ttr_matrixBgSolo.getSize().x / 2.0, ttr_matrixBgSolo.getSize().y / 2.0));
	matrixBg.setPosition(sf::Vector2f(player[0]->getPosition().x - 10.0, player[0]->getPosition().y));
	matrixBg.setColor(sf::Color(255, 255, 255, 128));

	for (int i = 0; i < 5; i++)
		player[0]->updateQueue(pcQueue);

	player[1] = NULL;

	// Pêtla gry

	sf::Sound sounds[7];
	/*
		7 kana³ów dŸwiêków, efekty s¹ po³¹czone ze sob¹ w grupy jeœli mog¹ siê nazwajem wypieraæ lub nigdy sobie nawzajem nie przeszkodz¹
			0 - shift
			1 - spin
			2 - lockDown, hold
			3 - gravity, hardDrop
			4 - clears, clearQuad
			5 - lockOut, tSpin
			6 - countdown, levelUp
	*/

	while (window.isOpen())
	{
		sf::Time elapsed1 = frameTimer.getElapsedTime();

		//if (elapsed1.asMicroseconds() != 0)
		//	fpsCount = 1000000 / elapsed1.asMicroseconds();
		//else
		//	fpsCount = INT_MAX;
		//sf::String titleText = "Tetris PSk - " + std::to_string(fpsCount) + " FPS";
		//window.setTitle(titleText);

		frameTimer.restart();

		if ((!player[0]->active) && (player[0]->countdown >= 0) && (!player[0]->gameOver))
		{
			player[0]->countdown += elapsed1.asMilliseconds();

			if (player[0]->countdown + elapsed1.asMilliseconds() >= 2700)
			{
				player[0]->active = true;
				mus_main.play();
				player[0]->countdown = -1;
			}
			
		}
		//if ((!player[0]->active) && (player[0]->countdown >= 0) && (player[0]->gameOver))
		//{
		//	player[0]->countdown += elapsed1.asMilliseconds();

		//	if (player[0]->countdown + elapsed1.asMilliseconds() >= 2600)
		//	{
		//		player[0]->active = true;
		//		player[0]->gameOver = false;
		//		mus_main.play();
		//		player[0]->countdown = -1;
		//	}
		//}

		// Kontrola
		if (player[0]->active)
		{
			for (int i = 0; i < 6; i++)
			{
				if (player[0]->appl[i]->aType != -1)
				{
					player[0]->appl[i]->updateVis(elapsed1.asMilliseconds());
				}
			}

			if ((player[0]->score > 0) || ((player[0]->activeAnchor == NULL) && (player[0]->spawnTime + elapsed1.asMilliseconds() >= SPAWN_RESET)) || (player[0]->activeAnchor != NULL) || (player[0]->pcsLocked > 0))
			{
				player[0]->time += elapsed1;
			}

			if (player[0]->activeAnchor == NULL)
			{
				if (player[0]->holdSwap)
				{
					player[0]->genPiece(pcQueue, ttr_Mino);
					if (player[0]->checkPos(0, -1))
						player[0]->activeAnchor->posY -= 1;
					player[0]->holdSwap = false;
				}
				else
				{
					if (player[0]->spawnTime == 0)
					{
						player[0]->checkVisuals(ttr_Mino);
					}

					if (player[0]->spawnTime >= SPAWN_RESET)
					{
						player[0]->clearLines(fnt_MobyM, sounds, soundCollection);
						player[0]->checkVisuals(ttr_Mino);

						player[0]->spawnTime = -1;
						player[0]->lockdownReset = 0;
						player[0]->lockdownReset = -1;
						player[0]->softDropTime = 0;
						player[0]->genPiece(pcQueue, ttr_Mino);
						for (int i = 0; i < 8; i++)
						{
							player[0]->ctrlState[i] = false;
						}
						player[0]->ctrlState[5] = true;
						player[0]->holdActive = true;
						player[0]->tSpin = -1;
						player[0]->tsMiniEligible = true;
						if (player[0]->checkPos(0, 0))
						{
							if (player[0]->checkPos(0, -1))
								player[0]->activeAnchor->posY -= 1;
						}
						else
						{
							int toDelete[4][2];
							toDelete[0][0] = player[0]->activeAnchor->posX;
							toDelete[0][1] = player[0]->activeAnchor->posY;
							toDelete[1][0] = player[0]->activeAnchor->posX + player[0]->activeAnchor->children[0]->posX;
							toDelete[1][1] = player[0]->activeAnchor->posY + player[0]->activeAnchor->children[0]->posY;
							toDelete[2][0] = player[0]->activeAnchor->posX + player[0]->activeAnchor->children[1]->posX;
							toDelete[2][1] = player[0]->activeAnchor->posY + player[0]->activeAnchor->children[1]->posY;
							toDelete[3][0] = player[0]->activeAnchor->posX + player[0]->activeAnchor->children[2]->posX;
							toDelete[3][1] = player[0]->activeAnchor->posY + player[0]->activeAnchor->children[2]->posY;
							for (int i = 0; i < 4; i++)
							{
								if (player[0]->matrix[toDelete[i][0]][toDelete[i][1]] != NULL)
									delete player[0]->matrix[toDelete[i][0]][toDelete[i][1]];
							}
							for (int i = 0; i < 10; i++)
							{
								for (int j = 0; j < 40; j++)
								{
									if (player[0]->matrix[i][j] != NULL)
										player[0]->matrix[i][j]->setColor(sf::Color(128, 128, 128, player[0]->matrix[i][j]->getColor().a));
								}
							}
							player[0]->lockIn(ttr_Mino);
							sounds[2].setBuffer(sfx_lockDown);
							sounds[2].play();
							sounds[5].setBuffer(sfx_lockOut);
							sounds[5].play();
							mus_main.stop();
							player[0]->active = false;
							player[0]->gameOver = true;
							player[0]->countdown = -1;

							if (player[0]->score > hiScore)
							{
								hiScore = player[0]->score;
							}
							hiScoF << hiScore;
							hiScoF.close();

							sf::FloatRect bounds;
							player[0]->txt_stats[1].setString("");
							int len = std::to_string(hiScore).length();
							for (int i = 0; i < (8 - len); i++)
								player[0]->txt_stats[1].setString(player[0]->txt_stats[1].getString() + "0");
							player[0]->txt_stats[1].setString(player[0]->txt_stats[1].getString() + std::to_string(hiScore));
							bounds = player[0]->txt_stats[1].getLocalBounds();
							player[0]->txt_stats[1].setOrigin(sf::Vector2f(bounds.width, bounds.height));
							player[0]->txt_stats[1].setPosition(sf::Vector2f(1632.0, 410.0));

							// Game Over - Block Out
						}
					}
					else
					{
						player[0]->spawnTime += elapsed1.asMilliseconds();
					}
				}
			}
			else if (player[0]->activeAnchor != NULL)
			{
				player[0]->ctrlDo(elapsed1.asMilliseconds(), ttr_Mino, sounds, soundCollection);
				if ((player[0]->lockdownReset > LOCK_RESET) && (player[0]->spawnTime == -1))
				{
					int check = player[0]->activeAnchor->posY;
					for (int i = 0; i < 3; i++)
					{
						if (player[0]->activeAnchor->posY + player[0]->activeAnchor->children[i]->posY < check)
						{
							check = player[0]->activeAnchor->posY + player[0]->activeAnchor->children[i]->posY;
						}
					}

					if (check < 20)
					{
						player[0]->lockIn(ttr_Mino);
						sounds[2].setBuffer(sfx_lockDown);
						sounds[2].play();
					}
					else
					{
						for (int i = 0; i < 10; i++)
						{
							for (int j = 0; j < 40; j++)
							{
								if (player[0]->matrix[i][j] != NULL)
									player[0]->matrix[i][j]->setColor(sf::Color(128, 128, 128, player[0]->matrix[i][j]->getColor().a));
							}
						}
						player[0]->lockIn(ttr_Mino);
						sounds[2].setBuffer(sfx_lockDown);
						sounds[2].play();
						sounds[5].setBuffer(sfx_lockOut);
						sounds[5].play();
						mus_main.stop();
						player[0]->active = false;
						player[0]->gameOver = true;
						player[0]->countdown = -1;

						if (player[0]->score > hiScore)
						{
							hiScore = player[0]->score;
						}
						hiScoF << hiScore;
						hiScoF.close();

						sf::FloatRect bounds;
						player[0]->txt_stats[1].setString("");
						int len = std::to_string(hiScore).length();
						for (int i = 0; i < (8 - len); i++)
							player[0]->txt_stats[1].setString(player[0]->txt_stats[1].getString() + "0");
						player[0]->txt_stats[1].setString(player[0]->txt_stats[1].getString() + std::to_string(hiScore));
						bounds = player[0]->txt_stats[1].getLocalBounds();
						player[0]->txt_stats[1].setOrigin(sf::Vector2f(bounds.width, bounds.height));
						player[0]->txt_stats[1].setPosition(sf::Vector2f(1632.0, 410.0));
						// Game Over - Lock Out
					}
				}
			}
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				hiScoF << hiScore;
				hiScoF.close();
				window.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Enter)
				{
					if ((player[0]->active == false) && (player[0]->gameOver == false) && (player[0]->countdown == -1))
					{
						player[0]->countdown = 0;
						sounds[6].setBuffer(sfx_countdown);
						sounds[6].play();
					}
				}

				if (event.key.code == sf::Keyboard::P)
				{
					if ((event.key.control == 1) && (event.key.shift == 1))
					{
						if (!ctrlSwitch)
						{
							
							if (ctrlSet == 0)
							{
								ctrlSet = 1;
								player[0]->ctrlKey["LMV"] = sf::Keyboard::A;		// Left MoVement
								player[0]->ctrlKey["RMV"] = sf::Keyboard::D;		// Right MoVement
								player[0]->ctrlKey["LSP"] = sf::Keyboard::Numpad4;	// Left SPin
								player[0]->ctrlKey["RSP"] = sf::Keyboard::Numpad6;	// Right SPin
								player[0]->ctrlKey["SDR"] = sf::Keyboard::S;		// Soft DRop
								player[0]->ctrlKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
								player[0]->ctrlKey["HLD"] = sf::Keyboard::Numpad5;	// HoLD
								player[0]->ctrlKey["PSE"] = sf::Keyboard::Escape;	// PauSE
							}
							else
							{
								ctrlSet = 0;
								player[0]->ctrlKey["LMV"] = sf::Keyboard::Left;		// Left MoVement
								player[0]->ctrlKey["RMV"] = sf::Keyboard::Right;	// Right MoVement
								player[0]->ctrlKey["LSP"] = sf::Keyboard::Z;		// Left SPin
								player[0]->ctrlKey["RSP"] = sf::Keyboard::X;		// Right SPin
								player[0]->ctrlKey["SDR"] = sf::Keyboard::Down;		// Soft DRop
								player[0]->ctrlKey["HDR"] = sf::Keyboard::Space;	// Hard DRop
								player[0]->ctrlKey["HLD"] = sf::Keyboard::C;		// HoLD
								player[0]->ctrlKey["PSE"] = sf::Keyboard::Escape;	// PauSE
							}
							ctrlSwitch = true;
						}
					}
				}

				//if (event.key.code == sf::Keyboard::Q)
				//{
				//	window.close();
				//}
			}
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::P)
				{
					ctrlSwitch = false;
				}
			}
		}

		// Rysowanie

		window.setView(view);

		window.clear();

		window.draw(bg);
		window.draw(matrixBg);
		window.draw(soloStats);

		window.draw(*player[0]);
		window.draw(player[0]->holdBox[0]);
		window.draw(player[0]->qBox);
		player[0]->drawPreview(window, ttr_Preview);
		player[0]->drawMatrix(window, ttr_Preview);

		// player[0]->keyTest(window);

		for (int i = 0; i < 6; i++)
		{
			if (player[0]->appl[i]->lifeTime >= 3000)
			{
				player[0]->appl[i]->clear();
			}
		}

		window.display();
	}

	return 0;
}