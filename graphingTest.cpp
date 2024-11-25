#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include "neurelNetworkClass.h"
#include <atomic>
#include <thread>

template<typename  Type>
void drawFunc(std::function<Type(Type)>& func, cv::Point2d& initPoint, cv::Mat& img, const int size, const Type spacing, const Type funcInitPoint, const cv::Scalar& color, const int thickness, const int lineType = cv::LINE_8, const Type width = 1, const Type height = 1) //rdraws function func with size size spacing is the distance between two points (smaller spacing the smoother func is) func init point is the starting point of a function eg if you did 0 it would go from 0 to size but if you did size/2 then it would be centerd kinda hard to explain also cv::point is the intial point the thing starts at
{
    Type prevPointY = func(funcInitPoint);
    Type currentPointY = prevPointY;
    const int initPointY = initPoint.y;
    cv::Point2d prevPoint = initPoint;
    prevPoint.y -= static_cast<int>(prevPointY) * height;
    cv::Point2d currentPoint = prevPoint;
    for (double i = funcInitPoint + spacing; i < size + funcInitPoint; i+=spacing) //use +=spacing bc if you have point at 10 and 0.1 spacing you want to get to 10 but go 0.1 each time also add spacing to 0 bc we start at prev point which is initaly the first ones func thing so ye hard to explain
    {
        // std::cout << i << std::endl;
        currentPointY = func(i) * height;
        currentPoint.y = initPointY - static_cast<int>(currentPointY);
        currentPoint.x += spacing * width;
        cv::line(img, prevPoint, currentPoint, color, thickness, lineType);
        prevPointY = currentPointY;
        prevPoint = currentPoint;
    }
}

// template<typename  Type>
// Type funcToLearn(Type input)
// {
    // return
// }

template<typename  Type>
Type funcToLearn(Type input)
{
    // double a = input-3;
    // double b = std::pow(a,2);
    // double c = std::sqrt(12 - b);
    // return 2+c;
    // std::cout << std::sin(input) << std::endl;
    // return 1 - (std::pow(input, 2)/2) + (std::pow(input, 4)/24);
    // return 1 - (std::pow(input, 2)/2);
    // return std::sin(input);
    // return (input*input*input*input)/24;
    return std::log(input);

    // return std::pow(input, 3);
    // if (input < 0)
    // {
        // return std::pow(input, 2);
    // } else
    // {
        // return sin(input) * std::exp(-input);
    // }
    // return 1 - (std::pow(input, 2.0) / 2) + (std::pow(input,4.0)/24);

}

