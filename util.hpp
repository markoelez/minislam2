#pragma once

#include <iostream>
#include <opencv2/core.hpp>

#include "opencv2/features2d.hpp"

class Camera {
   public:
    Camera(const double fx, const double fy, const double cx, const double cy)
        : fx(fx), fy(fy), cx(cx), cy(cy) {
        K = (cv::Mat_<float>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 1);
        Kinv = K.inv();
    };

    double fx, fy, cx, cy;
    cv::Mat K, Kinv;
};

inline std::vector<cv::Point2f> kps_to_pts(const std::vector<cv::KeyPoint>& kps) {
    std::vector<cv::Point2f> pts;
    std::transform(kps.begin(), kps.end(), std::back_inserter(pts),
                   [](const auto& p) { return cv::Point2f(p.pt.x, p.pt.y); });
    return pts;
};

inline std::vector<cv::Point3f> to_3d_homogenous(const std::vector<cv::Point2f>& pts) {
    std::vector<cv::Point3f> res;
    std::transform(pts.begin(), pts.end(), std::back_inserter(res),
                   [](const auto& p) { return cv::Point3f(p.x, p.y, 1.0f); });
    return res;
};

inline std::vector<cv::Point2f> denormalize(cv::Mat K, const std::vector<cv::Point2f>& pts) {
    auto homogenous = to_3d_homogenous(pts);

    auto Kinv = K.inv();

    std::vector<cv::Point2f> res;
    res.reserve(pts.size());

    for (const auto& p : homogenous) {
        cv::Mat homogeneous_pt = (cv::Mat_<float>(3, 1) << p.x, p.y, p.z);
        cv::Mat denormalized_pt = Kinv * homogeneous_pt;

        res.emplace_back(denormalized_pt.at<float>(0, 0) / denormalized_pt.at<float>(2, 0),
                         denormalized_pt.at<float>(1, 0) / denormalized_pt.at<float>(2, 0));
    }

    return res;
};