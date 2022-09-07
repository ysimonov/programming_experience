#include <unistd.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

/*
This program is scanning the image (some document) and
applying contour detection and homogeneous transformations
to get straight image contour.

* Compile with:
* g++ -o scanner scanner.cpp `pkg-config opencv4 --cflags --libs
*/

// Image container structure
struct ImageContainer {
   private:
    cv::Mat img_orig_;
    cv::Mat img_resized_;
    cv::Mat img_grey_;
    cv::Mat img_blur_;
    cv::Mat img_canny_;
    cv::Mat img_thres_;
    cv::Mat img_dil_;
    cv::Mat img_erode_;

   public:
    ImageContainer() {}

    ImageContainer(cv::Mat& image)
        : img_orig_(image) {}

    // **** Getter Methods **** //
    cv::Mat get_orig() {
        return img_orig_;
    }
    cv::Mat get_resized() {
        return img_resized_;
    }
    cv::Mat get_grey() {
        return img_grey_;
    }
    cv::Mat get_blur() {
        return img_blur_;
    }
    cv::Mat get_canny() {
        return img_canny_;
    }
    cv::Mat get_thres() {
        return img_thres_;
    }
    cv::Mat get_dil() {
        return img_dil_;
    }
    cv::Mat get_erode() {
        return img_erode_;
    }

    // **** Setter Methods **** //
    void set_orig(cv::Mat& img) {
        img_orig_ = img;
    }
    void set_resized(cv::Mat& img) {
        img_resized_ = img;
    }
    void set_grey(cv::Mat& img) {
        img_grey_ = img;
    }
    void set_blur(cv::Mat& img) {
        img_blur_ = img;
    }
    void set_canny(cv::Mat& img) {
        img_canny_ = img;
    }
    void set_thres(cv::Mat& img) {
        img_thres_ = img;
    }
    void set_dil(cv::Mat& img) {
        img_dil_ = img;
    }
    void set_erode(cv::Mat& img) {
        img_erode_ = img;
    }
};

void imgDisplay(cv::Mat img, std::string title = "Image") {
    if (img.rows > 0 && img.cols > 0) {
        cv::imshow(title, img);
        cv::waitKey(0);
        cv::destroyWindow(title);
    } else {
        std::cout << title << " is an empty image!\n";
    }
}

