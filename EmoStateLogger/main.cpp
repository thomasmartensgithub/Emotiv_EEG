/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - EmoStateLogger
** This example demonstrates the use of the core Emotiv API functions.
** It logs all Emotiv detection results for the attached users after
** successfully establishing a connection to Emotiv EmoEngineTM or
** EmoComposerTM
** The Performance Metrics detection works if you have license subscription Performance Metrics
** Example ActivateLicense need to run at least one time on your computer to active your license before run this example.
****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <conio.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SerialPort.h"
#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"

char *port_name = "\\\\.\\COM9";

char incomingData[MAX_DATA_LENGTH];

bool sendState = false;

int previousRead;

SerialPort arduino(port_name);

void readFromArduino() {
	if(!arduino.isConnected()) {
		std::cout << "FOUT: Geen verbinding! (Waarschijnlijk verkeerde port)" << std::endl;
		return;
	}
	if (arduino.isConnected()) {
		int read_result = arduino.readSerialPort(incomingData, MAX_DATA_LENGTH);
		puts(incomingData);
	}
}

void writeToArduino(std::string output) {
	if (!arduino.isConnected()) {
		std::cout << "FOUT: Geen verbinding! (Waarschijnlijk verkeerde port)" << std::endl;
		return;
	}
	else {
		char *c_string = new char[output.size() + 1];
		std::copy(output.begin(), output.end(), c_string);
		c_string[output.size()] = '\n'; 
		arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
		delete[] c_string;
	}
	readFromArduino();
}

#ifdef __cplusplus
extern "C"
{
#endif

void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader = false);

void useEmoState(EmoStateHandle eState);


int main(int argc, char** argv) {

	EmoEngineEventHandle eEvent			= IEE_EmoEngineEventCreate();
	EmoStateHandle eState				= IEE_EmoStateCreate();
	unsigned int userID					= 0;
	const unsigned short composerPort	= 1726;
	int option = 0;
	int state  = 0;
	std::string input;
    std::string filename = "emostate_logger.csv";
	try {
		
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Example to show how to log the EmoState from EmoEngine/EmoComposer."
                  << std::endl;
        std::cout << "==================================================================="
                  << std::endl;
        std::cout << "Press '1' to start and connect to the EmoEngine                    "
                  << std::endl;
        std::cout << "Press '2' to connect to the EmoComposer                            "
                  << std::endl;
		std::cout << ">> ";

		std::getline(std::cin, input, '\n');
		option = atoi(input.c_str());

		switch (option) {
        case 1:
        {
            if (IEE_EngineConnect() != EDK_OK) {
                throw std::runtime_error("Emotiv Driver start up failed.");
            }

            break;
        }
        case 2:
        {
            std::cout << "Target IP of EmoComposer? [127.0.0.1] ";
            std::getline(std::cin, input, '\n');

            if (input.empty()) {
                input = std::string("127.0.0.1");
            }

            if (IEE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
                std::string errMsg = "Cannot connect to EmoComposer on [" +
                                            input + "]";
                throw std::runtime_error(errMsg.c_str());
            }
            break;
        }
        default:
            throw std::runtime_error("Invalid option...");
            break;
		}
		

        std::cout << "Start receiving EmoState! Press any key to stop logging...\n"
                  << std::endl;

        std::ofstream ofs(filename.c_str(), std::ios::trunc);
		bool writeHeader = true;
		
		while (!_kbhit()) {

			state = IEE_EngineGetNextEvent(eEvent);

			// New event needs to be handled
			if (state == EDK_OK) {

				IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
				IEE_EmoEngineEventGetUserId(eEvent, &userID);

				// Log the EmoState if it has been updated
				if (eventType == IEE_EmoStateUpdated) {

					IEE_EmoEngineEventGetEmoState(eEvent, eState);
					const float timestamp = IS_GetTimeFromStart(eState);

                    std::cout << timestamp << ": " << "New EmoState from user " << userID << std::endl;
					
					logEmoState(ofs, userID, eState, writeHeader);
					useEmoState(eState);
					writeHeader = false;
				}
			}
			else if (state != EDK_NO_EVENT) {
				std::cout << "Internal error in Emotiv Engine!" << std::endl;
				break;
			}
            Sleep(1);
		}

		ofs.close();
	}
    catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		std::cout << "Press any key to exit..." << std::endl;
		getchar();
	}

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);

	return 0;
}


void useEmoState(EmoStateHandle eState){
	int data = IS_FacialExpressionIsBlink(eState);
	if (data > 0 && sendState == false) {
		std::cout << "Geknippert: " << data << std::endl;
		writeToArduino("AAN");
		sendState = true;
	} 
//	else if(sendState == true) {
//		writeToArduino("UIT");
//		sendState = false;
//	}

}

void logEmoState(std::ostream& os, unsigned int userID,
                 EmoStateHandle eState, bool withHeader) {

	// Create the top header
	if (withHeader) {
		os << "Time,";
		os << "Wireless Signal Status,";

        os << "AF3, F7, F3, FC5, T7, P7, O1, O2, P8, T8, FC6, F4, F8, AF4,";

        os << "||,";

		os << "Blink,";
		os << "Wink Left,";
		os << "Wink Right,";
		os << "Surprise,";
		os << "Frown,";
		os << "Smile,";
		os << "Clench,";
        
		os << std::endl;
	}

	// Log the time stamp and user ID
	os << IS_GetTimeFromStart(eState) << ",";
	os << static_cast<int>(IS_GetWirelessSignalStatus(eState)) << ",";

    //CQ 
    os << IS_GetContactQuality(eState, IEE_CHAN_AF3) << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F7)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F3)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_FC5) << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_T7)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_P7)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_O1)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_O2)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_P8)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_T8)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_FC6) << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F4)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_F8)  << ", ";
    os << IS_GetContactQuality(eState, IEE_CHAN_AF4) << ", ";

    os << "||,";

	// FacialExpression Suite results
	os << IS_FacialExpressionIsBlink(eState) << ",";
	os << IS_FacialExpressionIsLeftWink(eState) << ",";
	os << IS_FacialExpressionIsRightWink(eState) << ",";

	std::map<IEE_FacialExpressionAlgo_t, float> expressivStates;

    IEE_FacialExpressionAlgo_t upperFaceAction =
            IS_FacialExpressionGetUpperFaceAction(eState);
    float upperFacePower  = IS_FacialExpressionGetUpperFaceActionPower(eState);

    IEE_FacialExpressionAlgo_t lowerFaceAction =
            IS_FacialExpressionGetLowerFaceAction(eState);
    float	lowerFacePower  = IS_FacialExpressionGetLowerFaceActionPower(eState);

	expressivStates[ upperFaceAction ] = upperFacePower;
	expressivStates[ lowerFaceAction ] = lowerFacePower;
	
	os << expressivStates[ FE_SURPRISE] << ","; // eyebrow
	os << expressivStates[ FE_FROWN   ] << ","; // furrow
	os << expressivStates[ FE_SMILE   ] << ","; // smile
	os << expressivStates[ FE_CLENCH  ] << ","; // clench

	os << std::endl;
    os.flush();
}

#ifdef __cplusplus
}
#endif
