#include "Map.h"
#include <fstream>
#include <iostream>
using std::ifstream;
using std::ofstream;

Map::Map(const char * mapfile, MapSprites * sprites, MapSprites * objects)
{
    // przypisanie baz spriteow i obiektow
    m_sprites = sprites;
    m_objects = objects;
    m_showCollision = false;
    // pobranie glownych danych o mapie
    ifstream filein(mapfile);
    filein >> m_height >> m_width >> m_layers;

    // stworzenie obrazu o danym rozmiarze
    m_image.create(m_width*cord_to_pix, m_height*cord_to_pix, sf::Color(0, 0, 0, 0));

    // tymczasowy bufer na sprita
    int x;

    // aktywna warstwa
    m_curlay = 0;

    // zainicjalizowanie mapy
    m_intmap = new int ** [m_layers];

    for (int k = 0; k < m_layers; k++)
    {
        m_intmap[k] = new int * [m_height];
        for (int i = 0; i < m_height; i++)
        {
            m_intmap[k][i] = new int [m_width];
            for (int j = 0; j < m_width; j++)
            {
                // pobranie informacji o aktualnym spricie
                filein >> x;
                if (k == 0) // zainicjalizowanie pierwszej warstwy
                {
                    if (x < 0) m_intmap[k][i][j] = -1;
                    else m_intmap[k][i][j] = x;
                    if (x >= 0) m_image.copy((*m_sprites)[x], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), false);
                } else // zainicjalizowanie kolejnych wastw
                {
                    if (x < 0) m_intmap[k][i][j] = -1;
                    else m_intmap[k][i][j] = x;
                    m_image.copy((*m_objects)[x], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), k);
                }
                std::cout << m_intmap[k][i][j] << " ";

            }
            std::cout << "\n";
        }
    }

    // mapa kolizji
    m_collision = new bool * [m_height];
    for (int i = 0; i < m_height; i++)
    {
        m_collision[i] = new bool [m_width];
        for (int j = 0; j < m_width; j++)
            filein >> m_collision[i][j];
    }

    filein.close();

    // przypisanie obrazu do textury a potem do spritea
    m_maptexture.loadFromImage(m_image);
    m_map.setTexture(m_maptexture);

}

Map & Map::operator=(const Map & kopia)
{
    for (int j = 0; j < m_layers; j++)
    {
        for (int i = 0; i < m_height; i++) delete [] m_intmap[j][i];
        delete [] m_intmap[j];
    }

    delete [] m_intmap;

    m_width = kopia.m_width;
    m_height = kopia.m_height;
    m_layers = kopia.m_layers;
    m_curlay = kopia.m_curlay;
    m_sprites = kopia.m_sprites;
    m_objects = kopia.m_objects;
    m_showCollision = kopia.m_showCollision;
    m_intmap = new int ** [m_layers];

    m_image.create(m_width*cord_to_pix, m_height*cord_to_pix, sf::Color(0, 0, 0, 0));

    for (int k = 0; k < m_layers; k++)
    {
        m_intmap[k] = new int * [m_height];
        for (int i = 0; i < m_height; i++)
        {
            m_intmap[k][i] = new int [m_width];
            for (int j = 0; j < m_width; j++)
            {
                m_intmap[k][i][j] = kopia.m_intmap[k][i][j];
                if (k == 0) // zainicjalizowanie pierwszej warstwy
                {

                    if (m_intmap[k][i][j] >= 0) m_image.copy((*m_sprites)[m_intmap[k][i][j]], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), false);
                } else if (m_intmap[k][i][j] != -1) // zainicjalizowanie kolejnych wastw
                {
                    m_image.copy((*m_objects)[m_intmap[k][i][j]], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), true);
                }
                std::cout << m_intmap[k][i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    m_collision = new bool * [m_height];
    for (int i = 0; i < m_height; i++)
    {
        m_collision[i] = new bool [m_width];
        for (int j = 0; j < m_width; j++)
            m_collision[i][j] = kopia.m_collision[i][j];
    }

    m_maptexture.loadFromImage(m_image);
    m_map.setTexture(m_maptexture);

    return *this;

}

Map::Map(int width, int height, int layers, MapSprites * sprites, MapSprites * objects)
{


    // przypisanie podstawowych danych
    m_layers = layers;
    m_curlay = 0;
    if (m_layers < 1 || m_layers > max_layers) m_layers = 1;
    m_width = width;
    m_height = height;
    m_showCollision = false;

    // przypisanie baz spriteow i obiektow
    m_sprites = sprites;
    m_objects = objects;

    // stworzenie pustego obrazu
    m_image.create(m_width*cord_to_pix, m_height*cord_to_pix, sf::Color(255, 255, 255, 0));

    // zainicjalizowanie mapy
    m_intmap = new int ** [m_layers];
    for (int k = 0; k < m_layers; k++)
    {
        m_intmap[k] = new int * [m_height];
        for (int i = 0; i < m_height; i++)
        {
            m_intmap[k][i] = new int [m_width];
            for (int j = 0; j < m_width; j++)
            {
                if (k == 0) // zainicjalizowanie pierwszej warstwy
                {
                    m_intmap[k][i][j] = 0;
                    m_image.copy((*m_sprites)[0], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), false);
                } else // zainicjowanie kolejnych warstw
                    m_intmap[k][i][j] = -1;
            }
        }
    }

    m_collision = new bool * [m_height];
    for (int i = 0; i < m_height; i++)
    {
        m_collision[i] = new bool [m_width];
        for (int j = 0; j < m_width; j++)
            m_collision[i][j] = false;
    }

    // przypisanie obrazu do textury a potem do spritea
    m_maptexture.loadFromImage(m_image);
    m_map.setTexture(m_maptexture);
}

Map::~Map()
{
    for (int j = 0; j < m_layers; j++)
    {
        for (int i = 0; i < m_height; i++) delete [] m_intmap[j][i];
        delete [] m_intmap[j];
    }

    delete [] m_intmap;

}

const sf::Sprite & Map::draw() const
{
    return m_map;
}

void Map::update(int x, int y, int sprite)
{
    // zabezpieczenie przed zla wysokoscia lub szerokoscia
    x %= m_width;
    y %= m_height;

    // przypisanie sprite na miejsce
    if (m_curlay == -1) m_collision[y][x] = sprite;
    else m_intmap[m_curlay][y][x] = sprite;

    // skopiowanie z pierwszej warstwy
    m_image.copy((*m_sprites)[m_intmap[0][y][x]], x*cord_to_pix, y*cord_to_pix, sf::IntRect(0, 0, 32, 32), false);

    // skopiowanie z kolejnych warstw
    for (int i = 1; i < m_layers; i++)
    {
        if (m_intmap[i][y][x] != -1)
        {
            m_image.copy((*m_objects)[m_intmap[i][y][x]], x*cord_to_pix, y*cord_to_pix, sf::IntRect(0, 0, 32, 32), true);
        }
    }
    sf::Image temp;
    temp.create(32, 32, sf::Color(255, 0, 0, 100));
    if (m_showCollision && m_collision[y][x]) m_image.copy(temp, x*cord_to_pix, y*cord_to_pix, sf::IntRect(0, 0, 32, 32), true);

    // przypisanie obrazu do textury
    m_maptexture.loadFromImage(m_image);
}

// dodanie warstwy do mapy
void Map::addLayer()
{
    if (m_layers < max_layers)
    {
        // tworzenie nowej tablicy
        int cur = m_layers + 1;
        int *** temp;
        temp = new int ** [cur];

        for (int k = 0; k < m_layers +1; k++)
        {
            temp[k] = new int * [m_height];
            for (int i = 0; i < m_height; i++)
                temp[k][i] = new int [m_width];
        }
        std::cout << "stworzono tablice\n";
        // kopiowanie wartosci z poprzedniej tablicy
        for (int k = 0; k < m_layers; k++)
        {
            for (int i = 0; i < m_height; i++)
            {
                for (int j = 0; j < m_width; j++)
                {
                    temp[k][i][j] = m_intmap[k][i][j];
                }
            }
        }
        std::cout << "skopiowano tablice\n";
        // czyszczenie starej tablicy
        for (int j = 0; j < m_layers; j++)
        {
            for (int i = 0; i < m_height; i++) delete [] m_intmap[j][i];
            delete [] m_intmap[j];
        }
        delete [] m_intmap;

        // wyczyszczenie nowego pola tablicy
        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++) temp[m_layers][i][j] = -1;
        }
        std::cout << "wyczyszczono tablice\n";
        // przypisanie nowej tablicy do skladowej
        m_intmap = temp;

        // zwiekszenie liczby warstw
        m_layers++;
        std::cout << "przypisano tablice\n";
    }
}

