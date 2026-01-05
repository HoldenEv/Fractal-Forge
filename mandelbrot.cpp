#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <complex>
#include "fractal_forge.h"

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