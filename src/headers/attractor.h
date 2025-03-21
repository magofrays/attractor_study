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
    bool gui;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::vector<vec3> vertices;
    vec3 centroid;
    std::vector<std::string> method_names = {
        "euler",
        "midpoint",
        "rk4",
        "dopri8",
        "pred_corr",
        "backward_euler",
        "trapezoid"};
    vec3 (Attractor::*method)(double);
    Attractor(std::string method_name = "dopri8", bool gui = true);
    void start_gui();
    // instruments for methods
    void set_method(std::string method_name);
    void set_step(double h);
    Matrix3x3 create_jacobian(const vec3 &point, double h);
    vec3 backward_euler_corrector(const vec3 &predicted_point, double h);
    vec3 trapezoid_predictor(double h);
    vec3 trapezoid_corrector(const vec3 &predicted_point, double h);
    vec3 predictor_corrector(
        double h,
        vec3 (Attractor::*predictor)(double),
        vec3 (Attractor::*corrector)(const vec3 &, double));

    // methods
    vec3 vec_function(vec3 point);
    vec3 midpoint_method(double h);
    vec3 rk4_method(double h);
    vec3 dopri8_method(double h);
    vec3 euler_method(double h);
    vec3 backward_euler_method(double h);
    vec3 trapezoid_method(double h);
    vec3 predictor_corrector_method(double h);

    // point transform
    void find_next_point();
    void draw_big_point(size_t size, int x, int y);
    void resize_point(vec3 &point, double k);
    void move_point(vec3 &point, vec3 position);
    void rotate_point(vec3 &point, float xAngle = 0, float yAngle = 0, float zAngle = 0);
    void find_centroid();
    void draw_points(bool draw_lines, bool draw_dots, int count);

    // attractor transform
    void create_attractor(int count, int skip_count);
    void resize_attractor(double k);
    void move_attractor(vec3 position);
    void rotate_attractor(float x, float y, float z);
    void draw_attractor(bool draw_lines, bool draw_dots, bool animation, int count);
    ~Attractor();
};

#endif