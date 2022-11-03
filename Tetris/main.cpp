#include <SFML/Graphics.hpp>

int main()
{
	int windowWidth = 640;
	int windowHeight = 360;

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
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
	int matrixPosX = windowWidth / 2 - 122;
	int matrixPosY = windowHeight / 2 - 241;
	spr_Matrix.setPosition(matrixPosX, matrixPosY);

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