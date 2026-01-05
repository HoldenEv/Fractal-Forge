#ifndef FRACTAL_FORGE_H
#define FRACTAL_FORGE_H

#include <SFML/Graphics.hpp>

// Shared constants
constexpr int WIDTH  = 1000;
constexpr int HEIGHT = 800;
constexpr int MAX_ITER = 1000;

// Core rendering interface
void renderMandelbrot(
    sf::Image& image,
    double centerX,
    double centerY,
    double scale
);

// Utility helpers (shared by all fractals)
void zoomAt(
    int mouseX,
    int mouseY,
    double factor,
    double& centerX,
    double& centerY,
    double& scale
);

sf::Color palette(double t);

#endif
