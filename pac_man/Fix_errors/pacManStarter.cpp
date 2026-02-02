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
#include <stdlib.h> 
#include <time.h>

int main()
{
	const int SCREEN_WIDTH = 600;
	const int SCREEN_HEIGHT = 600;
	const float GAMEPLAY_Y = 300.0f;

	sf::RenderWindow window(sf::VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }, 32), "PacMan", sf::Style::Default);

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Clock frameClock;
	frameClock.restart();

	sf::Clock vulnerabilityClock;
	float vulnerabilityDuration = 5.0f;

	sf::Texture pacmanTexture;
	if (!pacmanTexture.loadFromFile("ASSETS/IMAGES/pacman3.png"))
	{
	}

	sf::Font gameFont;
	if (!gameFont.openFromFile("ASSETS/FONTS/arial.ttf"))
	{
	}

	int score = 0;
	int highScore = 0;
	bool isGameOver = false;
	bool isGhostVulnerable = false;

	sf::Text gameOverText(gameFont);
	gameOverText.setString("GAME OVER");
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setOrigin(gameOverText.getLocalBounds().size / 2.0f);
	gameOverText.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 50.f));

	sf::Text restartText(gameFont);
	restartText.setString("Press 'SPACE' to restart");
	restartText.setFillColor(sf::Color::White);
	restartText.setOrigin(restartText.getLocalBounds().size / 2.0f);
	restartText.setPosition(sf::Vector2f(gameOverText.getPosition().x, gameOverText.getPosition().y + 30.0f));

	sf::Text scoreText(gameFont);
	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setFillColor(sf::Color::White);
	scoreText.setOrigin(scoreText.getLocalBounds().size / 2.0f);
	scoreText.setPosition(sf::Vector2f(500.0f, 25.0f));

	sf::Text highScoreText(gameFont);
	highScoreText.setString("High Score: " + std::to_string(highScore));
	highScoreText.setFillColor(sf::Color::White);
	highScoreText.setOrigin(highScoreText.getLocalBounds().size / 2.0f);
	highScoreText.setPosition(sf::Vector2f(100.0f, 25.0f));

	sf::Sprite pacmanSprite(pacmanTexture);
	pacmanSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));
	pacmanSprite.setOrigin(sf::Vector2f(16, 16));

	float pacmanX = 50.0f;
	float pacmanY = GAMEPLAY_Y;
	float pacmanSpeed = 2.0f;
	bool spaceWasPressed = false;
	int pacmanFrameRate = 10;
	int pacmanTotalFrames = 3;
	int pacmanCurrentFrame = 0;
	int pacmanFrameCounter = 0;
	int pacmanFrameWidth = 32;
	int pacmanFrameHeight = 32;

	sf::RectangleShape ghostRect;
	float ghostWidth = 30.0f;
	float ghostHeight = 30.0f;
	ghostRect.setSize(sf::Vector2f(ghostWidth, ghostHeight));
	ghostRect.setFillColor(sf::Color::Red);

	float ghostX = 550.0f;
	float ghostY = GAMEPLAY_Y - 15.0f;
	float ghostSpeedNormal = 2.2f;
	float ghostSpeedVulnerable = 1.0f;

	srand(time(NULL));

	const int TOTAL_PELLETS = 10;
	const float PELLET_SPACING = 50.0f;
	sf::CircleShape pellets[TOTAL_PELLETS];

	for (int i = 0; i < TOTAL_PELLETS; i++)
	{
		pellets[i].setFillColor(sf::Color::Yellow);
		pellets[i].setRadius(10.0f);
		pellets[i].setOrigin(sf::Vector2f(10.0f, 10.0f));
		pellets[i].setPosition(sf::Vector2f(30.0f + PELLET_SPACING * i, GAMEPLAY_Y));
	}

	sf::CircleShape powerPellet;
	powerPellet.setFillColor(sf::Color::Yellow);
	powerPellet.setRadius(18.0f);
	powerPellet.setOrigin(sf::Vector2f(18.0f, 18.0f));
	powerPellet.setPosition(sf::Vector2f(30.0f + PELLET_SPACING * TOTAL_PELLETS, GAMEPLAY_Y));

	while (window.isOpen())
	{
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
			}
		}

		timeSinceLastUpdate += frameClock.restart();

		if (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate = sf::Time::Zero;

			if (isGameOver)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
				{
					isGameOver = false;
					isGhostVulnerable = false;
					score = 0;
					pacmanX = 50.0f;
					pacmanSpeed = 2.0f;

					if (pacmanX > SCREEN_WIDTH / 2.0f)
					{
						ghostX = 50.0f;
					}
					else
					{
						ghostX = SCREEN_WIDTH - ghostWidth - 20.0f;
					}

					scoreText.setString("Score: " + std::to_string(score));
					scoreText.setOrigin(scoreText.getLocalBounds().size / 2.0f);

					highScoreText.setString("High Score: " + std::to_string(highScore));
					highScoreText.setOrigin(highScoreText.getLocalBounds().size / 2.0f);

					for (int i = 0; i < TOTAL_PELLETS; i++)
					{
						pellets[i].setPosition(sf::Vector2f(30.0f + PELLET_SPACING * i, GAMEPLAY_Y));
					}

					powerPellet.setPosition(sf::Vector2f(30.0f + PELLET_SPACING * TOTAL_PELLETS, GAMEPLAY_Y));
					ghostRect.setFillColor(sf::Color::Red);
				}
			}
			else
			{
				if (pacmanFrameCounter >= 60 / pacmanFrameRate)
				{
					if (pacmanCurrentFrame < pacmanTotalFrames - 1)
					{
						pacmanCurrentFrame++;
					}
					else
					{
						pacmanCurrentFrame = 0;
					}
					pacmanFrameCounter = 0;
				}
				else
				{
					pacmanFrameCounter++;
				}

				int textureLeft = pacmanCurrentFrame * pacmanFrameWidth;
				int textureTop = 0;
				pacmanSprite.setTextureRect(sf::IntRect({ textureLeft, textureTop }, { pacmanFrameWidth, pacmanFrameHeight }));
				pacmanSprite.setOrigin(sf::Vector2f(16, 16));

				if (pacmanSpeed < 0)
				{
					pacmanSprite.setScale(sf::Vector2f(-1, 1));
				}
				else
				{
					pacmanSprite.setScale(sf::Vector2f(1, 1));
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
				{
					if (!spaceWasPressed)
					{
						pacmanSpeed *= -1;
						spaceWasPressed = true;
					}
				}
				else
				{
					spaceWasPressed = false;
				}

				ghostRect.setPosition(sf::Vector2f(ghostX, ghostY));

				if (isGhostVulnerable)
				{
					if (vulnerabilityClock.getElapsedTime().asSeconds() > vulnerabilityDuration)
					{
						isGhostVulnerable = false;
						ghostRect.setFillColor(sf::Color::Red);
					}
				}

				for (int i = 0; i < TOTAL_PELLETS; i++)
				{
					if (pacmanSprite.getGlobalBounds().findIntersection(pellets[i].getGlobalBounds()))
					{
						pellets[i].setPosition(sf::Vector2f(1000, 1000));
						score += 100;
						scoreText.setString("Score: " + std::to_string(score));
						scoreText.setOrigin(scoreText.getLocalBounds().size / 2.0f);
					}
				}

				if (pacmanSprite.getGlobalBounds().findIntersection(powerPellet.getGlobalBounds()))
				{
					powerPellet.setPosition(sf::Vector2f(1000, 1000));
					isGhostVulnerable = true;
					vulnerabilityClock.restart();
					ghostRect.setFillColor(sf::Color::Blue);
					score += 500;
					scoreText.setString("Score: " + std::to_string(score));
					scoreText.setOrigin(scoreText.getLocalBounds().size / 2.0f);
				}

				if (pacmanSprite.getGlobalBounds().findIntersection(ghostRect.getGlobalBounds()))
				{
					if (isGhostVulnerable)
					{
						if (pacmanX > SCREEN_WIDTH / 2.0f)
						{
							ghostX = 50.0f;
						}
						else
						{
							ghostX = SCREEN_WIDTH - ghostWidth - 20.0f;
						}

						score += 500;
						scoreText.setString("Score: " + std::to_string(score));
						scoreText.setOrigin(scoreText.getLocalBounds().size / 2.0f);
						isGhostVulnerable = false;
						ghostRect.setFillColor(sf::Color::Red);
					}
					else
					{
						isGameOver = true;
						if (highScore < score)
						{
							highScore = score;
						}
					}
				}

				pacmanX += pacmanSpeed;

				if (pacmanX < -16.0f)
				{
					pacmanX = SCREEN_WIDTH + 16.0f;
				}
				else if (pacmanX > SCREEN_WIDTH + 16.0f)
				{
					pacmanX = -16.0f;
				}

				pacmanSprite.setPosition(sf::Vector2f(pacmanX, pacmanY));

				float currentGhostSpeed = isGhostVulnerable ? ghostSpeedVulnerable : ghostSpeedNormal;

				if (isGhostVulnerable)
				{
					if (pacmanSprite.getPosition().x < ghostRect.getPosition().x)
					{
						ghostX += currentGhostSpeed;
					}
					else if (pacmanSprite.getPosition().x > ghostRect.getPosition().x)
					{
						ghostX -= currentGhostSpeed;
					}
				}
				else
				{
					if (pacmanSprite.getPosition().x < ghostRect.getPosition().x)
					{
						ghostX -= currentGhostSpeed;
					}
					else if (pacmanSprite.getPosition().x > ghostRect.getPosition().x)
					{
						ghostX += currentGhostSpeed;
					}
				}

				if (ghostX < 0.0f)
				{
					ghostX = 0.0f;
				}
				else if (ghostX > SCREEN_WIDTH - ghostWidth)
				{
					ghostX = SCREEN_WIDTH - ghostWidth;
				}

				bool allPelletsEaten = true;
				for (int i = 0; i < TOTAL_PELLETS; i++)
				{
					if (pellets[i].getPosition().x < 900)
					{
						allPelletsEaten = false;
						break;
					}
				}

				bool powerPelletEaten = (powerPellet.getPosition().x > 900);

				if (allPelletsEaten && powerPelletEaten)
				{
					int randomOffset = rand() % 100;
					for (int i = 0; i < TOTAL_PELLETS; i++)
					{
						float xPos = 30.0f + PELLET_SPACING * i + randomOffset;
						if (xPos > SCREEN_WIDTH - 30.0f)
						{
							xPos -= SCREEN_WIDTH;
						}
						pellets[i].setPosition(sf::Vector2f(xPos, GAMEPLAY_Y));
					}

					float powerX = 30.0f + PELLET_SPACING * TOTAL_PELLETS + randomOffset;
					if (powerX > SCREEN_WIDTH - 30.0f)
					{
						powerX -= SCREEN_WIDTH;
					}
					powerPellet.setPosition(sf::Vector2f(powerX, GAMEPLAY_Y));
				}
			}

			window.clear();

			for (int i = 0; i < TOTAL_PELLETS; i++)
			{
				window.draw(pellets[i]);
			}

			window.draw(powerPellet);
			window.draw(ghostRect);
			window.draw(pacmanSprite);
			window.draw(scoreText);
			window.draw(highScoreText);

			if (isGameOver)
			{
				window.draw(gameOverText);
				window.draw(restartText);
			}

			window.display();
		}
	}

	return 0;
}