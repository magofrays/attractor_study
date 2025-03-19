#include "headers/attractor.h"

Attractor::Attractor() : centroid(0, 0, 0), pos(.5, .5, .5)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Attractor",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width, height,
                              SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

double Attractor::find_min_point()
{
    double min_point = vertices[0].distance();
    for (auto &point : vertices)
    {
        if (point.distance() < min_point)
        {
            min_point = point.distance();
        }
    }
    return min_point;
}

double Attractor::find_max_point()
{
    double max_point = vertices[0].distance();
    for (auto &point : vertices)
    {
        if (point.distance() > max_point)
        {
            max_point = point.distance();
        }
    }
    return max_point;
}

vec3 Attractor::vec_function(vec3 point)
{
    double dx = (point.z - b) * point.x - d * point.y;
    double dy = d * point.x + (point.z - b) * point.y;
    double dz = c + a * point.z - pow(point.z, 3) * 0.33333f - (point.x * point.x + point.y * point.y) * (1 + e * point.z) + f * point.z * pow(point.x, 3);
    return {dx, dy, dz};
}

vec3 Attractor::euler_method(double h)
{
    vec3 k1 = vec_function(pos);
    return k1 * h;
}

vec3 Attractor::midpoint_method(double h)
{
    vec3 k1 = vec_function(pos);
    vec3 k2 = vec_function(pos + k1 * (h * 0.5));
    return k2 * h;
}

vec3 Attractor::rk4_method(double h)
{
    vec3 k1 = vec_function(pos);
    vec3 k2 = vec_function(pos + k1 * (h * 0.5f));
    vec3 k3 = vec_function(pos + k2 * (h * 0.5f));
    vec3 k4 = vec_function(pos + k3 * h);
    return (k1 * 0.16667f + k2 * 0.33333f + k3 * 0.33333f + k4 * 0.16667f) * h;
}

void Attractor::find_next_point()
{
    pos = pos + rk4_method(dt);
}
void Attractor::create_attractor(int count, int skip_count)
{
    vertices = std::vector<vec3>(count / skip_count);
    for (int i = 0; i != count; ++i)
    {
        find_next_point();
        if (i % skip_count == 0)
            vertices[i / skip_count] = pos;
    }
}

void Attractor::draw_big_point(size_t size, int x, int y)
{
    for (int i = 0; i != size; i++)
    {
        for (int j = 0; j != size; j++)
        {
            SDL_RenderDrawPoint(renderer, x + j, y + i);
        }
    }
}

void Attractor::resize_point(vec3 &point, double k)
{
    point.x = (point.x * k);
    point.y = (point.y * k);
    point.z = (point.z * k);
}
void Attractor::move_point(vec3 &point, vec3 position)
{
    point += position;
}
void Attractor::resize_attractor(double k)
{
    for (auto &point : vertices)
    {
        resize_point(point, k);
    };
}
void Attractor::move_attractor(vec3 position)
{
    for (auto &point : vertices)
    {
        move_point(point, position);
    };
}

void Attractor::find_centroid()
{
    centroid = {0, 0, 0};
    for (auto &point : vertices)
    {
        centroid.x += point.x;
        centroid.y += point.y;
        centroid.z += point.z;
    }
    centroid.x /= vertices.size();
    centroid.y /= vertices.size();
    centroid.z /= vertices.size();
}

void Attractor::rotate_point(vec3 &point, float xAngle, float yAngle, float zAngle)
{

    float xRad = xAngle * rad;
    float yRad = yAngle * rad;
    float zRad = zAngle * rad;

    float tempY = point.y * cos(xRad) - point.z * sin(xRad);
    float tempZ = point.y * sin(xRad) + point.z * cos(xRad);
    point.y = tempY;
    point.z = tempZ;

    float tempX = point.x * cos(yRad) + point.z * sin(yRad);
    tempZ = -point.x * sin(yRad) + point.z * cos(yRad);
    point.x = tempX;
    point.z = tempZ;

    tempX = point.x * cos(zRad) - point.y * sin(zRad);
    tempY = point.x * sin(zRad) + point.y * cos(zRad);
    point.x = tempX;
    point.y = tempY;
}

void Attractor::draw_points(bool draw_lines, bool draw_dots, int count)
{
    SDL_SetRenderDrawColor(renderer, 255, 218, 185, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i != count - 1; i++)
    {
        if (draw_lines && count > 1)
        {
            SDL_SetRenderDrawColor(renderer, 65, 105, 225, 255);
            SDL_RenderDrawLine(renderer, vertices[i].x, vertices[i].y, vertices[i + 1].x, vertices[i + 1].y);
        }
        if (draw_dots)
        {
            SDL_SetRenderDrawColor(renderer, 220, 20, 60, 150);
            draw_big_point(2, vertices[i].x, vertices[i].y);
        }
    }
    SDL_Delay(10);
    SDL_RenderPresent(renderer);
}

void Attractor::rotate_attractor(float x, float y, float z)
{
    find_centroid();
    for (auto &point : vertices)
    {
        point.x -= centroid.x;
        point.y -= centroid.y;
        point.z -= centroid.z;
        rotate_point(point, x, y, z);
        point.x += centroid.x;
        point.y += centroid.y;
        point.z += centroid.z;
    }
}
void Attractor::draw_attractor(bool draw_lines, bool draw_dots, bool animation, int count)
{
    SDL_Event event;
    bool running = true;
    resize_attractor(200);
    move_attractor(vec3(width / 2, width / 2, width / 2));
    int count_start = 3;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    rotate_attractor(5, 0, 0);
                    break;
                case SDLK_DOWN:
                    rotate_attractor(-5, 0, 0);
                    break;
                case SDLK_LEFT:
                    rotate_attractor(0, -5, 0);
                    break;
                case SDLK_RIGHT:
                    rotate_attractor(0, 5, 0);
                    break;

                case SDLK_w:
                    move_attractor(vec3(0, -10, 0));
                    break;
                case SDLK_s:
                    move_attractor(vec3(0, 10, 0));
                    break;
                case SDLK_a:
                    move_attractor(vec3(-10, 0, 0));
                    break;
                case SDLK_d:
                    move_attractor(vec3(10, 0, 0));
                    break;

                case SDLK_q:
                    move_attractor(vec3(-width / 2, -width / 2, -width / 2));
                    resize_attractor(1.05);
                    move_attractor(vec3(width / 2, width / 2, width / 2));
                    break;

                case SDLK_e:
                    move_attractor(vec3(-width / 2, -width / 2, -width / 2));
                    resize_attractor(0.95);
                    move_attractor(vec3(width / 2, width / 2, width / 2));
                    break;
                }
            }
            if (event.type == SDL_MOUSEWHEEL)
            {
                if (event.wheel.y > 0)
                {
                    move_attractor(vec3(-width / 2, -width / 2, -width / 2));
                    resize_attractor(1.1);
                    move_attractor(vec3(width / 2, width / 2, width / 2));
                }
                else if (event.wheel.y < 0)
                {
                    move_attractor(vec3(-width / 2, -width / 2, -width / 2));
                    resize_attractor(0.9);
                    move_attractor(vec3(width / 2, width / 2, width / 2));
                }
            }
        }
        if (!animation || count_start >= count)
        {
            draw_points(draw_lines, draw_dots, count);
            SDL_Delay(10);
        }
        else
        {

            draw_points(draw_lines, draw_dots, count_start);
            count_start += 5;
            SDL_Delay(10);
        }
    }
}

Attractor::~Attractor()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
