#pragma once

#include "../Shape/Shape3D.hpp"

#include <cstdlib>

#define PI 3.14159265358979323846

class Helper
{
    public:
        int wheight;
        int wwidth;

        Helper(vec2 res) {
            this->wwidth = res.x;
            this->wheight = res.y;
        }

        static void buildCircle(float cx, float cy, float raggiox, float raggioy, Shape3D* fig);

        /*
         * Generates random 2D coordinates by given ranges. If a range is (n, n), it retrieves 
         * only the n number without doing random generation.
        */
        vec2 static getRandomPosition2D(pair<int, int> xrange, pair<int, int> yrange);

        /*
         * Generates random 3D coordinates by given ranges. If a range is (n, n), it retrieves 
         * only the n number without doing random generation.
        */
        vec3 static getRandomPosition3D(pair<int, int> xrange, pair<int, int> yrange, pair<int, int> zrange);
};

