#include "utils.hpp"
#include <cstddef>
#include <cstdlib>
#include <ctime>

vec2 Helper::getRandomPosition2D(pair<int, int> xrange, pair<int, int> yrange)
{
    int xval = xrange.first - xrange.second == 0 ? xrange.first : rand() % (abs(xrange.first - xrange.second) + 1 + std::min(xrange.first, xrange.second));
    int yval = yrange.first - yrange.second == 0 ? yrange.first : (rand() % (abs(yrange.first - yrange.second)) + 1 + std::min(yrange.first, yrange.second)); 
    return vec2(xval, yval);
}

vec3 Helper::getRandomPosition3D(pair<int, int> xrange, pair<int, int> yrange, pair<int, int> zrange)
{
    int xval = xrange.first - xrange.second == 0 ? xrange.first : rand() % (abs(xrange.first - xrange.second) + 1 + std::min(xrange.first, xrange.second));
    int yval = yrange.first - yrange.second == 0 ? yrange.first : (rand() % (abs(yrange.first - yrange.second)) + 1 + std::min(yrange.first, yrange.second)); 
    int zval = zrange.first - zrange.second == 0 ? zrange.first : (rand() % (abs(zrange.first - zrange.second)) + 1 + std::min(zrange.first, zrange.second)); 
    return vec3(xval, yval, zval);
}
