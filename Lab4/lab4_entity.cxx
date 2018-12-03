#include "lab4_entity.hxx"
#include "lab4_parameters.hxx"

void Entity::settings(int x0,int y0,float a0 = 0){
    x=x0;
    y=y0;
    angle = a0;
}

void Entity::draw(sf::RenderWindow &app){
    sprite.setPosition(x,y);
    sprite.setRotation(angle/DEGTORAD);
    app.draw(sprite);
}

bool isCollide(Entity *a,Entity *b){
    return (b->x - a->x)*(b->x - a->x)+
           (b->y - a->y)*(b->y - a->y)<
           (a->r + b->r)*(a->r + b->r);
}
