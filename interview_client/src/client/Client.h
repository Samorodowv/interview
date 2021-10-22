#ifndef INTERVIEW_CLIENT_CLIENT_H
#define INTERVIEW_CLIENT_CLIENT_H
#include <boost/asio.hpp>
#include <opencv2/core/mat.hpp>
#include <thread>
#include "boost/circular_buffer.hpp"

namespace ip = boost::asio::ip;
class Client {
public:
    explicit Client(std::string connStr);
private:
    [[noreturn]] void imgSave();
    boost::asio::io_service io_service;
    std::unique_ptr<ip::tcp::socket> _socket;
    cv::Mat _res;
    boost::circular_buffer<cv::Mat> _buff{60};
    std::thread _thr_save;
    boost::system::error_code error;
    bool _isWorking;
};

#endif //INTERVIEW_CLIENT_CLIENT_H
