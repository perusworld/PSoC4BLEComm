#include "ble_comm_options.h"

CY_ISR(MY_ISR) {
    BLEMilliSecondsTimerPing();
}

void OnMessage(char* data, unsigned long len) {
    BLESend("Got your message",strlen("Got your message"));
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
