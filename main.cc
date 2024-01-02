#include <SDL2/SDL.h>
#include <stdio.h>

#include <filesystem>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "util.hpp"
#include "visual_odometry.hpp"

using namespace cv;

namespace fs = std::filesystem;

void show(Mat img, const std::string window) {
    imshow(window, img);
    waitKey(0);
}

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
    return Point2f(int(x + w / 2), int(h - h / 4 - z));
};

int main() {
    std::string base_path = "./eval/d1";

    const auto fx = 718.856;
    const auto fy = 718.856;
    const auto cx = 607.2;
    const auto cy = 185.2;

    const auto w = 1241;
    const auto h = 376;

    const auto tw = 800;
    const auto th = 800;

    auto camera = Camera(fx, fy, cx, cy);

    std::vector<std::string> fns;
    for (const auto& x : fs::directory_iterator(base_path)) {
        fns.emplace_back(x.path());
    }
    std::sort(fns.begin(), fns.end());

    auto vo = VisualOdometry(camera, 0.8);

    cv::Mat map = Mat::zeros(tw, th, CV_8UC3);

    for (size_t i = 0; i < fns.size(); ++i) {
        auto img = load(fns[i]);

        cv::resize(img, img, cv::Size(w, h));

        vo.process(img);

        if (i > 0) show(vo.draw_features(), "MAIN");

        vo.commit();

        // Draw map.
        auto pos = fit_point(tw, th, vo.cur_t);
        cv::circle(map, pos, 2, Scalar(0, 255, 0), 1);
        auto tmp = map.clone();
        std::string strpos = "(" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
        cv::putText(tmp, strpos, cv::Point(10, 50), cv::FONT_HERSHEY_DUPLEX, 1.0,
                    CV_RGB(255, 255, 255), 1.0);
        show(tmp, "MAP");
    }

    return 0;
}