void testHoughLines(ImageContainer& img_container) {
    // Standard Hough Line Transform
    std::vector<cv::Vec2f> lines;  // will hold the results of the detection

    cv::Mat cdst = img_container.get_resized().clone();
    cv::Mat cdstP = cdst.clone();

    cv::HoughLines(img_container.get_thres(), lines, 1, CV_PI / 180, 80, 0, 0);  // runs the actual detection
    // Draw the lines
    for (int i = 0; i < lines.size(); i++) {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        cv::line(cdst, pt1, pt2, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    // Probabilistic Line Transform
    std::vector<cv::Vec4i> linesP;                                                   // will hold the results of the detection
    cv::HoughLinesP(img_container.get_thres(), linesP, 1, CV_PI / 180, 30, 40, 20);  // runs the actual detection

    // Draw the lines
    for (int i = 0; i < linesP.size(); i++) {
        cv::Vec4i l = linesP[i];
        cv::line(cdstP, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    // Show results
    cv::imshow("Source", img_container.get_resized());
    cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    cv::imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);

    // Wait and Exit
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void imgPreprocessing(ImageContainer& img_container) {
    // convert to grey scale
    cv::Mat img_grey;
    cv::cvtColor(img_container.get_resized(), img_grey, cv::COLOR_BGR2GRAY);
    img_container.set_grey(img_grey);

    imgDisplay(img_grey, "Grey");

    // apply noise removal filter
    cv::Mat img_blur;
    cv::Size blur_ksize{5, 5};
    double sigmaX = 3;
    double sigmaY = 0;

    // cv::medianBlur(img_container.get_grey(), img_blur, 5);
    // cv::GaussianBlur(img_container.get_grey(), img_blur, blur_ksize, sigmaX, sigmaY);
    cv::bilateralFilter(img_container.get_grey(), img_blur, 5, 55, 55);
    img_container.set_blur(img_blur);

    imgDisplay(img_blur, "Blur");

    // pass blur through canny
    cv::Mat img_canny;
    double threshold1 = 80;
    double threshold2 = 200;
    int apertureSize = 3;
    bool L2gradient = true;
    cv::Canny(img_container.get_blur(), img_canny, threshold1, threshold2, apertureSize, L2gradient);
    img_container.set_canny(img_canny);

    imgDisplay(img_canny, "Canny");

    // dilate
    cv::Mat img_dil;
    cv::Size dil_ksize{3, 3};
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, dil_ksize);
    cv::dilate(img_container.get_canny(), img_dil, kernel);
    img_container.set_dil(img_dil);

    imgDisplay(img_container.get_dil(), "Dilated");

    // erode
    cv::Mat img_erode;
    cv::erode(img_container.get_dil(), img_erode, kernel);
    img_container.set_erode(img_erode);

    imgDisplay(img_container.get_erode(), "Eroded");

    // set thresholded image
    img_container.set_thres(img_erode);

    // Hough Line transform contours
    testHoughLines(img_container);
}

void drawPoints(cv::Mat img, std::vector<cv::Point> points, cv::Scalar color) {
    auto img_copy = img.clone();
    for (int i = 0; i < points.size(); i++) {
        cv::circle(img_copy, points[i], 30, color, cv::FILLED);
        cv::putText(img_copy, std::to_string(i), points[i], cv::FONT_HERSHEY_PLAIN, 5, color, 5);
    }
}

std::vector<cv::Point> imgContours(ImageContainer& img_container) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    int mode = cv::RETR_TREE;                 // cv::RETR_EXTERNAL;
    int method = cv::CHAIN_APPROX_TC89_KCOS;  // cv::CHAIN_APPROX_SIMPLE;
    cv::findContours(img_container.get_thres(), contours, hierarchy, mode, method);

    size_t num_contours = contours.size();
    std::vector<std::vector<cv::Point>> con_poly(num_contours);
    std::vector<cv::Rect> bound_rect(num_contours);

    std::vector<cv::Point> biggest_contour;

    double min_area = 1000.0;
    double max_area = 1000.0;

    for (int i = 0; i < num_contours; i++) {
        std::vector<cv::Point> approx_contour;
        cv::approxPolyDP(contours[i], approx_contour, 0.03 * cv::arcLength(contours[i], true), true);

        double area = cv::contourArea(approx_contour);
        // std::cout << area << std::endl;

        if (area > 1000.0) {
            double peri = cv::arcLength(approx_contour, true);

            // check if it is a rectangular area (4 corners)
            if (area > max_area && con_poly[i].size() == 4) {
                // update biggest area
                max_area = area;
                biggest_contour = {con_poly[i][0], con_poly[i][1], con_poly[i][2], con_poly[i][3]};
            }

            // draw contours on the original image
            // cv::drawContours(img_container.get_resized().clone(), con_poly, i, cv::Scalar(255, 0, 255), 2);
        }
    }

    return biggest_contour;
}

void imgDewarp(ImageContainer& img_container) {
}

void findDocumentOutline(ImageContainer& img_container) {
    cv::Mat img_resized;
    cv::resize(img_container.get_orig(), img_resized, cv::Size(), 0.5, 0.5);
    img_container.set_resized(img_resized);

    // Preprocessing
    imgPreprocessing(img_container);

    // Get Contours
    auto contour_points = imgContours(img_container);
    std::cout << "Number of contour points " << contour_points.size() << std::endl;

    if (!contour_points.empty()) {
        drawPoints(img_container.get_resized(), contour_points, cv::Scalar(0, 0, 255));
    }

    // Warp
}

int main() {
    std::string path = "test.jpg";
    std::ifstream image_file;
    image_file.open(path);

    // inside of build folder
    if (!image_file) {
        char cwd[256];
        getcwd(cwd, 256);
        std::cout << "Current working directory:  " << cwd << std::endl;
        auto curdir = std::string(cwd);
        auto pardir = curdir.substr(0, curdir.find_last_of("/\\") + 1);
        std::cout << "Parent directory: " << pardir << std::endl;
        path = pardir + "test.jpg";
    }

    cv::Mat img_orig{cv::imread(path)};
    if (!img_orig.rows) {
        std::cout << "Could not read image! \n";
        return EXIT_FAILURE;
    }

    // plot opiginal image
    imgDisplay(img_orig, "Original Image");

    ImageContainer img_container(img_orig);

    findDocumentOutline(img_container);

    return EXIT_SUCCESS;
}