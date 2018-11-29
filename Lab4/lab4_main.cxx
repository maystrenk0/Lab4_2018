#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include <SFML/Network.hpp>
using namespace sf;

float DEGTORAD = 0.017453f;
const int W = 1024;
const int H = 768;
Color colors[10] = {Color::White, Color::Red, Color::Blue, Color::Green, Color::Magenta};

class Entity{
public:
    float x,y,R,angle, speed=0;
    int life;
    int n;
    std::string name;
    Sprite sprite;

    Entity(){
        life=100;
    }

    void settings(int X,int Y,float Angle=0){
        x=X; y=Y;
        angle = Angle;
    }

virtual void update(){};

void draw(RenderWindow &app)
{
    sprite.setPosition(x,y);
    sprite.setRotation(angle/DEGTORAD);

    app.draw(sprite);
}

virtual ~Entity(){};
};

class Bullet: public Entity
{
public:
    Bullet(){
        name="bullet";
        speed = 8;
        R=16;
        sprite.setOrigin(R, R);
    }

void update(){
    //x+=cos(angle*DEGTORAD)*6;
    //y+=sin(angle*DEGTORAD)*6;
    x += sin(angle) * speed;
    y -= cos(angle) * speed;

   if (x>W || x<0 || y>H || y<0) life=0;
  }

};


class Player: public Entity{
public:
    bool Up=0,Right=0,Down=0,Left=0;


    Player(int n0){
        R=22;
        name="player";
        sprite.setOrigin(R, R);
        n = n0;
        sprite.setColor(colors[n]);
    }

    void update(){
        float maxSpeed=4.0;
        float acc=0.15, dec=0.3;
        float turnSpeed=0.05;
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

        if (Right && speed!=0)  angle += turnSpeed * speed/maxSpeed;
        if (Left && speed!=0)   angle -= turnSpeed * speed/maxSpeed;
        x += sin(angle) * speed;
        y -= cos(angle) * speed;
        if (x>W) x=0;  if (x<0) x=W;
        if (y>H) y=0;  if (y<0) y=H;
    }
};

bool isCollide(Entity *a,Entity *b)
{
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<
         (a->R + b->R)*(a->R + b->R);
}



int main()
{
    RenderWindow app(VideoMode(W, H), "Car Racing Game!");
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

    if (Keyboard::isKeyPressed(Keyboard::W)) player1->Up=1;
    else player1->Up=0;
    if (Keyboard::isKeyPressed(Keyboard::D)) player1->Right=1;
    else player1->Right=0;
    if (Keyboard::isKeyPressed(Keyboard::S)) player1->Down=1;
    else player1->Down=0;
    if (Keyboard::isKeyPressed(Keyboard::A)) player1->Left=1;
    else player1->Left=0;

    if (Keyboard::isKeyPressed(Keyboard::Up)) player2->Up=1;
    else player2->Up=0;
    if (Keyboard::isKeyPressed(Keyboard::Right)) player2->Right=1;
    else player2->Right=0;
    if (Keyboard::isKeyPressed(Keyboard::Down)) player2->Down=1;
    else player2->Down=0;
    if (Keyboard::isKeyPressed(Keyboard::Left)) player2->Left=1;
    else player2->Left=0;



    for(auto a:entities)
     for(auto b:entities){
      if (a->name=="player" && b->name=="bullet")
       if ( isCollide(a,b) &&(a->n!=b->n)){
            a->life-=10;
            b->life=0;
            if(a->life == 0){
                a->settings(W/2,H/2,0);
                a->life=100;
            }
        }
    }





for(auto i=entities.begin();i!=entities.end();){
      Entity *e = *i;

      e->update();

      if (e->life==0) {i=entities.erase(i); delete e;}
      else i++;
    }

sf::RectangleShape rectangle0;
rectangle0.setSize(sf::Vector2f(70, 7));
rectangle0.setOrigin(sf::Vector2f(35, 3.5));
rectangle0.setFillColor(Color::Red);
rectangle0.setPosition(player1->x, player1->y-1.5*player1->R);
sf::RectangleShape rectangle1;
rectangle1.setSize(sf::Vector2f(0.7*player1->life, 7));
rectangle1.setOrigin(sf::Vector2f(35, 3.5));
rectangle1.setFillColor(Color::Green);
rectangle1.setPosition(player1->x, player1->y-1.5*player1->R);

sf::RectangleShape rectangle01;
rectangle01.setSize(sf::Vector2f(70, 7));
rectangle01.setOrigin(sf::Vector2f(35, 3.5));
rectangle01.setFillColor(Color::Red);
rectangle01.setPosition(player2->x, player2->y-1.5*player2->R);
sf::RectangleShape rectangle11;
rectangle11.setSize(sf::Vector2f(0.7*player2->life, 7));
rectangle11.setOrigin(sf::Vector2f(35, 3.5));
rectangle11.setFillColor(Color::Green);
rectangle11.setPosition(player2->x, player2->y-1.5*player2->R);

   //////draw//////
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