int main()
{
    // std::cout << funcToLearn(-1000);
    using namespace NeurelNetwork::ArrayUtils;
    const int width = 1920;
    const int height = 1080;
    const int margin = 10;
    // const int size = width - 2 * margin;
    const int size = 20;
    const double spacing = 0.5;
    const double funcInitPoint = 1;
    // const double spacing = 1;
    constexpr int inputsSize = (size / spacing) + funcInitPoint;

    double* inputsPtr = new double[inputsSize];
    double* corrOutputsPtr = new double[inputsSize]; //same amount of outputs as inputs

    int k = 0;
    // for (int i = -inputsSize/2; i < inputsSize/2; i++)
    for (double i = funcInitPoint; i <= size + funcInitPoint; i+=spacing)
    {
        // const double currentIndex = i * spacing;
        inputsPtr[k] = i;
        corrOutputsPtr[k] = funcToLearn(i);
        k++;
    }
    std::cout << inputsPtr[inputsSize-1] << " " << inputsPtr[inputsSize] << std::endl;

    Array<double> inputsArr(inputsPtr, inputsSize, 1);
    Array<double> corrOutputsArr(corrOutputsPtr, inputsSize, 1);

    Array<double> outputsArr;

    const int layers[] = {1, 64, 64, 1};
    NeurelNetwork::neuralnetwork<double> network(layers, 4, "leakyRelu", "none", -0.005, "adam");
    // NeurelNetwork::neuralnetwork<double> network(layers, 4, "relu", "none", 0);

    double fWidth = 64;
    double fHeight = 96;
    // cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
    cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
    cv::Point2d initPoint((width/2) - (size * fWidth)/2, 980);
    // cv::Point2d initPoint(0, height - margin);

    std::function<double(double)> func = [&network](double input)
    {
        double* temp = new double;
        *(temp) = input;
        Array<double> inputArrTemp(temp, 1, 1);
        return network.forwards(inputArrTemp).getPtr()[0];
    };

    std::function<double(double)> funcToLearnInput = funcToLearn<double>;

    cv::imshow("img", img);
    const int batchTime = 1000000;
    bool tBreak(false);
    // std::thread listener(keyListener, batchTime);
    // for (int i = 0; i < batchTime; i++)
    int i = 0;
    while (true)
    {
        // if (i % (batchTime / 1000) == 0 && i != 0)
        if (i % 10 == 0 && i != 0)
        {
            cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
            // drawFunc<double>(funcToLearnInput, initPoint, img, size, spacing, -size/2, cv::Scalar(255, 0, 0), 1, cv::LINE_8, fWidth, fHeight);
            drawFunc<double>(funcToLearnInput, initPoint, img, size, spacing, funcInitPoint, cv::Scalar(255, 0, 0), 1, cv::LINE_8, fWidth, fHeight);
            // drawFunc<double>(func, initPoint, img, size, spacing, -size/2, cv::Scalar(0, 255, 0), 1, cv::LINE_8, fWidth, fHeight);
            drawFunc<double>(func, initPoint, img, size, spacing, funcInitPoint, cv::Scalar(0, 255, 0), 1, cv::LINE_8, fWidth, fHeight);
            // drawFunc<double>(funcToLearnInput, initPoint, img, 1000, spacing, -size/2, cv::Scalar(255, 0, 0), 1, cv::LINE_8, 1);
            // drawFunc<double>(func, initPoint, img, 1000, spacing, 0, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 1);
            cv::imshow("img", img);
            // cv::waitKey(1);
            // cv::updateWindow("img");
            // cv::pollKey();
            // if (keyHeld == 119) //keycode for w
            // {
                // initPoint.y -= 1;
            // }
            while (true)
            {
                int key = cv::pollKey();
                if (key == -1)
                {
                    break;
                }
                else if (key == 'w')
                {
                    initPoint.y -= 5;
                }
                else if (key == 'a')
                {
                    initPoint.x -= 5;
                }
                else if (key == 's')
                {
                    initPoint.y += 5;
                }
                else if (key == 'd')
                {
                    initPoint.x += 5;
                }
                else if (key == 'z')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    // fWidth += 1;
                    // fHeight += 1;
                    fWidth *= 2;
                    fHeight *= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'Z')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    fWidth /= 2;
                    fHeight /= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'u')
                {
                    fHeight *= 2;
                }
                else if (key == 'U')
                {
                    fHeight /= 2;
                }
                else if (key == 'j')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    fWidth *= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'J')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    fWidth /= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'q' || key == 27) //27 is key code for escape
                {
                    tBreak = true;
                }

                // std::cout << key << std::endl;
            }
            // if ()
            // std::cout << keyHeld << std::endl;
            // if (keyPressed ==
            // std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            // std::cout << i * 100 / batchTime << std::endl;
            // outputsArr.print();
            // std::cout << outputsArr.getPtr()[1] << " " << outputsArr.getPtr()[999] << std::endl;
            // std::cout << corrOutputsArr.getPtr()[499] << " " << outputsArr.getPtr()[499] << std::endl;
        }
        if (tBreak == true)
        {
            break;
        }

        outputsArr = network.forwards(inputsArr);

        // if (i %)
        // outputsArr.print();

        network.backwards(network.calculatedOutputs<Array<double>*>("mse", &corrOutputsArr));
        network.updateWeightsAndBiases();
        network.zeroGradient();

        i++;
    }
}





