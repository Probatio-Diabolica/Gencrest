#include "../include/Canvas.hpp"
#include "../include/Util.hpp"
#include "Ellipse.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cmath>
#include <format>
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

Canvas::Canvas(const std::string& path, sf::RenderWindow* renderWindow)
    : m_window(renderWindow)
{
    m_ellipseVec.reserve(923000);

    if(! m_inputImg.loadFromFile(path)) throw std::runtime_error("Failed to load image: " + path);

    const auto [width,height] = m_inputImg.getSize();
    m_height = height;
    m_width = width;
    std::cout<<std::format(" h {} , w {}\n",m_height,m_width);
    std::size_t size = (width * height) << 2;
    m_currRawOutput.resize(size);
    m_currRawEvoOutput.resize(size);


    m_canvasSize = sf::Vector2f(static_cast<unsigned>(m_inputImg.getSize().x),static_cast<unsigned>(m_inputImg.getSize().y));

    m_currentOutputImg = sf::Image({static_cast<unsigned>(m_inputImg.getSize().x),static_cast<unsigned>(m_inputImg.getSize().y)},sf::Color::White); 

    // m_evolvedOutputImg = m_currentOutputImg;
    std::copy(m_currRawOutput.begin(),m_currRawOutput.end(),m_currRawEvoOutput.begin());


    m_ellipseVec.emplace_back(
        utils::rndVector(m_canvasSize),
        utils::rndVector(radiusFromFitness()),
        utils::rndFloat(360.0f),
        utils::rndColor()
    );

    if(! m_inputTexture.loadFromImage(m_inputImg)) throw std::runtime_error("can't load simple texture");

    m_inputSprite.emplace(m_inputTexture);
    m_inputSprite->setScale(inputScale);
    m_inputSprite->setPosition({0.f, 400.f});

    if(! m_currentTexture.loadFromImage({{m_width,m_height},m_currRawEvoOutput.data()})) throw std::runtime_error("Can't load current texture");


    m_outputSprite.emplace(m_currentTexture);
    m_outputSprite->setScale(outputScale);

    float imgWidth = (m_canvasSize.x * m_outputSprite->getScale().x) + gap;
    m_inputSprite->setPosition({imgWidth ,0.f});
}



void Canvas::mutate()
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

    std::copy(m_currRawOutput.begin(),m_currRawOutput.end(),m_currRawEvoOutput.begin());

    m_ellipseVec[i].renderOnto(m_currRawEvoOutput,m_canvasSize);


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
                utils::rndVector(radiusFromFitness()),
                utils::rndFloat(360.0f),
                utils::rndColor()
            );
            m_noEvolutionSteps = 0;
        }
        else m_ellipseVec[i] = m_buffer;
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
            std::copy(m_currRawOutput.begin(), m_currRawOutput.end(), m_currRawEvoOutput.begin());

            m_buffer.renderOnto(m_currRawEvoOutput,m_canvasSize);
            std::copy(m_currRawEvoOutput.begin(), m_currRawEvoOutput.end(), m_currRawOutput.begin());
        } else 
        {
            m_ellipseVec.pop_back();
            std::copy(m_currRawOutput.begin(), m_currRawOutput.end(), m_currRawEvoOutput.begin());
        }


        m_ellipseVec.emplace_back(
            utils::rndVector(m_canvasSize), 
            utils::rndVector(radiusFromFitness()),
            utils::rndFloat(360.0f),
            utils::rndColor()
        );

        std::cout << "Ellipse count = " << m_ellipseVec.size() << '\n';

        m_noEvolutionSteps = 0;
        m_lastMutationAccepted = false;
    }
}

void Canvas::draw()
{
    sf::Image outputImg({m_width,m_height},m_currRawEvoOutput.data());

    if(! m_currentTexture.loadFromImage(outputImg)) throw  std::runtime_error("failed loading image in draw");

    if (m_outputSprite.has_value()) m_outputSprite->setTexture(m_currentTexture);
    else m_outputSprite.emplace(m_currentTexture);

    m_window->clear(sf::Color::Black);

    if (m_outputSprite) m_window->draw(*m_outputSprite);

    if (m_inputSprite) m_window->draw(*m_inputSprite);

    m_window->display();
}

sf::Vector2f Canvas::radiusFromFitness()
{
    sf::Vector2f maxRadius = m_canvasSize * 0.2f;
    sf::Vector2f minRadius = m_canvasSize * 0.01f;

    float t = std::log10(m_fitnessCurr + 1e-6f); 
    t = std::clamp(t, -6.0f, 0.0f); 
    t = 1.0f + (t / 6.0f); 
    return minRadius + t * (maxRadius - minRadius);
}


void Canvas::computeFitness()
{
    m_fitnessPrev = m_fitnessCurr;
    m_fitnessCurr = 0.0f;

    const auto [width, height] = m_inputImg.getSize();

    const std::size_t expectedSize = static_cast<std::size_t>(width * height) << 2;

    if (m_currRawEvoOutput.size() < expectedSize)
        throw std::runtime_error("Raw pixel buffer size mismatch in computeFitness()");


    for(unsigned x = 0; x < width ; ++x)
    {
        for(unsigned y = 0; y < height ; ++y)
        {
            unsigned idx = (y * width + x) << 2;
            const auto inputImgPixel = m_inputImg.getPixel({x,y});

            const int r = m_currRawEvoOutput[idx];
            const int g = m_currRawEvoOutput[idx+1]; 
            const int b = m_currRawEvoOutput[idx+2]; 

            
            m_fitnessCurr += std::abs(static_cast<int>(inputImgPixel.r) - static_cast<int>(r));
            m_fitnessCurr += std::abs(static_cast<int>(inputImgPixel.g) - static_cast<int>(g));
            m_fitnessCurr += std::abs(static_cast<int>(inputImgPixel.b) - static_cast<int>(b));
            
        }
    }

    const float pixelCount = static_cast<float>(width * height * 3);

    m_fitnessCurr /= (pixelCount * 255.0f) * 100.0f;

    if(m_initialFitness < 0.0f) m_initialFitness = m_fitnessCurr;

    m_bestFitness = std::min(m_bestFitness, m_fitnessCurr);
}
