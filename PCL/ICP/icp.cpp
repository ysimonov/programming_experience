#include <pcl/console/time.h>  // TicToc
#include <pcl/filters/filter.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>
#include <pcl/visualization/pcl_visualizer.h>

#include <chrono>
#include <cmath>
#include <cstdio>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <string>
#include <thread>

int start_data_row = 13;
bool next_iteration = true;

void print4x4Matrix(const Eigen::Matrix4d& matrix) {
    printf("\nRotation matrix :\n");
    printf("    | %6.3f %6.3f %6.3f | \n", matrix(0, 0), matrix(0, 1),
           matrix(0, 2));
    printf("R = | %6.3f %6.3f %6.3f | \n", matrix(1, 0), matrix(1, 1),
           matrix(1, 2));
    printf("    | %6.3f %6.3f %6.3f | \n", matrix(2, 0), matrix(2, 1),
           matrix(2, 2));
    printf("\nTranslation vector :\n");
    printf("t = < %6.3f, %6.3f, %6.3f >\n\n", matrix(0, 3), matrix(1, 3),
           matrix(2, 3));
}

// called when user presses a 'space' key and the key press triggered the event
// sets 'next_generation' bool to true
void keyboardEventOccurred(const pcl::visualization::KeyboardEvent& event,
                           void*) {
    if (event.getKeySym() == "space" && event.keyDown()) {
        next_iteration = true;
    }
}

