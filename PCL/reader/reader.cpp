#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>

#include <iostream>
#include <string>
#include <thread>

std::string PATH = "/home/simon/programming_experience/lidar_data/bunny/data/bun000.ply";

int main() {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PLYReader Reader;
    Reader.read(PATH, *cloud);

    std::cerr << "Cloud data: " << std::endl;
    std::cerr << *cloud << std::endl;

    // Visualize point cloud
    pcl::visualization::PCLVisualizer::Ptr viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    viewer->addPointCloud<pcl::PointXYZ>(cloud, "sample cloud");
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
    viewer->addCoordinateSystem(1.0);
    viewer->initCameraParameters();

    while (!viewer->wasStopped()) {
        std::this_thread::sleep_for(std::chrono::microseconds(100000));
        viewer->spin();
    }

    return 0;
}
