//*
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
using namespace sf;

float DEGTORAD = 0.017453f;
const int W = 1200;
const int H = 800;

class Entity{
public:
    float x,y,R,angle;
    int life;
    std::string name;
    Sprite sprite;

    Entity(){
        life=100;
    }

    void settings(int X,int Y,float Angle=0,int radius=1){
        x=X; y=Y;
        angle = Angle;
        R = radius;
    }

virtual void update(){};

void draw(RenderWindow &app)
{
    sprite.setPosition(x,y);
    sprite.setRotation(angle/DEGTORAD);

    //sprite.setRotation(angle+90);
    app.draw(sprite);
}

virtual ~Entity(){};
};

class Bullet: public Entity
{
public:
  Bullet()
  {
    name="bullet";
  }

void  update()
  {
    //x+=cos(angle*DEGTORAD)*6;
    //y+=sin(angle*DEGTORAD)*6;
    x += sin(angle) * 6;
    y -= cos(angle) * 6;

   if (x>W || x<0 || y>H || y<0) life=0;
  }

};


class Car: public Entity{
    public:
    float speed;
    bool Up=0,Right=0,Down=0,Left=0;
    Car(){
        speed=2;
        R=22;
        name="player";
        sprite.setOrigin(R, R);
    }

    void update(){
        float maxSpeed=10.0;
        float acc=0.2, dec=0.3;
        float turnSpeed=0.08;
        if (Up && speed<maxSpeed)
            if (speed < 0)  speed += dec;
            else  speed += acc;

        if (Down && speed>-maxSpeed)
            if (speed > 0) speed -= dec;
            else  speed -= acc;

        if (!Up && !Down)
            if (speed - dec > 0) speed -= dec;
            else if (speed + dec < 0) speed += dec;
            else speed = 0;

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

    Car *car1 = new Car();
    car1->settings(300,100,0,22);
    entities.push_back(car1);
    Texture t1, t2,t3;
    t1.setSmooth(true);
    t2.setSmooth(true);
    t3.setSmooth(true);
    t1.loadFromFile("images/car.png");
    t2.loadFromFile("images/fire_red.png");
    car1->sprite.setTexture(t1);

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
            if (e.type == Event::KeyPressed)
             if (e.key.code == Keyboard::Space)
              {
                Bullet *b = new Bullet();
                b->settings(car1->x+sin(car1->angle)*car1->R,car1->y-cos(car1->angle)*car1->R,car1->angle,10);
                b->sprite.setTexture(t2);
                entities.push_back(b);
              }
        }

    if (Keyboard::isKeyPressed(Keyboard::W)) car1->Up=1;
    else car1->Up=0;
    if (Keyboard::isKeyPressed(Keyboard::D)) car1->Right=1;
    else car1->Right=0;
    if (Keyboard::isKeyPressed(Keyboard::S)) car1->Down=1;
    else car1->Down=0;
    if (Keyboard::isKeyPressed(Keyboard::A)) car1->Left=1;
    else car1->Left=0;



	//car1->update();
    //app.clear(Color::White);
    //app->draw(sBackground);
    //car->draw(app);
    //app.display();
    //std::cout<<car1->angle<<std::endl;






for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->update();

      if (e->life==0) {i=entities.erase(i); delete e;}
      else i++;
    }



   //////draw//////
   app.draw(sBackground);

   for(auto i:entities)
     i->draw(app);

CircleShape circle(3);
  circle.setFillColor(Color(255,0,0,170));
  circle.setPosition(car1->x,car1->y);
  circle.setOrigin(3,3);
  app.draw(circle);

   app.display();











    }

    return 0;
}//*/

