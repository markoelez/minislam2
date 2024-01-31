#include <pangolin/display/display.h>
#include <pangolin/display/view.h>
#include <pangolin/gl/gldraw.h>
#include <pangolin/handler/handler.h>

#include "visual_odometry.h"

class Display3d {
   public:
    Display3d(const double w, const double h);
    ~Display3d();

    void draw(VisualOdometry& vo);

   private:
    const double viewpoint_x = 0;
    const double viewpoint_y = 50;
    const double viewpoint_z = -80;
    const double viewpoint_f = 1000;

    pangolin::View* d_cam;
    pangolin::OpenGlRenderState s_cam;

    pangolin::OpenGlMatrix twc;
};