#include "rv-libbot-opencv-demo.hpp"

using namespace std;

LibbotOpenCV::LibbotOpenCV(lcm_t* lcm){
	this->lcm_ = lcm;
	this->cv_bridge = new cvBridgeLCM(lcm, lcm);

	string frame_channel_ = "IMAGE_HIGH_RES_RGB_1";
	bot_core_image_t_subscription_t* sub = bot_core_image_t_subscribe(
			lcm, frame_channel_.c_str(), LibbotOpenCV::on_libbot_aux, this);
	cout << "Subscribe bot_core_image_t channel:\t" << frame_channel_ << endl;

}

LibbotOpenCV::~LibbotOpenCV() {
}

void LibbotOpenCV::on_libbot(const bot_core_image_t* msg) {

	cv::Mat im = cv::Mat::zeros(msg->height, msg->width, CV_8UC3);

	if(msg->pixelformat == BOT_CORE_IMAGE_T_PIXEL_FORMAT_MJPEG){
		// jpeg decompress
	    jpegijg_decompress_8u_rgb (msg->data, msg->size,
	    		im.data, msg->width, msg->height, msg->width * 3);
	}else {
		im.data = msg->data;
	}

	this->cv_bridge->publish_mjpg(im, "IMAGE_OUT");

}

void LibbotOpenCV::on_libbot_aux(const lcm_recv_buf_t* rbuf,
		const char* channel,
		const bot_core_image_t* msg,
		void* user_data) {
	(static_cast<LibbotOpenCV *>(user_data))->on_libbot(msg);
}

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

LibbotOpenCV* libbot_opencv;

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

    lcm_t* lcm = lcm_create(NULL);
    libbot_opencv = new LibbotOpenCV(lcm);


	while(0 == lcm_handle(lcm));

	delete libbot_opencv;

    return 0;
}


