#ifndef PICTURE_HPP
#define PICTURE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <limits>
#include <vector>
#include <optional>

#include "Ellipse.hpp"

using rawPixels = std::vector<u_int8_t>;


class Canvas
{
public:
    // Picture(const std::string& path, int ellipseCount, sf::RenderWindow* renderWindow);
    Canvas(const std::string& path,  sf::RenderWindow* renderWindow);



    void mutate();
    void computeFitness();

private:
    //heuristic func
    sf::Vector2f radiusFromFitness();
    void draw();
    
    sf::Image m_inputImg;
    sf::Image m_currentOutputImg;
    // sf::Image m_evolvedOutputImg;

    std::vector<Ellipse>    m_ellipseVec;
    Ellipse                 m_buffer;


    // int m_ellipseCount     = 0;
    int m_noEvolutionSteps      = 0;
    float m_fitnessPrev         = 255.0f;
    float m_fitnessCurr         = 255.0f;
    //////
    float m_initialFitness      = -1.f;
    float m_bestFitness         = std::numeric_limits<float>::max();
    //////
    bool m_lastMutationAccepted = false;
    

    sf::Vector2f m_canvasSize;

    ////////////////////////////////
    rawPixels m_currRawOutput;
    rawPixels m_currRawEvoOutput;
    unsigned m_width = 0;
    unsigned m_height = 0;
    ////////////////////////////////
    sf::Texture m_currentTexture;
    sf::Texture m_inputTexture;
    std::optional<sf::Sprite>  m_outputSprite;
    std::optional<sf::Sprite>  m_inputSprite;

    sf::RenderWindow* m_window = nullptr;
};

#endif
