#include <iostream>
#include "Server.h"
#include "opencv2/core/mat.hpp"
#include "utils/serialization.h"

Server::Server(const int port) : port_num(port) {
    _acceptor = std::make_unique<tcp::acceptor>(ioService, tcp::endpoint(tcp::v4(), port_num));
    _thr = std::thread(&Server::sendAvailable, this);
    _thr.detach();
    _socket = std::make_unique<tcp::socket>(ioService);
    isWorking = true;
}

cv::Mat Server::operator<<(cv::Mat &mat) {
    _buff.push_back(mat.clone());
    return {};
}

void Server::sendAvailable() {
    _acceptor->accept(*_socket); //блокирует поток в ожидании подключения
    while (isWorking) {
        if (!_buff.empty()) {
            try {
                //Кодирование mat происходит здесь, в функции save
                //Можно было бы делать cv::imendode, можно самостоятельно упаковать data, но так работает быстрее.
                boost::asio::write(*_socket, boost::asio::buffer(save(_buff.front())));
            } catch (const boost::system::system_error &e) {
                std::cerr << e.what();
                //за вот это тоже стыдно, по хорошему, нужно бы нормально определять что клиент отключился
                //и делать этот метод асинхронным.
                boost::system::error_code ec;
                _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
                _socket = std::make_unique<tcp::socket>(ioService);
                _acceptor->accept(*_socket); //просто ждём новое подключение
            }
            _buff.pop_front();
        }
    }
}
