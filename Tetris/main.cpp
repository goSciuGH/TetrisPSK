#include <SFML/Graphics.hpp>

int FULLHD_W = 1920;
int FULLHD_H = 1080;

int main()
{
	bool fullScreen = false;
	
	double windowScale = 1.0/3.0;

	int windowW = FULLHD_W * windowScale;
	int windowH = FULLHD_H * windowScale;

	sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Tetris PSk", sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	//sf::CircleShape shape(100.f);
	//shape.setFillColor(sf::Color::Green);
	sf::Texture ttr_Matrix;
	if (!ttr_Matrix.loadFromFile("sprites/spr_matrix.png"))
	{
		// nyeh
	}
	ttr_Matrix.setSmooth(true);

	sf::Sprite spr_Matrix;
	spr_Matrix.setTexture(ttr_Matrix);
	spr_Matrix.setScale(windowScale, windowScale);
	sf::Vector2u matrixSize = ttr_Matrix.getSize();
	int matrixPosX = windowW / 2.0 - matrixSize.x * windowScale / 2.0;
	int matrixPosY = windowH / 2.0 - matrixSize.y * windowScale / 2.0;
	spr_Matrix.setPosition(matrixPosX, matrixPosY);
	//spr_Matrix.setPosition(200, 20);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		window.draw(spr_Matrix);

		window.display();
	}

	return 0;
}