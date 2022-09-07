#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

// dataset https://raw.github.com/PointCloudLibrary/data/master/tutorials/table_scene_lms400.pcd

using namespace pcl;

std::mutex mutex_;
auto user_data = 0ull;

void test_filter() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);

    // read point cloud data
    pcl::PCDReader reader;
    reader.read<pcl::PointXYZ>("table_scene_lms400.pcd", *cloud);

    std::cerr << "Cloud before filtering: " << std::endl;
    std::cerr << *cloud << std::endl;

    // create filtering object
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor_filter;
    sor_filter.setInputCloud(cloud);
    sor_filter.setMeanK(30);
    sor_filter.setStddevMulThresh(0.2);
    sor_filter.filter(*cloud_filtered);

    std::cerr << "Cloud after filtering: " << std::endl;
    std::cerr << *cloud_filtered << std::endl;

    pcl::PCDWriter writer;

    sor_filter.setNegative(true);
    sor_filter.filter(*cloud_filtered);
    writer.write<pcl::PointXYZ>("table_scene_lms400_outliers.pcd", *cloud_filtered, false);

    //! Segmentation fault when plotting ??

    // // Visualize point cloud
    // pcl::visualization::PCLVisualizer::Ptr viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
    // viewer->setBackgroundColor (0, 0, 0);
    // viewer->addPointCloud<pcl::PointXYZ> (cloud, "sample cloud");
    // viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
    // viewer->addCoordinateSystem (1.0);
    // viewer->initCameraParameters ();

    // while (!viewer->wasStopped())
    // {
    //     std::this_thread::sleep_for(std::chrono::microseconds(100000));
    //     viewer->spin();
    // }

    // Visualize point cloud
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addPointCloud<pcl::PointXYZ>(cloud_filtered, "filtered cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "filtered cloud");
    viewer->addCoordinateSystem(1.0);
    viewer->initCameraParameters();

    while (!viewer->wasStopped()) {
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        viewer->spin();
    }
}

int main() {
    test_filter();

    return EXIT_SUCCESS;
}