int main(int argc, char* argv[]) {
    // check argument list
    if (argc < 2) {
        printf("Usage: \n");
        printf("\t\t%s file.ply number_of_ICP_iterations\n", argv[0]);
        PCL_ERROR("Provide one ply file.\n");
        return -1;
    }

    // default number of icp iterations
    int iterations = 1;

    // check number of iterations provided by user
    if (argc > 2) {
        iterations = atoi(argv[2]);
        if (iterations < 1) {
            PCL_ERROR("Number of initial iterations must be >= 1\n");
            return -1;
        }
    }

    // original point cloud (shared pointer)
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(
        new pcl::PointCloud<pcl::PointXYZ>);

    // transformed point cloud (shared pointer)
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_tr(
        new pcl::PointCloud<pcl::PointXYZ>);

    // result of ICP (shared pointer)
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_icp(
        new pcl::PointCloud<pcl::PointXYZ>);

    // load point cloud into 'cloud_in' and measure loading time
    pcl::console::TicToc time;
    time.tic();

    pcl::PLYReader Reader;
    auto read_status =
        Reader.read(std::string(argv[1]), *cloud_in, start_data_row);

    if (read_status < 0) {  //(pcl::io::loadPLYFile(argv[1], *cloud_in) < 0) {
        PCL_ERROR("Error loading cloud %s.\n", argv[1]);
        return -1;
    } else {
        std::cout << "\nLoaded file " << argv[1] << " (" << cloud_in->size()
                  << " points) in " << std::to_string(time.toc()) << " ms\n"
                  << std::endl;
    }

    // remove nan values (if any)
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_tmp(
        new pcl::PointCloud<pcl::PointXYZ>);

    pcl::Indices indices;
    pcl::removeNaNFromPointCloud(*cloud_in, *cloud_tmp, indices);
    *cloud_in = *cloud_tmp;
    cloud_tmp.reset();

    // matrix of homogeneous transformation (4x4)
    Eigen::Matrix4d transformation_matrix = Eigen::Matrix4d::Identity();

    // A rotation matrix (see https://en.wikipedia.org/wiki/Rotation_matrix)
    double theta = M_PI / 8;  // angle of rotation in radians
    transformation_matrix(0, 0) = std::cos(theta);
    transformation_matrix(0, 1) = -std::sin(theta);
    transformation_matrix(1, 0) = std::sin(theta);
    transformation_matrix(1, 1) = std::cos(theta);

    // A translation on x axis
    transformation_matrix(0, 3) = 0.1;

    // A translation on y axis
    transformation_matrix(1, 3) = -0.1;

    // A translation on z axis
    transformation_matrix(2, 3) = 0.1;

    // Display the transformation matrix
    std::cout << "Applying the rigid transformation to cloud_in -> cloud_icp"
              << std::endl;
    print4x4Matrix(transformation_matrix);

    // Apply transformation
    pcl::transformPointCloud(*cloud_in, *cloud_icp, transformation_matrix);
    *cloud_tr = *cloud_icp;

    // std::cout << "First 10 points of cloud_in:\n";
    // for (int i = 0; i < 10; ++i) {
    //     printf("%.3f ", cloud_in->points[i].x);
    //     printf("%.3f ", cloud_in->points[i].y);
    //     printf("%.3f\n", cloud_in->points[i].z);
    // }

    // std::cout << "\nFirst 10 points of cloud_tr:\n";
    // for (int i = 0; i < 10; ++i) {
    //     printf("%.3f ", cloud_tr->points[i].x);
    //     printf("%.3f ", cloud_tr->points[i].y);
    //     printf("%.3f\n", cloud_tr->points[i].z);
    // }

    // std::cout << "\nFirst 10 points of cloud_icp:\n";
    // for (int i = 0; i < 10; ++i) {
    //     printf("%.3f ", cloud_icp->points[i].x);
    //     printf("%.3f ", cloud_icp->points[i].y);
    //     printf("%.3f\n", cloud_icp->points[i].z);
    // }

    std::cout << std::endl << std::endl;

    // ICP
    pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;

    // number of icp iterations (per loop)
    icp.setMaximumIterations(1);

    icp.setMaxCorrespondenceDistance(0.2);

    // this is the cloud that needs to be transformed into 'cloud_in'
    icp.setInputSource(cloud_icp);

    // this is the reference point cloud
    icp.setInputTarget(cloud_in);

    // Visualize results
    pcl::visualization::PCLVisualizer viewer("Iterative Closest Point Demo");

    // vertically separated viewpoints
    int v1{0};
    int v2{1};

    viewer.createViewPort(0.0, 0.0, 0.5, 1.0, v1);
    viewer.createViewPort(0.5, 0.0, 1.0, 1.0, v2);

    // color
    float black_color_norm = 0.0;
    float text_color_norm = 1.0 - black_color_norm;
    int text_color = (int)(255 * text_color_norm);

    // Original point cloud is white
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>
        cloud_in_color(cloud_in, text_color, text_color, text_color);
    viewer.addPointCloud(cloud_in, cloud_in_color, "cloud_in_v1", v1);
    viewer.addPointCloud(cloud_in, cloud_in_color, "cloud_in_v2", v2);

    // Transformed point cloud is green
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>
        cloud_tr_color(cloud_tr, 20, 180, 20);
    viewer.addPointCloud(cloud_tr, cloud_tr_color, "cloud_tr_v1", v1);

    // ICP aligned point cloud is red
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>
        cloud_icp_color(cloud_icp, 180, 20, 20);
    viewer.addPointCloud(cloud_icp, cloud_icp_color, "cloud_icp_v2", v2);

    // Text descriptions (legends)
    viewer.addText(
        "White: Original point cloud\nGreen: Matrix transformed point cloud",
        10, 15, 16, text_color_norm, text_color_norm, text_color_norm,
        "icp_info_1", v1);

    viewer.addText("White: Original point cloud\nRed: ICP aligned point cloud",
                   10, 15, 16, text_color_norm, text_color_norm,
                   text_color_norm, "icp_info_2", v2);

    // diosplay number of iterations
    std::stringstream ss;
    ss << iterations;
    std::string iterations_count = "ICP iterations" + ss.str();
    viewer.addText(iterations_count, 10, 60, 16, text_color_norm,
                   text_color_norm, text_color_norm, "iterations_count", v2);

    // Background color
    viewer.setBackgroundColor(black_color_norm, black_color_norm,
                              black_color_norm, v1);
    viewer.setBackgroundColor(black_color_norm, black_color_norm,
                              black_color_norm, v2);

    // Camera position & Orientation
    viewer.setCameraPosition(-3.68332, 2.94092, 5.71266, 0.289847, 0.921947,
                             -0.256907, 0);

    // Plotting window's size
    viewer.setSize(900, 600);

    // Keyboard event callback
    viewer.registerKeyboardCallback(&keyboardEventOccurred, (void*)NULL);

    int iteration = 0;

    // Display results
    while (!viewer.wasStopped()) {
        viewer.spinOnce();

        // check if the user pressed 'space'
        if (next_iteration) {
            // icp
            time.tic();
            icp.align(*cloud_icp);
            std::cout << "Applied 1 ICP iteration in " << time.toc() << " ms"
                      << std::endl;

            if (icp.hasConverged()) {
                // printf("\033[11A");  // go up 11 lines in terminal
                printf("\nICP has converged, score is %+.0e\n",
                       icp.getFitnessScore());
                std::cout << "\nICP transformation " << ++iteration
                          << " : cloud_icp -> cloud_in" << std::endl;

                // this is not accurate, for educational purposes only
                transformation_matrix *=
                    icp.getFinalTransformation().cast<double>();

                print4x4Matrix(transformation_matrix);

                ss.str("");
                ss << iteration;
                std::string iterations_count = "ICP iterations = " + ss.str();
                viewer.updateText(iterations_count, 10, 60, 16, text_color_norm,
                                  text_color_norm, text_color_norm,
                                  "iterations_count");
                viewer.updatePointCloud(cloud_icp, cloud_icp_color,
                                        "cloud_icp_v2");

            } else {
                PCL_ERROR("\nICP has not converged.\n");
                return -1;
            }
            next_iteration = false;
        }
    }

    return 0;
}