#include <stdio.h>

#include <lcm/lcm.h>
#include <bot_core/bot_core.h>
#include <bot_lcmgl_client/lcmgl.h>
#include <GL/gl.h>

#include <lcmtypes/bot_core_pose_t.h>
#include <lcmtypes/bot_core_image_t.h>

int main(int argc, char** argv)
{

    lcm_t* lcm_ = lcm_create(NULL);
    bot_lcmgl_t* lcmgl_ = bot_lcmgl_init(lcm_, "LCMGL_DEMO");

    // lcmgl setup
    bot_lcmgl_translated(lcmgl_, 0, 0, 0);
    bot_lcmgl_line_width(lcmgl_, 2.0f);
    bot_lcmgl_point_size(lcmgl_, 12.0f);
    bot_lcmgl_begin(lcmgl_, GL_POINTS);

    // setup color and draw a point
    bot_lcmgl_color3f(lcmgl_, 1.0, 0, 0);
    bot_lcmgl_vertex3f(lcmgl_, 0, 0, 1);

    // end and refresh to viewer
    bot_lcmgl_end(lcmgl_);
    bot_lcmgl_switch_buffer(lcmgl_);

    printf("Dosvedanya!\n");
    return 0;
}
