#include <SDL2/SDL.h>
#include <stdio.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <thread>

#include "config.h"
#include "display.h"
#include "display3d.h"
#include "util.h"
#include "visual_odometry.h"

using namespace cv;

namespace fs = std::filesystem;

Mat load(const std::string& path) {
    auto img = imread(path, IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "Error: unable to read image" << std::endl;
        throw;
    }

    return img;
}

Point2f fit_point(const int rows, const int cols, Mat mat_3b1) {
    auto w = rows, h = cols;
    auto x = mat_3b1.at<double>(0, 0), y = mat_3b1.at<double>(1, 0), z = mat_3b1.at<double>(2, 0);
    return Point2f(int(-x + w / 2), int(h - h / 4 - z));
};

int main() {
    auto camera = Camera(Config::fx, Config::fy, Config::cx, Config::cy);

    SDLWindow imgd("Img", Config::w / 2, Config::h / 2);
    SDLWindow map2d("Map", Config::tw, Config::th);
    Display3d map3d(Config::map3d_w, Config::map3d_h);

    if (Config::show_img) imgd.init();
    if (Config::show_map2d) map2d.init();
    if (Config::show_map3d) map3d.init();

    std::vector<std::string> fns;
    for (const auto& x : fs::directory_iterator(Config::dataset_path)) {
        fns.emplace_back(x.path());
    }
    std::sort(fns.begin(), fns.end());

    auto vo = VisualOdometry(camera, Config::scale);

    cv::Mat map = Mat::zeros(Config::tw, Config::th, CV_8UC3);

    SDL_Event e;
    for (size_t i = 0; i < fns.size(); ++i) {
        if (Config::show_img || Config::show_map2d) poll_sdl(e);

        auto img = load(fns[i]);

        cv::resize(img, img, cv::Size(Config::w, Config::h));

        vo.process(img);

        if (Config::show_img && i > 0) imgd.show(vo.draw_features());

        vo.commit();

        if (Config::show_map2d) {
            // Draw 2d map.
            auto pos = fit_point(Config::tw, Config::th, vo.cur_t);
            cv::circle(map, pos, 2, Scalar(0, 255, 0), 1);
            auto tmp = map.clone();
            std::string strpos = "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
            cv::putText(tmp, strpos, cv::Point(10, 50), cv::FONT_HERSHEY_DUPLEX, 1.0,
                        CV_RGB(255, 255, 255), 1.0);
            map2d.show(tmp);
        }

        if (Config::show_map3d) map3d.draw(vo);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}