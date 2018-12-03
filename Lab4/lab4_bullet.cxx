#include "lab4_bullet.hxx"
#include "lab4_parameters.hxx"

Bullet::Bullet(){
    name = "bullet";
    speed = 8;
    r = 16;
    sprite.setOrigin(r, r);
}

void Bullet::update(){
    x += sin(angle) * speed;
    y -= cos(angle) * speed;
    if (x > width || x < 0 || y > height || y < 0)
        life=0;
}

