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
	restartText.setPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 80.0f));

	sf::Text scoreText(gameFont);
	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(sf::Vector2f(450.0f, 10.0f));

	sf::Text highScoreText(gameFont);
	highScoreText.setString("High Score: " + std::to_string(highScore));
	highScoreText.setFillColor(sf::Color::White);
	highScoreText.setPosition(sf::Vector2f(10.0f, 10.0f));

	sf::Sprite pacmanSprite(pacmanTexture);
	pacmanSprite.setTextureRect(sf::IntRect({ 0, 0 }, { 32, 32 }));
	pacmanSprite.setOrigin(sf::Vector2f(16, 16));

	float pacmanX = 50.0f;
	float pacmanY = GAMEPLAY_Y;
	float pacmanSpeed = 2.0f;
	bool spaceWasPressed = false;
	int pacmanTotalFrames = 3;
	int pacmanCurrentFrame = 0;
	int pacmanFrameCounter = 0;

	sf::RectangleShape ghostRect;
	ghostRect.setSize(sf::Vector2f(30.0f, 30.0f));
	ghostRect.setFillColor(sf::Color::Red);
	float ghostX = 550.0f;
	float ghostY = GAMEPLAY_Y - 15.0f;
	float ghostSpeed = 2.0f;

	srand(time(NULL));

	const int TOTAL_PELLETS = 10;
	sf::CircleShape pellets[TOTAL_PELLETS];
	for (int i = 0; i < TOTAL_PELLETS; i++)
	{
		pellets[i].setFillColor(sf::Color::Yellow);
		pellets[i].setRadius(10.0f);
		pellets[i].setOrigin(sf::Vector2f(10.0f, 10.0f));
		pellets[i].setPosition(sf::Vector2f(30.0f + 50.0f * i, GAMEPLAY_Y));
	}

	sf::CircleShape powerPellet;
	powerPellet.setFillColor(sf::Color::Yellow);
	powerPellet.setRadius(18.0f);
	powerPellet.setOrigin(sf::Vector2f(18.0f, 18.0f));
	powerPellet.setPosition(sf::Vector2f(550.0f, GAMEPLAY_Y));

	while (window.isOpen())
	{
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					window.close();
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
					ghostX = 550.0f;
					ghostRect.setFillColor(sf::Color::Red);
					scoreText.setString("Score: " + std::to_string(score));
					highScoreText.setString("High Score: " + std::to_string(highScore));
					for (int i = 0; i < TOTAL_PELLETS; i++)
						pellets[i].setPosition(sf::Vector2f(30.0f + 50.0f * i, GAMEPLAY_Y));
					powerPellet.setPosition(sf::Vector2f(550.0f, GAMEPLAY_Y));
				}
			}
			else
			{
				pacmanFrameCounter++;
				if (pacmanFrameCounter >= 6)
				{
					pacmanCurrentFrame = (pacmanCurrentFrame + 1) % pacmanTotalFrames;
					pacmanFrameCounter = 0;
				}
				pacmanSprite.setTextureRect(sf::IntRect({ pacmanCurrentFrame * 32, 0 }, { 32, 32 }));
				pacmanSprite.setScale(sf::Vector2f(pacmanSpeed > 0 ? 1.0f : -1.0f, 1.0f));

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
				{
					if (!spaceWasPressed)
					{
						pacmanSpeed *= -1;
						spaceWasPressed = true;
					}
				}
				else
					spaceWasPressed = false;

				pacmanX += pacmanSpeed;
				if (pacmanX < -16.0f) pacmanX = SCREEN_WIDTH + 16.0f;
				if (pacmanX > SCREEN_WIDTH + 16.0f) pacmanX = -16.0f;
				pacmanSprite.setPosition(sf::Vector2f(pacmanX, pacmanY));

				ghostRect.setPosition(sf::Vector2f(ghostX, ghostY));

				if (isGhostVulnerable)
				{
					if (vulnerabilityClock.getElapsedTime().asSeconds() > vulnerabilityDuration)
					{
						isGhostVulnerable = false;
						ghostRect.setFillColor(sf::Color::Red);
					}
					if (pacmanX < ghostX) ghostX += 1.0f;
					else ghostX -= 1.0f;
				}
				else
				{
					if (pacmanX < ghostX) ghostX -= ghostSpeed;
					else ghostX += ghostSpeed;
				}

				if (ghostX < 0.0f) ghostX = 0.0f;
				if (ghostX > SCREEN_WIDTH - 30.0f) ghostX = SCREEN_WIDTH - 30.0f;

				for (int i = 0; i < TOTAL_PELLETS; i++)
				{
					if (pacmanSprite.getGlobalBounds().findIntersection(pellets[i].getGlobalBounds()))
					{
						pellets[i].setPosition(sf::Vector2f(1000.0f, 1000.0f));
						score += 100;
						scoreText.setString("Score: " + std::to_string(score));
					}
				}

				if (pacmanSprite.getGlobalBounds().findIntersection(powerPellet.getGlobalBounds()))
				{
					powerPellet.setPosition(sf::Vector2f(1000.0f, 1000.0f));
					isGhostVulnerable = true;
					vulnerabilityClock.restart();
					ghostRect.setFillColor(sf::Color::Blue);
					score += 500;
					scoreText.setString("Score: " + std::to_string(score));
				}

				if (pacmanSprite.getGlobalBounds().findIntersection(ghostRect.getGlobalBounds()))
				{
					if (isGhostVulnerable)
					{
						ghostX = 550.0f;
						score += 500;
						scoreText.setString("Score: " + std::to_string(score));
						isGhostVulnerable = false;
						ghostRect.setFillColor(sf::Color::Red);
					}
					else
					{
						isGameOver = true;
						if (score > highScore) highScore = score;
					}
				}

				bool allEaten = true;
				for (int i = 0; i < TOTAL_PELLETS; i++)
					if (pellets[i].getPosition().x < 900.0f) allEaten = false;
				if (allEaten && powerPellet.getPosition().x > 900.0f)
				{
					for (int i = 0; i < TOTAL_PELLETS; i++)
						pellets[i].setPosition(sf::Vector2f(30.0f + 50.0f * i, GAMEPLAY_Y));
					powerPellet.setPosition(sf::Vector2f(550.0f, GAMEPLAY_Y));
				}
			}

			window.clear();
			for (int i = 0; i < TOTAL_PELLETS; i++)
				window.draw(pellets[i]);
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