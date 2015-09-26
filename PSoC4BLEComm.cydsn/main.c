#include "ble_comm_options.h"

CY_ISR(MY_ISR) {
    BLEMilliSecondsTimerPing();
}

void OnMessage(char* data, unsigned long len) {
    char resp[200];
    strcpy(resp,"Got your message ");
    strcat(resp,data);
    BLESend(resp,strlen(resp));
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    Ping_Timer_Start();
    Ping_ISR_StartEx(MY_ISR);
    
    CyBle_Start(BLEEventHandler);    
    BLEInit(OnMessage);

    for(;;)
    {
    	CyBle_ProcessEvents();
        CheckNotifications();
    }
}

/* [] END OF FILE */
