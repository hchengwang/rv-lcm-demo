/*
 * rv-libbot-opencv-demo.hpp
 *
 *  Created on: Mar 6, 2016
 *      Author: hchengwang
 */

#ifndef RV_LIBBOT_OPENCV_DEMO_HPP_
#define RV_LIBBOT_OPENCV_DEMO_HPP_

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

#include <jpeg-utils/jpeg-utils.h>
#include <jpeg-utils/jpeg-utils-ijg.h>
#include <zlib.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge_lcm/rv-cv-bridge-lcm.h>

class LibbotOpenCV
{
public:

	LibbotOpenCV(lcm_t* lcm);
	~LibbotOpenCV();

	void on_libbot(const bot_core_image_t *msg);
	static void on_libbot_aux(const lcm_recv_buf_t* rbuf,
			const char* channel,
			const bot_core_image_t* msg,
			void* user_data);

private:

	lcm_t* lcm_;
	cvBridgeLCM* cv_bridge;
};

#endif /* RV_LIBBOT_OPENCV_DEMO_HPP_ */