/*
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
using namespace sf;

const int W = 1200;
const int H = 800;

float DEGTORAD = 0.017453f;

class Animation
{
public:
	float Frame, speed;
	Sprite sprite;
    std::vector<IntRect> frames;

	Animation(){}

    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
	    Frame = 0;
        speed = Speed;

		for (int i=0;i<count;i++)
         frames.push_back( IntRect(x+i*w, y, w, h)  );

		sprite.setTexture(t);
		sprite.setOrigin(w/2,h/2);
        sprite.setTextureRect(frames[0]);
	}


	void update()
	{
    	Frame += speed;
		int n = frames.size();
		if (Frame >= n) Frame -= n;
		if (n>0) sprite.setTextureRect( frames[int(Frame)] );
	}

	bool isEnd()
	{
	  return Frame+speed>=frames.size();
	}

};


class Entity
{
public:
float x,y,dx,dy,R,angle;
bool life;
std::string name;
Animation anim;

Entity()
{
  life=1;
}

void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
{
  anim = a;
  x=X; y=Y;
  angle = Angle;
  R = radius;
}

virtual void update(){};

void draw(RenderWindow &app)
{
  anim.sprite.setPosition(x,y);
  anim.sprite.setRotation(angle+90);
  app.draw(anim.sprite);

  CircleShape circle(R);
  circle.setFillColor(Color(255,0,0,170));
  circle.setPosition(x,y);
  circle.setOrigin(R,R);
  //app.draw(circle);
}

virtual ~Entity(){};
};


class asteroid: public Entity
{
public:
  asteroid()
  {
    dx=rand()%8-4;
    dy=rand()%8-4;
    name="asteroid";
  }

void  update()
  {
   x+=dx;
   y+=dy;

   if (x>W) x=0;  if (x<0) x=W;
   if (y>H) y=0;  if (y<0) y=H;
  }

};



class bullet: public Entity
{
public:
  bullet()
  {
    name="bullet";
  }

void  update()
  {
   dx=cos(angle*DEGTORAD)*6;
   dy=sin(angle*DEGTORAD)*6;
  // angle+=rand()%6-3;
   x+=dx;
   y+=dy;

   if (x>W || x<0 || y>H || y<0) life=0;
  }

};


class player: public Entity
{
public:
   bool thrust;

   player()
   {
     name="player";
   }

   void update()
   {
     if (thrust)
      { dx+=cos(angle*DEGTORAD)*0.2;
        dy+=sin(angle*DEGTORAD)*0.2; }
     else
      { dx*=0.99;
        dy*=0.99; }

    int maxSpeed=15;
    float speed = sqrt(dx*dx+dy*dy);
    if (speed>maxSpeed)
     { dx *= maxSpeed/speed;
       dy *= maxSpeed/speed; }

    x+=dx;
    y+=dy;

    if (x>W) x=0; if (x<0) x=W;
    if (y>H) y=0; if (y<0) y=H;
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
    srand(time(0));

    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);

    Texture t1,t2,t3,t4,t5,t6,t7;
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);

    Animation sRock(t4, 0,0,64,64, 16, 0.2);
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);
    Animation sPlayer(t1, 40,0,40,40, 1, 0);
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);


    std::list<Entity*> entities;

    for(int i=0;i<15;i++)
    {
      asteroid *a = new asteroid();
      a->settings(sRock, rand()%W, rand()%H, rand()%360, 25);
      entities.push_back(a);
    }

    player *p = new player();
    p->settings(sPlayer,200,200,0,20);
    entities.push_back(p);

    /////main loop/////
    while (app.isOpen())
    {
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
             if (event.key.code == Keyboard::Space)
              {
                bullet *b = new bullet();
                b->settings(sBullet,p->x,p->y,p->angle,10);
                entities.push_back(b);
              }
        }

    if (Keyboard::isKeyPressed(Keyboard::Right)) p->angle+=3;
    if (Keyboard::isKeyPressed(Keyboard::Left))  p->angle-=3;
    if (Keyboard::isKeyPressed(Keyboard::Up)) p->thrust=true;
    else p->thrust=false;



    for(auto a:entities)
     for(auto b:entities)
    {
      if (a->name=="asteroid" && b->name=="bullet")
       if ( isCollide(a,b) )
           {
            a->life=false;
            b->life=false;

            Entity *e = new Entity();
            e->name="explosion";
            entities.push_back(e);


            for(int i=0;i<2;i++)
            {
             if (a->R==15) continue;
             Entity *e = new asteroid();
             e->settings(sRock_small,a->x,a->y,rand()%360,15);
             entities.push_back(e);
            }

           }

      if (a->name=="player" && b->name=="asteroid")
       if ( isCollide(a,b) )
           {
            b->life=false;

            Entity *e = new Entity();
            e->name="explosion";
            entities.push_back(e);

            p->settings(sPlayer,W/2,H/2,0,20);
            p->dx=0; p->dy=0;
           }
    }


    if (p->thrust)  p->anim = sPlayer_go;
    else   p->anim = sPlayer;


    for(auto e:entities)
     if (e->name=="explosion")
      if (e->anim.isEnd()) e->life=0;

    if (rand()%150==0)
     {
       asteroid *a = new asteroid();
       a->settings(sRock, 0,rand()%H, rand()%360, 25);
       entities.push_back(a);
     }

    for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->update();
      e->anim.update();

      if (e->life==false) {i=entities.erase(i); delete e;}
      else i++;
    }



   //////draw//////
   app.draw(background);

   for(auto i:entities)
     i->draw(app);

   app.display();
    }

    return 0;
}
//*/


