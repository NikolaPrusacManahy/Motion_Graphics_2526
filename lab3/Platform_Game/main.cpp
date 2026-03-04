// platform game.....
// Author :	Nikola Prusac Manahy
// Student num: C00309098

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h> 
class Game
{
public:
	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum;


	sf::RectangleShape playerShape;


	float velocityX = 0, velocityY = 0, gravity = 0.3;
	bool levelComplete = false;




	static const int numRows = 45;
	static const int numCols = 20;
	int crumbleTimers[numRows][numCols];
	bool crumbleTriggered[numRows][numCols];
	static const int CRUMBLE_DELAY = 30;

	// helping
	// 0 - black (nothing)
	// 1 - red - platform
	// 2 - Blue - Obstacle = dead
	// 3 - Green - goal tile = win
	// 4 - Yellow - jump tile (jump boost)
	// 5 - Magenta - crumbing tile

	int levelData[numRows][numCols] =
	{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,2 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,1,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,0,0,0,0,5,5,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0 },
	{ 0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,4,1,1,1,1,0,1,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0 },
	{ 0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1,0,1,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,3 } };

	int originalLevelData[numRows][numCols];

	sf::RectangleShape level[numRows][numCols];

	Game()
	{

		window.create(sf::VideoMode({ 800, 600 }), "Endless Runner Game");

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				originalLevelData[row][col] = levelData[row][col];
			}
		}
	}
	void init()
	{
		velocityX = 0;
		velocityY = 0;
		gravity = 0.3;
		levelComplete = false;

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				levelData[row][col] = originalLevelData[row][col];
			}
		}

		view = window.getDefaultView();
		playerShape.setSize(sf::Vector2f(20, 20));
		playerShape.setPosition(sf::Vector2f(160, 500));

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{

				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(sf::Vector2f(row * 70, col * 30));

				crumbleTimers[row][col] = CRUMBLE_DELAY;
				crumbleTriggered[row][col] = false;

				switch (levelData[row][col])
				{
				case 0:
					level[row][col].setFillColor(sf::Color::Black);
					break;
				case 1:
					level[row][col].setFillColor(sf::Color::Red);
					break;
				case 2:
					level[row][col].setFillColor(sf::Color::Blue);
					break;
				case 3:
					level[row][col].setFillColor(sf::Color::Green);
					break;
				case 4:
					level[row][col].setFillColor(sf::Color::Yellow);
					break;
				case 5:
					level[row][col].setFillColor(sf::Color::Magenta);
					break;
				}
			}
			std::cout << std::endl;
		}
	}
	void run()
	{


		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


		sf::Time timeSinceLastUpdate = sf::Time::Zero;


		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{


			// check if the close window button is clicked on. 
			while (const std::optional event = window.pollEvent())
			{
				if (event->is<sf::Event::Closed>())
				{
					window.close();
				}
				else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
				{
					if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
						window.close();

					if (levelComplete && keyPressed->scancode == sf::Keyboard::Scancode::Space)
					{
						init();
					}
				}
			}

			timeSinceLastUpdate += clock.restart();

			if (levelComplete)
			{
				window.clear();
				sf::RectangleShape winBanner;
				winBanner.setSize(sf::Vector2f(400, 80));
				winBanner.setFillColor(sf::Color::Green);
				winBanner.setPosition(sf::Vector2f(200, 260));
				window.draw(winBanner);
				window.display();
				timeSinceLastUpdate = sf::Time::Zero;
				continue;
			}

			if (timeSinceLastUpdate > timePerFrame)
			{

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{

						level[row][col].move(sf::Vector2f(-3.7, 0));
					}

				}

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						if (levelData[row][col] == 5 && crumbleTriggered[row][col])
						{
							crumbleTimers[row][col]--;

							if (crumbleTimers[row][col] > 0)
							{
								if (crumbleTimers[row][col] % 4 < 2)
									level[row][col].setFillColor(sf::Color::Magenta);
								else
									level[row][col].setFillColor(sf::Color(100, 0, 100));
							}

							if (crumbleTimers[row][col] <= 0)
							{
								level[row][col].setFillColor(sf::Color::Black);
								levelData[row][col] = 0;
							}
						}
					}
				}


				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space) && velocityY == 0)
				{
					velocityY = -11.8;
				}

				velocityY = velocityY + gravity;
				playerShape.move(sf::Vector2f(0, velocityY));


				gravity = 0.6;

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						int tile = levelData[row][col];

						if (tile == 3)
						{
							if (playerShape.getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
							{
								levelComplete = true;
								break;
							}
						}

						if (tile == 2)
						{
							if (playerShape.getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
							{
								init();
							}
						}

						if (tile == 1 || tile == 5)
						{
							if (velocityY >= 0)
							{
								if (playerShape.getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									if (playerShape.getPosition().y < level[row][col].getPosition().y)
									{
										gravity = 0;
										velocityY = 0;
										playerShape.setPosition(sf::Vector2f(playerShape.getPosition().x, level[row][col].getPosition().y));
										playerShape.move(sf::Vector2f(0, -playerShape.getGlobalBounds().size.y));

										if (tile == 5 && !crumbleTriggered[row][col])
										{
											crumbleTriggered[row][col] = true;
										}

										break;
									}
									else
									{
										init();
									}
								}
							}
							if (velocityY < 0)
							{
								if (playerShape.getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									init();
								}
							}
						}

						if (tile == 4)
						{
							if (velocityY >= 0)
							{
								if (playerShape.getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									if (playerShape.getPosition().y < level[row][col].getPosition().y)
									{
										velocityY = -18.0;
										gravity = 0.6;
										playerShape.setPosition(sf::Vector2f(playerShape.getPosition().x, level[row][col].getPosition().y));
										playerShape.move(sf::Vector2f(0, -playerShape.getGlobalBounds().size.y));
										break;
									}
									else
									{
										init();
									}
								}
							}
							if (velocityY < 0)
							{
								if (playerShape.getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									init();
								}
							}
						}
					}
					if (levelComplete) break;
				}

				if (playerShape.getPosition().y > 600)
				{
					init();
				}

				window.clear();

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						window.draw(level[row][col]);

					}
				}
				window.draw(playerShape);


				window.display();

				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};


int main()
{


	Game game;


	game.init();

	game.run();



	return 0;
}
