#ifndef KALMAN_SIMPLE
#define KALMAN_SIMPLE

#include <eigen3/Eigen/Dense>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

using Eigen::EigenBase;

template <typename Derived> void print_shape(const EigenBase<Derived> &x)
{
    std::cout << "(" << x.rows() << ", " << x.cols() << ")";
}

/*
 * Inputs: proc_acc_err, meas_pos_err, alpha
 * alpha is the memory decay coefficient
 * the higher alpha is, the more less kalman filter is affeted by previous measurements
 * See Equations: https://www.kalmanfilter.net/multiSummary.html
 */
class LinearKalman
{
  private:
    double proc_acc_err_;
    double meas_pos_err_;
    double alpha_;

    double dt_{0.05};

    double var_acc_;
    double var_pos_;

    double pos_;
    double vel_;
    double acc_;

    double R;
    Eigen::Matrix<double, 3, 1> x{{0.0}, {0.0}, {0.0}};
    Eigen::Matrix<double, 1, 3> H{1.0, 0.0, 0.0};
    Eigen::Matrix<double, 3, 1> HT = H.transpose();
    Eigen::Matrix<double, 3, 1> K{{0.0}, {0.0}, {0.0}};
    Eigen::Matrix<double, 3, 3> P{{100.0, 0.0, 0.0}, {0.0, 100.0, 0.0}, {0.0, 0.0, 100.0}};
    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();

    double t_prev_;
    bool time_start_set_ = false;

  public:
    // constructor with initializations
    LinearKalman(double proc_acc_err = 0.1, double meas_pos_err = 0.1, double alpha = 1.0)
        : proc_acc_err_(proc_acc_err), meas_pos_err_(std::abs(meas_pos_err)), alpha_(std::abs(alpha))
    {
        var_acc_ = std::pow(proc_acc_err, 2);
        var_pos_ = std::pow(meas_pos_err, 2);
        R = var_pos_;
    }

    void Update(double meas, double meas_time)
    {

        // set start time
        if (time_start_set_)
        {
            dt_ = meas_time - t_prev_;
        }
        else
        {
            time_start_set_ = true;
        }

        // update previous time
        t_prev_ = meas_time;

        auto dt = dt_;
        auto dt2 = std::pow(dt, 2);
        auto dt3 = std::pow(dt, 3);
        auto dt4 = std::pow(dt, 4);

        // Covariance matrix
        Eigen::Matrix<double, 3, 3> cov_mat{
            {0.25 * dt4, 0.5 * dt3, 0.5 * dt2}, {0.5 * dt3, dt2, dt}, {0.5 * dt2, dt, 1.0}};

        // Process noise matrix
        auto Q = var_acc_ * cov_mat;

        // State transition matrix
        Eigen::Matrix<double, 3, 3> F{{1.0, dt, 0.5 * dt2}, {0.0, 1.0, dt}, {0.0, 0.0, 1.0}};

        // Updade estimation uncertainty
        P = F * P * F.transpose() * std::pow(alpha_, 2) + Q;

        // Extrapolate the state vector
        x = F * x;

        // Update Kalman Gain
        auto PHT = P * HT;             // [3, 3] * [3, 1] = [3, 1]
        auto HPHT = H * PHT;           // [1, 3] * [3, 1] = [1, 1]
        auto S = HPHT.coeff(0, 0) + R; // scalar

        // Kalman Gain
        // Note that denominator is constant, hence no inversion required
        K = P * HT * (1.0 / S);

        // Update estimation with the measurement
        auto xp = H * x;
        auto dx = meas - xp.coeff(0, 0);

        // Update the estimate uncertainty
        auto IKH = I - (K * H);
        auto KRKT = R * K * K.transpose();
        P = IKH * P * IKH.transpose() + KRKT;

        // update state vector
        x += K * dx;

        // update state variables
        pos_ = x.coeff(0, 0);
        vel_ = x.coeff(1, 0);
        acc_ = x.coeff(2, 0);
    }

    double get_pos()
    {
        return pos_;
    }

    double get_vel()
    {
        return vel_;
    }

    double get_acc()
    {
        return acc_;
    }

    std::tuple<double, double> get_pos_vel()
    {
        return std::make_tuple(pos_, vel_);
    }

    std::tuple<double, double, double> get_pos_vel_acc()
    {
        return std::make_tuple(pos_, vel_, acc_);
    }
};

#endif