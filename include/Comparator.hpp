#ifndef COMPARATOR_HPP
#define COMPARATOR_HPP

#include<SFML/Graphics/Image.hpp>

class Comparator
{
public:
    Comparator(const sf::Image& model, const sf::Image& current);

    [[nodiscard]] float compare();

private:
    const sf::Image& m_model;
    const sf::Image& m_current;
};

#endif
