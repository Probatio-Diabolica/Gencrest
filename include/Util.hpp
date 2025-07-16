#ifndef UTIL_HPP
#define UTIL_HPP

#include<SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

namespace utils 
{
    //random vecs
    [[nodiscard]] sf::Vector2f rndVector(const sf::Vector2f& max);
    [[nodiscard]] sf::Vector2f rndVector(const sf::Vector2f& min,const sf::Vector2f& max);

    //random floats
    [[nodiscard]] float rndFloat(float max);
    [[nodiscard]] float rndFloat(float min, float max);

    //Color
    [[nodiscard]] sf::Color rndColor();
    [[nodiscard]] sf::Color rndColor(const sf::Color& base, float minOffset, float maxOffset);

    //Debug
    void logVector(const sf::Vector2f& vec);

    //Raadians and euclidian conversions
    [[nodiscard]] float toRadians(float degrees);
    [[nodiscard]] float euclideanDistance(const sf::Vector2f& a, const sf::Vector2f& b);

    //Rendering
    void drawCirclePoint(const sf::Vector2f& position, float radius, sf::RenderWindow& window, const sf::Color& color);

}

#endif
