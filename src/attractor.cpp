#include "headers/attractor.h"

Attractor::Attractor() : centroid(0, 0, 0), pos(.5, .5, .5)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Attractor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
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

vec3 Attractor::midpoint_method(double h)
{
    vec3 k1 = vec_function(pos);
    vec3 k2 = vec_function(pos + k1 * (h * 0.5));
    return pos + k2 * h;
}

vec3 Attractor::rk4_method(double h)
{
    vec3 k1 = vec_function(pos);
    vec3 k2 = vec_function(pos + k1 * (h * 0.5f));
    vec3 k3 = vec_function(pos + k2 * (h * 0.5f));
    vec3 k4 = vec_function(pos + k3 * h);
    return pos + (k1 * 0.16667f + k2 * 0.33333f + k3 * 0.33333f + k4 * 0.16667f) * h;
}
vec3 Attractor::dopri8_method(double h)
{
    double tol = 0.0001;
    double min_h = 0.001;
    vec3 k1 = vec_function(pos);
    vec3 k2 = vec_function(pos + k1 * (h / 18.0));
    vec3 k3 = vec_function(pos + k1 * (h / 48.0) + k2 * (h / 16.0));
    vec3 k4 = vec_function(pos + k1 * (h / 32.0) + k3 * (h * 3.0 / 32.0));
    vec3 k5 = vec_function(pos + k1 * (h * 5.0 / 16.0) - k3 * (h * 75.0 / 64.0) + k4 * (h * 75.0 / 64.0));
    vec3 k6 = vec_function(pos + k1 * (h * 3.0 / 80.0) + k4 * (h * 3.0 / 16.0) + k5 * (h * 3.0 / 20.0));
    vec3 k7 = vec_function(pos + k1 * (h * 29443841.0 / 614563906.0) + k4 * (h * 77736538.0 / 692538347.0) - k5 * (h * 28693883.0 / 1125000000.0) + k6 * (h * 23124283.0 / 1800000000.0));
    vec3 k8 = vec_function(pos + k1 * (h * 16016141.0 / 946692911.0) + k4 * (h * 61564180.0 / 158732637.0) + k5 * (h * 22789713.0 / 633445777.0) + k6 * (h * 545815736.0 / 2771057229.0) - k7 * (h * 180193667.0 / 1043307555.0));
    vec3 k9 = vec_function(pos + k1 * (h * 39632708.0 / 573591083.0) - k4 * (h * 433636366.0 / 683701615.0) - k5 * (h * 421739975.0 / 2616292301.0) + k6 * (h * 100302831.0 / 723423059.0) + k7 * (h * 790204164.0 / 839813087.0) + k8 * (h * 800635310.0 / 3783071287.0));
    vec3 k10 = vec_function(pos + k1 * (h * 246121993.0 / 1340847787.0) - k4 * (h * 37695042795.0 / 15268766246.0) - k5 * (h * 309121744.0 / 1061227803.0) - k6 * (h * 12992083.0 / 490766935.0) + k7 * (h * 6005943493.0 / 2108947869.0) + k8 * (h * 393006217.0 / 1396673457.0) + k9 * (h * 123872331.0 / 1001029789.0));
    vec3 k11 = vec_function(pos - k1 * (h * 1028468189.0 / 846180014.0) + k4 * (h * 8478235783.0 / 508512852.0) + k5 * (h * 1311729495.0 / 1432422823.0) - k6 * (h * 10304129995.0 / 1701304382.0) - k7 * (h * 48777925059.0 / 3047939560.0) + k8 * (h * 15336726248.0 / 1032824649.0) - k9 * (h * 45442868181.0 / 3398467696.0) + k10 * (h * 3065993473.0 / 597172653.0));
    vec3 k12 = vec_function(pos + k1 * (h * 185892177.0 / 718116043.0) - k4 * (h * 3185094517.0 / 667107341.0) - k5 * (h * 477755414.0 / 1098053517.0) - k6 * (h * 703635378.0 / 230739211.0) + k7 * (h * 5731566787.0 / 1027545527.0) + k8 * (h * 5232866602.0 / 850066563.0) - k9 * (h * 4093664535.0 / 808688257.0) + k10 * (h * 3962137247.0 / 1805957418.0) + k11 * (h * 65686358.0 / 487910083.0));
    vec3 k13 = vec_function(pos + k1 * (h * 403863854.0 / 491063109) - k4 * (h * 5068492393.0 / 434740067.0) - k5 * (h * 411421997.0 / 543043805.0) + k6 * (h * 652783627.0 / 914296604.0) + k7 * (h * 11173962825.0 / 925320556.0) - k8 * (h * 13158990841.0 / 6184727024.0) + k9 * (h * 3936647629.0 / 1978049680.0) - k10 * (h * 160528059.0 / 685178525.0) + k11 * (h * 248638103.0 / 1413531060.0));
    vec3 add_point1 = pos + k1 * (h * 14005451.0 / 335480064.0) - k6 * (h * 59238493.0 / 1068277825.0) + k7 * (h * 181606767.0 / 758867731.0) + k8 * (h * 561292985.0 / 797845732.0) - k9 * (h * 1041891430.0 / 1371343529.0) + k10 * (h * 760417239.0 / 1151165299.0) + k11 * (h * 118820643.0 / 751138087.0) - k12 * (h * 528747749.0 / 2220607170.0);
    vec3 add_point2 = pos + k1 * (h * 13451932.0 / 455176623.0) - k6 * (h * 808719846.0 / 976000145.0) + k7 * (h * 1757004468.0 / 5645159321.0) + k8 * (h * 656045339.0 / 265891186.0) - k9 * (h * 3867574721.0 / 1518517206.0) + k10 * (h * 465885868.0 / 322736535.0) + k11 * (h * 53011238.0 / 667516719.0) + k12 * (h * 2.0 / 45.0);

    vec3 error_vec = add_point1 - add_point2;
    double error = error_vec.distance();

    if (error < tol || h * pow(tol / error, 1.0 / 8.0) < min_h)
    {
        h = std::max(min_h, h * pow(tol / error, 1.0 / 9.0));
        return add_point1;
    }
    else
    {
        h *= pow(tol / error, 1.0 / 8.0);
        return dopri8_method(h);
    }
}

Matrix3x3 Attractor::create_jacobian(const vec3 &point, double h)
{
    Matrix3x3 J;
    J[0][0] = 1 - h * (point.z - b);
    J[0][1] = h * d;
    J[0][2] = -h * (point.x);
    J[1][0] = -h * d;
    J[1][1] = 1 - h * (point.z - b);
    J[1][2] = -h * point.y;
    J[2][0] = 2 * (point.x) * (1 + e * point.z) * h;
    J[2][1] = h * 2 * (point.y) * (1 + e * point.z);
    J[2][2] = 1 + h * e * (pow(point.x, 2) + pow(point.y, 2)) -
              a * h + pow(point.z, 2) * h - f * h * pow(point.x, 3);
    return J;
}

vec3 Attractor::euler_method(double h)
{
    vec3 k1 = vec_function(pos);
    return pos + k1 * h;
}

vec3 Attractor::backward_euler_method(const vec3 &predicted_point, double h)
{
    // euler method for first k1 = vec_function(pos + h*k1)
    vec3 k1 = vec_function(predicted_point);
    double tol = 1e-6;
    int maxIter = 50;
    for (int iter = 0; iter < maxIter; iter++)
    {
        vec3 F = k1 - vec_function(pos + k1 * h);
        if (F.distance() < tol)
        {
            break;
        }
        vec3 point = pos + k1 * h;
        Matrix3x3 J = create_jacobian(point, h);
        Matrix3x3 invJ = J.find_inverse();
        k1 = k1 - invJ * F;
    }

    return pos + k1 * h;
}

vec3 Attractor::trapezoid_method_predictor(double h)
{
    vec3 k1 = vec_function(pos);
    return pos + k1 * h * 0.5;
}

vec3 Attractor::trapezoid_method(const vec3 &predicted_point, double h)
{
    vec3 k1 = vec_function(pos);
    // k2 = vec_function(pos + (k1 + k2) * 0.5 * h)
    vec3 k2 = vec_function(predicted_point);
    double tol = 1e-6;
    int maxIter = 50;
    for (int iter = 0; iter < maxIter; iter++)
    {
        vec3 F = k2 - vec_function(pos + (k1 + k2) * 0.5 * h);
        if (F.distance() < tol)
        {
            break;
        }
        vec3 point = pos + (k1 + k2) * 0.5 * h;
        Matrix3x3 J = create_jacobian(point, h);
        Matrix3x3 invJ = J.find_inverse();
        k2 = k2 - invJ * F;
    } // approximating with Newton

    return pos + (k1 + k2) * h * 0.5;
}

vec3 Attractor::predictor_corrector_method(
    double h,
    vec3 (Attractor::*predictor)(double),
    vec3 (Attractor::*corrector)(const vec3 &, double))
{
    vec3 predicted_point = (this->*predictor)(h);
    return (this->*corrector)(predicted_point, h);
}

void Attractor::find_next_point()
{
    pos = predictor_corrector_method(
        dt,
        &Attractor::dopri8_method,
        &Attractor::trapezoid_method);
    // pos = dopri8(dt);
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
