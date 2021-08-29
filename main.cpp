#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <sstream>
#include "map.h"



enum class Directions
{
	RIGHT = 1,
	DOWN,
	LEFT,
	UP
};

class Snake
{
private:

	float axisX, axisY;

	sf::Image imageSnake;
	sf::Texture textureSnake;
	sf::Sprite spriteSnakeHead, spriteSnakeTail;
	std::vector<std::pair<sf::Sprite, bool>> spriteSnakeBody;
	bool foodIsEaten = false;

	Directions direction = Directions::RIGHT;
	Directions previousDirection = direction;

	bool rotate = false;
	int ID = 0;
	std::vector<sf::Sprite> rotationSprites;

public:

	Snake(const sf::String& roadToImage, float axisX, float axisY)
	{
		this->axisX = axisX;
		this->axisY = axisY;

		imageSnake.loadFromFile(roadToImage);
		textureSnake.loadFromImage(imageSnake);
		spriteSnakeHead.setTexture(textureSnake);
		spriteSnakeTail.setTexture(textureSnake);

		spriteSnakeHead.setTextureRect(sf::IntRect(64, 0, 32, 32));
		spriteSnakeHead.setOrigin(16, 16);
		spriteSnakeTail.setTextureRect(sf::IntRect(0, 0, 32, 32));
		spriteSnakeTail.setOrigin(16, 16);

		eatFood();
		eatFood();
		eatFood();

		setPosition(axisX, axisY);
	}

	void setPosition(const float axisX, const float axisY)
	{
		spriteSnakeHead.setPosition(axisX, axisY);
		spriteSnakeTail.setPosition((axisX - spriteSnakeBody.size() * 32) - 32, axisY);

		for (unsigned int i = 0; i < spriteSnakeBody.size(); ++i)
		{
			spriteSnakeBody[i].first.setPosition(axisX - (i + 1) * 32, axisY);
		}
	}

	void move(const Directions direction, bool foodEaten)
	{
		foodIsEaten = foodEaten;

		switch (direction)
		{
		case Directions::RIGHT:

			if (this->direction != Directions::RIGHT)
			{
				previousDirection = this->direction;
				rotate = true;
			}

			this->direction = direction;
			moveOnRightDirection(32, 0, 0);

			break;

		case Directions::DOWN:

			if (this->direction != Directions::DOWN)
			{
				previousDirection = this->direction;
				rotate = true;
			}

			this->direction = direction;
			moveOnRightDirection(0, 32, 90);

			break;

		case Directions::LEFT:

			if (this->direction != Directions::LEFT)
			{
				previousDirection = this->direction;
				rotate = true;
			}

			this->direction = direction;
			moveOnRightDirection(-32, 0, 180);

			break;

		case Directions::UP:

			if (this->direction != Directions::UP)
			{
				previousDirection = this->direction;
				rotate = true;
			}

			this->direction = direction;
			moveOnRightDirection(0, -32, 270);

			break;
		}
	}

