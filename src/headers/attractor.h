#ifndef ATTRACTOR_H
#define ATTRACTOR_H

#include <SDL2/SDL.h>
#include <vector>
#include "vector3.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <functional>
#include "matrix3x3.h"

struct Attractor
{
    double dt = 0.001;
    double a = .95, b = .7, c = .6, d = 3.5, e = .25, f = .1;
    vec3 pos;
    int height = 1000;
    int width = 1000;
    float rad = (M_PI / 180.0f);
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::vector<vec3> vertices;
    vec3 centroid;

    Attractor();
    double find_min_point();
    double find_max_point();
    vec3 vec_function(vec3 point);
    vec3 midpoint_method(double h);
    vec3 rk4_method(double h);
    vec3 dopri8_method(double h);
    Matrix3x3 create_jacobian(const vec3 &point, double h);
    vec3 euler_method(double h);
    vec3 backward_euler_method(const vec3 &predicted_point, double h);
    vec3 trapezoid_method_predictor(double h);
    vec3 trapezoid_method(const vec3 &predicted_point, double h);
    vec3 predictor_corrector_method(
        double h,
        vec3 (Attractor::*predictor)(double),
        vec3 (Attractor::*corrector)(const vec3 &, double));
    void find_next_point();
    void draw_big_point(size_t size, int x, int y);
    void resize_point(vec3 &point, double k);
    void move_point(vec3 &point, vec3 position);
    void rotate_point(vec3 &point, float xAngle = 0, float yAngle = 0, float zAngle = 0);
    void find_centroid();
    void draw_points(bool draw_lines, bool draw_dots, int count);

    void create_attractor(int count, int skip_count);
    void resize_attractor(double k);
    void move_attractor(vec3 position);
    void rotate_attractor(float x, float y, float z);
    void draw_attractor(bool draw_lines, bool draw_dots, bool animation, int count);
    ~Attractor();
};

#endif