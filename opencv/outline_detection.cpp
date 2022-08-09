#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>

#include <opencv2/opencv.hpp>

// compillation:
// g++ -o outline_detection outline_detection.cpp `pkg-config opencv4 --cflags --libs`

class ImageProcessor {
    public:
        cv::Mat original_image;
        cv::Mat rescaled_image;
        cv::Mat grey_scale_image;

        ImageProcessor() 
        {};

        ImageProcessor(cv::Mat& image) 
            : original_image(image)
        {};

        void ReadImage(std::string image_path);
        void RescaleImage(cv::Mat& input_imag, int scale_percent, int interpolation);
        void CreateGreyScaleImage(cv::Mat& input_imag);
        void DisplayImage(cv::Mat& input_imag, std::string title);
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
        R"(/)"
    );

    // remove double or single quote if included in path
    image_path_norm = std::regex_replace(
        image_path_norm,
		std::regex(R"([\"'])"),
		R"()"
    );

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
    int interpolation = cv::INTER_AREA
) {

    // get original image dimensions
    int width = input_image.cols;
    int height = input_image.rows;

    std::cout << "Height: " << height << " Width: " << width << std::endl;

    // find new image dimensions
    double scale_factor = scale_percent / 100.0;
    int rescaled_width = int(width * scale_factor);
    int rescaled_height = int(width * scale_factor);

    // resize image using Lancsoz interpolation
    cv::resize(
        input_image, rescaled_image, 
        cv::Size(rescaled_width, rescaled_height), 
        interpolation
    );
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


int main() {
    
    auto image_processor = ImageProcessor();

    std::string image_path;
    std::cout << "Please specify image path: ";
    std::cin >> image_path;

    try {
        image_processor.ReadImage(image_path);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    try {
        image_processor.RescaleImage(image_processor.original_image, 50);
    } catch (cv::Exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE; 
    }
    
    try {
        image_processor.CreateGreyScaleImage(image_processor.rescaled_image);
    } catch (cv::Exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE; 
    }

    try {
        image_processor.DisplayImage(image_processor.grey_scale_image, "Grey Image");
    } catch (cv::Exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE; 
    }
    
    return EXIT_SUCCESS;
}