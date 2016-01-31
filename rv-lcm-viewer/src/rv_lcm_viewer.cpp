#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <iostream>
#include <getopt.h>

#include <bot_vis/bot_vis.h>
#include <lcm/lcm.h>
#include <bot_core/bot_core.h>

//renderers
#include <bot_lcmgl_render/lcmgl_bot_renderer.h>
#include <bot_frames/bot_frames_renderers.h>

using namespace std;

#define USE_CAMERA_THUMB

#ifdef USE_CAMERA_THUMB
#include <image_utils/renderer_cam_thumb.h>
#endif

static void
usage(const char *name)
{
	fprintf(
			stderr,
			"usage: %s [options]\n"
			"\n"
			"  -h, --help                            Shows this help text and exits\n"
			"  -b, --bot-param-config <config file>  use a config file (default)\n"
			"  -c, --use-cam-thumb                   use cam thumb_renderer\n"
			"  -a, --use-articulated-body            use articulated_body_renderer\n"
			"  -s, --bot-param-server                bot-param-server\n"
			"\n",
			name);

	exit(1);
}

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	glutInit(&argc, argv);
	g_thread_init(NULL);

	lcm_t * lcm = bot_lcm_get_global(NULL);
	bot_glib_mainloop_attach_lcm(lcm);

	BotViewer* viewer = bot_viewer_new("Robotic Vision LCM Viewer");
	//die cleanly for control-c etc :-)
	bot_gtk_quit_on_interrupt();

	// load bot-param from server or file
	BotParam * bot_param;
	string config_file_path = "./config/rv-bot-param.cfg";
	int bot_param_ready = 0;

	int use_cam_thumb = 1;
	int use_articulated_body = 1;

	const char *optstring = "hb:cas";
	int c;
	struct option long_opts[] =
	{
			{ "help", no_argument, 0, 'h' },
			{ "bot-param-config", required_argument, 0, 'b' },
			{ "bot-cam-thumb", no_argument, 0, 'c' },
			{ "bot-articulated-body", no_argument, 0, 'a' },
			{ "bot-param-server", no_argument, 0, 's' },
			{ 0, 0, 0, 0 }
	};

	while ((c = getopt_long(argc, argv, optstring, long_opts, 0)) >= 0) {
		switch (c){
		case 'b':
			config_file_path = strdup(optarg);
			break;
		case 's':
			bot_param = bot_param_new_from_server(lcm, 0);
			if(bot_param == NULL){
				fprintf(stderr, "Couldn't get bot param from server.\n");
				return 1;
			}else{
				bot_param_ready = 1;
			}
			break;
		case 'c':
			use_cam_thumb = 1;
			break;
		case 'a':
			use_articulated_body = 1;
			break;
		case 'h':
			usage(argv[0]);
			break;
		}
	}

	if (bot_param_ready == 0) {
		bot_param = bot_param_new_from_file(config_file_path.c_str());
		fprintf(stderr, "Use bot-param config file: %s.\n", config_file_path.c_str());
		if(bot_param == NULL){
			fprintf(stderr, "Couldn't get bot param from file.\n");
			return 1;
		}else{
			bot_param_ready = 1;
		}
	}

	// load bot-frames
	BotFrames* bot_frames = bot_frames_new(lcm, bot_param);

	// setup renderers
	bot_viewer_add_stock_renderer(viewer, BOT_VIEWER_STOCK_RENDERER_GRID, 1);
	bot_lcmgl_add_renderer_to_viewer(viewer,lcm, 2);
#ifdef USE_CAMERA_THUMB
	if (use_cam_thumb == 1)
		add_cam_thumb_renderer_to_viewer(viewer, 0, lcm, bot_param, bot_frames);
#endif
	if (use_articulated_body == 1)
		bot_frames_add_articulated_body_renderer_to_viewer(
				viewer, 1, bot_param, bot_frames, "", "articulated_body_name");
	bot_frames_add_renderer_to_viewer(viewer, 3, bot_frames );

	//load the renderer params from the config file.
	char *fname = g_build_filename(g_get_user_config_dir(), ".bot-plugin-viewerrc", NULL);
	bot_viewer_load_preferences(viewer, fname);

	gtk_main();

	//save the renderer params to the config file.
	bot_viewer_save_preferences(viewer, fname);

	bot_viewer_unref(viewer);
}
