#pragma once

#define TOUCH_BOARD_VID 10862
#define TOUCH_BOARD_PID 32771

namespace ofxTB {
	// TODO: check what is the 13th electrode
	const int ELECTRODES_NB = 12;

	const int DATA_NB_LINE = 6;

	struct Electrode {
		// Touch
		bool touch;
		// Touch threshold
		float tths;
		// Release threshold
		float rths;
		// Filtered data
		float fdat;
		// Baseline val
		float bval;	
		// Difference
		float diff;
	};
};
