#ifndef LAB4_PLAYER_HXX_INCLUDED
#define LAB4_PLAYER_HXX_INCLUDED
#include <SFML/Graphics.hpp>
#include "lab4_entity.hxx"

class Player: public Entity{
public:
    const float maxSpeed = 4.0;
    const float acc = 0.15;
    const float dec = 0.3;
    const float turnSpeed = 0.05;

    bool Up = false, Right = false, Down = false, Left = false;

    Player(int n0);

    void update();
    void drawHP(sf::RenderWindow &app);
};

#endif // LAB4_PLAYER_HXX_INCLUDED
