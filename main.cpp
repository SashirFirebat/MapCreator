#include <iostream>
#include <fstream>
#include "MapSprites.h"
#include "Map.h"
#include <string>
#include <cstring>
using namespace std;
const int WINDOW_HEIGHT = 640;
const int WINDOW_WIDTH = 950;

int main()
{
    // wczytanie grafik
    MapSprites maps("sprites.txt", "maps.png");
    MapSprites objects("objects.txt", "objects.png");
    objects.mode(true); // ustawienie trybu pojedynczych grafik

    // wczytanie/utworzenie mapy
    char createNew;
    cout << "Load map? (y/n)\n";
    cin >> createNew;
    string name;
    char buf[20];
    cin.get();

    cout << "Name of the map (without \".txt\"):\n";
    getline(cin, name);

    Map mapa(25, 20, 1, &maps, &objects);

    name += ".txt";

    if ( createNew == 'y' || createNew == 'Y')
    {
        ifstream check;
        check.open(name.c_str());
        if(check.is_open())
        {
            Map temp(name.c_str(), &maps, &objects);
            mapa = temp;
            cout << "Map loaded from file " << name << endl;
        }
        else
        {
            cout << "Map not found, creating new one...\n";
        }

        check.close();
    }

    // stworzenie okna
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sashir RPG - Map Creator");
    window.setFramerateLimit(60);

    int i = 0, j = 0;
    bool col = true;
    sf::Vector2f posMouse;

    // sprite wybranej grafiki
    sf::Texture temp;
    temp.loadFromImage(maps[i]);
    sf::Sprite chosen;
    chosen.setTexture(temp);
    chosen.setPosition(834, 320);
    sf::Image red, black;
    red.create(32, 32, sf::Color(255, 0, 0, 255));
    black.create(32, 32, sf::Color(0, 0, 0, 255));

    // utworzenie napisow
    sf::Font arial;
    arial.loadFromFile("arial.ttf");
    sf::Text title, curspr, info, outfile;
    title.setFont(arial);
    title.setCharacterSize(15);
    title.setString("Sashir RPG\nMap Creator");
    title.setPosition(805, 5);
    curspr.setFont(arial);
    curspr.setCharacterSize(15);
    curspr.setString("Current Sprite:");
    curspr.setPosition(805, 300);
    info.setFont(arial);
    info.setCharacterSize(15);
    string instructions = "\"ESC\" to exit\n\"Z\" to save\n\"S\" previous sprite\n\"D\" next sprite\n";
    instructions += "\"T\" change name\n(enter to finish)\n\"Q\" clear object\n\"A\" add layer\n";
    instructions += "\"W\" previous layer\n\"E\" next layer";
    info.setString(instructions);
    info.setPosition(805, 350);
    outfile.setFont(arial);
    outfile.setCharacterSize(15);
    outfile.setPosition(805, 150);
    bool nameflag = false;

    sf::Text currentLayer;
    std::string curLayer("0");
    currentLayer.setFont(arial);
    currentLayer.setCharacterSize(15);
    currentLayer.setString(curLayer);
    currentLayer.setPosition(805, 200);

    curLayer = "Current Layer:\n0/";
    sprintf(buf, "%d", mapa.returnAllLayers());
    curLayer += buf;
    currentLayer.setString(curLayer);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
            if (event.type == sf::Event::TextEntered && nameflag)
            {
                if(event.text.unicode == 8 && name.size()!=0)
                {
                    name.pop_back();
                }
                else if(event.text.unicode == 13 && name.size()!=0)
                {
                    name += ".txt";
                    nameflag = false;
                }
                else if (event.text.unicode < 128 && event.text.unicode != 8)
                {
                    name.push_back(event.text.unicode);
                }
                outfile.setString(name);
            }
            if (!nameflag)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z)
                {

                    cout << "Map saved as: ";
                    cout << mapa.save(name.c_str()) << endl;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
                {
                    if (mapa.returnLayer() > 0)
                    {
                        posMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        mapa.update(int(posMouse.x)/32, int(posMouse.y)/32, -1);
                    }
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
                {
                    mapa.addLayer();
                    int x = mapa.returnLayer();
                    cout << x;
                    sprintf(buf, "%d", x);
                    cout << buf;
                    curLayer = "Current Layer:\n";
                    curLayer += buf;
                    cout << curLayer;
                    sprintf(buf, "%d", mapa.returnAllLayers());
                    curLayer += "/";
                    curLayer += buf;
                    currentLayer.setString(curLayer);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
                {
                    int x = mapa.previousLayer();
                    if (x == -1)
                    {
                        mapa.showCollision(true);
                        if (col)
                            temp.loadFromImage(red);
                        else
                            temp.loadFromImage(black);
                    }
                    else if (x == 0)
                    {
                        mapa.showCollision(false);
                        temp.loadFromImage(maps[i]);
                    }
                    else
                        temp.loadFromImage(objects[j]);
                    sprintf(buf, "%d", x);
                    curLayer = "Current Layer:\n";
                    curLayer += buf;
                    sprintf(buf, "%d", mapa.returnAllLayers());
                    curLayer += "/";
                    curLayer += buf;
                    currentLayer.setString(curLayer);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
                {
                    int x = mapa.nextLayer();
                    if (x == 0)
                    {
                        mapa.showCollision(false);
                        temp.loadFromImage(maps[i]);
                    }
                    else
                        temp.loadFromImage(objects[j]);
                    sprintf(buf, "%d", x);
                    curLayer = "Current Layer:\n";
                    curLayer += buf;
                    sprintf(buf, "%d", mapa.returnAllLayers());
                    curLayer += "/";
                    curLayer += buf;
                    currentLayer.setString(curLayer);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T)
                {
                    nameflag = true;
                    name.clear();
                    window.pollEvent(event);
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
                {
                    if (mapa.returnLayer() == -1)
                    {
                        col = true;
                        temp.loadFromImage(red);
                    }
                    else if (mapa.returnLayer() == 0)
                    {
                        if(i < maps.returnRows() - 1) i++;
                        else i = 0;
                        temp.loadFromImage(maps[i]);
                    }
                    else
                    {
                        if(j < objects.returnRows() - 1) j++;
                        else j = 0;
                        temp.loadFromImage(objects[j]);
                    }

                }
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
                {
                    if (mapa.returnLayer() == -1)
                    {
                        col = false;
                        temp.loadFromImage(black);
                    }
                    else if (mapa.returnLayer() == 0)
                    {
                        if(i > 0) i--;
                        else i = maps.returnRows() - 1;
                        temp.loadFromImage(maps[i]);
                    }
                    else
                    {
                        if(j > 0) j--;
                        else j = objects.returnRows() - 1;
                        temp.loadFromImage(objects[j]);
                    }
                }
            }



        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            posMouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (posMouse.x > 0 && posMouse.x < WINDOW_WIDTH && posMouse.y > 0 && posMouse.y < WINDOW_HEIGHT)
            {
                if (mapa.returnLayer() == -1)
                {
                    mapa.update(int(posMouse.x)/32, int(posMouse.y)/32, col);
                }
                else if (mapa.returnLayer() == 0)
                {
                    mapa.update(int(posMouse.x)/32, int(posMouse.y)/32, i);
                }
                else
                {
                    mapa.update(int(posMouse.x)/32, int(posMouse.y)/32, j);
                }
            }

        }

        window.clear();
        window.draw(mapa.draw());
        window.draw(chosen);
        window.draw(title);
        window.draw(curspr);
        window.draw(info);
        window.draw(outfile);
        window.draw(currentLayer);
        window.display();


    }
    return 0;
}