void Map::showCollision(bool yes)
{

    if(yes && yes != m_showCollision)
    {
        sf::Image temp;
        temp.create(32, 32, sf::Color(255, 0, 0, 100));
        for (int i = 0; i < m_height; i++)
            for (int j = 0; j < m_width; j++)
            {
                if (m_collision[i][j]) m_image.copy(temp, j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), true);
            }
    }
    else if (yes != m_showCollision)
    {

        for (int k = 0; k < m_layers; k++)
            for (int i = 0; i < m_height; i++)
            {
                for (int j = 0; j < m_width; j++)
                {
                    if (k == 0) // zainicjalizowanie pierwszej warstwy
                    {
                        if (m_intmap[k][i][j] >= 0) m_image.copy((*m_sprites)[m_intmap[k][i][j]], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), false);
                    }
                    else if (m_intmap[k][i][j] != -1)   // zainicjalizowanie kolejnych wastw
                    {
                        m_image.copy((*m_objects)[m_intmap[k][i][j]], j*cord_to_pix, i*cord_to_pix, sf::IntRect(0, 0, 32, 32), true);
                    }
                }
            }
    }
    m_maptexture.loadFromImage(m_image);
    m_showCollision = yes;

}

bool Map::collision(int x, int y)
{
    return m_collision[x][y];
}

int Map::returnSprite(int x, int y)
{
    return m_intmap[m_layers-1][x][y];
}

// zwraca numer aktywnej warstwy
int Map::returnLayer()
{
    return m_curlay;
}

int Map::returnAllLayers()
{
    return m_layers - 1;
}

// warstwa wyzej
int Map::nextLayer()
{
    if (m_curlay < m_layers - 1) m_curlay++;

    return m_curlay;
}

// warstwa nizej
int Map::previousLayer()
{
    if (m_curlay > -1) m_curlay--;
    return m_curlay;
}
const char * Map::save(const char * newmap)
{
    ofstream fileout(newmap);
    fileout << m_height << " " << m_width << " " << m_layers << "\n";
    for (int k = 0; k < m_layers; k++)
    {
        for (int i = 0; i < m_height; i++)
        {
            for (int j = 0; j < m_width; j++)
            {
                fileout << m_intmap[k][i][j] << " ";
            }
            fileout << "\n";
        }
    }

    for (int i = 0; i < m_height; i++)
    {
        for (int j = 0; j < m_width; j++)
        {
            fileout << m_collision[i][j] << " ";
        }
        fileout << "\n";
    }

    fileout.close();
    return newmap;
}

