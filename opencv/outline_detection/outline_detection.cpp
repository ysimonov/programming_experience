#include <iostream>
#include <opencv2/opencv.hpp>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

// compillation:
// g++ -o outline_detection outline_detection.cpp `pkg-config opencv4 --cflags --libs`

class ImageProcessor {
   public:
    cv::Mat original_image;
    cv::Mat rescaled_image;
    cv::Mat grey_scale_image;
    cv::Mat contour_image;

    ImageProcessor(){};

    ImageProcessor(cv::Mat& image)
        : original_image(image){};

    void ReadImage(std::string image_path);
    void RescaleImage(cv::Mat& input_imag, int scale_percent, int interpolation);
    void CreateGreyScaleImage(cv::Mat& input_imag);
    void DisplayImage(cv::Mat& input_imag, std::string title);
    void FindImageContours(cv::Mat& input_imag);
};

/*
 * Reads image from specified image path
 * Throws runtime_error if image has no data
 */
void ImageProcessor::ReadImage(std::string image_path) {
    // normalize path (replace double back slash with single forward slash)
    std::string image_path_norm = std::regex_replace(
        image_path,
        std::regex(R"(\\)"),
        R"(/)");

    // remove double or single quote if included in path
    image_path_norm = std::regex_replace(
        image_path_norm,
        std::regex(R"([\"'])"),
        R"()");

    original_image = cv::imread(image_path_norm);

    if (!original_image.data)
        throw std::runtime_error("No Image Data!");

    // std::cout << "Original Height: " << original_image.rows << " Width: " << original_image.cols << std::endl;
}

/*
 * Rescales input_image (cv::Mat) based on scale_percent (int) (1-100)
 * interpolation (enum cv::InterpolationFlags, int) one of
 * cv::INTER_LINEAR, cv::INTER_NEAREST, cv::INTER_AREA, cv::INTER_CUBIC,  cv::INTER_LANCZOS4, etc.
 */
void ImageProcessor::RescaleImage(
    cv::Mat& input_image,
    int scale_percent,
    int interpolation = cv::INTER_AREA) {
    // get original image dimensions
    int width = input_image.cols;
    int height = input_image.rows;

    // std::cout << "Height: " << height << " Width: " << width << std::endl;

    // find new image dimensions
    double scale_factor = scale_percent / 100.0;
    int rescaled_width = int(width * scale_factor);
    int rescaled_height = int(width * scale_factor);

    // resize image using Lancsoz interpolation
    cv::resize(
        input_image, rescaled_image,
        cv::Size(rescaled_width, rescaled_height),
        interpolation);
}

/*
 * Takes default colorful image read by OpenCV (BGR) and create a grey scale image.
 */
void ImageProcessor::CreateGreyScaleImage(cv::Mat& input_image) {
    cv::cvtColor(input_image, grey_scale_image, cv::COLOR_BGR2GRAY);
}

/*
 * Displays Images
 * If key '0' is pressed, it will destroy window and exit the program
 */
void ImageProcessor::DisplayImage(cv::Mat& input_image, std::string title = "Image") {
    cv::imshow(title, input_image);
    char exit_key = '0';
    int key = cv::waitKey(0);
    if (key == exit_key) {
        // destroy all active windows
        cv::destroyAllWindows();
        exit(0);
    }
    cv::destroyWindow(title);
}

/*
 * Applies contours on the input image
 */
void ImageProcessor::FindImageContours(cv::Mat& input_imag) {
    // convert RGB to Grey Scale
    if (input_imag.channels() > 1) {
        this->CreateGreyScaleImage(input_imag);
    }

    // apply binary thresholding
    cv::Mat thresholded;
    cv::threshold(grey_scale_image, thresholded, 230, 255, cv::THRESH_OTSU);

    // detect contours on binary image
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresholded, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

    // draw contours on the copy of the original image
    if (input_imag.cols == original_image.cols) {
        contour_image = original_image.clone();
    } else {
        contour_image = rescaled_image.clone();
    }
    cv::drawContours(contour_image, contours, -1, cv::Scalar(0, 255, 0), 2);
}

int main() {
    auto image_processor = ImageProcessor();

    std::string image_path;
    std::cout << "Please specify image path: ";
    std::cin >> image_path;

    image_processor.ReadImage(image_path);
    image_processor.RescaleImage(image_processor.original_image, 50);
    image_processor.CreateGreyScaleImage(image_processor.rescaled_image);
    image_processor.DisplayImage(image_processor.grey_scale_image, "Grey Image");
    image_processor.FindImageContours(image_processor.original_image);
    image_processor.DisplayImage(image_processor.contour_image, "Image with contours");

    return EXIT_SUCCESS;
}