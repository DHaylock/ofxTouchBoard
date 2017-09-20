#include "Serial.h"

#ifdef TARGET_OSX
	#include "InterfacesOSX.h"
#endif

#ifdef TARGET_WIN32  

#endif



//-----------------------------------------------------
void Serial::setup()
{
	bFoundDevice = false;
	
	#ifdef TARGET_OSX
		Cougar::InterfacesOSX *interfaces;
		interfaces = new Cougar::InterfacesOSX();
		touchBoardDeviceName = "";
		
		for (int i = 0; i < interfaces->GetDevices().size(); i++)
		{
			if(interfaces->GetDevices()[i].parent.productId == TOUCH_BOARD_PID && interfaces->GetDevices()[i].parent.vendorId == TOUCH_BOARD_VID && interfaces->GetDevices()[i].parent.type != Cougar::DeviceType::BLUETOOTH_DEVICE)
			{
				touchBoardDeviceName = "/dev/tty."+ interfaces->GetDevices()[i].name;
				bFoundDevice = true;
				break;
			}
		}
	#endif
	
	init();
}

//-----------------------------------------------------
void Serial::init()
{
	data.resize(ofxTB::ELECTRODES_NB);
	normalizedData.resize(ofxTB::ELECTRODES_NB);
	baudRate = 57600;
	bConnected = false;
	connect();
}

//-----------------------------------------------------
void Serial::connect()
{
	if(touchBoardDeviceName.size() == 0) cout << "Blah" << endl;
	else
	{
		serial.setup(touchBoardDeviceName, baudRate);
		if(!serial.isInitialized()){
			ofLog() << "Connection to device " << touchBoardDeviceName << " failed.";
			ofSleepMillis(1000);
	//		connect();
		}
	}
}

//-----------------------------------------------------
bool Serial::foundDevice()
{
	return bFoundDevice;
}

//-----------------------------------------------------
void Serial::threadedFunction()
{
	while(isThreadRunning())
	{
		readLine();
	}
}

//-----------------------------------------------------
int Serial::available()
{
    return serial.available();
}

//-----------------------------------------------------
vector<ofxTB::Electrode> Serial::getData()
{
	return data;
}

//-----------------------------------------------------
vector<ofxTB::Electrode> Serial::getNormalizedData()
{
	copy(data.begin(), data.end(), normalizedData.begin());
	float tenBits = 1024.0;
	float eightBits = 256.0;
	for(vector<ofxTB::Electrode>::iterator d = normalizedData.begin(); d != normalizedData.end(); ++d){
		d->tths = float(d->tths) / eightBits;
		d->rths = float(d->rths) / eightBits;
		d->fdat = float(d->fdat) / tenBits;
		d->bval = float(d->bval) / tenBits;
		d->diff = float(d->diff) / tenBits;
	}		
	return normalizedData;
}

//-----------------------------------------------------
void Serial::readLine()
{
	stringstream ss;
	char dataByte = serial.readByte();
	while(dataByte != '\r'){
		if(serial.available() > 0){
			ss << dataByte;
			dataByte = serial.readByte();
		}
		else{
			if(!serial.isInitialized()){
				connect();
			}
		}
	}
	string dataKey;
	if(ss >> dataKey){
		int j = 0;
		int val;
		for(int i = 0; i < data.size(); ++i){
			if(ss >> val){
				if(dataKey == "TOUCH:"){
					if(val == 1){
						data[i].touch = true;				
					}
					else{
						data[i].touch = false;				
					}
				}
				else if(dataKey == "TTHS:"){
					data[i].tths = val;
				}
				else if(dataKey == "RTHS:"){
					data[i].rths = val;
				}
				else if(dataKey == "FDAT:"){
					data[i].fdat = val;						
				}
				else if(dataKey == "BVAL:"){
					data[i].bval = val;
				}
				else if(dataKey == "DIFF:"){
					data[i].diff = val;
				}
				else{
					// ofLogError() << "Unrecognized key: " << dataKey; 
				}
			}
		}
	}
}

//-----------------------------------------------------
void Serial::logData()
{
	cout << left << setw(4) << "EX"
	<< left << setw(6) << "TOUCH"
	<< left << setw(5) << "TTHS" 
	<< left << setw(5) << "RTHS"
	<< left << setw(5) << "FDAT"
	<< left << setw(5) << "BVAL"
	<< left << setw(5) << "DIFF" << endl;

	for(int i = 0; i < data.size(); ++i)
	{
		ofxTB::Electrode e(data[i]);
		cout << left << setw(1) << "E"
		<< left << setw(3) << i
		<< left << setw(6) << e.touch
		<< left << setw(5) << e.tths
		<< left << setw(5) << e.rths
		<< left << setw(5) << e.fdat
		<< left << setw(5) << e.bval
		<< left << setw(5) << e.diff << endl;
	}
}
