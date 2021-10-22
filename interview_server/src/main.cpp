#include <opencv4/opencv2/highgui.hpp>
#include "imggen/ImageGenerator.h"
#include "server/Server.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

int main() {
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("../config.ini", pt);
    const int WIDTH     = pt.get<int>("width");
    const int HEIGHT    = pt.get<int>("height");
    const int FPS       = pt.get<int>("fps");
    const int PORT      = pt.get<int>("port");

    ImageGenerator imageGenerator(WIDTH, HEIGHT);
    Server server(PORT);

    cv::Mat frame;
    //тут жульничаю, waitKey делает паузу в указанное количество ms если цикл завершился раньше.
    //этим добиваюсь необходимого fps
    //совсем не сложно сделать это в потоке imageGenerator-a тем же condition_variable wait_until timeout
    while (cv::waitKey(1000/FPS) != 27 && server.isWorking){
        imageGenerator >> frame;
        server << frame;
        cv::imshow("frame", frame);
    }
    return EXIT_SUCCESS;
}
