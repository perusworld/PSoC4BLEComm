#if !defined(ble_comm_options_H)
#define ble_comm_options_H

    #include <project.h>
    #include "blecomm.h"

    #define RX_HANDLE       (CYBLE_COMMUNICATION_SERVICE_INCOMING_CHAR_HANDLE)
    #define TX_CCCD_HANDLE  (CYBLE_COMMUNICATION_SERVICE_OUTGOING_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE)
    #define TX_CCCD_INDEX   (CYBLE_COMMUNICATION_SERVICE_OUTGOING_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX)
    #define TX_HANDLE       (CYBLE_COMMUNICATION_SERVICE_OUTGOING_CHAR_HANDLE)
        
    CYBLE_CONN_HANDLE_T connHandle;
    
    void BLEEventHandler(unsigned long event, void * eventParam)
    {
    	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    	char incoming[BLECOMM_BUF_LEN];
    	
    	switch(event)
        {
            case CYBLE_EVT_STACK_ON:
            case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
    			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
    			break;
    			
    		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
                if(CYBLE_STATE_DISCONNECTED == CyBle_GetState())
                {
                    CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
                }
    			break;
    			
            case CYBLE_EVT_GATT_CONNECT_IND:
    			deviceConnected = TRUE;
                connHandle = *(CYBLE_CONN_HANDLE_T *)eventParam;
                BLEConnected();
    			break;
            
            case CYBLE_EVT_GATT_DISCONNECT_IND:
    			deviceConnected = FALSE;
    			toNotify = FALSE;
    			ackNotificationReq = TRUE;
                msgResp[0] = '\0';
    			break;
                
            case CYBLE_EVT_GATTS_WRITE_REQ: 
                wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
    			
    			if(RX_HANDLE == wrReqParam->handleValPair.attrHandle)
                {
                    if (wrReqParam->handleValPair.value.len > BLECOMM_BUF_LEN) {
                        //OverLoad Ignore
                    } else {
                        int index = 0;
                        for(index=0;index<wrReqParam->handleValPair.value.len;index++) {
                            incoming[index] = wrReqParam->handleValPair.value.val[index];
                        }
                        incoming[index + 1] = '\0';
                        BLEOnMessage(incoming, wrReqParam->handleValPair.value.len);
                    }
                }
                
                
                if(TX_CCCD_HANDLE == wrReqParam->handleValPair.attrHandle)
                {
                    if(FALSE == (wrReqParam->handleValPair.value.val[TX_CCCD_INDEX] & (~CCCD_VALID_BIT_MASK)))
                    {
                        toNotify = wrReqParam->handleValPair.value.val [TX_CCCD_INDEX];
        				ackNotificationReq = TRUE;
                    }
                }             
                
    			CyBle_GattsWriteRsp(cyBle_connHandle);
    			break;
    			
            default:

           	break;
        }   	
    }

    void UpdateNotificationCCCD(void)
    {
    	uint8 ccdValue[2];
    	CYBLE_GATT_HANDLE_VALUE_PAIR_T notificationHandle;
    	if(ackNotificationReq)
    	{
    		ackNotificationReq = FALSE;
    		ccdValue[0] = toNotify;
    		ccdValue[1] = 0x00;
    		notificationHandle.attrHandle = TX_CCCD_HANDLE;
    		notificationHandle.value.val = ccdValue;
    		notificationHandle.value.len = CCC_DATA_LEN;
    		CyBle_GattsWriteAttributeValue(&notificationHandle, ZERO, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
    		CyBle_ProcessEvents();
    	}	
    }

    void CheckDataToSend(void)
    {
    	if(hasDataToSend)
    	{
            hasDataToSend = FALSE;
        	CYBLE_GATTS_HANDLE_VALUE_NTF_T		notificationHandle;	
        	notificationHandle.attrHandle = TX_HANDLE;				
        	notificationHandle.value.val = (uint8 *)msgResp;
        	notificationHandle.value.len = strlen(msgResp);
        	CyBle_GattsNotification(cyBle_connHandle, &notificationHandle);
    	}	
    }

    void BLEDisconnect() 
    {
        CYBLE_API_RESULT_T apiResult;
        apiResult = CyBle_GapDisconnect(connHandle.bdHandle);
        if(CYBLE_ERROR_OK == apiResult) {
            //OK
            BLEDisconnected();
        } else {
            //ERROR
            BLEDisconnected();
        }
    }

#endif /* ble_comm_options_H */
