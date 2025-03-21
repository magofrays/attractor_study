#include "headers/test_methods.h"
#include <map>
#include <fstream>

methodTester::methodTester()
{
}

int methodTester::gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void methodTester::steps_test()
{
    test_steps["step start"] = step_start;
    test_steps["step end"] = step_end;
    test_steps["step step"] = step_step;
    test_steps["ideal step"] = ideal_step;
    for (int i = 0; i != attractors.size(); i++)
    {
        std::cout << i << "\n";
        json method;
        Attractor main_attractor(method_names[i], false);
        main_attractor.set_step(ideal_step);
        main_attractor.create_attractor(number_of_points, skip_points);

        std::vector<double> max_distance_per_step;
        for (double h = step_start; h < step_end; h += step_step)
        {
            attractors[i].set_method(method_names[i]);
            attractors[i].set_step(h);
            attractors[i].create_attractor(number_of_points, skip_points);
            int decimal_h = mul_step * h;
            int decimal_ideal_step = mul_step * ideal_step;
            int iter_count = decimal_h * decimal_ideal_step / gcd(decimal_h, decimal_ideal_step);
            std::vector<double> distances;
            for (int j = 0; j != number_of_points; j++)
            {

                if (j % iter_count == 0)
                {
                    distances.push_back((main_attractor.vertices[j] - attractors[i].vertices[j]).distance());
                }
            }
            double max_distance = *std::max_element(distances.begin(), distances.end());
            max_distance_per_step.push_back(max_distance);
        }
        method["max distance per step"] = max_distance_per_step;
        test_steps[method_names[i]] = method;
    }
}

void methodTester::test_methods_difference()
{
    for (auto &method : method_names)
    {
        Attractor attractor(method, false);
        attractor.create_attractor(number_of_points, skip_points);
        attractors.push_back(attractor);
    }

    for (int i = 0; i != attractors.size(); i++)
    {
        for (int j = i + 1; j != attractors.size(); j++)
        {
            std::vector<double> distances;
            for (int t = 0; t != number_of_points; t++)
            {
                distances.push_back((attractors[i].vertices[t] - attractors[j].vertices[t]).distance());
            }
            test_methods[method_names[i] + " " + method_names[j]] = count_points_same_dipazone(distances);
        }
    }
}

std::vector<int> methodTester::count_points_same_dipazone(std::vector<double> &distances)
{
    std::vector<int> count_same_dipazone;
    std::sort(distances.begin(), distances.end());
    double dipazone = dipazone_for_count;
    int count = 0;
    for (int i = 0; i != distances.size(); i++)
    {
        if (distances[i] < dipazone)
        {
            count++;
        }
        else
        {
            dipazone += dipazone_for_count;
            count_same_dipazone.push_back(count);
            count = 0;
        }
    }
    return count_same_dipazone;
}

void methodTester::save_test_methods()
{
    std::ofstream file("../test_methods.json");
    if (file.is_open())
    {
        file << test_methods.dump(4);
        file.close();
        std::cout << "JSON saved to ../test_methods.json" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
}

void methodTester::save_test_steps()
{
    std::ofstream file("../test_steps.json");
    if (file.is_open())
    {
        file << test_steps.dump(4);
        file.close();
        std::cout << "JSON saved to ../test_steps.json" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
}
