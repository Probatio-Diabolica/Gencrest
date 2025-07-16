#ifndef PICTURE_HPP
#define PICTURE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <optional>

#include "Ellipse.hpp"

class Picture
{
public:
    Picture(const std::string& path, int ellipseCount, sf::RenderWindow* renderWindow);

    void mutate();
    void computeFitness();

private:
    void draw();
    
    sf::Image m_model;
    sf::Image m_currentImg;
    sf::Image m_currentEvolved;

    std::vector<Ellipse>    m_ellipseVec;
    Ellipse                 m_buffer;


    int m_ellipseCount     = 0;
    int m_noEvolutionSteps = 0;
    float m_fitnessPrev = 255.0f;
    float m_fitnessCurr = 255.0f;
    bool m_lastMutationAccepted = false;

    sf::Vector2f m_canvasSize;

    sf::Texture m_currentTexture;
    sf::Texture m_textureModel;
    std::optional<sf::Sprite>  m_spriteCurrent;
    std::optional<sf::Sprite>  m_spriteModel;

    sf::RenderWindow* m_window = nullptr;
};

#endif
