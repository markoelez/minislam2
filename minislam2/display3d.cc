#include "display3d.h"

#include <pangolin/display/display.h>
#include <pangolin/display/view.h>
#include <pangolin/gl/gldraw.h>
#include <pangolin/handler/handler.h>

void check_gl_error(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        std::cerr << "OpenGL error in \"" << op << "\": " << gluErrorString(error) << " (" << error
                  << ")" << std::endl;
    }
}

pangolin::OpenGlMatrix get_opengl_mat(const cv::Mat& cvMat) {
    pangolin::OpenGlMatrix matrix;
    // Ensure cvMat is a 4x4 transformation matrix
    if (cvMat.rows != 4 || cvMat.cols != 4 || cvMat.type() != CV_64F) {
        throw std::invalid_argument("Input cv::Mat must be a 4x4 double matrix.");
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            matrix.m[j * 4 + i] = cvMat.at<double>(i, j);
        }
    }
    return matrix;
}

Display3d::Display3d(const double w, const double h) {
    pangolin::CreateWindowAndBind("Map", w, h);
    // glEnable(GL_DEPTH_TEST);

    s_cam = pangolin::OpenGlRenderState(
        pangolin::ProjectionMatrix(w, h, 718, 718, w / 2, h / 2, 0.1, 10000),
        pangolin::ModelViewLookAt(0, 0, -3, 0, 0, 0, pangolin::AxisY));

    // Create interactive view
    d_cam = &pangolin::CreateDisplay()
                 .SetBounds(0.0, 1.0, 0.0, 1.0, static_cast<double>(w) / h)
                 .SetHandler(new pangolin::Handler3D(s_cam));

    // Adjust viewport
    d_cam->Resize(pangolin::Viewport(0, 0, w * 2, h * 2));
}

Display3d::~Display3d() {}

void Display3d::draw(VisualOdometry& vo) {
    std::vector<cv::Mat> poses = vo.get_poses(), translations = vo.get_translations();

    if (!poses.empty()) {
        pangolin::OpenGlMatrix latest_pose = get_opengl_mat(poses.back());
        // s_cam.Follow(latest_pose);
    }

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Activate the display
    d_cam->Activate(s_cam);
    check_gl_error("init");

    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    for (const auto& t : translations) {
        std::cout << t << std::endl;
        glVertex3d(t.at<double>(0, 0), t.at<double>(1, 0), t.at<double>(2, 0));
    }
    glEnd();

    std::cout << "************************************************************" << std::endl;
    check_gl_error("draw");

    pangolin::FinishFrame();
}
