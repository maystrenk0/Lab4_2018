#ifndef LAB4_ENTITY_HXX_INCLUDED
#define LAB4_ENTITY_HXX_INCLUDED
#include <SFML/Graphics.hpp>
#include <cstring>

class Entity{
public:
    float x, y, r, angle, speed = 0;
    int life = 100, n;
    std::string name;
    sf::Sprite sprite;

    void settings(int x0,int y0,float a0);
    virtual void update(){};
    void draw(sf::RenderWindow &app);
    virtual ~Entity(){};
};

bool isCollide(Entity *a,Entity *b);

#endif // LAB4_ENTITY_HXX_INCLUDED
