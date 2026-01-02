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

void renderMandelbrot(
    sf::Image& image,
    double centerX,
    double centerY,
    double scale
) {
    for (unsigned int py = 0; py < HEIGHT; ++py) {
        for (unsigned int px = 0; px < WIDTH; ++px) {

            double x0 = centerX + (px - WIDTH / 2.0) * scale / WIDTH;
            double y0 = centerY + (py - HEIGHT / 2.0) * scale / WIDTH;

            std::complex<double> c(x0, y0);
            std::complex<double> z(0, 0);

            int iteration = 0;
            while (std::abs(z) <= 2.0 && iteration < MAX_ITER) {
                z = z * z + c;
                iteration++;
            }

            int color = static_cast<int>(255.0 * iteration / MAX_ITER);
            image.setPixel({px, py}, sf::Color(color, color, color));
        }
    }
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode({WIDTH, HEIGHT}),
        "Mandelbrot"
    );

    sf::Image image({WIDTH, HEIGHT}, sf::Color::Black);
    sf::Texture texture;

    double centerX = -0.5;
    double centerY = 0.0;
    double scale   = 4.0;

    renderMandelbrot(image, centerX, centerY, scale);
    // load texture to image
    if(!texture.loadFromImage(image)){
        std::cout << "Error Loading Texture";
        return 1;
    }

    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }
}
