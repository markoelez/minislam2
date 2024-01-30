#pragma once

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include "opencv2/features2d.hpp"

using namespace cv;

class FeatureManager {
   public:
    FeatureManager() { feature_extractor = cv::ORB::create(); }

    std::pair<std::vector<KeyPoint>, Mat> detect_and_compute(Mat img);

    std::pair<std::vector<KeyPoint>, std::vector<KeyPoint>> get_matches(
        const std::vector<KeyPoint>& cur_kps, const std::vector<KeyPoint>& ref_kps,
        const Mat& cur_des, const Mat& ref_des);

   private:
    cv::Ptr<cv::ORB> feature_extractor;
};