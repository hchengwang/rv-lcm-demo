#include <stdio.h>
#include <inttypes.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <signal.h>
#include <getopt.h>

#include <lcm/lcm.h>
#include <bot_core/bot_core.h>
#include <bot_lcmgl_client/lcmgl.h>
#include <GL/gl.h>

#include <Eigen/Dense>

#include <lcmtypes/bot_core_pose_t.h>
#include <lcmtypes/bot_core_image_t.h>

namespace rv{

void draw_line_lcmgl(bot_lcmgl_t* lcmgl_, Eigen::Vector3d st_pt, Eigen::Vector3d end_pt,
        float r, float g, float b){

    // position

    bot_lcmgl_line_width(lcmgl_, 4.0f);
    bot_lcmgl_point_size(lcmgl_, 7.0f);
    bot_lcmgl_color3f(lcmgl_, r, g, b);

    bot_lcmgl_begin(lcmgl_, GL_LINE_STRIP);
    bot_lcmgl_vertex3d(lcmgl_, st_pt[0], st_pt[1], st_pt[2]);
    bot_lcmgl_vertex3d(lcmgl_,end_pt[0], end_pt[1], end_pt[2]);
    bot_lcmgl_end(lcmgl_);

}

void draw_pose_lcmgl(bot_lcmgl_t* lcmgl_, Eigen::Matrix4d pose,
			float r, float g, float b){

	float normal_length = 0.4;

	bot_lcmgl_translated(lcmgl_, 0, 0, 0);
	bot_lcmgl_line_width(lcmgl_, 2.0f);
	bot_lcmgl_point_size(lcmgl_, 7.0f);
	bot_lcmgl_color3f(lcmgl_, r, g, b);

	Eigen::Vector3d pt;
	pt << pose(0, 3), pose(1, 3), pose(2, 3);

	Eigen::Matrix4d M_fd, M_left, M_up;
	Eigen::Matrix4d T_fd, T_left, T_up;
	T_fd << 1, 0, 0, normal_length,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
	M_fd = pose * T_fd;

	T_left << 	1, 0, 0, 0,
			0, 1, 0, normal_length/2,
			0, 0, 1, 0,
			0, 0, 0, 1;
	M_left = pose * T_left;

	T_up << 1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, normal_length/2,
			0, 0, 0, 1;
	M_up = pose * T_up;

	Eigen::Vector3d pt_fd, pt_left, pt_up;
	pt_fd << M_fd(0, 3), M_fd(1, 3), M_fd(2, 3);
	pt_left << M_left(0, 3), M_left(1, 3), M_left(2, 3);
	pt_up << M_up(0, 3), M_up(1, 3), M_up(2, 3);

	bot_lcmgl_begin(lcmgl_, GL_LINE_STRIP);
	bot_lcmgl_vertex3f(lcmgl_, pt[0], pt[1], pt[2]);
	bot_lcmgl_vertex3f(lcmgl_, pt_fd[0], pt_fd[1], pt_fd[2]);
	bot_lcmgl_end(lcmgl_);

	bot_lcmgl_begin(lcmgl_, GL_LINE_STRIP);
	bot_lcmgl_vertex3f(lcmgl_, pt[0], pt[1], pt[2]);
	bot_lcmgl_vertex3f(lcmgl_, pt_left[0], pt_left[1], pt_left[2]);
	bot_lcmgl_end(lcmgl_);

	bot_lcmgl_begin(lcmgl_, GL_LINE_STRIP);
	bot_lcmgl_vertex3f(lcmgl_, pt[0], pt[1], pt[2]);
	bot_lcmgl_vertex3f(lcmgl_, pt_up[0], pt_up[1], pt_up[2]);
	bot_lcmgl_end(lcmgl_);

	bot_lcmgl_begin(lcmgl_, GL_POINTS);
	bot_lcmgl_vertex3f(lcmgl_, pt[0], pt[1], pt[2]);
	bot_lcmgl_end(lcmgl_);
}

} // end of namespace rv

static void
usage(const char *name)
{
    fprintf(
            stderr,
            "usage: %s [options]\n"
            "\n"
            "  -h, --help                            Shows this help text and exits\n"
            "\n",
            name);

    exit(1);
}

int main(int argc, char** argv)
{

    const char *optstring = "h";
    int c;
    struct option long_opts[] =
    {
        { "help", no_argument, 0, 'h' },
        { 0, 0, 0, 0 }
    };

    while ((c = getopt_long(argc, argv, optstring, long_opts, 0)) >= 0) {
        switch (c){
            case 'h':
                usage(argv[0]);
                break;
        }
    }

    lcm_t* lcm_ = lcm_create(NULL);
    bot_lcmgl_t* lcmgl_pose = bot_lcmgl_init(lcm_, "LCMGL_POSE_DEMO");

    // Draw a pose at origin
    Eigen::Matrix4d P_o;
	P_o <<  1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
	rv::draw_pose_lcmgl(lcmgl_pose, P_o, 1, 0, 0);

	// Do a forward (at X axis)
	// and 90 degree yaw transformation
	Eigen::Matrix4d T_x;
	T_x <<  0,-1, 0, 1,
			1, 0, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1;
	Eigen::Matrix4d P_1;
	P_1 = P_o * T_x;
	rv::draw_pose_lcmgl(lcmgl_pose, P_1, 0, 1, 0);

    bot_lcmgl_switch_buffer(lcmgl_pose);

    return 0;
}


