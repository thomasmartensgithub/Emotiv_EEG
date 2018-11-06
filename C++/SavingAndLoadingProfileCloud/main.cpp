/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - SavingAndLoadingProfileCloud
** How to saving and loading prfile from Emotiv Cloud
**
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#if __linux__ || __APPLE__
    #include <unistd.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if __linux__ || __APPLE__
    int _kbhit(void);
#endif

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "EmotivCloudClient.h"


    int  main() {
        std::string userName = "EmotivID";
        std::string password = "Password";
        std::string profileName = "";
        std::string pathOfProfile;

        int version = -1; // Lastest version

        int option = 0;
        std::string input;
        bool ready = false;

        EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
        EmoStateHandle eState = IEE_EmoStateCreate();
        unsigned int engineUserID = 0;
        int userCloudID = -1;
        int state = 0;

        int result = -1;

        result = IEE_EngineConnect();
        if (result != EDK_OK) {
            std::cout << "Emotiv Driver start up failed.";
            return result;
        }

        result = EC_Connect();
        if (result != EDK_OK)
        {
            std::cout << "Cannot connect to Emotiv Cloud";
            return result;
        }

        result = EC_Login(userName.c_str(), password.c_str());
        if (result != EDK_OK)
        {
            std::cout << "Your login attempt has failed. The username or password may be incorrect";
#ifdef _WIN32
            _getch();
#endif
            return result;
        }

        std::cout << "Logged in as " << userName << std::endl;

        result = EC_GetUserDetail(&userCloudID);
        if (result != EDK_OK)
        {
            std::cout << "Can not get the cloud ID of user" << std::endl;
            return result;
        }


        std::cout << "==================================================================="
            << std::endl;
        std::cout << "Example for saving and loading profile from Emotiv Cloud. "
            << std::endl;
        std::cout << "==================================================================="
            << std::endl;
        std::cout << "Press '1' to update or save a profile to Emotiv Cloud "
            << std::endl;
        std::cout << "Press '2' to load all profiles from Emotiv Cloud "
            << std::endl;
        std::cout << "Press '3' to upload a local profile to Emotiv Cloud "
            << std::endl;
        std::cout << "Press '4' to download a profile from Emotiv Cloud and save to local "
            << std::endl;
        std::cout << ">> ";

        //get option
        std::cout << "Option: " << std::endl;

        std::getline(std::cin, input, '\n');
        option = atoi(input.c_str());
        switch (option)
        {
            case 1:
            case 2:
                std::cout << "Please connect to headset..." << std::endl;
                break;
            case 3:
            {            
                std::cout << "Please enter the absolute local profile path: " << std::endl; //example //E:/profile_1.emu
                std::getline(std::cin, pathOfProfile);

                std::cout << "Please enter the name of profile: " << std::endl; //name of profile will be shown on Emotiv Cloud
                std::getline(std::cin, profileName);

                bool overwrite_if_exists = true;
                result = EC_UploadProfileFile(userCloudID, profileName.c_str(), pathOfProfile.c_str(), TRAINING, overwrite_if_exists);

                if (result == EDK_OK)
                    std::cout << "The profile is uploaded successfully\n";
                else
                    std::cout << "Uploaded unsuccessfully\n";

                goto END;
            }
            case 4:
            {
                std::cout << "Please enter the profile name: " << std::endl;
                std::getline(std::cin, profileName);

                result = -1; //reset result
                int profileID = -1;
                result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);
                if (result == EDK_OK && profileID >= 0)
                {
                    std::cout << "Absolute destination profile path (profile name included) to save: " << std::endl; //Example E:/profile_1.emu
                    std::getline(std::cin, pathOfProfile);

                    result = EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str());
                    if (result == EDK_OK)
                        std::cout << "Download the profile successfully\n" << std::endl;
                    else
                    {
                        std::cout << "Download the profile unsuccessfully\n" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Can not get the profile. Please make sure the correct profile name.\n" << std::endl;
                }

                goto END;
            }
            default:
                break;
        }

    while (!_kbhit())
    {
        state = IEE_EngineGetNextEvent(eEvent);

        if (state == EDK_OK) {

            IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
            IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);

            if (eventType == IEE_UserAdded) {
                std::cout << "User added" << std::endl;
                ready = true;
            }

        }
        if (ready)
        {
            //option 1
            if (option == 1)
            {
                int profileID = -1;
                std::cout << "Please enter the profile name: " << std::endl;
                std::getline(std::cin, profileName);

                result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

                if (profileID >= 0) {
                    std::cout << "Profile " << profileName << " is existed" << std::endl;
                    result = EC_UpdateUserProfile(userCloudID, engineUserID, profileID);

                    if (result == EDK_OK) {
                        std::cout << "The profile has updated successfully\n";
                    }
                    else std::cout << "The profile has updated unsuccessfully\n";
                }
                else {
                    std::cout << "Profile " << profileName << " is non-existed" << std::endl;
                    result = EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING);

                    if (result == EDK_OK)
                    {
                        std::cout << "The profile has saved successfully \n";
                    }
                    else std::cout << "The profile has saved unsuccessfully\n";
                }
                break;
            }
            else if (option == 2)
            {
                //Get all profiles from Emotiv Cloud
                int getNumberProfile = EC_GetAllProfileName(userCloudID);

                std::cout << "Number of profiles: " << getNumberProfile << "\n";

                if (getNumberProfile > 0){
                    for (int i = 0; i < getNumberProfile; i++)
                    {
                        std::string profileName(EC_ProfileNameAtIndex(userCloudID, i));
                        int profileID = EC_ProfileIDAtIndex(userCloudID, i);
                        std::cout << "Profile Name: " << profileName << ", ";

                        std::cout << "Profile ID: " << profileID << ", ";
                        std::cout << "Profile type: " <<
                            ((EC_ProfileTypeAtIndex(userCloudID, i) == profileFileType::TRAINING) ? "TRAINING" : "EMOKEY") << ", ";

                        std::cout << EC_ProfileLastModifiedAtIndex(userCloudID, i) << ",\r\n";

                        //Load profile
                        result = EC_LoadUserProfile(userCloudID, (int)engineUserID, profileID);

                        if (result == EDK_OK)
                            std::cout << "Profile: " << profileName << " loaded successfully\n";
                        else
                            std::cout << "Profile: " << profileName << " loaded unsuccessfully\n";
                    }
                }
                break;
            }
            else
            {
                std::cout << "Invalid option..." << std::endl;
                break;
            }
            
        }

#ifdef _WIN32
        Sleep(1);
#endif
#if __linux__ || __APPLE__
        usleep(10000);
#endif
    }
    
END:
    IEE_EngineDisconnect();
    IEE_EmoStateFree(eState);
    IEE_EmoEngineEventFree(eEvent);
#ifdef _WIN32
    _getch();
#endif
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
#ifdef __cplusplus
}
#endif
