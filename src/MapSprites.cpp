#include "MapSprites.h"
#include <fstream>
#include <cmath>
#include <iostream>
using std::ifstream;
MapSprites::MapSprites(const char * spriteamount, const char * image, bool separate)
{
    ifstream filei(spriteamount);
    int x;
    filei >> x;
    m_rows = x;
    m_position = new int[x];

    m_mode = separate;

    if(!m_curSprite.loadFromFile(image))
        std::cout << "NIe wczytano";


    sf::Image temp;

    for (int i = 0; i < x; i++)
    {
        filei >> m_position[i];
        for (int j = 0; j < m_position[i]; j++)
        {
            temp.create(32,32, sf::Color(255, 255, 255, 0));
            temp.copy(m_curSprite, 0, 0, sf::IntRect(j*32, i*32, j*32+32, i*32+32));
            m_sprites.push_back(temp);
        }
    }
    filei.close();

}

MapSprites::~MapSprites()
{
    delete [] m_position;
}

const sf::Image & MapSprites::operator[](int x)
{

    int temp = 0;

    if(m_mode)
    {
        for (int i = 0; i < m_rows; i++)
            temp += m_position[i];
        x %= temp;
        return m_sprites[x];
    } else
    {
        x %= m_rows;
        for (int i = 0; i < x; i++)
            temp += m_position[i];
        temp += rand()%m_position[x];
        return m_sprites[temp];
    }



}

const sf::Image & MapSprites::returnImage(int x)
{
    return m_sprites[x];
}

int MapSprites::returnRows()
{
    if(m_mode)
    {
        int temp = 0;
        for (int i = 0; i < m_rows; i++)
            temp += m_position[i];
        return temp;
    } else
        return m_rows;
}

int MapSprites::returnSize()
{
    return m_sprites.size();
}

void MapSprites::mode(bool separate)
{
    m_mode = separate;
}
