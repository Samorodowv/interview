#ifndef INTERVIEW_SERVER_IMAGEGENERATOR_H
#define INTERVIEW_SERVER_IMAGEGENERATOR_H

#include <opencv2/core/mat.hpp>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


class ImageGenerator {
public:
    ImageGenerator(int width, int height) : WIDTH(width), HEIGHT(height), isWorking(true) {
        _thrGenerate = std::thread(&ImageGenerator::update, this);
        _thrGenerate.detach();
    };

    cv::Mat operator>>(cv::Mat &image);

private:
    std::thread _thrGenerate;
    std::queue<cv::Mat> _framebuff;

    void update();

    const int WIDTH, HEIGHT;
    int BUFF_SIZE{30};
    std::mutex _mtx;
    std::condition_variable conditionVar;
    bool isWorking;
    size_t counter{0};
};


#endif //INTERVIEW_SERVER_IMAGEGENERATOR_H