/*#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include "neurelNetworkClass.h"
#include <atomic>
#include <thread>

template<typename  Type>
void drawFunc(std::function<Type(Type)>& func, cv::Point2d& initPoint, cv::Mat& img, const int size, const Type spacing, const Type funcInitPoint, const cv::Scalar& color, const int thickness, const int lineType = cv::LINE_8, const Type width = 1, const Type height = 1) //rdraws function func with size size spacing is the distance between two points (smaller spacing the smoother func is) func init point is the starting point of a function eg if you did 0 it would go from 0 to size but if you did size/2 then it would be centerd kinda hard to explain also cv::point is the intial point the thing starts at
{
    Type prevPointY = func(funcInitPoint);
    Type currentPointY = prevPointY;
    const int initPointY = initPoint.y;
    cv::Point2d prevPoint = initPoint;
    prevPoint.y -= static_cast<int>(prevPointY) * height;
    cv::Point2d currentPoint = prevPoint;
    for (double i = funcInitPoint + spacing; i < size + funcInitPoint; i+=spacing) //use +=spacing bc if you have point at 10 and 0.1 spacing you want to get to 10 but go 0.1 each time also add spacing to 0 bc we start at prev point which is initaly the first ones func thing so ye hard to explain
    {
        // std::cout << i << std::endl;
        currentPointY = func(i) * height;
        currentPoint.y = initPointY - static_cast<int>(currentPointY);
        currentPoint.x += spacing * width;
        cv::line(img, prevPoint, currentPoint, color, thickness, lineType);
        prevPointY = currentPointY;
        prevPoint = currentPoint;
    }
}

// template<typename  Type>
// Type funcToLearn(Type input)
// {
    // return
// }

template<typename  Type>
Type funcToLearn(Type input)
{
    // double a = input-3;
    // double b = std::pow(a,2);
    // double c = std::sqrt(12 - b);
    // return 2+c;
    // std::cout << std::sin(input) << std::endl;
    // return 1 - (std::pow(input, 2)/2) + (std::pow(input, 4)/24);
    // return 1 - (std::pow(input, 2)/2);
    return (input*input*input*input)/24;
    // return 1 - (std::pow(input, 2.0) / 2) + (std::pow(input,4.0)/24);

}

std::atomic<int> keyHeld(100);
void keyListener(int batchTime)
{
    for (int i = 0; i < batchTime; i++)
    {
        // std::cout << "test" << std::endl;
        int key = cv::waitKey(1);
        std::cout << key << std::endl;
        keyHeld = key;
    }
}
int main()
{
    std::cout << funcToLearn(-1000);
    using namespace NeurelNetwork::ArrayUtils;
    const int width = 1920;
    const int height = 1080;
    const int margin = 10;
    // const int size = width - 2 * margin;
    const int size = 2000;
    const double spacing = 1;
    // const double spacing = 1;
    constexpr int inputsSize = size / spacing;

    double* inputsPtr = new double[inputsSize];
    double* corrOutputsPtr = new double[inputsSize]; //same amount of outputs as inputs

    int k = 0;
    for (int i = -inputsSize/2; i < inputsSize/2; i++)
    {
        const double currentIndex = i * spacing;
        inputsPtr[k] = currentIndex;
        corrOutputsPtr[k] = funcToLearn(currentIndex);
        k++;
    }

    Array<double> inputsArr(inputsPtr, inputsSize, 1);
    Array<double> corrOutputsArr(corrOutputsPtr, inputsSize, 1);

    Array<double> outputsArr;

    const int layers[] = {1, 16, 16, 1};
    NeurelNetwork::neuralnetwork<double> network(layers, 4, "leakyRelu", "none", -0.05, "adam");
    // NeurelNetwork::neuralnetwork<double> network(layers, 4, "relu", "none", 0);

    double fWidth = 1;
    double fHeight = 0.001;
    // cv::namedWindow("img", cv::WINDOW_AUTOSIZE);
    cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
    cv::Point2d initPoint((width/2) - (size * fWidth)/2, (height/2)-(size*fHeight)/2);
    // cv::Point2d initPoint(0, height - margin);

    std::function<double(double)> func = [&network](double input)
    {
        double* temp = new double;
        *(temp) = input;
        Array<double> inputArrTemp(temp, 1, 1);
        return network.forwards(inputArrTemp).getPtr()[0];
    };

    std::function<double(double)> funcToLearnInput = funcToLearn<double>;

    cv::imshow("img", img);
    const int batchTime = 1000000;
    bool tBreak(false);
    // std::thread listener(keyListener, batchTime);
    // for (int i = 0; i < batchTime; i++)
    int i = 0;
    while (true)
    {
        // if (i % (batchTime / 1000) == 0 && i != 0)
        if (i % 10 == 0 && i != 0)
        {
            cv::Mat img = cv::Mat::zeros(height, width, CV_8UC3);
            drawFunc<double>(funcToLearnInput, initPoint, img, size, spacing, -size/2, cv::Scalar(255, 0, 0), 1, cv::LINE_8, fWidth, fHeight);
            drawFunc<double>(func, initPoint, img, size, spacing, -size/2, cv::Scalar(0, 255, 0), 1, cv::LINE_8, fWidth, fHeight);
            // drawFunc<double>(funcToLearnInput, initPoint, img, 1000, spacing, -size/2, cv::Scalar(255, 0, 0), 1, cv::LINE_8, 1);
            // drawFunc<double>(func, initPoint, img, 1000, spacing, 0, cv::Scalar(0, 0, 255), 1, cv::LINE_8, 1);
            cv::imshow("img", img);
            // cv::waitKey(1);
            // cv::updateWindow("img");
            // cv::pollKey();
            // if (keyHeld == 119) //keycode for w
            // {
                // initPoint.y -= 1;
            // }
            while (true)
            {
                int key = cv::pollKey();
                if (key == -1)
                {
                    break;
                }
                else if (key == 'w')
                {
                    initPoint.y -= 5;
                }
                else if (key == 'a')
                {
                    initPoint.x -= 5;
                }
                else if (key == 's')
                {
                    initPoint.y += 5;
                }
                else if (key == 'd')
                {
                    initPoint.x += 5;
                }
                else if (key == 'z')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    // fWidth += 1;
                    // fHeight += 1;
                    fWidth *= 2;
                    fHeight *= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'Z')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    fWidth /= 2;
                    fHeight /= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'u')
                {
                    fHeight *= 2;
                }
                else if (key == 'U')
                {
                    fHeight /= 2;
                }
                else if (key == 'j')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    fWidth *= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'J')
                {
                    double preInitX = initPoint.x + (size * fWidth)/2;
                    fWidth /= 2;
                    initPoint.x = preInitX - (size * fWidth)/2;
                }
                else if (key == 'q' || key == 27) //27 is key code for escape
                {
                    tBreak = true;
                }

                // std::cout << key << std::endl;
            }
            // if ()
            // std::cout << keyHeld << std::endl;
            // if (keyPressed ==
            // std::this_thread::sleep_for(std::chrono::nanoseconds(1));
            // std::cout << i * 100 / batchTime << std::endl;
            // outputsArr.print();
            // std::cout << outputsArr.getPtr()[1] << " " << outputsArr.getPtr()[999] << std::endl;
            // std::cout << corrOutputsArr.getPtr()[499] << " " << outputsArr.getPtr()[499] << std::endl;
        }

        outputsArr = network.forwards(inputsArr);

        // if (i %)
        // outputsArr.print();

        network.backwards(network.calculatedOutputs<Array<double>*>("mse", &corrOutputsArr));
        network.updateWeightsAndBiases();
        network.zeroGradient();

        if (tBreak == true)
        {
            break;
        }
        i++;
    }
    outputsArr.print();
}
*/