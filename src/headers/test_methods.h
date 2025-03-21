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

    // settings for method tester
    double dipazone_for_count = 0.1;

    methodTester();
    void test_steps();
    void test_methods_difference();
    std::map<double, int> count_points_same_dipazone(std::vector<double> &distances);

    void save_test_methods();
    void save_test_steps();
};