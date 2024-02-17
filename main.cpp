#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>

using namespace std;
using namespace sf;

void movement(sf::CircleShape& shape, int pos1, int pos2, int& punkt)
{
    sf::Vector2f position = shape.getPosition();
    
	if (position.x != pos1) // X kleiner als X
	{
		if (position.x < pos1) // Y kleiner als Y
		{
			shape.setPosition(position.x + 1, position.y);
		}
		else if (position.x > pos1) // Y größer als Y
		{
			shape.setPosition(position.x - 1, position.y);
		}
	}
	if (position.y != pos2) // X größer als X
	{
		if (position.y > pos2) // Y kleiner als Y
		{
			shape.setPosition(position.x, position.y - 1);
		}
		else if (position.y < pos2) // Y größer als Y
		{
			shape.setPosition(position.x, position.y + 1);
		}
	}

	if (position.x == pos1 && position.y == pos2)
	{
		punkt += 1;
	}
}

void towerShoot(Vector2f& position, CircleShape& ammunitionProjectile)
{
    Vector2f positionAP = ammunitionProjectile.getPosition();

    // Calculate unit vector in the direction of the target position
    sf::Vector2f direction(position - ammunitionProjectile.getPosition());
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0.f)
        direction /= length;

    // Move the shape in the direction of the target position
    ammunitionProjectile.move(direction * 2.f);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Test Game", sf::Style::Default);
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.loadFromFile("map.png");

    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("enemy.png");
    
    sf::Texture towerTexture;
    towerTexture.loadFromFile("tower.png");

    //Enemy
    sf::CircleShape shape(25.f);
    shape.setFillColor(sf::Color::Magenta);
    shape.setPosition(-30, 140);
    shape.setOrigin(25.f, 25.f);

    sf::Sprite enemy;
    enemy.setTexture(enemyTexture);
    enemy.setPosition(100, 100);

    //Shooting Projectile
    CircleShape ammunitionProjectile(5.f);
    ammunitionProjectile.setFillColor(Color::Black);
    ammunitionProjectile.setPosition(200, 200);
    ammunitionProjectile.setOrigin(5.f, 5.f);
    
    RectangleShape tower2(Vector2f(50.f, 50.f));
    tower2.setTexture(&towerTexture); // texture is a sf::Texture
    tower2.setTextureRect(sf::IntRect(0, 0, 190, 266));
    tower2.setOrigin(25, 25);
    tower2.setOutlineColor(Color::Black);
    tower2.setOutlineThickness(10.f);
    tower2.setFillColor(Color::Yellow);
    tower2.setPosition(200.f, 200.f);

    /*
    // create an array of 3 vertices that define a triangle primitive
    sf::VertexArray tower(sf::Quads, 4);

    // define the position of the triangle's points
    tower[0].position = sf::Vector2f(10.f, 10.f);
    tower[1].position = sf::Vector2f(100.f, 10.f);
    tower[2].position = sf::Vector2f(100.f, 100.f);
    tower[3].position = sf::Vector2f(10.f, 100.f);

    // define the color of the triangle's points
    tower[0].color = sf::Color::Red;
    tower[1].color = sf::Color::Blue;
    tower[2].color = sf::Color::Green;
    tower[3].color = sf::Color::Yellow;
    */

    sf::Sprite background;
    background.setTexture(texture);
    background.setOrigin(400, 300);
    background.setPosition(400, 300);
    background.setRotation(0);

    sf::Font font;
    font.loadFromFile("tuffy.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString("Test Text");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(400, 300);

	int punkt = 1;
    bool collisionOfEnemy = false;

    //Bullets
    vector<CircleShape> bullets;
    vector<float> angles;


    //Solange das Fenster offen ist wird ausgeführt
    while (window.isOpen())
    {
        // Event to close window
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            shape.move(-10.f, 0.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            shape.move(10.f, 0.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            shape.move(0.f, -10.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            shape.move(0.f, 10.f);
        }

        sf::Vector2f position = shape.getPosition();

		switch (punkt)
		{
			case 1:
				movement(shape, -30, 140, punkt);
				break;
			case 2: 
				movement(shape, 610, 140, punkt);
				break;
			case 3:
				movement(shape, 610, 430, punkt);
				break;
			case 4:
				movement(shape, -30, 430, punkt);
				break;
		}

        //towerShoot(position, ammunitionProjectile, shape);

        sf::Vector2f towerPosition = tower2.getPosition();

        window.clear();

        window.draw(background);

        if (shape.getGlobalBounds().intersects(ammunitionProjectile.getGlobalBounds()))
        {
            // The shapes are colliding
            //collisionOfEnemy = true;
        }
        else
        {
            // The shapes are not colliding
        }

        window.draw(tower2);

        if (!collisionOfEnemy)
        {
            //window.draw(ammunitionProjectile);
            window.draw(shape);
        }


        if (Mouse::isButtonPressed(Mouse::Left))
        {
            bullets.push_back(CircleShape());
            bullets.back().setRadius(5);
            bullets.back().setOrigin(5, 5);
            bullets.back().setPosition(tower2.getPosition());

            angles.push_back(atan2(position.y - towerPosition.y, position.x - towerPosition.x));
        }

        for (int i = 0; i < bullets.size(); i++)
        {
            window.draw(bullets[i]);
            bullets[i].move(10 * cos(angles[i]), 10 * sin(angles[i]));
            
            Vector2f bulletPos = bullets[i].getPosition();

            if (bulletPos.x > 800 || bulletPos.x < 0 || bulletPos.y > 600 || bulletPos.y < 0)
            {
                bullets.erase(bullets.begin() + i);
                angles.erase(angles.begin() + i);
                i--; // Decrement i to account for erased element
            }
        }

        std::cout << bullets.size() << endl;

        window.display(); 
    }

    return 0;
}
