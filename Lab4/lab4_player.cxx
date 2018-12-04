#include <SFML/Graphics.hpp>
#include "lab4_player.hxx"
#include "lab4_parameters.hxx"

sf::Color colors[10] = {sf::Color::White,
                        sf::Color(255, 31, 31),
                        sf::Color(31, 31, 255),
                        sf::Color::Green,
                        sf::Color::Magenta};

Player::Player(int n0){
    r = 22;
    name = "player";
    sprite.setOrigin(r, r);
    n = n0;
    sprite.setColor(colors[n]);
}

void Player::update(){
    if ((Up) && (speed<maxSpeed)){
        if (speed < 0)  speed += dec;
        else  speed += acc;
    }
    if ((Down) && (speed>-maxSpeed)){
        if (speed > 0) speed -= dec;
        else  speed -= acc;
    }
    if (!(Up) && !(Down)){
        if (speed - dec > 0) speed -= dec;
        else if (speed + dec < 0) speed += dec;
        else speed = 0;
    }

    if (Right && (speed != 0))  angle += turnSpeed * speed/maxSpeed;
    if (Left && (speed != 0))   angle -= turnSpeed * speed/maxSpeed;

    x += sin(angle) * speed;
    y -= cos(angle) * speed;

    if (x > width) x = 0;
    if (x < 0) x = width;
    if (y > height) y = 0;
    if (y < 0) y = height;
}

void Player::drawHP(sf::RenderWindow &app){
    sf::RectangleShape rectangle0;
    rectangle0.setSize(sf::Vector2f(70, 7));
    rectangle0.setOrigin(sf::Vector2f(35, 3.5));
    rectangle0.setFillColor(sf::Color::Red);
    rectangle0.setPosition(x, y-1.5*r);
    sf::RectangleShape rectangle1;
    rectangle1.setSize(sf::Vector2f(0.7*life, 7));
    rectangle1.setOrigin(sf::Vector2f(35, 3.5));
    rectangle1.setFillColor(sf::Color::Green);
    rectangle1.setPosition(x, y-1.5*r);
    app.draw(rectangle0);
    app.draw(rectangle1);
}
