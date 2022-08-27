#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/visualization/cloud_viewer.h>

// dataset https://raw.github.com/PointCloudLibrary/data/master/tutorials/table_scene_lms400.pcd

using namespace pcl;

std::mutex mutex_;
auto user_data = 0ull;


void test_filter()
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZ>);

    // read point cloud data
    pcl::PCDReader reader;
    reader.read<pcl::PointXYZ> ("table_scene_lms400.pcd", *cloud);

    std::cerr << "Cloud before filtering: " << std::endl;
    std::cerr << *cloud << std::endl;

    // create filtering object
    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor_filter;
    sor_filter.setInputCloud(cloud);
    sor_filter.setMeanK(50);
    sor_filter.setStddevMulThresh(1.0);
    sor_filter.filter(*cloud_filtered);

    std::cerr << "Cloud after filtering: " << std::endl;
    std::cerr << *cloud_filtered << std::endl;

    pcl::PCDWriter writer;
    writer.write<pcl::PointXYZ> ("table_scene_lms400.pcd", *cloud_filtered, false);

    sor_filter.setNegative (true);
    sor_filter.filter (*cloud_filtered);
    writer.write<pcl::PointXYZ> ("table_scene_lms400_outliers.pcd", *cloud_filtered, false);
    

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
    //     // std::this_thread::sleep_for(std::chrono::microseconds(100000));

    //     viewer->spinOnce(100);
    //     {
    //         std::lock_guard<std::mutex> lock(mutex_);
    //         user_data++;
    //     }
    // }

}

int main()
{
    test_filter();

    return EXIT_SUCCESS;
}