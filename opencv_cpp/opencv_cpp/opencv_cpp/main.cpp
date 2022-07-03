#include <iostream>
#include <string>
#include <regex>
#include <opencv2/opencv.hpp>

int main() {
    
    std::string imagePath;
    std::cout << "Please specify image path: ";
    std::cin >> imagePath;

    // substitute single backslashes with double backslashes
    imagePath = std::regex_replace(
        imagePath, 
        std::regex(R"(\\)"), 
        R"(\\)"
    );

    // remove double or single quote
    imagePath = std::regex_replace(
        imagePath, 
        std::regex(R"([\"'])"), 
        R"()"
    );

    cv::Mat image = cv::imread(imagePath);

    if (!image.data) {
        std::cout << "No image data" << std::endl;
        return EXIT_FAILURE;
    }

    std::string displayName = "Display Image";
    cv::namedWindow(displayName, cv::WINDOW_KEEPRATIO);

    int newWidth = 700;
    int newHeight = int(double(newWidth) * image.size().height / image.size().width);
    std::cout << "New Height: " << newHeight << std::endl;

    cv::resizeWindow(displayName, newWidth, newHeight);

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return EXIT_SUCCESS;
}