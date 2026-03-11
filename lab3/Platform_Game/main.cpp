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
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h> 
#include <optional>

class Particle
{
public:
	int timetoLive;
	sf::Vector2f velocity;
	sf::RectangleShape shape;

	void Draw(sf::RenderWindow& win)
	{
		if (timetoLive > 0)
			win.draw(shape);
	}
	void Update()
	{
		if (timetoLive > 0)
		{
			shape.move(velocity);
			timetoLive--;
		}
	}
	Particle() {}
	Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color col)
	{
		shape.setSize(sf::Vector2f(6, 6));
		shape.setPosition(pos);
		shape.setFillColor(col);
		velocity = vel;
		timetoLive = rand() % 150;
	}
};

#define maxParticles 50
class ParticleSystem
{
public:
	Particle particles[maxParticles];
	sf::Vector2f position;

	void Initialise(sf::Vector2f pos, sf::Color col, float spread)
	{
		position = pos;
		for (int i = 0; i < maxParticles; i++)
			particles[i] = Particle(position,
				sf::Vector2f(rand() / double(RAND_MAX) * spread - spread / 2,
					rand() / double(RAND_MAX) * spread - spread / 2),
				col);
	}
	void Update()
	{
		for (int i = 0; i < maxParticles; i++)
			particles[i].Update();
	}
	void Draw(sf::RenderWindow& win)
	{
		for (int i = 0; i < maxParticles; i++)
			particles[i].Draw(win);
	}
	ParticleSystem() {}
};

class Game
{
public:
	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum;

	sf::Texture m_playerTextures;
	std::optional<sf::Sprite> m_playerSprite;

	sf::Texture m_basicTex, m_crumbleTex, m_jumpTex, m_lavaTex, m_obstacleTex, m_winTex;

	float velocityX = 0, velocityY = 0, gravity = 0.3;
	bool levelComplete = false;

	sf::Font m_font;

	static const int numRows = 20;
	static const int numCols = 100;
	int crumbleTimers[numRows][numCols];
	bool crumbleTriggered[numRows][numCols];
	static const int CRUMBLE_DELAY = 30;

	float lavaVelocities[numRows][numCols];

	ParticleSystem m_dustParticles;   // landing
	ParticleSystem m_deathParticles;  // death

	sf::SoundBuffer m_jumpBuffer, m_jumpPadBuffer, m_landingBuffer, m_deathBuffer, m_crumblingBuffer;
	std::optional<sf::Sound> m_jumpSound, m_jumpPadSound, m_landingSound, m_deathSound, m_crumblingSound;


	// helping
	// 0 - black (nothing)
	// 1 - red - platform
	// 2 - Blue - Obstacle = dead
	// 3 - Green - goal tile = win
	// 4 - Yellow - jump tile (jump boost)
	// 5 - Magenta - crumbing tile

