//Copyright (c) Lockheed Martin Corporation.  All rights reserved. 
//------------------------------------------------------------------------------
//
//  SimConnect Input Sample
//  
//	Description:
//				Ctrl-Shift-U key combination sets the brakes
//------------------------------------------------------------------------------

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "SimConnect.h"
#include <strsafe.h>

int     quit = 0;
HANDLE  hSimConnect = NULL;


enum GROUP_ID {
    GROUP0,
};

enum EVENT_ID {
    EVENT_BRAKES,
 };

void CALLBACK MyDispatchProc1(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
    switch(pData->dwID)
    {
        case SIMCONNECT_RECV_ID_EVENT:
        {
            SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

            switch(evt->uEventID)
            {
                case EVENT_BRAKES:
                    printf("\nEvent brakes: %d", evt->dwData);
                    break;

                default:
                    break;
            }
            break;
        }


        case SIMCONNECT_RECV_ID_QUIT:
        {
            quit = 1;
            break;
        }

        default:
            break;
    }
}

enum INPUT_ID {
    INPUT0,
};

void testInputEvents()
{
    HRESULT hr;

    if (SUCCEEDED(SimConnect_Open(&hSimConnect, "Input Event", NULL, 0, 0, 0)))
    {
        printf("\nConnected to Prepar3D!");   
        
        hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_BRAKES, "brakes");

        hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_BRAKES);

        hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

        // Note that this does not override "." for brakes - both with be transmitted

        hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+u", EVENT_BRAKES);

        hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

  
        while( 0 == quit )
        {
            SimConnect_CallDispatch(hSimConnect, MyDispatchProc1, NULL);
            Sleep(1);
        } 

        hr = SimConnect_Close(hSimConnect);
    }
    else
        printf("\nFailed to Connect");
}

int __cdecl _tmain(int argc, _TCHAR* argv[])
{

    testInputEvents();

	return 0;
}





