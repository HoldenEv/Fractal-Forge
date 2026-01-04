#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>

const int WIDTH = 1000;
const int HEIGHT = 800;
const int MAX_ITER = 1000;

void zoomAt(
    int mouseX,
    int mouseY,
    double factor,
    double& centerX,
    double& centerY,
    double& scale
) {
    // Convert mouse position → complex plane
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

void renderMandelbrot(
    sf::Image& image,
    double centerX,
    double centerY,
    double scale
) {
    // Loop over every pixel in the plane
    for (unsigned int py = 0; py < HEIGHT; ++py) {
        for (unsigned int px = 0; px < WIDTH; ++px) {
            
            // CenterX and CenterY are anchored cordinated that get offset
            // x0 is real number cord and y0 is imaginary cord
            double x0 = centerX + (px - WIDTH / 2.0) * scale / WIDTH;
            double y0 = centerY + (py - HEIGHT / 2.0) * scale / WIDTH;

            // use x0 and y0 to find complex const
            std::complex<double> c(x0, y0); // c = x0 + (y0 * i) where i is imaginary
            std::complex<double> z(0, 0); // By def z0 = 0 

            // We iterate 1000 times to see if z^2 + c explodes or is stable
            // Bread and Butter right here :) 
            int iteration = 0;
            while (std::abs(z) <= 2.0 && iteration < MAX_ITER) {
                z = z * z + c;
                iteration++;
            }

            // Smooth coloring shows how far past the escape radius did it go?
            double smooth = iteration;

            if (iteration < MAX_ITER) {
                smooth = iteration + 1 - std::log2(std::log2(std::abs(z)));
            }
            
            double t = smooth / MAX_ITER;
            if (iteration == MAX_ITER) {
                image.setPixel({px, py}, sf::Color::Black);
            } else {
                image.setPixel({px, py}, palette(t));
            }
        }
    }
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
