#ifndef MAPSPRITES_H
#define MAPSPRITES_H
#include <vector>
#include <SFML/Graphics.hpp>

class MapSprites
{
    public:
        explicit MapSprites(const char * filename, const char * image, bool separate = false);
        void mode(bool separate);
        const sf::Image & operator[](int x);
        const sf::Image & returnImage(int x);
        int returnRows();
        int returnSize();
        ~MapSprites();
    private:
        std::vector<sf::Image> m_sprites;
        sf::Image m_curSprite;
        int m_rows;
        int * m_position;
        bool m_mode;


};

#endif // MAPSPRITES_H
