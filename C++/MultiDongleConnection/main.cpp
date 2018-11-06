/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - Multi Dongle Connection
** this example demonstrates how to connect to two headsets at the same time
** It captures event when you plug or unplug dongle .
** Every time you plug or unplug dongle, there is a notice that dongle ID
** is added or removed
**
****************************************************************************/

#include <iostream>
#include <list>

#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IEmoStateDLL.h"

using namespace std;

#ifdef _WIN32
#include <conio.h>
#endif

#if __linux__ || __APPLE__
    #include <unistd.h>
    int _kbhit(void);
#endif

int main(int argc,char* argv[])
{
	EmoEngineEventHandle hEvent = IEE_EmoEngineEventCreate();
	EmoStateHandle eState = IEE_EmoStateCreate();
	unsigned int userID = 0;
	list<int> listUser;
    

	if( IEE_EngineConnect() == EDK_OK )
	{
		while(!_kbhit()) 
		{
			int state = IEE_EngineGetNextEvent(hEvent);
			if( state == EDK_OK )
			{
				IEE_Event_t eventType = IEE_EmoEngineEventGetType(hEvent);				
				IEE_EmoEngineEventGetUserId(hEvent, &userID);		

				if(eventType == IEE_EmoStateUpdated  )
				{								
					IEE_EmoEngineEventGetEmoState(hEvent, eState);
					cout<< "User " << userID << ": " << IS_GetTimeFromStart(eState) << endl;										
				}
				// userremoved event
				else if( eventType == IEE_UserRemoved )
				{
					cout <<"user ID: "<<userID<<" have removed" << endl;	
					listUser.remove(userID);
				}
				// useradded event 
				else if(eventType == IEE_UserAdded)
				{
					listUser.push_back(userID);
					cout <<"user ID: "<<userID<<" have added" << endl;
				}		
			}			
		}
	}

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(hEvent);	

	return 0;
}

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);

    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
    return 0;

    if(FD_ISSET(0,&read_fd))
        return 1;

    return 0;
}
#endif
#ifdef __APPLE__
int _kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif
