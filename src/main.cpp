#include "headers/attractor.h"
#include "headers/test_methods.h"
#include <string>

bool str_is_digit(std::string str)
{
    for (auto i = 0; i != str.size(); i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    if (argv[1] == "-t")
    {
        methodTester mt;

        return 0;
    }
    bool draw_lines = false;
    bool draw_dots = true;
    bool animation = false;
    int point_count = 100000;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
        {
            draw_lines = true;
        }
        else if (strcmp(argv[i], "-d") == 0)
        {
            draw_dots = true;
        }
        else if (strcmp(argv[i], "-a") == 0)
        {
            animation = true;
        }
        else if (str_is_digit(argv[i]))
        {
            point_count = std::stoi(argv[i]);
        }
        else
        {
            std::cout << "some commands seem strange\n";
        }
    }
    Attractor atr("rk4");
    atr.create_attractor(point_count, 1);
    atr.draw_attractor(draw_lines, draw_dots, animation, point_count);
    return 0;
}