	int levelData[numRows][numCols] =
	{
		//row 0  - top of screen (empty sky)
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 1
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 2
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,0,0, 6,0,0,0,0,0,0,0,0,0, 6,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,6, 0,0,0,0,0,0,0,0,0,1 },
		//row 3
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 4
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,1, 6,0,0,0,0,0,6,0,0,1, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 5
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 6
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,5,1, 1,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 7
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 8
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,1,0,0,0, 0,0,0,0,0,6,0,0,0,0, 6,0,0,0,6,0,0,0,0,0, 0,0,1,0,0,0,0,0,0,0, 0,1,1,2,5,5,0,5,5,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 9
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,5,5,5,5, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 10
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 11
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,1,1,1,5,5,5,5, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,5, 5,0,1,1,0,0,0,0,0,0 },
		//row 12
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 6,0,0,0,0,0,0,0,0,0, 5,5,1,2,0,5,1,1,4,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 13
		{ 6,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,6,0,0,0, 0,0,0,1,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 1,1,5,1,1,4,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 14 - STARTING POINT
		{ 0,0,1,1,1,1,1,1,5,5, 0,0,1,1,5,0,1,4,0,0, 1,5,0,0,0,0,0,0,0,0, 0,4,2,2,2,2,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 15 - obstacles on floor
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,5,1,1,5,5,5,0,1,2, 1,1,5,5,0,1,1,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 16
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 17
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 18
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0 },
		//row 19 - goal at the end
		{ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 2,2,2,2,2,3,2,2,2,2 },
	};

	int originalLevelData[numRows][numCols];

	sf::RectangleShape level[numRows][numCols];

	Game()
	{

		window.create(sf::VideoMode({ 800, 600 }), "Endless Runner Game");

		if (m_basicTex.loadFromFile("images/basic.jpg"))
		{
		}
		if (m_crumbleTex.loadFromFile("images/crumble.jpg"))
		{
		}
		if (m_jumpTex.loadFromFile("images/jump_pad.jpg"))
		{
		}
		if (m_lavaTex.loadFromFile("images/lava_block.jpg"))
		{
		}
		if (m_obstacleTex.loadFromFile("images/obstacle.jpg"))
		{
		}
		if (m_winTex.loadFromFile("images/win.jpg"))
		{
		}

		m_jumpBuffer.loadFromFile("audio/jump.wav");
		m_jumpSound.emplace(m_jumpBuffer);

		m_jumpPadBuffer.loadFromFile("audio/jumpPad.wav");
		m_jumpPadSound.emplace(m_jumpPadBuffer);

		m_landingBuffer.loadFromFile("audio/landing.wav");
		m_landingSound.emplace(m_landingBuffer);

		m_deathBuffer.loadFromFile("audio/death.wav");
		m_deathSound.emplace(m_deathBuffer);

		m_crumblingBuffer.loadFromFile("audio/crumbling.wav");
		m_crumblingSound.emplace(m_crumblingBuffer);


		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				originalLevelData[row][col] = levelData[row][col];
			}
		}

		if(m_font.openFromFile("fonts/arial.ttf"))
		{
		}
	}
	void init()
	{
		

		if (!m_playerTextures.loadFromFile("images/running.jpg"))
		{
		}
		m_playerSprite.emplace(m_playerTextures);
		m_playerSprite->setTextureRect(sf::IntRect({ 0,0 }, { 32,62 }));
		m_playerSprite->setPosition(sf::Vector2f{ 160, 14 * 30 - 62 });

		velocityX = 0;
		velocityY = 0;
		gravity = 0.3;
		levelComplete = false;

		m_deathParticles.Initialise(m_playerSprite->getPosition(), sf::Color::Red, 3.0f);


		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				levelData[row][col] = originalLevelData[row][col];
			}
		}

		view = window.getDefaultView();

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{

				level[row][col].setSize(sf::Vector2f(70, 30));
				level[row][col].setPosition(sf::Vector2f(col * 70, row * 30));

				crumbleTimers[row][col] = CRUMBLE_DELAY;
				crumbleTriggered[row][col] = false;

				lavaVelocities[row][col] = (levelData[row][col] == 6) ? 2.0f : 0.0f;

				switch (levelData[row][col])
				{
				case 0:
					level[row][col].setFillColor(sf::Color::Black);
					break;
				case 1:
					level[row][col].setTexture(&m_basicTex);
					break;
				case 2:
					level[row][col].setTexture(&m_obstacleTex);
					break;
				case 3:
					level[row][col].setTexture(&m_winTex);
					break;
				case 4:
					level[row][col].setTexture(&m_jumpTex);
					break;
				case 5:
					level[row][col].setFillColor(sf::Color::White);
					level[row][col].setTexture(&m_crumbleTex);
					break;
				case 6:
					level[row][col].setTexture(&m_lavaTex);
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
				sf::Text winText(m_font);
				winText.setString("YOU WIN!\nPress Space to restart");
				winText.setCharacterSize(48);
				winText.setFillColor(sf::Color::Green);
				winText.setPosition(sf::Vector2f(150, 230));
				window.draw(winText);
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

							if (crumbleTimers[row][col] % 4 < 2)
								level[row][col].move(sf::Vector2f(2.0f, 0));
							else
								level[row][col].move(sf::Vector2f(-2.0f, 0));

							if (crumbleTimers[row][col] <= 0)
							{
								level[row][col].setTexture(nullptr);
								level[row][col].setFillColor(sf::Color::Black);
								levelData[row][col] = 0;
							}
						}

						if (levelData[row][col] == 6)
						{
							level[row][col].move(sf::Vector2f(lavaVelocities[row][col], 0));

							for (int otherCol = 0; otherCol < numCols; otherCol++)
							{
								if (otherCol == col) continue;
								int otherTile = levelData[row][otherCol];
								if (otherTile == 1 || otherTile == 2 || otherTile == 4 || otherTile == 5)
								{
									float lavaX = level[row][col].getPosition().x;
									float otherX = level[row][otherCol].getPosition().x;
									float dist = lavaX - otherX;

									if (dist > -70.0f && dist < 70.0f)
									{
										lavaVelocities[row][col] = -lavaVelocities[row][col];
										level[row][col].move(sf::Vector2f(lavaVelocities[row][col] * 2, 0));
										break;
									}
								}
							}
						}
					}
				}


				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space) && velocityY == 0)
				{
					velocityY = -11.8;
					m_jumpSound->play();
				}

				velocityY = velocityY + gravity;
				m_playerSprite->move(sf::Vector2f(0, velocityY));
				gravity = 0.6;

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						int tile = levelData[row][col];

						if (tile == 3)
						{
							if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
							{
								levelComplete = true;
								break;
							}
						}

						if (tile == 2)
						{
							if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
							{
								m_deathSound->play();
								init();
							}
						}

						if (tile == 1 || tile == 5)
						{
							if (velocityY >= 0)
							{
								if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									if (m_playerSprite->getPosition().y < level[row][col].getPosition().y)
									{
										gravity = 0;
										velocityY = 0;
										m_playerSprite->setPosition(sf::Vector2f(m_playerSprite->getPosition().x, level[row][col].getPosition().y));
										m_playerSprite->move(sf::Vector2f(0, -m_playerSprite->getGlobalBounds().size.y));
										m_landingSound->play();
										m_dustParticles.Initialise(m_playerSprite->getPosition(), sf::Color::White, 1.0f);

										if (tile == 5 && !crumbleTriggered[row][col])
										{
											crumbleTriggered[row][col] = true;
											m_crumblingSound->play();
										}

										break;
									}
									else
									{
										m_deathSound->play();
										init();
									}
								}
							}
							if (velocityY < 0)
							{
								if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									m_deathSound->play();
									init();
								}
							}
						}

						if (tile == 4)
						{
							if (velocityY >= 0)
							{
								if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									if (m_playerSprite->getPosition().y < level[row][col].getPosition().y)
									{
										velocityY = -18.0;
										gravity = 0.6;
										m_playerSprite->setPosition(sf::Vector2f(m_playerSprite->getPosition().x, level[row][col].getPosition().y));
										m_playerSprite->move(sf::Vector2f(0, -m_playerSprite->getGlobalBounds().size.y));
										m_jumpPadSound->play();
										break;
									}
									else
									{
										m_deathSound->play();
										init();
									}
								}
							}
							if (velocityY < 0)
							{
								if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
								{
									m_deathSound->play();
									init();
								}
							}
						}

						if (tile == 6)
						{
							if (m_playerSprite->getGlobalBounds().findIntersection(level[row][col].getGlobalBounds()))
							{
								m_deathSound->play();
								init();
							}
						}
					}
					if (levelComplete) break;
				}

				if (m_playerSprite->getPosition().y > 600)
				{
					m_deathSound->play();
					init();
				}

				window.clear();
				
				

				for (int row = 0; row < numRows; row++)
				{
					for (int col = 0; col < numCols; col++)
					{
						if (levelData[row][col] != 0)
							window.draw(level[row][col]);
					}
				}

				m_dustParticles.Update();
				m_dustParticles.Draw(window);
				m_deathParticles.Update();
				m_deathParticles.Draw(window);

				window.draw(*m_playerSprite);


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
