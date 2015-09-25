#if !defined(blecomm_H)
    #define blecomm_H
        
    #include "blecomm_options.h"

    
    #if BLECOMM_MODE == BLECOMM_MODE_CHAR
        typedef void (*ON_MSG)(char*, unsigned long);
        void BLEOnMessage(char*, unsigned long);
        void BLESend(char* data, unsigned long len);
    #endif

    void BLEInit(ON_MSG onMessage);
    void BLESend(char* data, unsigned long len);
    void BLEDisconnect();
    void BLEConnected();
    void BLEDisconnected();
    void BLEEventHandler(unsigned long event, void * eventParam);
    void UpdateNotificationCCCD(void);
    void CheckDataToSend(void);
    void CheckNotifications();

    unsigned short ms_count = 0;
    unsigned long in_sync = FALSE;
    unsigned long deviceConnected = FALSE;
    unsigned long toNotify = FALSE;	
    unsigned long ackNotificationReq = FALSE;
    unsigned long hasDataToSend = FALSE;
    char msgResp[BLECOMM_BUF_LEN];
    ON_MSG onMsgPtr;

    void BLEInit(ON_MSG onMessage)
    {
        onMsgPtr = onMessage;
    }

    void BLESecondsTimerPing() 
    {
    }

    void BLEMilliSecondsTimerPing() 
    {
        ms_count++;
        if(ms_count == 1000) { // 1 second
            ms_count = 0;
            BLESecondsTimerPing();
        }
    }

    void BLEOnMessage(char* data, unsigned long len) {
        onMsgPtr(data, len);
    }

    void BLESend(char* data, unsigned long len) {
        unsigned long index = 0;
        msgResp[0] = '\0';
        for(index=0;index < len;index++) {
            msgResp[index] = data[index];
        }
        msgResp[index] = '\0';
        hasDataToSend = TRUE;
    }

    void BLEConnected()
    {
    }
    
    void BLEDisconnected()
    {
    }

    void CheckNotifications() 
    {
		if(TRUE == deviceConnected)
		{
			UpdateNotificationCCCD();
			if(TRUE == toNotify)
			{
				CheckDataToSend();
			}
		}
    }

#endif /* blecomm_H */
