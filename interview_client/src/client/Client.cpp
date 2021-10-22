#include <iostream>
#include "Client.h"
#include "utils/serialization.h"
#include <opencv2/highgui.hpp>
#include <boost/date_time.hpp>
#include <chrono>

Client::Client(std::string connStr) {
    std::string delim = ":";
    auto pos = connStr.find(delim);
    if (pos == std::string::npos) {
        throw std::domain_error("ip/port separator not found");
    }
    auto ip = connStr.substr(0, pos);
    auto port = std::stoi(connStr.erase(0, pos + delim.length()));
    _socket = std::make_unique<ip::tcp::socket>(io_service);
    _socket->connect(ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port));
    _thr_save = std::thread(&Client::imgSave, this);
    _thr_save.detach();
    _isWorking = true;
    while (_isWorking) {
        boost::asio::streambuf receive_buffer;
        try {
            boost::asio::read_until(*_socket, receive_buffer, "\n", error);
            if (error == boost::asio::error::eof){
                _isWorking = false;
                throw std::runtime_error("eof, probably lost connection");
                break;
            }
            load(_res, boost::asio::buffer_cast<const char *>(receive_buffer.data()));
            _buff.push_back(_res.clone());
            cv::imshow("client", _res);
        }
        catch (const boost::archive::archive_exception &e) {
            std::cerr << "while decoding: " << e.what() << std::endl;
        }
        _isWorking = cv::waitKey(1) != 27; //esc code
    }
}

[[noreturn]] void Client::imgSave() {
    while (_isWorking) {
        if (!_buff.empty()) {
            namespace bp = boost::posix_time;
            auto time = bp::second_clock::universal_time();
            cv::imwrite("../data/" + bp::to_simple_string(time) + ":" +
                        //Совсем не здорово так делать и никогда в проде ToDo поправить
                        std::to_string(std::chrono::system_clock::now().time_since_epoch().count() % 100) + ".jpg",
                        _buff.front());
            _buff.pop_front();
        }
    }
    cv::destroyAllWindows();
}

