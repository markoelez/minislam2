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

pangolin::OpenGlMatrix get_opengl_mat(const cv::Mat& mat) {
    if (mat.rows != 4 || mat.cols != 4 || mat.type() != CV_64F) {
        throw std::invalid_argument("Input cv::Mat must be a 4x4 double matrix.");
    }

    pangolin::OpenGlMatrix res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[j * 4 + i] = mat.at<double>(i, j);
        }
    }
    return res;
}

void draw_camera(const pangolin::OpenGlMatrix& pose, float r, float g, float b, float psize = 2.0,
                 float size = 1.0) {
    glLineWidth(psize);
    glColor3f(r, g, b);

    glPushMatrix();
    glMultMatrixd(pose.m);

    glBegin(GL_LINES);

    glVertex3f(0, 0, 0);
    glVertex3f(size, size, size);

    glVertex3f(0, 0, 0);
    glVertex3f(-size, size, size);

    glVertex3f(0, 0, 0);
    glVertex3f(-size, -size, size);

    glVertex3f(0, 0, 0);
    glVertex3f(size, -size, size);

    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);

    glVertex3f(-size, size, size);
    glVertex3f(-size, -size, size);

    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);

    glVertex3f(size, -size, size);
    glVertex3f(size, size, size);

    glEnd();

    glPopMatrix();
}

Display3d::Display3d(const double w, const double h) : w(w), h(h){};
Display3d::~Display3d() {}

void Display3d::init() {
    pangolin::CreateWindowAndBind("Map", w, h);
    glEnable(GL_DEPTH_TEST);

    s_cam = pangolin::OpenGlRenderState(
        pangolin::ProjectionMatrix(w, h, vpf, vpf, w / 2, h / 2, 0.1, 10000),
        pangolin::ModelViewLookAt(vpx, vpy, vpz, lpx, lpy, lpz, pangolin::AxisY));

    // Create interactive view
    d_cam = &pangolin::CreateDisplay()
                 .SetBounds(0.0, 1.0, 0.0, 1.0, static_cast<double>(w) / h)
                 .SetHandler(new pangolin::Handler3D(s_cam));

    // Adjust viewport
    d_cam->Resize(pangolin::Viewport(0, 0, w * 2, h * 2));
}

void Display3d::draw(VisualOdometry& vo) {
    std::vector<cv::Mat> poses = vo.get_poses(), translations = vo.get_translations();

    if (follow_cam && !poses.empty()) {
        pangolin::OpenGlMatrix latest_pose = get_opengl_mat(poses.back());
        s_cam.Follow(latest_pose);
    }

    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Activate the display
    d_cam->Activate(s_cam);
    check_gl_error("init");

    if (poses.size() >= 2) {
        for (size_t i = 0; i < poses.size() - 1; ++i) {
            pangolin::OpenGlMatrix glPose = get_opengl_mat(poses[i]);
            draw_camera(glPose, 0.0, 1.0, 0.0, cam_psize);
        }
    }

    if (poses.size() >= 1) {
        pangolin::OpenGlMatrix glPose = get_opengl_mat(poses.back());
        draw_camera(glPose, 1.0, 0.0, 0.0, cam_psize);
    }

    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(path_psize);
    glBegin(GL_LINE_STRIP);
    for (const auto& t : translations) {
        glVertex3d(t.at<double>(0, 0), t.at<double>(1, 0), t.at<double>(2, 0));
    }
    glEnd();

    check_gl_error("draw");

    pangolin::FinishFrame();
}
