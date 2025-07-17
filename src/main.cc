#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "../include/Picture.hpp"

void usage()
{
    std::cout << "+ usage: hill <image_path> [ellipse_count]\n";
    std::cout << "+ <image_path>     : Path to the target image (required)\n";
    std::cout << "+ [ellipse_count]  : Initial ellipse count (optional, default: 1)\n"; 
}

int main(int argc, char* argv[]) 
{
    if(argc < 2 or std::string(argv[1]) == "--help")
    {
        usage();
        return EXIT_SUCCESS;
    }

    const std::string imgPath = argv[1];
    int ellipseCount = 1;

    if(argc >= 3)
    {
        try 
        {
            ellipseCount = std::stoi(argv[2]);
            if(ellipseCount < 1) throw std::invalid_argument("Ellipse count must be greater than 0");
        } catch (const std::exception& e) 
        {
            std::cerr << "Invalid ellipse count: " <<e.what() <<'\n';
            usage();
            return EXIT_FAILURE;
        }
    }


    constexpr unsigned winWidth  = 1900;
    constexpr unsigned winHeight = 1080;

    sf::RenderWindow window(sf::VideoMode({winWidth,winHeight}),"Gen");
    //window.setFramerateLimit(120);

    try
    {
        Picture pic(imgPath,ellipseCount, &window);

        while(window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                window.close();
            }
            pic.mutate();
        }
    }
    catch(const std::exception & e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
}

