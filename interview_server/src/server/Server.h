#ifndef INTERVIEW_SERVER_SERVER_H
#define INTERVIEW_SERVER_SERVER_H

#include <opencv2/core/mat.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/circular_buffer.hpp>
#include <thread>

using namespace boost::asio;
using ip::tcp;
class Server {
public:
    explicit Server(int port);
    cv::Mat operator<<(cv::Mat &mat);
    bool isWorking;

private:
    void sendAvailable();
    int port_num;
    std::unique_ptr<tcp::acceptor> _acceptor;
    std::unique_ptr<tcp::socket> _socket;
    boost::asio::io_service ioService;
    boost::circular_buffer<cv::Mat> _buff{60};
    std::thread _thr;
};


#endif //INTERVIEW_SERVER_SERVER_H
