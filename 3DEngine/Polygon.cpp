#include "stdafx.h"
#include "Polygon.h"

Polygon::Polygon (std::vector<Point> vertexes) {
    vertices = vertexes;

    center = Point ();
}

Polygon::Polygon (std::vector<Point> vertexes, Point centerOfShape) {
    vertices = vertexes;

    center = centerOfShape;
}

void Polygon::rotate (double degreesZ, double degreesY, double degreesX) {
    for (unsigned int i = 0; i < sizeof (vertices) / sizeof (vertices [0]); i++) {
        vertices [i] = vertices [i].getRotation (degreesZ, degreesY, degreesX, center);
    }
}

void Polygon::rotate (double degreesZ, double degreesY, double degreesX, Point centerOfRotation) {
    for (unsigned int i = 0; i < sizeof (vertices) / sizeof (vertices [0]); i++) {
        vertices [i] = vertices [i].getRotation (degreesZ, degreesY, degreesX, centerOfRotation);
    }
}

void Polygon::scale (double scaleX, double scaleY, double scaleZ) {
    for (unsigned int i = 0; i < sizeof (vertices) / sizeof (vertices [0]); i++) {
        vertices [i] = vertices [i].getScaling (scaleX, scaleY, scaleX, center);
    }
}

void Polygon::scale (double scaleX, double scaleY, double scaleZ, Point centerOfScaling) {
    for (unsigned int i = 0; i < sizeof (vertices) / sizeof (vertices [0]); i++) {
        vertices [i] = vertices [i].getScaling (scaleX, scaleY, scaleZ, centerOfScaling);
    }
}

std::vector<Point> Polygon::getPoints () {
    std::vector<Point> points;

    return points;
}