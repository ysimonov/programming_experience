#ifndef KALMAN_UNSCENTED
#define KALMAN_UNSCENTED

#include <iostream>
#include <vector>
#include <numeric>
#include <tuple>
#include <string>

#include <eigen3/Eigen/Dense>
#include <eigen3/unsupported/Eigen/MatrixFunctions>

using Eigen::EigenBase;

template <typename Derived>
void print_shape(const EigenBase<Derived>& x, std::string title)
{
    std::cout << "Shape of " + title << "(" << x.rows() << ", " << x.cols() << ")";
}


class UnscentedKalmanSimplePendulum 
{

    public:
        UnscentedKalmanSimplePendulum(
            Eigen::Matrix<double, 2, 2> Qmat = Eigen::Matrix2d{{0.05, 0.01}, {0.05, 0.01}},
            double Rval = 0.1
        )
        : Q(Qmat), R(Rval)
        {
            // Weights
            wc = Eigen::RowVectorXd::Ones(num_sigmas) / (2.0 * (double(n) + lambda_));
            wm = Eigen::RowVectorXd::Ones(num_sigmas) / (2.0 * (double(n) + lambda_));
            wc[0] = lambda_ / (double(n) + lambda_) + (1.0 - std::pow(alpha_, 2) + beta_);
            wm[0] = lambda_ / (double(n) + lambda_);

            // Covariance matrices
            P << 100.0, 0.0,
                0.0, 100.0;

            Pp << 100.0, 0.0,
                0.0, 100.0;

            // Create sigma points
            sigmas_f = Eigen::MatrixXd(2*n+1, n).setZero();

            // P must be diagonal!
            Eigen::LDLT<Eigen::MatrixXd> LDLT; 

            // Robust cholesky decomposition, A = (L * D * L^T), D - diagonal matrix
            // Eigen::MatrixXd L = LDLT.compute((double(n) + lambda_) * P).matrixL();

        
            Eigen::MatrixXcd Uc =  Eigen::MatrixSquareRootReturnValue<Eigen::MatrixXcd>(P * (double(n) + lambda_)).eval(); //P * (double(n) + lambda_).sqrt();
            Eigen::MatrixXd U = Uc.real();

            // Set sigma points
            sigmas_f(0, Eigen::all) = x;
            for (int k = 0; k < n; k++)
            {
                sigmas_f(k + 1, Eigen::all) = x + U(k, Eigen::all);
                sigmas_f(n + k + 1, Eigen::all) = x - U(k, Eigen::all);
            }

            // Observation matrix
            H = Eigen::Matrix2Xd({{1.0}, {0.0}});

        }

    private:
        Eigen::Matrix<double, 2, 2> Q;
        double R;

        double dt = 0.05; // measurement interval time, s
        const double g = 9.81; // gravitational constant, m/s^2
        const double min_line_len = 1.0; // length of the pendulum, m
        const int n = 2; // number of dimensions in the state vector, 2 because skew and skew dot

        const int dim_x = 2; // number of state variables in a filter
        const int dim_z = 1; // number of input measurements (angle of pendulum)
        const int num_sigmas = 2 * n + 1; // number of sigma points

        double alpha_ = 0.1; // related to distance between sigma points
        double beta_ = 2.0; // optimal for normal distribution
        double kappa_ = 3.0 - double(n);
        double lambda_ = std::pow(alpha_, 2) * (n + kappa_) - n;

        // create weights
        // 5 = 2 * 2 + 1
        Eigen::RowVectorXd wc;
        Eigen::RowVectorXd wm;

        Eigen::Matrix2d P;
        Eigen::Matrix2d Pp;

        Eigen::RowVector2d x = Eigen::RowVector2d::Zero();
        Eigen::RowVector2d xp = Eigen::RowVector2d::Zero();
        
        Eigen::MatrixXd sigmas_f;

        Eigen::MatrixXd H;
        
        double sway_dot_dot_rad = 0.0;
        double line_len = 3.0;

        double t_prev_ = 0.0;
        bool time_start_set_ = false;

