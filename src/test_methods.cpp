#include "headers/test_methods.h"
#include <map>
#include <fstream>

methodTester::methodTester()
{
    for (auto &method : method_names)
    {
        Attractor attractor(method);
        attractor.create_attractor(number_of_points, skip_points);
        attractors.push_back(attractor);
    }
}

void methodTester::test_steps()
{
}

void methodTester::test_methods_difference()
{
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

std::map<double, int> methodTester::count_points_same_dipazone(std::vector<double> &distances)
{
    std::map<double, int> count_same_dipazone;
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
            count_same_dipazone[dipazone - dipazone_for_count] = count;
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
        file << test_methods.dump(4);
        file.close();
        std::cout << "JSON saved to ../test_steps.json" << std::endl;
    }
    else
    {
        std::cerr << "Failed to open file for writing." << std::endl;
    }
}
