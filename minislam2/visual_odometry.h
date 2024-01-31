#pragma once

#include "feature_manager.h"
#include "util.h"

class VisualOdometry {
   public:
    VisualOdometry(const Camera& camera, const double scale) : camera(camera), scale(scale) {
        fm = FeatureManager();
        cur_R = Mat::eye(3, 3, CV_64F);
        cur_t = Mat::zeros(3, 1, CV_64F);
    }

    void process(Mat img);

    void commit();

    Mat draw_features();

    std::vector<Mat> get_poses();

    std::vector<Mat> get_translations();

    std::tuple<cv::Mat, cv::Mat> estimate_pose();

    Mat cur_t;

   private:
    Camera camera;

    FeatureManager fm;
    Mat cur_img, ref_img;

    std::vector<KeyPoint> cur_kps, ref_kps;
    std::vector<KeyPoint> cur_matched_kps, ref_matched_kps;

    Mat cur_des, ref_des;

    Mat cur_R;

    std::vector<Mat> poses;
    std::vector<Mat> translations;

    double scale;
};