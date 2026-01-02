#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>

const int WIDTH = 800;
const int HEIGHT = 600;
const int MAX_ITER = 1000;

void zoomAt(int mouseX, int mouseY, double factor, double centerX, double centerY, double scale) {
    double mx = centerX + (mouseX - WIDTH / 2.0) * scale / WIDTH;
    double my = centerY + (mouseY - HEIGHT / 2.0) * scale / WIDTH;
    centerX = mx + (centerX - mx) * factor;
    centerY = my + (centerY - my) * factor;
    scale *= factor;
}

int mandelbrot(double centerX, double centerY, double scale) {
    for (int py = 0; py < HEIGHT; ++py) {
        for (int px = 0; px < WIDTH; ++px) {
            // Map pixel to complex plane
            double x0 = centerX + (px - WIDTH / 2.0) * scale / WIDTH;
            double y0 = centerY + (py - HEIGHT / 2.0) * scale / WIDTH;
            std::complex<double> c(x0, y0);
            std::complex<double> z(0, 0);

            int iteration = 0;
            while (abs(z) <= 2.0 && iteration < MAX_ITER) {
                z = z*z + c;
                iteration++;
            }

            // Color pixel based on iteration
            // COLOR BLACK and WHite Value Somewhere in the scale of 1-255
            int color = (int)(255.0 * iteration / MAX_ITER);
            return color;
        }
        // if 0 print new line
        return 0;
    }
    return -1;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "SFML window");

    // Load a music to play
    sf::Music music("Fantascape.ogg");

    // Play the music
    music.play();

    double centerX = -0.5;
    double centerY = 0.0;
    double scale = 4.0;

     // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Clear screen
        window.clear();


        // Update the window
        window.display();
    }
    
    
    return 0;
}
