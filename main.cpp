#include <iostream>
//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include "fractal_forge.h"

void zoomAt(
    int mouseX,
    int mouseY,
    double factor,
    double& centerX,
    double& centerY,
    double& scale
) {
    // Convert mouse position into complex plane
    double mx = centerX + (mouseX - WIDTH / 2.0) * scale / WIDTH;
    double my = centerY + (mouseY - HEIGHT / 2.0) * scale / WIDTH;

    // Zoom
    scale *= factor;

    // Move center toward mouse
    centerX = mx - (mouseX - WIDTH / 2.0) * scale / WIDTH;
    centerY = my - (mouseY - HEIGHT / 2.0) * scale / WIDTH;
}

sf::Color palette(double t) {
    int r = static_cast<int>(9 * (1 - t) * t * t * t * 255);
    int g = static_cast<int>(15 * (1 - t) * (1 - t) * t * t * 255);
    int b = static_cast<int>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return sf::Color(r, g, b);
}

int main() {

    double centerX = -0.5; 
    double centerY = 0.0; 
    double scale = 3; // How zoomed in we are

    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Mandelbrot"
    );

    sf::Image image({WIDTH, HEIGHT}, sf::Color::Black);
    sf::Texture texture;

    renderMandelbrot(image, centerX, centerY, scale);
    // loads pixel data from cpu memory to gpu memory 
    if(!texture.loadFromImage(image)){
        std::cout << "Error Loading Texture";
        return 1;
    }

    // Use a sprite so that we can simply redraw 
    sf::Sprite sprite(texture);
    
    bool needsRedraw = true;

    while (window.isOpen()) {

        while (auto event = window.pollEvent()) {

            // Close window
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // Mouse wheel zoom
            else if (auto wheel =
                    event->getIf<sf::Event::MouseWheelScrolled>()) {
                
                // delta > 0 is zoom in and delta < 0 is zoom out
                // 0.15 is how fast we are zooming
                double zoomFactor = std::exp(-wheel->delta * 0.15);

                zoomAt(
                    wheel->position.x,
                    wheel->position.y,
                    zoomFactor,
                    centerX,
                    centerY,
                    scale
                );

                // Redraw if we change scale
                needsRedraw = true;
            }
        }

        if (needsRedraw) {
            renderMandelbrot(image, centerX, centerY, scale);
            if(!texture.loadFromImage(image)){
                std::cout << "Error Loading Texture";
                return 1;
            }            
            sprite.setTexture(texture, true);
            needsRedraw = false;
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}