#include "../include/Picture.hpp"
#include "../include/Util.hpp"
#include "Ellipse.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <stdexcept>
#include <iostream>

namespace  
{
    float iScale = 1.f;
    int gap = 10;

    sf::Vector2f maxRadius{0,0};
    sf::Vector2f minRadius{0,0};

    sf::Vector2f scale{1.f,1.f}; 
    sf::Vector2f compressedScale{0.25f,0.25f}; 
    sf::Vector2f extraScale{4.f,4.f};
    sf::Vector2f ultra{8.f,8.f};

    sf::Vector2f inputScale = extraScale;
    sf::Vector2f outputScale = extraScale;
    sf::Vector2f processScale = scale;

}

//Picture::Picture(const std::string& path, int ellipseCount, sf::RenderWindow* renderWindow)
// :   m_ellipseCount(ellipseCount),

Picture::Picture(const std::string& path, sf::RenderWindow* renderWindow)
    : m_window(renderWindow)
{
    m_ellipseVec.reserve(923000);

    if(! m_inputImg.loadFromFile(path)) throw std::runtime_error("Failed to load image: " + path);

    m_canvasSize = sf::Vector2f(static_cast<unsigned>(m_inputImg.getSize().x),static_cast<unsigned>(m_inputImg.getSize().y));

    m_currentOutputImg = sf::Image({static_cast<unsigned>(m_inputImg.getSize().x),static_cast<unsigned>(m_inputImg.getSize().y)},sf::Color::White); 

    m_evolvedOutputImg = m_currentOutputImg;



    m_ellipseVec.emplace_back(
        utils::rndVector(m_canvasSize),
        // utils::rndVector(m_canvasSize/2.00f),
        utils::rndVector(radiusFromFitness()),
        // radiusFromFitness(),
        utils::rndFloat(360.0f),
        utils::rndColor()
    );

    if(! m_inputTexture.loadFromImage(m_inputImg)) throw std::runtime_error("can't load simple texture");

    m_inputSprite.emplace(m_inputTexture);
    m_inputSprite->setScale(inputScale);
    m_inputSprite->setPosition({0.f, 400.f});


    if(! m_currentTexture.loadFromImage(m_evolvedOutputImg)) throw std::runtime_error("Can't load current texture");

    m_outputSprite.emplace(m_currentTexture);
    m_outputSprite->setScale(outputScale);

    float imgWidth = (m_canvasSize.x * m_outputSprite->getScale().x) + gap;
    m_inputSprite->setPosition({imgWidth ,0.f});
}



void Picture::mutate()
{
    const int i = static_cast<int>(m_ellipseVec.size()) - 1;

    m_buffer = m_ellipseVec[i];

    m_ellipseVec[i].move(utils::rndVector({-5.f, -5.f}, {5.f, 5.f}));
    m_ellipseVec[i].scale(utils::rndVector({-5.f, -5.f}, {5.f, 5.f}));
    m_ellipseVec[i].rotate(utils::rndFloat(-5.f, 5.f));

    auto& color = m_ellipseVec[i].getColor();
    color.r = std::clamp<int>(color.r + static_cast<int>(utils::rndFloat(-10.f, 10.f)), 0, 255);
    color.g = std::clamp<int>(color.g + static_cast<int>(utils::rndFloat(-10.f, 10.f)), 0, 255);
    color.b = std::clamp<int>(color.b + static_cast<int>(utils::rndFloat(-10.f, 10.f)), 0, 255);
    color.a = std::clamp<int>(color.a + static_cast<int>(utils::rndFloat(-10.f, 10.f)), 0, 255);

    m_evolvedOutputImg = m_currentOutputImg;

    m_ellipseVec[i].renderOnto(m_evolvedOutputImg);

    computeFitness();

    //rejecting the mutaion in the image. 
    if (m_fitnessCurr >= m_fitnessPrev) 
    {
        m_fitnessCurr = m_fitnessPrev;
        m_noEvolutionSteps++;

        if (!m_lastMutationAccepted) 
        {
            m_ellipseVec[i] = Ellipse(
                utils::rndVector(m_canvasSize),
                // utils::rndVector(m_canvasSize / 2.0f),
                utils::rndVector(radiusFromFitness()),
                // radiusFromFitness(),
                utils::rndFloat(360.0f),
                utils::rndColor()
            );
            m_noEvolutionSteps = 0;
        }
        else m_ellipseVec[i] = m_buffer;
        // std::cout<<std::format("Rejected mutation {}\n" ,m_fitnessCurr );
    }
    else
    {
        std::cout << "Fitness = " << m_fitnessCurr << " %\n";
        m_lastMutationAccepted = true;
        m_noEvolutionSteps = 0;
        draw();
    }

    if (m_noEvolutionSteps > 500)
    {
        if (m_lastMutationAccepted) 
        {
            m_evolvedOutputImg = m_currentOutputImg;
            m_buffer.renderOnto(m_evolvedOutputImg);
            m_currentOutputImg = m_evolvedOutputImg;
        } else 
        {
            m_ellipseVec.pop_back();
            m_evolvedOutputImg = m_currentOutputImg;
        }


        m_ellipseVec.emplace_back(
            utils::rndVector(m_canvasSize), 
            // utils::rndVector(m_canvasSize / 2.0f),
            utils::rndVector(radiusFromFitness()),
            // radiusFromFitness(),
            utils::rndFloat(360.0f),
            utils::rndColor()
        );

        std::cout << "Ellipse count = " << m_ellipseVec.size() << '\n';

        m_noEvolutionSteps = 0;
        m_lastMutationAccepted = false;
    }
}

