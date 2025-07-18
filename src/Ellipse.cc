#include "../include/Ellipse.hpp"
#include "../include/Util.hpp"

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>


Ellipse::Ellipse(sf::Vector2f const& pos, const sf::Vector2f& diameter, float angleDegrees, const sf::Color& color)
    :   m_pos(pos),
        m_radius(diameter/2.0f),
        m_angleDegree(angleDegrees),
        m_color(color)
{
    computeFoci();
}


sf::Color& Ellipse::getColor() 
{
    return m_color;
}
void Ellipse::computeFoci()
{
    //needs better optimizations
    m_majorRadius = std::max(m_radius.x, m_radius.y);
    float minorSquared = std::pow(std::min(m_radius.x,m_radius.y), 2);
    float majorSquared = m_majorRadius * m_majorRadius;
    // float focalDist = std::sqrt(std::sqrt(std::max(0.0f,majorSquared - minorSquared)));
    float focalDist = std::sqrt(std::max(0.0f, majorSquared - minorSquared));

    float angleRad = utils::toRadians(m_angleDegree); 
    sf::Vector2f direction(std::cos(angleRad),std::sin(angleRad));
    m_focus1 = m_pos + direction * focalDist;
    m_focus2 = m_pos - direction * focalDist;
}

void Ellipse::move(const sf::Vector2f& delta)
{
    m_pos +=delta;
    computeFoci();
}

void Ellipse::rotate(float angleDegrees)
{
    m_angleDegree += angleDegrees;
    computeFoci();
}

void Ellipse::scale(const sf::Vector2f& factor)
{
    m_radius.x *= factor.x;
    m_radius.y *= factor.y;
    computeFoci();
}

void Ellipse::renderOnto(std::vector<u_int8_t> &pixels, sf::Vector2f& canvasSize) const
{
    const int width  = static_cast<int>(canvasSize.x);
    const int height = static_cast<int>(canvasSize.y);

    const int left   = static_cast<int>(std::clamp(m_pos.x - m_majorRadius, 0.0f, canvasSize.x));
    const int right  = static_cast<int>(std::clamp(m_pos.x + m_majorRadius, 0.0f, canvasSize.x));
    const int top    = static_cast<int>(std::clamp(m_pos.y - m_majorRadius, 0.0f, canvasSize.y));
    const int bottom = static_cast<int>(std::clamp(m_pos.y + m_majorRadius, 0.0f, canvasSize.y));

    const float alpha = static_cast<float>(m_color.a) / 255.0f;

    for (int y = top; y < bottom; ++y)
    {
        for (int x = left; x < right; ++x)
        {
            const sf::Vector2f point(static_cast<float>(x), static_cast<float>(y));
            float distSum = utils::euclideanDistance(point, m_focus1) + utils::euclideanDistance(point, m_focus2);

            if (distSum < 2.0f * m_majorRadius)
            {
                std::size_t idx = (y * width + x) << 2;

                const uint8_t oldR = pixels[idx];
                const uint8_t oldG = pixels[idx + 1];
                const uint8_t oldB = pixels[idx + 2];

                pixels[idx    ] = static_cast<uint8_t>(alpha * m_color.r + (1 - alpha) * oldR);
                pixels[idx + 1] = static_cast<uint8_t>(alpha * m_color.g + (1 - alpha) * oldG);
                pixels[idx + 2] = static_cast<uint8_t>(alpha * m_color.b + (1 - alpha) * oldB);
                pixels[idx + 3] = 255; 
            }
        }
    }
}
