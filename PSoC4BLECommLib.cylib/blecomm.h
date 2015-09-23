#if !defined(blecomm_H)
    #define blecomm_H
        
    #include "blecomm_options.h"

    
    #if BLECOMM_MODE == BLECOMM_MODE_CHAR
        typedef void (*ON_MSG)(char*, unsigned long);
        void BLEOnMessage(char*, unsigned long);
        void BLESend(char* data, unsigned long len);
    #endif

    void BLEInit(ON_MSG onMessage);
    void BLEPingOut();
    void BLEPingIn();
    void BLETimoutWarn();
    void BLETimout();
    void BLESendData(char* data, unsigned long len);
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
    unsigned long secondsSince = 0;
    unsigned long deviceConnected = FALSE;
    unsigned long toNotify = FALSE;	
    unsigned long ackNotificationReq = FALSE;
    unsigned long hasDataToSend = FALSE;
    char msgResp[BLECOMM_BUF_LEN];
    char pingIn[] = {BLECOMM_PING_IN,BLECOMM_EOM_FIRST,BLECOMM_EOM_SECOND};
    char pingOut[] = {BLECOMM_PING_OUT,BLECOMM_EOM_FIRST,BLECOMM_EOM_SECOND};
    char timoutWarn[] = {BLECOMM_TIMEOUT_WARN,BLECOMM_EOM_FIRST,BLECOMM_EOM_SECOND};
    char timeout[] = {BLECOMM_TIMEOUT,BLECOMM_EOM_FIRST,BLECOMM_EOM_SECOND};
    ON_MSG onMsgPtr;

    void BLEInit(ON_MSG onMessage)
    {
        onMsgPtr = onMessage;
    }

    void BLESecondsTimerPing() 
    {
        if (FALSE == in_sync) {
            BLEPingIn();
        }
        if (TRUE == deviceConnected) {
            secondsSince++;
            if (BLECOMM_TIMEOUT_WARNING_SECONDS == secondsSince) {
                BLETimoutWarn();
            }
            if (BLECOMM_TIMEOUT_SECONDS == secondsSince) {
                BLETimout();
            }
            if (BLECOMM_TIMEOUT_SECONDS < secondsSince) {
                BLEDisconnect();
            }
        }
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
        unsigned long index = 0;
        if (2 < len) {
            if (BLECOMM_EOM_FIRST == data[len-2] && BLECOMM_EOM_SECOND == data[len-1]) {
                switch(data[0]) {
                    case BLECOMM_PING_IN:
                        BLEPingIn();
                    break;
                    case BLECOMM_PING_OUT:
                        BLEPingOut();
                    break;
                    case BLECOMM_DATA:
                        if (NULL != onMsgPtr) {
                            for(index=1;index < len-2;index++) {
                                data[index-1] = data[index];
                            }
                            onMsgPtr(data, len - 3);
                        }
                    break;
                }
            } else {
                //Invalid Packet Format
            }
        }
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

    void BLESendData(char* data, unsigned long len)
    {
        if (BLECOMM_BUF_LEN >= (len + 4)) {
            unsigned long index = 0;
            msgResp[0] = '\0';
            msgResp[0] = BLECOMM_DATA;
            for(index=0;index < len;index++) {
                msgResp[index+1] = data[index];
            }
            msgResp[index++] = BLECOMM_EOM_FIRST;
            msgResp[index++] = BLECOMM_EOM_SECOND;
            msgResp[index++] = '\0';
            hasDataToSend = TRUE;
        } else {
            //Invalid packet size
        }
    }

    void BLEPingOut() 
    {
        in_sync = TRUE;
        BLESend(pingOut, 3);
    }

    void BLEPingIn()
    {
        BLESend(pingIn, 3);
    }

    void BLETimoutWarn()
    {
        BLESend(timoutWarn, 3);
    }
    
    void BLETimout()
    {
        BLESend(timeout, 3);
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