	void moveOnRightDirection(float axisX, float axisY, float angleOfRotation)
	{
		std::vector<std::pair<int, int>> coordinatesSnakesBody;
		std::vector<float> anglesForRotations;

		if (foodIsEaten)
		{
			eatFood();
		}

		for (unsigned int i = 0; i < spriteSnakeBody.size(); ++i)
		{
			spriteSnakeBody[i].second = false;
		}

		if (rotate)
		{
			sf::Sprite temp;
			temp.setTexture(textureSnake);
			temp.setTextureRect(sf::IntRect(128, 0, 32, 32));
			temp.setPosition(sf::Vector2f(spriteSnakeHead.getPosition().x, spriteSnakeHead.getPosition().y));
			temp.setOrigin(16, 16);

			if ((previousDirection == Directions::RIGHT) && (direction == Directions::DOWN))
			{
				temp.setRotation(90);
			}
			else if ((previousDirection == Directions::RIGHT) && (direction == Directions::UP))
			{
				temp.setRotation(180);
			}
			else if ((previousDirection == Directions::DOWN) && (direction == Directions::RIGHT))
			{
				temp.setRotation(270);
			}
			else if ((previousDirection == Directions::DOWN) && (direction == Directions::LEFT))
			{
				temp.setRotation(180);
			}
			else if ((previousDirection == Directions::LEFT) && (direction == Directions::DOWN))
			{
				temp.setRotation(0);
			}
			else if ((previousDirection == Directions::LEFT) && (direction == Directions::UP))
			{
				temp.setRotation(270);
			}
			else if ((previousDirection == Directions::UP) && (direction == Directions::RIGHT))
			{
				temp.setRotation(0);
			}
			else if ((previousDirection == Directions::UP) && (direction == Directions::LEFT))
			{
				temp.setRotation(90);
			}

			rotationSprites.push_back(temp);
		}

		if (angleOfRotation == 180)
		{
			spriteSnakeHead.setTextureRect(sf::IntRect(64, 0, 32, 32));
			spriteSnakeHead.setScale(1, -1);
		}
		else if (angleOfRotation == 90)
		{
			spriteSnakeHead.setTextureRect(sf::IntRect(96, 0, 32, 32));
			spriteSnakeHead.setScale(1, -1);
		}
		else if (angleOfRotation == 270)
		{
			spriteSnakeHead.setTextureRect(sf::IntRect(96, 0, 32, 32));
			spriteSnakeHead.setScale(1, 1);
		}
		else
		{
			spriteSnakeHead.setTextureRect(sf::IntRect(64, 0, 32, 32));
			spriteSnakeHead.setScale(1, 1);
		}

		coordinatesSnakesBody.push_back(std::make_pair(spriteSnakeHead.getPosition().x, spriteSnakeHead.getPosition().y));
		anglesForRotations.push_back(angleOfRotation);

		for (unsigned int i = 0; i < spriteSnakeBody.size(); ++i)
		{
			coordinatesSnakesBody.push_back(std::make_pair(spriteSnakeBody[i].first.getPosition().x, spriteSnakeBody[i].first.getPosition().y));
			anglesForRotations.push_back(spriteSnakeBody[i].first.getRotation());
		}

		spriteSnakeHead.move(axisX, axisY);
		spriteSnakeHead.setRotation(angleOfRotation);

		for (unsigned int i = 0; i < spriteSnakeBody.size(); ++i)
		{
			spriteSnakeBody[i].first.setPosition(coordinatesSnakesBody[i].first, coordinatesSnakesBody[i].second);

			if (!rotationSprites.empty())
			{
				for (unsigned int j = 0; j < rotationSprites.size(); ++j)
				{
					if ((spriteSnakeBody[i].first.getPosition().x == rotationSprites[j].getPosition().x)
						&& (spriteSnakeBody[i].first.getPosition().y == rotationSprites[j].getPosition().y))
					{
						spriteSnakeBody[i].first = rotationSprites[j];
						spriteSnakeBody[i].second = true;
					}
					else if (spriteSnakeBody[i].second == false)
					{
						spriteSnakeBody[i].first.setTextureRect(sf::IntRect(32, 0, 32, 32));
						spriteSnakeBody[i].first.setRotation(anglesForRotations[i]);
					}
				}
			}
			else
			{
				for (unsigned int i = 0; i < spriteSnakeBody.size(); ++i)
				{
					spriteSnakeBody[i].first.setTextureRect(sf::IntRect(32, 0, 32, 32));
					spriteSnakeBody[i].first.setRotation(anglesForRotations[i]);
				}
			}

			if (i == spriteSnakeBody.size() - 1)
			{
				spriteSnakeTail.setPosition(coordinatesSnakesBody[coordinatesSnakesBody.size() - 1].first, coordinatesSnakesBody[coordinatesSnakesBody.size() - 1].second);

				if (spriteSnakeBody[i].second == false)
				{
					spriteSnakeTail.setRotation(spriteSnakeBody[i].first.getRotation());
				}
				else
				{
					if (spriteSnakeBody[i].first.getRotation() == 0)
					{
						if (spriteSnakeBody[i].first.getPosition().y + 32 == spriteSnakeTail.getPosition().y)
						{
							spriteSnakeTail.setRotation(270);
						}
						else
						{
							spriteSnakeTail.setRotation(180);
						}
					}
					else if (spriteSnakeBody[i].first.getRotation() == 90)
					{
						if (spriteSnakeBody[i].first.getPosition().y + 32 == spriteSnakeTail.getPosition().y)
						{
							spriteSnakeTail.setRotation(270);
						}
						else
						{
							spriteSnakeTail.setRotation(0);
						}
					}
					else if (spriteSnakeBody[i].first.getRotation() == 180)
					{
						if (spriteSnakeBody[i].first.getPosition().y - 32 == spriteSnakeTail.getPosition().y)
						{
							spriteSnakeTail.setRotation(90);
						}
						else
						{
							spriteSnakeTail.setRotation(0);
						}
					}
					else
					{
						if (spriteSnakeBody[i].first.getPosition().y - 32 == spriteSnakeTail.getPosition().y)
						{
							spriteSnakeTail.setRotation(90);
						}
						else
						{
							spriteSnakeTail.setRotation(180);
						}
					}

				}
			}
		}

		if (!rotationSprites.empty())
		{
			if ((spriteSnakeTail.getPosition().x == rotationSprites[0].getPosition().x)
				&& (spriteSnakeTail.getPosition().y == rotationSprites[0].getPosition().y))
			{
				rotationSprites.erase(std::begin(rotationSprites));
			}
		}

		rotate = false;
	}

