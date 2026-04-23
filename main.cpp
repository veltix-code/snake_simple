#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <random>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

float vitesse = 4.f;//lent 2 normal 4 grand 8 tres rapide 15
int dimension = 40;//petit 20 normal 40 grand 80

pair<int,int> nombreAleatoire()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disX(0, 800/dimension-1);
    uniform_int_distribution<> disY(0, 400/dimension-1);
    int posX = disX(gen) * dimension;
    int posY = disY(gen) * dimension;
    return {posX, posY};
}

struct boutSerpent
{
    int x = 0;
    int y = 0;
    Color couleur = Color(0,0,0);
};

void draw_bout_serpent(RenderWindow& window,int x, int y, Color couleur)
{
    RectangleShape cube(Vector2f((float)dimension, (float)dimension));
    cube.setPosition({(float)x, (float)y});
    cube.setFillColor(couleur);
    window.draw(cube);
}

void draw(RenderWindow& window,int Posx, int Posy,int dimx, int dimy, Color couleur)
{
    RectangleShape cube(Vector2f((float)dimx, (float)dimy));
    cube.setPosition({(float)Posx, (float)Posy});
    cube.setFillColor(couleur);
    window.draw(cube);
}

vector<boutSerpent> add_bout_serpent(vector<boutSerpent> list, boutSerpent ancien)
{
    list.push_back(ancien);
    return list;
}

// FIX 3 : détection collision sur soi-même
bool selfCollision(const vector<boutSerpent>& list)
{
    for (int i = 1; i < (int)list.size(); i++)
        if (list[0].x == list[i].x && list[0].y == list[i].y)
            return true;
    return false;
}

bool CollisionSerpent(const vector<boutSerpent>& list, int x, int y)
{
    for (int i = 1; i < (int)list.size(); i++)
        if (x == list[i].x && y == list[i].y)
            return true;
    return false;
}


