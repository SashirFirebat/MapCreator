#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include "MapSprites.h"
class Map
{
    public:
        Map(const char * mapfile, MapSprites * sprites, MapSprites * objects);
        Map(int width, int height, int layers, MapSprites * sprites, MapSprites * objects);
        Map & operator=(const Map & kopia);
        void update(int x, int y, int sprite);
        void addLayer();
        void showCollision(bool yes = true);
        bool collision(int x, int y);
        int returnSprite(int x, int y);
        int returnLayer();
        int returnAllLayers();
        int nextLayer();
        int previousLayer();
        const char * save(const char * newmap = "testmap.txt");
        const sf::Sprite & draw() const;
        ~Map();
    private:
        const int cord_to_pix = 32, max_layers = 5;
        sf::Image m_image;
        sf::Texture m_maptexture;
        sf::Sprite m_map;
        int m_width, m_height, m_layers, m_curlay;
        int ***m_intmap;
        bool **m_collision, m_showCollision;
        MapSprites * m_sprites;
        MapSprites * m_objects;

};

#endif // MAP_H
