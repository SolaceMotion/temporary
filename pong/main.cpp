#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>

float dt = 0.0f;
uint32_t width{900}, height{600};

// Randomizer setup
std::random_device rd;
std::mt19937 gen(rd());
/* std::uniform_int_distribution<int> distribution(0, sizeof(coords) / sizeof(*coords) - 1); */

int scoreCounter = 0;

sf::RenderWindow *window;

class Racket
{
  float x, y, vx, vy;
  sf::RectangleShape shape;

public:
  Racket(float x, float y)
  {
    shape.setSize(sf::Vector2f(10, 200));
    this->x = x;
    this->y = y;
    shape.setPosition(this->x, this->y);
  }
  void draw()
  {
    shape.setFillColor(sf::Color::White);
    shape.setPosition(x, y);
    window->draw(shape);
  }
  sf::Vector2f getRacketPosition()
  {
    return shape.getPosition();
  }

  sf::FloatRect getRacketBounds()
  {
    return shape.getGlobalBounds();
  }
  void setRacketPositionY(float y)
  {
    this->y += y;
    shape.setPosition(this->x, this->y);
  }
};

std::vector<Racket *> racket;

class Ball
{
  float x, y, vx, vy;

  sf::CircleShape ball;

public:
  Ball()
  {
    ball.setRadius(10);
    x = width / 2 - 10;
    y = height / 2 - 10;
    vx = 200;
    vy = 200;
    ball.setPosition(x, y);
  }

  void update(float deltaTime)
  {
    x += vx * deltaTime;
    y += vy * deltaTime;

    /*  std::cout << racket[1]->getRacketPosition().y << std::endl; */
    /*  std::cout << y << std::endl; */
    if (x >= racket[1]->getRacketPosition().x - 20 && (y >= abs(racket[1]->getRacketPosition().y)))
    {
      vx = -vx;
    }
    if (x <= racket[0]->getRacketPosition().x)
    {
      std::cout << "HIT" << std::endl;
      vx = -vx;
      scoreCounter++;
      std::cout << scoreCounter << std::endl;
    }
    if (y >= window->getSize().y - 20)
    {
      vy = -vy;
    }
    if (y <= 0)
    {
      vy = -vy;
    }
  }

  void draw()
  {
    ball.setFillColor(sf::Color::White);
    ball.setPosition(x, y);
    window->draw(ball);
  }
};

Ball *ball;

void start()
{
  window = new sf::RenderWindow(sf::VideoMode(width, height), "Snake");

  sf::Text scoreText;
  sf::Font font;
  ball = new Ball();

  racket.push_back(new Racket(40, height / 2));
  racket.push_back(new Racket(window->getSize().x - 40, height / 2));

  font.loadFromFile("res/fonts/04B_19__.TTF");
  scoreText.setFont(font);
  scoreText.setString("HELLO");
  scoreText.setFillColor(sf::Color::White);
  scoreText.setCharacterSize(24);
  scoreText.setPosition(30, 30);

  window->draw(scoreText);
}

void destroy()
{
}

void draw()
{
  window->clear();
  ball->draw();

  for (int i = 0; i < racket.size(); i++)
  {
    racket[i]->draw();
  }
}

void update()
{
  ball->update(dt);
}

struct AABB
{
  sf::Vector2f min;
  sf::Vector2f max;
};

// Axis align bounding box
bool AABBvsAABB(const AABB &a, const AABB &b)
{
  // Exit with no intersection if found separated along an axis
  if (a.max.x < b.min.x or a.min.x > b.max.x)
    return false;
  if (a.max.y < b.min.y or a.min.y > b.max.y)
    return false;

  // No separating axis found, therefore there is at least one overlapping axis
  return true;
}

void eventHandler(sf::Event *event)
{
  if (event->type == sf::Event::Closed)
  {
    window->close();
  }
  if (event->type == sf::Event::KeyPressed)
  {
    if (event->key.code == sf::Keyboard::W)
    {
      racket[0]->setRacketPositionY(-30);
    }
    if (event->key.code == sf::Keyboard::S)
    {
      racket[0]->setRacketPositionY(30);
    }

    if (event->key.code == sf::Keyboard::Up)
    {
      racket[1]->setRacketPositionY(-30);
    }
    if (event->key.code == sf::Keyboard::Down)
    {
      racket[1]->setRacketPositionY(30);
    }
  }
}

int main(void)
{
  try
  {
    start();

    sf::Clock *deltaClock = new sf::Clock;

    while (window->isOpen())
    {
      sf::Event event;

      while (window->pollEvent(event))
      {
        eventHandler(&event);
      }

      // Seconds since previous frame
      dt = deltaClock->getElapsedTime().asMicroseconds() / 1000.f / 1000.f;
      // Restarts after every frame, this is necessary. Otherwise the timer will keep going
      deltaClock->restart();

      draw();
      update();

      window->display();
    }

    destroy();
  }

  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}