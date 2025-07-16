#include "../include/Picture.hpp"
#include "../include/Util.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include <iostream>

Picture::Picture(const std::string& path, int ellipseCount, sf::RenderWindow* renderWindow)
    :   m_ellipseCount(ellipseCount),
        m_window(renderWindow)
{
    if(!m_model.loadFromFile(path)) throw std::runtime_error("Failed to load image: " + path);


    m_canvasSize = sf::Vector2f(static_cast<unsigned>(m_model.getSize().x),static_cast<unsigned>(m_model.getSize().y));
    m_currentImg = sf::Image({static_cast<unsigned>(m_model.getSize().x),static_cast<unsigned>(m_model.getSize().y)},sf::Color::White);
   
    m_currentEvolved = m_currentImg;
    
    m_ellipseVec.emplace_back(
        utils::rndVector(m_canvasSize),
        utils::rndVector(m_canvasSize/2.00f),
        utils::rndFloat(360.0f),
        utils::rndColor()
    );


    if(! m_textureModel.loadFromImage(m_model)) throw std::runtime_error("can't load simple texture");

    m_spriteModel.emplace(m_textureModel);
    m_spriteModel->setScale({4.f,4.f});
    m_spriteModel->setPosition({0.f, 400.f});


    if(! m_currentTexture.loadFromImage(m_currentEvolved)) throw std::runtime_error("Can't load current texture");

    m_spriteCurrent.emplace(m_currentTexture);
    m_spriteCurrent->setScale({4.f,4.f});
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

    m_currentEvolved = m_currentImg;
    m_ellipseVec[i].renderOnto(m_currentEvolved);

    computeFitness();

    //rejecting the mutaion in the image. i.e, reject the hypothesis 
    if (m_fitnessCurr >= m_fitnessPrev) 
    {
        m_fitnessCurr = m_fitnessPrev;
        m_noEvolutionSteps++;

        if (!m_lastMutationAccepted) 
        {
            m_ellipseVec[i] = Ellipse(
                utils::rndVector(m_canvasSize),
                utils::rndVector(m_canvasSize / 2.0f),
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
            m_currentEvolved = m_currentImg;
            m_buffer.renderOnto(m_currentEvolved);
            m_currentImg = m_currentEvolved;
        } else 
        {
            m_ellipseVec.pop_back();
            m_currentEvolved = m_currentImg;
        }

        m_ellipseVec.emplace_back(
            utils::rndVector(m_canvasSize),
            utils::rndVector(m_canvasSize / 2.0f),
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
    if(! m_currentTexture.loadFromImage(m_currentEvolved)) throw  std::runtime_error("failed loading image in draw");

    if (m_spriteCurrent.has_value()) m_spriteCurrent->setTexture(m_currentTexture);
    else 
    { 
        m_spriteCurrent.emplace(m_currentTexture);
        m_spriteCurrent->setScale({4.f, 4.f});
    }

    m_window->clear(sf::Color::Black);

    if (m_spriteCurrent) m_window->draw(*m_spriteCurrent);

    if (m_spriteModel) m_window->draw(*m_spriteModel);

    m_window->display();
}


void Picture::computeFitness()
{
    m_fitnessPrev = m_fitnessCurr;
    m_fitnessCurr = 0.0f;

    const auto [width, height] = m_model.getSize();

    for(unsigned x = 0; x < width ; ++x)
    {
        for(unsigned y = 0; y < height ; ++y)
        {
            const auto modelPixel = m_model.getPixel({x,y});
            const auto currentPixel = m_currentEvolved.getPixel({x,y});

            m_fitnessCurr +=std::abs(static_cast<int>(modelPixel.r) - static_cast<int>(currentPixel.r));
            m_fitnessCurr +=std::abs(static_cast<int>(modelPixel.g) - static_cast<int>(currentPixel.g));
            m_fitnessCurr +=std::abs(static_cast<int>(modelPixel.b) - static_cast<int>(currentPixel.b));
        }
    }

    const float pixelCount = static_cast<float>(width * height * 3);

    m_fitnessCurr /= (pixelCount * 255.0f) * 100.0f;

}

