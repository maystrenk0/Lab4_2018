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
    TcpListener listener;
    if(listener.listen(65065) != Socket::Done)
        std::cout << "Listen failed!" << std::endl;

    TcpSocket client;
    if (listener.accept(client) != Socket::Done)
        std::cout << "Accept failed!" << std::endl;
    size_t bytesRead = 0;
    char buffer[100] = {};


    std::string data0 = "0,0,0,0,0,";
    client.send(data0.c_str(), data0.length());

    RenderWindow app(VideoMode(width, height), "Lab4 SERVER");
	app.setFramerateLimit(60);

    Texture t0;
    t0.loadFromFile("images/background.png");
    t0.setSmooth(true);
    Sprite sBackground(t0);

    std::list<Entity*> entities;

    Player *player1 = new Player(1);
    player1->settings(400,100,0);
    entities.push_back(player1);
    Player *player2 = new Player(2);
    player2->settings(100,100,0);
    entities.push_back(player2);
    Texture t[3];
    t[0].setSmooth(true);
    t[1].setSmooth(true);
    t[2].setSmooth(true);
    t[0].loadFromFile("images/spaceship.png");
    t[1].loadFromFile("images/fire_red.png");
    t[2].loadFromFile("images/fire_blue.png");
    player1->sprite.setTexture(t[0]);
    player2->sprite.setTexture(t[0]);

    while (app.isOpen()){
        std::string data = "";
        Event e;
        while (app.pollEvent(e)){
            if (e.type == Event::Closed)
                app.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::W)){
            player1->Up = true;
            data += "1,";
        }
        else{
            player1->Up = false;
            data += "0,";
        }
        if (Keyboard::isKeyPressed(Keyboard::D)){
            player1->Right = true;
            data += "1,";
        }
        else{
            player1->Right = false;
            data += "0,";
        }
        if (Keyboard::isKeyPressed(Keyboard::S)){
            player1->Down = true;
            data += "1,";
        }
        else{
            player1->Down = false;
            data += "0,";
        }
        if (Keyboard::isKeyPressed(Keyboard::A)){
            player1->Left = true;
            data += "1,";
        }
        else{
            player1->Left = false;
            data += "0,";
        }

        if (Keyboard::isKeyPressed(Keyboard::Space)){
            Bullet *b = new Bullet();
            b->settings(player1->x,player1->y,player1->angle);
            b->sprite.setTexture(t[player1->n]);
            b->n = player1->n;
            entities.push_back(b);
            data += "1,";
        }
        else data += "0,";

        if (client.receive(buffer, 100, bytesRead) != Socket::Done)
            std::cout << "Error receive!" << std::endl;
        else{
            std::string s = buffer;
            std::string delimiter = ",";
            std::string opponent[5];
            size_t pos = 0;
            int i = 0;
            std::string token;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                opponent[i] = token;
                ++i;
                s.erase(0, pos + delimiter.length());
            }
            player2->Up = std::stoi(opponent[0]);
            player2->Right = std::stoi(opponent[1]);
            player2->Down = std::stoi(opponent[2]);
            player2->Left = std::stoi(opponent[3]);
            if(std::stoi(opponent[4])){
                Bullet *b = new Bullet();
                b->settings(player2->x,player2->y,player2->angle);
                b->sprite.setTexture(t[player2->n]);
                b->n = player2->n;
                entities.push_back(b);
            }
        }

        for(auto a:entities)
            for(auto b:entities){
                if (a->name=="player" && b->name=="bullet")
                    if ( isCollide(a,b) &&(a->n!=b->n)){
                        a->life -= 10;
                        b->life = 0;
                        if(a->life == 0){
                            a->settings(width/2, height/2, 0);
                            a->life = 1000;
                        }
                    }
            }

        for(auto i=entities.begin();i!=entities.end();){
            Entity *e = *i;
            e->update();
            if (e->life==0) {i=entities.erase(i); delete e;}
            else i++;
        }

        int dx=0, dy=0;
        while (dx*dx+dy*dy<4*player1->r*player1->r){
            player1->x += dx/10.0;
            player1->y += dy/10.0;
            player2->x -= dx/10.0;
            player2->y -= dy/10.0;
            dx = player1->x - player2->x;
            dy = player1->y - player2->y;
            if (!dx && !dy) break;
        }

        client.send(data.c_str(), data.length());

        app.draw(sBackground);
        for(auto i:entities)
            i->draw(app);
        player1->drawHP(app);
        player2->drawHP(app);

        app.display();
    }

    return 0;
}
