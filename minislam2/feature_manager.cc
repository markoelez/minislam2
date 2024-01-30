#include "feature_manager.hpp"

#include "opencv2/features2d.hpp"

using namespace cv;

std::pair<std::vector<KeyPoint>, Mat> FeatureManager::detect_and_compute(Mat img) {
    std::vector<Point2f> pts;
    goodFeaturesToTrack(img, pts, 2000, 0.01, 3);

    std::vector<KeyPoint> kps;
    std::transform(pts.begin(), pts.end(), std::back_inserter(kps),
                   [](const auto& p) { return KeyPoint(p.x, p.y, 5); });

    Mat des;
    feature_extractor->compute(img, kps, des);

    return {kps, des};
}

std::pair<std::vector<KeyPoint>, std::vector<KeyPoint>> FeatureManager::get_matches(
    const std::vector<KeyPoint>& cur_kps, const std::vector<KeyPoint>& ref_kps, const Mat& cur_des,
    const Mat& ref_des) {
    BFMatcher matcher(NORM_HAMMING);
    std::vector<std::vector<DMatch>> matches;
    matcher.knnMatch(ref_des, cur_des, matches, 2);

    std::vector<DMatch> good;
    for (const auto& match : matches) {
        if (match[0].distance < 0.75 * match[1].distance) {
            good.push_back(match[0]);
        }
    }

    std::vector<KeyPoint> n_cur_kps, n_ref_kps;
    for (const auto& m : good) {
        n_cur_kps.push_back(cur_kps[m.trainIdx]);
        n_ref_kps.push_back(ref_kps[m.queryIdx]);
    }

    return {n_cur_kps, n_ref_kps};
}