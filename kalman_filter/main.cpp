#include <iostream>
#include <math.h>
#include <random>
#include <utility>
#include <thread>
#include <exception>

#include "kalman_simple.hpp"
#include "kalman_unscented.hpp"

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// returns randomly generated position and time 
// of the sinusoidal wave with spikes
std::pair<std::vector<double>, std::vector<double>> generate_dataset()
{

    double dt = 0.05;
    double period = 10.0;
    double total_time = period * 10;
    int N = int(total_time / dt);
    double noise_amp = 0.2;
    double pos_err_multiplier = 1.5;


    std::vector<double> time(N);
    std::vector<double> pos(N);

    std::cout << "Size of time: " << time.size() << std::endl;

    double exp_falloff = 0.005;

    // generate waveform
    for (int i = 0; i < N; i++)
    {
        double cur_time = i * dt;
        double cur_pos = std::cos(2*M_PI*cur_time/period) * 
                        std::exp(-exp_falloff*cur_time) + 
                        noise_amp*(2.0*(rand()/RAND_MAX)-1.0);

        time[i] = cur_time;
        pos[i] = cur_pos;
    }

    // add spikes
    const int range_from = 0;
    const int range_to = N - 1;

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);

    for (int i = 0; i < int(N / 20); i++)
    {
        auto rand_pos = distr(generator);
        pos[rand_pos] *= pos_err_multiplier;
    }

    return std::make_pair(pos, time);

}

void plot_filtered_values(std::string title, std::vector<double>& time, std::vector<double>& pos, std::vector<double>& pos_filt)
{
    plt::figure_size(700, 400);
    plt::named_plot("original waveform", time, pos, "r--");
    plt::named_plot("filtered waveform", time, pos_filt, "g--");
    plt::title(title);
    plt::legend();
    plt::save(title + ".jpg", 150);

    plt::show();
    plt::close();


    // without this line termination of matplotlib results in segmentation fault
    // see https://stackoverflow.com/questions/67533541/py-finalize-resulting-in-segmentation-fault-for-python-3-9-but-not-for-python
    plt::detail::_interpreter::kill();
}

void run_kalman_filter_test()
{
    auto data = generate_dataset();
    auto pos = std::get<0>(data);
    auto time = std::get<1>(data);

    auto kalman_filter = LinearKalman(0.05, 0.3, 1.01);

    std::vector<double> pos_filt;

    for (size_t i = 0; i < pos.size(); i++)
    {

        kalman_filter.Update(pos[i], time[i]);

        auto pos_t = kalman_filter.get_pos();
        pos_filt.push_back(pos_t);
    }

    plot_filtered_values("Linear Kalman", time, pos, pos_filt);
}


void run_unscented_kalman_filter_test()
{
    auto data = generate_dataset();
    auto pos = std::get<0>(data);
    auto time = std::get<1>(data);

    auto kalman_filter = UnscentedKalmanSimplePendulum();

    const double line_length = 3.0;

    std::vector<double> pos_filt;

    for (size_t i = 0; i < pos.size(); i++)
    {

        kalman_filter.Update(pos[i], line_length, time[i]);

        auto pos_t = kalman_filter.get_pos();
        pos_filt.push_back(pos_t);
    }

    plot_filtered_values("Unscented Kalman", time, pos, pos_filt);
}

int main()
{
    run_kalman_filter_test();
    run_unscented_kalman_filter_test();
    return EXIT_SUCCESS;
}