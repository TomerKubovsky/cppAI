#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <iostream>
// #include
int main() {
    // Create a blank canvas (400x400 white image)
    cv::Mat canvas = cv::Mat::zeros(400, 400, CV_8UC3);

    // Set the canvas to white
    canvas.setTo(cv::Scalar(255, 255, 255));

    // Draw a blue line (BGR color format: 255, 0, 0)
    cv::line(canvas, cv::Point(50, 50), cv::Point(350, 350), cv::Scalar(255, 0, 0), 2);

    // Display the canvas
    cv::imshow("Canvas", canvas);

    // Wait for a key press indefinitely
    cv::waitKey(0);

    return 0;
}