int main()
{
    float posX =0.f,posY =0.f,timer =0.f;
    pair<int,int> PosPomme= nombreAleatoire();
    char direction = 'R';
    char nextDirection = 'R'; // FIX 2 : tampon pour éviter le demi-tour
    bool gameOver = false;    // FIX 3
    vector<boutSerpent> ListBoutSerpent;
    vector<boutSerpent> tempListBoutSerpent;
    boutSerpent ancienBout;
    ListBoutSerpent.push_back({0,0,Color(0,153,0)});
    RenderWindow window(VideoMode({800, 400}), "Snake C++", Style::Close);
    window.setFramerateLimit(60);
    Clock clock;
    RectangleShape carre(Vector2f((float)dimension, (float)dimension));
    carre.setFillColor(Color(0,153,0));
    carre.setPosition({posX,posY});
    
    while (window.isOpen()) 
    {
        Time dt = clock.restart();
        float deltaTime = dt.asMilliseconds();
        if (!gameOver) timer += deltaTime; // FIX 3 : on arrête le timer en game over

        while (const std::optional event = window.pollEvent()) 
        {
            if (event->is<Event::Closed>()) { 
                window.close();
            }

            if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) {
                if (keyPressed->code == Keyboard::Key::Escape) {
                    window.close();
                }
                // FIX 2 : on refuse le demi-tour
                if(keyPressed->code == Keyboard::Key::Right && direction != 'L')
                    nextDirection = 'R';
                if(keyPressed->code == Keyboard::Key::Left && direction != 'R')
                    nextDirection = 'L';
                if(keyPressed->code == Keyboard::Key::Up && direction != 'D')
                    nextDirection = 'U';
                if(keyPressed->code == Keyboard::Key::Down && direction != 'U')
                    nextDirection = 'D';

                if(keyPressed->code == Keyboard::Key::A)
                {
                    ListBoutSerpent = add_bout_serpent(ListBoutSerpent, {ancienBout.x, ancienBout.y, Color(0, 255, 0)});
                }
                // FIX 3 : redémarrer avec Entrée
                if(gameOver && keyPressed->code == Keyboard::Key::Enter)
                {
                    ListBoutSerpent.clear();
                    ListBoutSerpent.push_back({0,0,Color(0,153,0)});
                    direction = nextDirection = 'R';
                    PosPomme = nombreAleatoire();
                    gameOver = false;
                    timer = 0.f;
                }
            }
        }

        if(!gameOver && timer >= 1000/vitesse)
        {
            direction = nextDirection; // FIX 2 : appliquer la direction tampon

            if(direction == 'R')
            {
                //if((ListBoutSerpent[0].x+40)==800){ListBoutSerpent[0].x=0-40;}
                if((ListBoutSerpent[0].x+dimension)==PosPomme.first && ListBoutSerpent[0].y==PosPomme.second)
                {
                    PosPomme = nombreAleatoire();
                    while(CollisionSerpent(ListBoutSerpent, PosPomme.first, PosPomme.second))
                    {
                        PosPomme = nombreAleatoire();
                    }
                    ListBoutSerpent = add_bout_serpent(ListBoutSerpent, {ancienBout.x, ancienBout.y, Color(0, 255, 0)});
                }
                ListBoutSerpent.insert(ListBoutSerpent.begin(),{ListBoutSerpent[0].x+dimension,ListBoutSerpent[0].y,ListBoutSerpent[0].couleur});
            }
            else if(direction == 'L')
            {
                //if((ListBoutSerpent[0].x-40)==-40){ListBoutSerpent[0].x=800+40;}
                if((ListBoutSerpent[0].x-dimension)==PosPomme.first && ListBoutSerpent[0].y==PosPomme.second)
                {
                    PosPomme = nombreAleatoire();
                    while(CollisionSerpent(ListBoutSerpent, PosPomme.first, PosPomme.second))
                    {
                        PosPomme = nombreAleatoire();
                    }
                    ListBoutSerpent = add_bout_serpent(ListBoutSerpent, {ancienBout.x, ancienBout.y, Color(0, 255, 0)});
                }
                ListBoutSerpent.insert(ListBoutSerpent.begin(),{ListBoutSerpent[0].x-dimension,ListBoutSerpent[0].y,ListBoutSerpent[0].couleur});
            }
            else if(direction == 'U')
            {
                //if((ListBoutSerpent[0].y-40)==-40){ListBoutSerpent[0].y=360+40;}
                // FIX 1 : y+40 → y-40
                if((ListBoutSerpent[0].x)==PosPomme.first && (ListBoutSerpent[0].y-dimension)==PosPomme.second)
                {
                    PosPomme = nombreAleatoire();
                    while(CollisionSerpent(ListBoutSerpent, PosPomme.first, PosPomme.second))
                    {
                        PosPomme = nombreAleatoire();
                    }
                    ListBoutSerpent = add_bout_serpent(ListBoutSerpent, {ancienBout.x, ancienBout.y, Color(0, 255, 0)});
                }
                ListBoutSerpent.insert(ListBoutSerpent.begin(),{ListBoutSerpent[0].x,ListBoutSerpent[0].y-dimension,ListBoutSerpent[0].couleur});
            }
            else if(direction == 'D')
            {
                //if((ListBoutSerpent[0].y+40)==400){ListBoutSerpent[0].y=0-40;}
                if((ListBoutSerpent[0].x)==PosPomme.first && (ListBoutSerpent[0].y+dimension)==PosPomme.second)
                {
                    PosPomme = nombreAleatoire();
                    while(CollisionSerpent(ListBoutSerpent, PosPomme.first, PosPomme.second))
                    {
                        PosPomme = nombreAleatoire();
                    }
                    ListBoutSerpent = add_bout_serpent(ListBoutSerpent, {ancienBout.x, ancienBout.y, Color(0, 255, 0)});
                }
                ListBoutSerpent.insert(ListBoutSerpent.begin(),{ListBoutSerpent[0].x,ListBoutSerpent[0].y+dimension,ListBoutSerpent[0].couleur});
            }
            else {}
            
            if((ListBoutSerpent[0].x)>=800){ListBoutSerpent[0].x=0;}
            if((ListBoutSerpent[0].x)<=-40){ListBoutSerpent[0].x=800-dimension;}
            if((ListBoutSerpent[0].y)<=-40){ListBoutSerpent[0].y=400-dimension;}
            if((ListBoutSerpent[0].y)>=400){ListBoutSerpent[0].y=0;}

            ancienBout.x = ListBoutSerpent[ListBoutSerpent.size()-1].x;
            ancienBout.y = ListBoutSerpent[ListBoutSerpent.size()-1].y;
            ListBoutSerpent.pop_back();

            // FIX 3 : vérifier la collision sur soi-même
            if(selfCollision(ListBoutSerpent))
                gameOver = true;

            cout << "\n\n" << endl;
            /*
            for(int i =0; i < (int)ListBoutSerpent.size(); i++)
            {
                cout << "Bout de serpent " << i << ": (" << ListBoutSerpent[i].x << ", " << ListBoutSerpent[i].y << ")" << endl;
            }
            */
            for(int i =0; i < (int)ListBoutSerpent.size(); i++)
            {
                if(i==0)
                {

                }
                else
                {
                    int couleur = 255 - (i*255)/ListBoutSerpent.size();
                    if(couleur < 100) couleur = 100;
                    ListBoutSerpent[i].couleur = Color(0,153,0,couleur);
                }
            }
            timer -= 1000/vitesse;
        }

        window.clear(Color(0,204,0));
        
        for(boutSerpent _boutSerpent_ : ListBoutSerpent)
        {
            draw_bout_serpent(window,_boutSerpent_.x,_boutSerpent_.y,_boutSerpent_.couleur);
        }
        draw_bout_serpent(window,PosPomme.first,PosPomme.second,Color(255,0,0));

        // FIX 3 : overlay rouge game over
        if(gameOver)
        {
            RectangleShape overlay(Vector2f(800.f, 400.f));
            overlay.setFillColor(Color(255, 0, 0, 80));
            window.draw(overlay);
        }

        window.display();
    }

    return 0;
}