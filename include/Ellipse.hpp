#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <sys/types.h>
#include <vector>


class Ellipse
{
public:
    Ellipse() = default;
    Ellipse(sf::Vector2f const& pos, const sf::Vector2f& diameter, float angleDegrees, const sf::Color& color);

    void move(const sf::Vector2f& delta);
    void rotate(float angleDegrees);
    void scale(const sf::Vector2f& factor);

    void renderOnto(sf::Image& image) const;
    void renderOnto(std::vector<u_int8_t> &pixles, sf::Vector2f& canvasSize) const;

    sf::Color& getColor();

private:
    void computeFoci();

    sf::Vector2f    m_pos{};
    sf::Vector2f    m_radius{};
    float           m_angleDegree;
    sf::Color       m_color{};
    sf::Vector2f    m_focus1;
    sf::Vector2f    m_focus2;
    float           m_majorRadius = 0.0f;

};

#endif
