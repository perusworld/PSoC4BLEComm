#if !defined(blecomm_options_H)
#define blecomm_options_H

    #if !defined(NO)
        #define NO                          (0u)
    #endif
    #if !defined(YES)
        #define YES                         (1u)
    #endif
    #if !defined(FALSE)
        #define FALSE                          (0u)
    #endif
    #if !defined(TRUE)
        #define TRUE                         (1u)
    #endif
    #if !defined(ZERO)
        #define ZERO        (0u)
    #endif

    #if !defined(ENCRYPTION_ENABLED)
        #define ENCRYPTION_ENABLED          (NO)
    #endif

    #if !defined(DO_DEBUG)
        #define DO_DEBUG                    (NO)
    #endif

    #if !defined(NULL)
        #define NULL ((void *)0)
    #endif

    #if !defined(BLECOMM_TIMEOUT_WARNING_SECONDS)
        #define BLECOMM_TIMEOUT_WARNING_SECONDS     10
    #endif
    #if !defined(BLECOMM_TIMEOUT_SECONDS)
        #define BLECOMM_TIMEOUT_SECONDS             15
    #endif

    #define ERR_INVALID_PDU         0x04 
    #define CCCD_VALID_BIT_MASK     0x03 
    #define NOTIFY_BIT_MASK         0x01
    #define CCC_DATA_LEN            (2)

    #define BLECOMM_MODE_BYTE       (0x01)
    #define BLECOMM_MODE_CHAR       (0x02)
    #define BLECOMM_MODE_JSON       (0x03)

    #if !defined(BLECOMM_MODE)
        #define BLECOMM_MODE       (BLECOMM_MODE_CHAR)
    #endif
        
    #define BLECOMM_BUF_LEN        (200)
    #define BLECOMM_PING_IN        (0XCC)
    #define BLECOMM_PING_OUT       (0XDD)
    #define BLECOMM_DATA           (0XEE)
    #define BLECOMM_TIMEOUT_WARN   (0XFC)
    #define BLECOMM_TIMEOUT        (0XFD)
    #define BLECOMM_EOM_FIRST      (0XFE)
    #define BLECOMM_EOM_SECOND     (0XFF)


#endif /* blecomm_options_H */
