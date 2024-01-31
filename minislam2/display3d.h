#include <pangolin/display/display.h>
#include <pangolin/display/view.h>
#include <pangolin/gl/gldraw.h>
#include <pangolin/handler/handler.h>

#include "visual_odometry.h"

class Display3d {
   public:
    Display3d(const double w, const double h);
    ~Display3d();

    void init();

    void draw(VisualOdometry& vo);

   private:
    double w, h;

    const bool follow_cam = false;

    const double cam_psize = 1;
    const double path_psize = 4;

    const double vpx = 0;
    const double vpy = 80;
    const double vpz = -60;
    const double vpf = 500;

    const double lpx = 0;
    const double lpy = 0;
    const double lpz = 60;

    pangolin::View* d_cam;
    pangolin::OpenGlRenderState s_cam;

    pangolin::OpenGlMatrix twc;
};