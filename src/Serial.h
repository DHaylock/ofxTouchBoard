#pragma once

#include "ofMain.h"

#include "defs.h"

class Serial : public ofThread
{
	ofSerial serial;

	vector<ofxTB::Electrode> data;
	vector<ofxTB::Electrode> normalizedData;

	int deviceNb;
	int baudRate;
	bool bConnected;

	public:
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
		void setup();
	
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
		void threadedFunction();
	
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
        int available();
	
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
		void logData();

	
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
		bool foundDevice();
	
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
		vector<ofxTB::Electrode> getData();
	
		//-----------------------------------------------------
		/// \brief
		//-----------------------------------------------------
		vector<ofxTB::Electrode> getNormalizedData();
		
	private:
		void connect();
		void init();
		void readLine();
	
		string touchBoardDeviceName;
		bool bFoundDevice;
};