	void eatFood()
	{
		sf::Sprite tempBody;
		tempBody.setTexture(textureSnake);
		tempBody.setTextureRect(sf::IntRect(32, 0, 32, 32));
		tempBody.setOrigin(16, 16);

		tempBody.setPosition(spriteSnakeTail.getPosition().x, spriteSnakeTail.getPosition().y);

		spriteSnakeBody.push_back(std::make_pair(tempBody, false));
	}

	sf::Sprite getHead() { return spriteSnakeHead; }
	sf::Sprite getHead() const { return spriteSnakeHead; }
	sf::Sprite getBody(const int index) { return spriteSnakeBody[index].first; }
	sf::Sprite getBody(const int index) const { return spriteSnakeBody[index].first; }
	sf::Sprite getTail() { return spriteSnakeTail; }
	sf::Sprite getTail() const { return spriteSnakeTail; }

	int getLengthOfBody() { return spriteSnakeBody.size(); }

	~Snake()
	{

	}
};

void changePositionForSnakeFood(sf::Sprite& foodforSnake, sf::Vector2f& randomCellForFood, const int width, const int height, Snake& snake)
{
	randomCellForFood.x = static_cast<float>((rand() % (width - 2) + 1) * 32);
	randomCellForFood.y = static_cast<float>((rand() % (height - 2) + 1) * 32);

	foodforSnake.setPosition(randomCellForFood);

	if (foodforSnake.getGlobalBounds().intersects(snake.getHead().getGlobalBounds()))
	{
		changePositionForSnakeFood(foodforSnake, randomCellForFood, width, height, snake);
	}

	for (int i = 0; i < snake.getLengthOfBody(); ++i)
	{
		if (foodforSnake.getGlobalBounds().intersects(snake.getBody(i).getGlobalBounds()))
		{
			changePositionForSnakeFood(foodforSnake, randomCellForFood, width, height, snake);
		}
	}

	if (foodforSnake.getGlobalBounds().intersects(snake.getTail().getGlobalBounds()))
	{
		changePositionForSnakeFood(foodforSnake, randomCellForFood, width, height, snake);
	}

	return;
}

