#include "visual_odometry.h"

#include <iostream>
#include <opencv2/calib3d.hpp>

#include "util.h"

void VisualOdometry::process(Mat img) {
    cur_img = img;

    Mat tmp;
    cvtColor(cur_img, tmp, COLOR_BGR2GRAY);
    std::tie(cur_kps, cur_des) = fm.detect_and_compute(tmp);

    if (!ref_kps.empty()) {
        std::tie(cur_matched_kps, ref_matched_kps) =
            fm.get_matches(cur_kps, ref_kps, cur_des, ref_des);

        // Compute pose.
        auto [R, t] = estimate_pose();

        // Update current position.
        cur_t = cur_t + scale * (cur_R * t);
        cur_R = cur_R * R;
    }
};

std::tuple<cv::Mat, cv::Mat> VisualOdometry::estimate_pose() {
    auto cur_kps = cur_matched_kps, ref_kps = ref_matched_kps;

    // Convert keypoints to points.
    auto cur_pts = kps_to_pts(cur_kps);
    auto ref_pts = kps_to_pts(ref_kps);

    // Convert image coordinates to normalized camera coordinates.
    cur_pts = denormalize(camera.K, cur_pts);
    ref_pts = denormalize(camera.K, ref_pts);

    // Compute the essential matrix using the identity camera matrix.
    Mat E = cv::findEssentialMat(cur_pts, ref_pts, 1.0, Point2d(0, 0), RANSAC, 0.999, 0.003);

    // Compute the pose using the essential matrix.
    Mat R, t, mask;
    cv::recoverPose(E, cur_pts, ref_pts, R, t, 1.0, Point2d(0, 0), mask);

    // Apply mask.
    std::vector<cv::KeyPoint> filtered_cur_kps, filtered_ref_kps;
    auto is_inlier = [&mask](const size_t& idx) { return mask.at<uchar>(idx); };
    std::copy_if(cur_kps.begin(), cur_kps.end(), std::back_inserter(filtered_cur_kps),
                 [&, i = 0](const cv::KeyPoint&) mutable { return is_inlier(i++); });
    std::copy_if(ref_kps.begin(), ref_kps.end(), std::back_inserter(filtered_ref_kps),
                 [&, i = 0](const cv::KeyPoint&) mutable { return is_inlier(i++); });
    cur_matched_kps = filtered_cur_kps;
    ref_matched_kps = filtered_ref_kps;

    return {R, t};
};

void VisualOdometry::commit() {
    ref_img = cur_img;
    ref_kps = cur_kps;
    ref_des = cur_des;
    ref_matched_kps = cur_matched_kps;
};

Mat VisualOdometry::draw_features() {
    Mat img = cur_img;

    for (size_t i = 0; i < cur_matched_kps.size() && i < ref_matched_kps.size(); ++i) {
        auto cur = cur_matched_kps[i].pt, ref = ref_matched_kps[i].pt;
        circle(img, cur, 2, Scalar(255, 0, 0), 1);
        circle(img, ref, 2, Scalar(255, 0, 0), 1);
        line(img, ref, cur, Scalar(0, 255, 0), 1);
    }

    return img;
};