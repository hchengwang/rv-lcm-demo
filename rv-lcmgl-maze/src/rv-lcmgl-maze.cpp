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

void draw_maze(bot_lcmgl_t* lcmgl, std::vector<std::vector<float> > maze_segments){

    for(int i = 0; i < maze_segments.size(); i++){

        Eigen::Vector3d st_pt, end_pt;
        st_pt << maze_segments[i][0], maze_segments[i][1], 0;
        end_pt << maze_segments[i][2], maze_segments[i][3], 0;

        draw_line_lcmgl(lcmgl, st_pt, end_pt,
                0.5, 0.5, 0);

    }
}

void get_maze_segments(std::string csv_file, std::vector<std::vector<float> > & maze_segments){

	std::vector<float> segments, segments_t;
	std::ifstream file (csv_file);
	std::string value;

	std::vector<Eigen::Vector3d> holodeck_corners;
	Eigen::Vector3d ll, lr, tr, tl;
	ll << -2.575, -1.945, 0;
	lr << 2.565, -2,022, 0;
	tr << 2.480, 3.180, 0;
	tl << -2.588, 3.121, 0;

	double dx, dy, sx, sy;
	dx = 0.5 * (lr[0] - ll[0]) + 0.5 * (tr[0] - tl[0]);
	dy = 0.5 * (tr[1] - lr[1]) + 0.5 * (tl[1] - ll[1]);

	sx = dx / 4;
	sy = dy / 4;

	while ( file.good() )
	{

		float y1, x1, y2, x2;
		getline ( file, value, ',' );
		x1 = atof(value.c_str()) * sx + ll[1];
		getline ( file, value, ',' );
		y1 = (4 - atof(value.c_str())) * sy + ll[0];
		getline ( file, value, ',' );
		x2 = atof(value.c_str()) * sx + ll[1];
		getline ( file, value);
		y2 = (4 - atof(value.c_str())) * sy + ll[0];

		segments.push_back(y1);
		segments.push_back(x1);
		segments.push_back(y2);
		segments.push_back(x2);

		maze_segments.push_back(segments);

		segments.clear();
	}

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
            "  -m, --maze-id                         maze id (1-4)\n"
            "\n",
            name);

    exit(1);
}

int main(int argc, char** argv)
{

    int maze_id = 1;

    const char *optstring = "hm:";
    int c;
    struct option long_opts[] =
    {
        { "help", no_argument, 0, 'h' },
        { "maze", required_argument, 0, 'm' },
        { 0, 0, 0, 0 }
    };

    while ((c = getopt_long(argc, argv, optstring, long_opts, 0)) >= 0) {
        switch (c){
            case 'm':
                maze_id = atoi(optarg);
                break;
            case 'h':
                usage(argv[0]);
                break;
        }
    }

    lcm_t* lcm_ = lcm_create(NULL);
    bot_lcmgl_t* lcmgl_maze = bot_lcmgl_init(lcm_, "LCMGL_MAZE");

    std::vector<std::vector<float> > maze_segments;
    
    std::stringstream ss_maze;
    ss_maze << "./config/maze/maze-" << maze_id << ".csv";
    rv::get_maze_segments(ss_maze.str(), maze_segments);
    std::cout << ss_maze.str() << " contains: " 
        << maze_segments.size() << " segments." << std::endl;
    if(maze_segments.size() == 0){
        std:: cout << "check file path " << std::endl;
    }

    rv::draw_maze(lcmgl_maze, maze_segments);
    bot_lcmgl_switch_buffer(lcmgl_maze);

    return 0;
}