    public:

        // recalculate sigma points for a new state vector
        void UpdateSigmaPoints(Eigen::RowVector2d x_new)
        {
            Eigen::LDLT<Eigen::MatrixXd> LDLT; 

            // Robust cholesky decomposition, A = (L * D * L^T), D - diagonal matrix
            // Eigen::MatrixXd L = LDLT.compute((double(n) + lambda_) * P).matrixL();
        
            Eigen::MatrixXcd Uc = Eigen::MatrixSquareRootReturnValue<Eigen::MatrixXcd>(P * (double(n) + lambda_)).eval(); //Eigen::sqrt((double(n) + lambda_) * P);
            Eigen::MatrixXd U = Uc.real();

            // Set sigma points
            sigmas_f(0, Eigen::all) = x_new;
            for (int k = 0; k < n; k++)
            {
                sigmas_f(k + 1, Eigen::all) = x_new + U(k, Eigen::all);
                sigmas_f(n + k + 1, Eigen::all) = x_new - U(k, Eigen::all);
            }
            
        }

        // predict new state
        void Predict(double line_length, double dt)
        {
            double sway_new = x[0] + dt * x[1];
            double sway_dot_new = x[1] - dt / line_len * g * std::sin(x[0]);

            auto x_new = Eigen::RowVector2d({sway_new, sway_dot_new});

            UpdateSigmaPoints(x_new);

            double sway_dot_dot_rad = (sway_dot_new - x[1]) / dt;

            xp.setZero();
            for (int i = 0; i < num_sigmas; i++)
            {
                xp += wm[i] * sigmas_f(i, Eigen::all);
            }

            Pp.setZero();
            for (int i = 0; i < num_sigmas; i++)
            {
                auto y = sigmas_f(i, Eigen::all) - xp;
                // outer product - column vector times row vector (y)
                Pp += wc[i] * (y.transpose() * y); 
            }

            Pp += Q;
        }

        void Update(double sway_rad, double line_length, double meas_time)
        {

            // set start time
            if (time_start_set_) { dt = meas_time - t_prev_; }
            else { time_start_set_ = true; }

            // update previous time
            t_prev_ = meas_time;
    
            line_len = line_length;

            Predict(line_length, dt);

            // transform sigma points into measurement space [2 * n + 1, 1]
            auto sigmas_h = sigmas_f * H;   

            // mean of sigmas
            auto zp = (wm * sigmas_h).coeff(0);

            // covariance of prediction
            Eigen::MatrixXd Pz(dim_z, dim_z);   // [1, 1]
            Pz.setZero();

            for (int i = 0; i < num_sigmas; i++)
            {
                double temp = sigmas_h.coeff(i) - zp;
                Pz.array() += wc[i] * temp * temp;
            }
            Pz.array() += R;

            // compute cross covariance of the state of measurements
            Eigen::MatrixXd Pxz(dim_x, dim_z);  // [2, 1]
            Pxz.setZero();

            for (int i = 0; i < num_sigmas; i++)
            {
                auto temp2 = sigmas_f(i, Eigen::all) - xp;
                Pxz += wc[i] * (temp2.transpose() * temp2);
            }

            // Kalman Gain
            auto K = Pxz * Pz.inverse();

            print_shape(K, "K");

            // measurement residual
            double residual = sway_rad - zp;

            // compute new state estimate using the residual and kalman gain
            x[0] = xp[0] + K.coeff(0) * residual;
            x[1] = xp[1] + K.coeff(1) * residual;

            // compute new covariance matrix
            P = Pp - (K * Pz * K.transpose());

        }

        double get_pos()
        {
            return x[0];
        }

        double get_vel()
        {
            return x[1];
        }
        
        double get_acc()
        {
            return sway_dot_dot_rad;
        }

        std::tuple<double, double> get_pos_vel()
        {
            return std::make_tuple(x[0], x[1]);
        }

        std::tuple<double, double, double> get_pos_vel_acc()
        {
            return std::make_tuple(x[0], x[1], sway_dot_dot_rad);
        }
};

#endif