int main()
{
	srand(time(0));
	rand();

	sf::RenderWindow windowWithGame(sf::VideoMode(640, 640), "Snake", sf::Style::Close);

	sf::SoundBuffer bufferCreeps;
	bufferCreeps.loadFromFile("audio/creeps.ogg");
	sf::Sound soundCreeps;
	soundCreeps.setBuffer(bufferCreeps);
	soundCreeps.setVolume(70);

	sf::Music music;
	music.openFromFile("audio/music.ogg");
	music.setVolume(70);
	music.play();
	music.setLoop(true);

	sf::Font font;
	font.loadFromFile("sunday.ttf");
	sf::Text text("", font, 20);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	int scores = 0;

	sf::Image imageWall;
	imageWall.loadFromFile("images/wall.png");
	sf::Texture textureWall;
	textureWall.loadFromImage(imageWall);
	sf::Sprite spriteWall;
	spriteWall.setTexture(textureWall);
	spriteWall.setTextureRect(sf::IntRect(0, 0, 32, 32));

	Snake snake("images/snake.png", widthOfGameField / 2 * 32 - 16, heighthOfGameField / 2 * 32 - 16);

	sf::Image imageFoodForSnake;
	imageFoodForSnake.loadFromFile("images/foodForSnake.png");
	sf::Texture textureFoodSnake;
	textureFoodSnake.loadFromImage(imageFoodForSnake);
	sf::Sprite spriteFoodSnake;
	spriteFoodSnake.setTexture(textureFoodSnake);
	spriteFoodSnake.setTextureRect(sf::IntRect(0, 0, 32, 32));

	sf::Vector2f randomCellForSnakeFood;
	changePositionForSnakeFood(spriteFoodSnake, randomCellForSnakeFood, widthOfGameField, heighthOfGameField, snake);
	
	sf::Clock clock;
	float time = 0.0f;
	bool keyCanBePressed = true;
	bool foodIsEaten = false;
	bool isDead = false;
	bool isLose = false;
	float delay = 0.5f;

	Directions direction = Directions::RIGHT;

	while (windowWithGame.isOpen())
	{
		time += clock.getElapsedTime().asSeconds();
		clock.restart();

		sf::Event event;

		while (windowWithGame.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				windowWithGame.close();
			}

			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Right:

					if ((direction != Directions::LEFT) && (keyCanBePressed))
					{
						direction = Directions::RIGHT;
						keyCanBePressed = false;
					}

					break;

				case sf::Keyboard::Down:

					if ((direction != Directions::UP) && (keyCanBePressed))
					{
						direction = Directions::DOWN;
						keyCanBePressed = false;
					}

					break;

				case sf::Keyboard::Left:

					if ((direction != Directions::RIGHT) && (keyCanBePressed))
					{
						direction = Directions::LEFT;
						keyCanBePressed = false;
					}

					break;

				case sf::Keyboard::Up:

					if ((direction != Directions::DOWN) && (keyCanBePressed))
					{
						direction = Directions::UP;
						keyCanBePressed = false;
					}

					break;

				default:

					direction = direction;
					break;
				}
			}
		}

		if ((time > delay) && (!isLose))
		{
			snake.move(direction, foodIsEaten);
			soundCreeps.play();
			time = 0.0f;
			keyCanBePressed = true;

			foodIsEaten = false;

			for (int i = (snake.getHead().getGlobalBounds().top + 1) / 32; i < (snake.getHead().getGlobalBounds().top + snake.getHead().getGlobalBounds().height) / 32; ++i)
			{
				for (int j = (snake.getHead().getGlobalBounds().left + 1) / 32; j < (snake.getHead().getGlobalBounds().left + snake.getHead().getGlobalBounds().width) / 32; ++j)
				{
					if (fieldWithGame[i][j] == 'w')
					{
						isLose = true;
					}
				}
			}

			for (int i = 1; i < snake.getLengthOfBody(); ++i)
			{
				if (snake.getHead().getGlobalBounds().intersects(snake.getBody(i).getGlobalBounds()))
				{
					isLose = true;
				}
			}

			if (snake.getHead().getGlobalBounds().intersects(snake.getTail().getGlobalBounds()))
			{
				isLose = true;
			}

			if (snake.getHead().getGlobalBounds().intersects(spriteFoodSnake.getGlobalBounds()))
			{
				changePositionForSnakeFood(spriteFoodSnake, randomCellForSnakeFood, widthOfGameField, heighthOfGameField, snake);
				++scores;

				if ((scores > 0) && (scores % 10 == 0))
				{
					delay -= 0.1f;
				}

				foodIsEaten = true;
			}
		}

		windowWithGame.clear(sf::Color(69, 213, 226, 255));

		windowWithGame.draw(snake.getHead());

		for (int i = 0; i < snake.getLengthOfBody(); ++i)
		{
			windowWithGame.draw(snake.getBody(i));
		}

		windowWithGame.draw(snake.getTail());

		windowWithGame.draw(spriteFoodSnake);

		for (unsigned int i = 0; i < heighthOfGameField; ++i)
		{
			for (unsigned int j = 0; j < widthOfGameField; ++j)
			{
				if (fieldWithGame[i][j] == 'w')
				{
					spriteWall.setPosition(j * 32, i * 32);
					windowWithGame.draw(spriteWall);
				}
			}
		}

		std::ostringstream playerScoreString;
		playerScoreString << scores;
		text.setString("Scores: " + playerScoreString.str());
		text.setPosition(32, 32);
		windowWithGame.draw(text);

		windowWithGame.display();

		if (isLose)
		{
			sf::SoundBuffer bufferEnd;
			bufferEnd.loadFromFile("audio/end.ogg");
			sf::Sound soundEnd(bufferEnd);
			music.stop();
			soundEnd.play();

			time = 0.0f;

			while (time < 2)
			{
				time += clock.getElapsedTime().asSeconds();
				clock.restart();

				windowWithGame.clear(sf::Color::Red);

				windowWithGame.draw(snake.getHead());

				for (int i = 0; i < snake.getLengthOfBody(); ++i)
				{
					windowWithGame.draw(snake.getBody(i));
				}

				windowWithGame.draw(snake.getTail());

				windowWithGame.draw(spriteFoodSnake);

				for (unsigned int i = 0; i < heighthOfGameField; ++i)
				{
					for (unsigned int j = 0; j < widthOfGameField; ++j)
					{
						if (fieldWithGame[i][j] == 'w')
						{
							spriteWall.setPosition(j * 32, i * 32);
							windowWithGame.draw(spriteWall);
						}
					}
				}

				std::ostringstream playerScoreString;
				playerScoreString << scores;
				text.setString("Scores: " + playerScoreString.str());
				text.setPosition(32, 32);
				windowWithGame.draw(text);

				windowWithGame.display();
			}

			while (time < 5)
			{
				time += clock.getElapsedTime().asSeconds();
				clock.restart();

				if (scores == 1)
				{
					text.setString("You lose with " + playerScoreString.str() + " score");
				}
				else
				{
					text.setString("You lose with " + playerScoreString.str() + " scores");
				}
				
				text.setFillColor(sf::Color::Black);
				text.setScale(2, 2);
				text.setPosition(50, 300);
				windowWithGame.clear(sf::Color::White);
				windowWithGame.draw(text);
				windowWithGame.display();
			}

			break;
		}
	}

	return 0;
}