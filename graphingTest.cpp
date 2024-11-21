#include <opencv2/opencv.hpp>
#include <cmath>

int main() {
    // Create an empty image, 800x600 pixels, with 3 color channels (BGR), initialized to white
    cv::Mat image = cv::Mat::ones(600, 800, CV_8UC3) * 255;

    // Scaling factors
    int width = image.cols;
    int height = image.rows;
    double scale_x = 0.01;  // Adjust to scale the x axis
    double scale_y = 0.01;  // Adjust to scale the y axis

    // Draw the x and y axes
    cv::line(image, cv::Point(0, height / 2), cv::Point(width, height / 2), cv::Scalar(0, 0, 0), 1);
    cv::line(image, cv::Point(width / 2, 0), cv::Point(width / 2, height), cv::Scalar(0, 0, 0), 1);

    // Plot the function y = x^2
    for (int x_pixel = 0; x_pixel < width; ++x_pixel) {
        double x = (x_pixel - width / 2) * scale_x;  // Convert pixel to coordinate system
        double y = x * x;

        int y_pixel = height / 2 - static_cast<int>(y / scale_y);  // Convert y coordinate to pixel

        // Check if y_pixel is within the image bounds
        if (y_pixel >= 0 && y_pixel < height) {
            image.at<cv::Vec3b>(y_pixel, x_pixel) = cv::Vec3b(0, 0, 255);  // Plot the point in red
        }
    }

    // Display the image in a window
    cv::imshow("Plot y = x^2", image);

    // Wait for a key press indefinitely or for a specific amount of time in milliseconds
    cv::waitKey(0);

    return 0;
}