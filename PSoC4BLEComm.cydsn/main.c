#include "ble_comm_options.h"

CY_ISR(MY_ISR) {
    BLEMilliSecondsTimerPing();
}

void OnMessage(char* data, unsigned long len) {
//    SerialDataBegin();
//    SerialSendString("got your message data");
//    SerialSendBytes(data, len);
//    SerialDataEnd();
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    Ping_Timer_Start();
    Ping_ISR_StartEx(MY_ISR);
    
    CyBle_Start(BLEEventHandler);    

    for(;;)
    {
    	CyBle_ProcessEvents();
        CheckNotifications();
    }
}

/* [] END OF FILE */
