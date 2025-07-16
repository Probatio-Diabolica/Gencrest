#include "../include/Comparator.hpp"
    
Comparator::Comparator(const sf::Image& model, const sf::Image& current)
    : m_model(model), m_current(current)
{}

[[nodiscard]] float Comparator::compare()
{
    float netDiff = 0.0f;
    const auto [width, height] = m_model.getSize();

    for(unsigned x =0; x < width;++x)
    {
        for(unsigned y = 0; y < height ; ++y)
        {
            const auto pixA = m_model.getPixel({x,y});
            const auto pixB = m_current.getPixel({x,y});
        
            netDiff += std::abs(static_cast<int>(pixA.r) - static_cast<int>(pixB.r));
            netDiff += std::abs(static_cast<int>(pixA.g) - static_cast<int>(pixB.g));
            netDiff += std::abs(static_cast<int>(pixA.b) - static_cast<int>(pixB.b));
        }
    }

    const float maxScore = static_cast<float>(width*height*3*255);

    return netDiff / maxScore * 100.0f;
}

