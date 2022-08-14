#include <iostream>
#include <string>
#include <open3d/Open3D.h>

using namespace open3d;

int main() {

    std::string bag_filename = "";

    t::io::RSBagReader bag_reader;
    bag_reader.Open(bag_filename);

    auto im_rgbd = bag_reader.NextFrame();

    while (!bag_reader.IsEOF()) {
        // process im_rgbd.depth_ and im_rgbd.color_
        im_rgbd = bag_reader.NextFrame();
    }
    
    bag_reader.Close();

    return EXIT_SUCCESS;
}