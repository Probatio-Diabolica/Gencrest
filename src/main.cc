#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>

#include "../include/Canvas.hpp"

void usage()
{
    std::cout << "+ usage: hill <image_path> [ellipse_count]\n";
    std::cout << "+ <image_path>     : Path to the target image (required)\n";
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

    constexpr unsigned winWidth  = 1900;
    constexpr unsigned winHeight = 1080;

    sf::RenderWindow window(sf::VideoMode({winWidth,winHeight}),"Picaso");
    try
    {
        // Picture pic(imgPath,ellipseCount, &window);
        Canvas pic(imgPath, &window);

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

