#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
using namespace std;

class PongBall
{
    sf::RenderWindow& window;
    float x, y, velx, vely, radius;

    void Init()
    {
        x = window.getSize().x / 2;
        y = window.getSize().y / 2;
        radius = 10.0f; // ten pixels

        // give the ball a random velocity
        random_device rd;
        uniform_real_distribution<float> dist(-600.0f, 600.0f);
        velx = dist(rd);
        vely = dist(rd);
    }

public:
    PongBall(sf::RenderWindow& window) : window(window)
    {
        Init();
    }
    void update(float deltaTime)
    {
        x += velx * deltaTime;
        y += vely * deltaTime;

        if (x < 0) {
            velx = -velx;
        }

        if (x > window.getSize().x - (2 * radius)) {
            velx = -velx;
        }

        if (y < 0) {
            vely = -vely;
        }

        if (y > window.getSize().y - (2 * radius)) {
            vely = -vely;
        }
    }
    void draw()
    {
        sf::CircleShape circle;
        circle.setFillColor(sf::Color(255, 255, 255));
        circle.setPosition(sf::Vector2f(x, y));
        circle.setRadius(radius);
        window.draw(circle);
    }
    sf::Vector2f getPos() { return sf::Vector2f(x, y); }
    float getRadius() { return radius; }
    sf::Vector2f getVel() { return sf::Vector2f(velx, vely); }
    void setVel(sf::Vector2f vel) { velx = vel.x; vely = vel.y; }
};

class PongBat
{
    sf::RenderWindow& window;
    float x, y, w, h;
public:
    PongBat(sf::RenderWindow& window) : window(window)
    {
        w = 15.0f;
        h = 60.0f;
        x = 40.0f;
        y = window.getSize().y / 2.0f + h / 2.0f;
    }

    void update(float deltaTime)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            y -= 300.0f * deltaTime;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            y += 300.0f * deltaTime;
        if (y < 0) {
            y = 1;
        }
        if (y > window.getSize().y -h) {
            y = window.getSize().y -h;
        }
    }
    void draw()
    {
        sf::RectangleShape rect;
        rect.setFillColor(sf::Color(255, 255, 255));
        rect.setPosition(sf::Vector2f(x, y));
        rect.setSize(sf::Vector2f (w, h));
        window.draw(rect);
    }
    sf::Vector2f getPos() { return sf::Vector2f(x, y); }
    sf::Vector2f getSize() { return sf::Vector2f(w, h); }
};

bool BallRectangleOverlap(PongBall& ball, const sf::Vector2f& rectPos, const sf::Vector2f& rectSize)
{
    // test for overlap
    sf::Vector2f ballPos = ball.getPos();
    float ballRadius = ball.getRadius();

    // treat the ball as a square for this
    float x0ball, x1ball, y0ball, y1ball;
    float x0rect, x1rect, y0rect, y1rect;
    x0ball = ballPos.x - ballRadius;
    x1ball = ballPos.x + ballRadius;
    y0ball = ballPos.y - ballRadius;
    y1ball = ballPos.y + ballRadius;
    x0rect = rectPos.x;
    x1rect = rectPos.x + rectSize.x;
    y0rect = rectPos.y;
    y1rect = rectPos.y + rectSize.y;

    return !(y0ball > y1rect || x0ball > x1rect || x1ball < x0rect || y1ball < y0rect);
}

void Collision(PongBall& ball, PongBat& bat)
{
    // test for overlap
    if (BallRectangleOverlap(ball, bat.getPos(), bat.getSize()))
    {
        // collision
        sf::Vector2f ballVel = ball.getVel();
        if (ballVel.x < 0.0f)
            ball.setVel(sf::Vector2f(-ballVel.x, ballVel.y));
    }
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1200, 800), "SFML Program");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    // create a ball
    PongBall ball = PongBall(window);
    PongBat bat = PongBat(window);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();

        Collision(ball, bat);

        ball.update(1.0f / 60.0f);
        ball.draw();

        bat.update(1.0f / 60.0f);
        bat.draw();
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}