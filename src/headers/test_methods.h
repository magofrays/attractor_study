#include "attractor.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct methodTester
{

    std::vector<Attractor> attractors;
    std::vector<std::string> method_names = {
        "euler",
        "midpoint",
        "rk4",
        "dopri8",
        "pred_corr",
        "backward_euler",
        "trapezoid"};
    // common settings
    int number_of_points = 100000; // number of points predicted
    int skip_points = 1;           // if you need to skip points
    json test_methods;
    json test_steps;
    // settings for step tester
    double step_end = 0.2;
    double step_start = 0.001;
    double step_step = 0.001; // step for step
    double ideal_step = 0.01;
    int mul_step = (double(1) / step_step);
    // settings for method tester
    double dipazone_for_count = 0.01;

    methodTester();
    int gcd(int a, int b);
    void steps_test();
    void test_methods_difference();
    std::vector<int> count_points_same_dipazone(std::vector<double> &distances);

    void save_test_methods();
    void save_test_steps();
};