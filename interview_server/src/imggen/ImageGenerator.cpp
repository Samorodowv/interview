#include "opencv4/opencv2/imgproc.hpp"
#include <iostream>
#include "ImageGenerator.h"

cv::Mat ImageGenerator::operator>>(cv::Mat &image) {
    //Вообще можно было использовать boost_circular_buffer
    // но после собеседования хотелось показать condition_variable
    //в одном потоке кладем кадры в буфер- в другом - сохраняем по сети
    std::unique_lock<std::mutex> lock(_mtx);
    conditionVar.wait(lock, [this] { return !_framebuff.empty(); });
    cv::Mat frame = _framebuff.front().clone();
    _framebuff.pop();
    image = frame;
    return frame;
}

void ImageGenerator::update() {
    while (isWorking) {
        cv::Mat res = cv::Mat::zeros(cv::Size(WIDTH, HEIGHT), CV_8UC1);
        for (int i = 0; i < res.rows; i++)
            for (int j = 0; j < res.cols; j++)
                res.at<uchar>(i, j) = (i + j + counter) / 2 % 255;
        counter++;
        cv::putText(res, std::to_string(counter), {30, 30}, cv::FONT_HERSHEY_COMPLEX, 1, {255, 255, 255});
        std::unique_lock<std::mutex> lock(_mtx);
        if (_framebuff.size() > BUFF_SIZE) {
            _framebuff.pop();
        }
        _framebuff.push(res.clone());
        conditionVar.notify_all();
    }
}
