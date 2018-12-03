#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "lab4_entity.hxx"
#include "lab4_bullet.hxx"
#include "lab4_player.hxx"
#include "lab4_parameters.hxx"

using namespace sf;

int main(){
    RenderWindow app(VideoMode(width, height), "Lab4");
	app.setFramerateLimit(60);

    Texture t0;
    t0.loadFromFile("images/background.png");
    t0.setSmooth(true);
    Sprite sBackground(t0);

    std::list<Entity*> entities;

    Player *player1 = new Player(1);
    player1->settings(300,100,0);
    entities.push_back(player1);
    Player *player2 = new Player(2);
    player2->settings(600,100,0);
    entities.push_back(player2);
    Texture t[3];
    t[0].setSmooth(true);
    t[1].setSmooth(true);
    t[2].setSmooth(true);
    t[0].loadFromFile("images/car.png");
    t[1].loadFromFile("images/fire_red.png");
    t[2].loadFromFile("images/fire_blue.png");
    player1->sprite.setTexture(t[0]);
    player2->sprite.setTexture(t[0]);

    while (app.isOpen()){
        Event e;
        while (app.pollEvent(e)){
            if (e.type == Event::Closed)
                app.close();

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space){
                Bullet *b = new Bullet();
                b->settings(player1->x,player1->y,player1->angle);
                b->sprite.setTexture(t[player1->n]);
                b->n = player1->n;
                entities.push_back(b);
            }
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter){
                Bullet *b = new Bullet();
                b->settings(player2->x,player2->y,player2->angle);
                b->sprite.setTexture(t[player2->n]);
                b->n = player2->n;
                entities.push_back(b);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::W)) player1->Up = true;
        else player1->Up = false;
        if (Keyboard::isKeyPressed(Keyboard::D)) player1->Right = true;
        else player1->Right = false;
        if (Keyboard::isKeyPressed(Keyboard::S)) player1->Down = true;
        else player1->Down = false;
        if (Keyboard::isKeyPressed(Keyboard::A)) player1->Left = true;
        else player1->Left = false;

        if (Keyboard::isKeyPressed(Keyboard::Up)) player2->Up = true;
        else player2->Up = false;
        if (Keyboard::isKeyPressed(Keyboard::Right)) player2->Right = true;
        else player2->Right = false;
        if (Keyboard::isKeyPressed(Keyboard::Down)) player2->Down = true;
        else player2->Down = false;
        if (Keyboard::isKeyPressed(Keyboard::Left)) player2->Left = true;
        else player2->Left = false;

        for(auto a:entities)
            for(auto b:entities){
                if (a->name=="player" && b->name=="bullet")
                    if ( isCollide(a,b) &&(a->n!=b->n)){
                        a->life -= 10;
                        b->life = 0;
                        if(a->life == 0){
                            a->settings(width/2, height/2, 0);
                            a->life = 100;
                        }
                    }
            }

        for(auto i=entities.begin();i!=entities.end();){
            Entity *e = *i;
            e->update();
            if (e->life==0) {i=entities.erase(i); delete e;}
            else i++;
        }

        for(auto a:entities)
            for(auto b:entities){
                if (a->name=="player" && b->name=="player"){
                    int dx=0, dy=0;
                    while (dx*dx+dy*dy<4*a->r*a->r){
                        a->x += dx/10.0;
                        a->y += dy/10.0;
                        b->x -= dx/10.0;
                        b->y -= dy/10.0;
                        dx = a->x - b->x;
                        dy = a->y - b->y;
                        if (!dx && !dy) break;
                    }
                }
            }

        sf::RectangleShape rectangle0;
        rectangle0.setSize(sf::Vector2f(70, 7));
        rectangle0.setOrigin(sf::Vector2f(35, 3.5));
        rectangle0.setFillColor(Color::Red);
        rectangle0.setPosition(player1->x, player1->y-1.5*player1->r);
        sf::RectangleShape rectangle1;
        rectangle1.setSize(sf::Vector2f(0.7*player1->life, 7));
        rectangle1.setOrigin(sf::Vector2f(35, 3.5));
        rectangle1.setFillColor(Color::Green);
        rectangle1.setPosition(player1->x, player1->y-1.5*player1->r);

        sf::RectangleShape rectangle01;
        rectangle01.setSize(sf::Vector2f(70, 7));
        rectangle01.setOrigin(sf::Vector2f(35, 3.5));
        rectangle01.setFillColor(Color::Red);
        rectangle01.setPosition(player2->x, player2->y-1.5*player2->r);
        sf::RectangleShape rectangle11;
        rectangle11.setSize(sf::Vector2f(0.7*player2->life, 7));
        rectangle11.setOrigin(sf::Vector2f(35, 3.5));
        rectangle11.setFillColor(Color::Green);
        rectangle11.setPosition(player2->x, player2->y-1.5*player2->r);

        app.draw(sBackground);
        for(auto i:entities)
            i->draw(app);
        app.draw(rectangle0);
        app.draw(rectangle1);
        app.draw(rectangle01);
        app.draw(rectangle11);

        app.display();
    }

    return 0;
}
