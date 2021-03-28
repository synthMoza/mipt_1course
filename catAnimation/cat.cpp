#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#define WIDTH 320
#define HEIGHT 320

void RunCat(sf::RenderWindow& window)
{
    int counter = 0;
    int frames = 4;
    int number = 0;
    sf::Vector2<int> cat_size(64, 64);
    sf::Texture cat_tex;
    sf::Clock clock;
    sf::Music secret;

    if (!cat_tex.loadFromFile("Textures/cat.png"))
    {
        std::cerr << "Error: can't open the file \"cat.png\"";
        abort();
    }

    if (!secret.openFromFile("CMakeFiles/theme.wav"))
    {
        std::cerr << "Error: can't open the file \"theme.wav\"";
        abort();
    }
    secret.setLoop(true);

    sf::Sprite cat_sprite(cat_tex);
    cat_sprite.setOrigin(cat_size.x / 2, cat_size.y / 2);
    cat_sprite.setPosition(WIDTH / 2, HEIGHT / 2);

    while (window.isOpen())
    {
        sf::Event event;
        clock.restart();

        while (window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed :
                    window.close();
                    break;
                case sf::Event::KeyPressed :
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    else if (event.key.code == sf::Keyboard::Right )
                    {
                        if (number == 3)
                            number = 0;
                        else
                            number++;
                    }
                    else if (event.key.code == sf::Keyboard::Left  && number >= 0)
                    {
                        if (number == 0)
                            number = 3;
                        else
                            number--;
                    }
                    else if (event.key.code == sf::Keyboard::Tab)
                    {
                        if (secret.getStatus() == sf::SoundSource::Playing)
                            secret.stop();
                        else
                            secret.play();
                    }
                    break;
            }
        }

        cat_sprite.setTextureRect(sf::IntRect(number * cat_size.x, (counter % frames) * cat_size.x, cat_size.x, cat_size.y));
        cat_sprite.setScale(sf::Vector2f(5, 5));

        window.clear(sf::Color::White);
        window.draw(cat_sprite);
        window.display();

        while (clock.getElapsedTime().asMicroseconds() <= 100000) ;

        counter++;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Kitten");

    RunCat(window);
    return 0;
}