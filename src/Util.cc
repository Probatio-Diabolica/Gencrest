#include "../include/Util.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cstdint>
#include <numbers>
#include <random>
#include <iostream>

namespace utils
{

static std::mt19937 rng(std::random_device{}());

sf::Vector2f rndVector(const sf::Vector2f& max)
{
    std::uniform_real_distribution<float> distX(0.0f,max.x);
    std::uniform_real_distribution<float> distY(0.0f, max.y);
    return sf::Vector2f(distX(rng),distY(rng));
}

sf::Vector2f rndVector(const sf::Vector2f& min,const sf::Vector2f& max)
{ 
    std::uniform_real_distribution<float> distX(min.x,max.x);
    std::uniform_real_distribution<float> distY(min.y, max.y);
    return sf::Vector2f(distX(rng),distY(rng));
}

float rndFloat(float max)
{
    std::uniform_real_distribution<float> dist(0.0f,max);
    return dist(rng);
}

float rndFloat(float min, float max)
{
    std::uniform_real_distribution<float> dist(min,max);
    return dist(rng);
}

sf::Color rndColor()
{
    std::uniform_real_distribution<float> dist(0,255);
    return sf::Color(dist(rng),dist(rng),dist(rng),dist(rng));
}

sf::Color rndColor(const sf::Color& base, float minOffset, float maxOffset)
{
    return sf::Color(
    static_cast<std::uint8_t>(std::clamp(static_cast<int>(base.r + rndFloat(minOffset, maxOffset)),0,255)),
    static_cast<std::uint8_t>(std::clamp(static_cast<int>(base.g + rndFloat(minOffset, maxOffset)),0,255)),
    static_cast<std::uint8_t>(std::clamp(static_cast<int>(base.b + rndFloat(minOffset, maxOffset)),0,255)),
    static_cast<std::uint8_t>(std::clamp(static_cast<int>(base.a + rndFloat(minOffset, maxOffset)),0,255))
    );
}

void logVector(const sf::Vector2f& vec)
{
    std::cout<<vec.x << " ; " << vec.y << '\n';
}

float toRadians(float degrees)
{
   return degrees * std::numbers::pi_v<float>/180.0f; 
}

float euclideanDistance(const sf::Vector2f& a, const sf::Vector2f& b)
{
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;

    return std::sqrt(dx * dx + dy * dy);
}

void drawCirclePoint(const sf::Vector2f& position, float radius, sf::RenderWindow& window, const sf::Color& color)
{
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(position);
    circle.setOrigin({radius,radius});
    window.draw(circle);
}

}