void Picture::draw()
{
    if(! m_currentTexture.loadFromImage(m_evolvedOutputImg)) throw  std::runtime_error("failed loading image in draw");

    if (m_outputSprite.has_value()) 
    {
        m_outputSprite->setTexture(m_currentTexture);

    }else 
    { 
        m_outputSprite.emplace(m_currentTexture);
        // m_outputSprite->setScale(outputScale);
    }

    // m_inputSprite->setScale(scale);

    m_window->clear(sf::Color::Black);

    if (m_outputSprite) m_window->draw(*m_outputSprite);

    if (m_inputSprite) m_window->draw(*m_inputSprite);

    m_window->display();
}

sf::Vector2f Picture::radiusFromFitness()
{
    sf::Vector2f maxRadius = m_canvasSize * 0.2f;
    sf::Vector2f minRadius = m_canvasSize * 0.01f;

    // float t = 1.0f - m_fitnessCurr;
    // sf::Vector2f radius
    // return minRadius + m_fitnessCurr * (maxRadius - minRadius);
    float t = std::log10(m_fitnessCurr + 1e-6f); // avoids log(0)
    t = std::clamp(t, -6.0f, 0.0f);  // log10 of [1e-6, 1] → [-6, 0]
    t = 1.0f + (t / 6.0f); // remap to [0, 1]
    return minRadius + t * (maxRadius - minRadius);
}


void Picture::computeFitness()
{
    m_fitnessPrev = m_fitnessCurr;
    m_fitnessCurr = 0.0f;

    const auto [width, height] = m_inputImg.getSize();

    for(unsigned x = 0; x < width ; ++x)
    {
        for(unsigned y = 0; y < height ; ++y)
        {
            const auto inputImgPixel = m_inputImg.getPixel({x,y});
            const auto outputPixel = m_evolvedOutputImg.getPixel({x,y});

            m_fitnessCurr +=std::abs(static_cast<int>(inputImgPixel.r) - static_cast<int>(outputPixel.r));
            m_fitnessCurr +=std::abs(static_cast<int>(inputImgPixel.g) - static_cast<int>(outputPixel.g));
            m_fitnessCurr +=std::abs(static_cast<int>(inputImgPixel.b) - static_cast<int>(outputPixel.b));
        }
    }

    const float pixelCount = static_cast<float>(width * height * 3);

    m_fitnessCurr /= (pixelCount * 255.0f) * 100.0f;

    ////////
    if(m_initialFitness < 0.0f) m_initialFitness = m_fitnessCurr;

    m_bestFitness = std::min(m_bestFitness, m_fitnessCurr);
}

