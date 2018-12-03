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
