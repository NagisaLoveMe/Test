/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  MobileTech(Shanghai) Co., Ltd.
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   oa_global.h
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 * 
 *
 * Author:
 * -------
 *   simon.
 *
 ****************************************************************************/

#include "oa_type.H"

#ifndef _OA_GLOBAL_H_
#define _OA_GLOBAL_H_

#define SOURCECODE_VERSION  "SOURCE CODE VERSION: 1.1.0.0\r\n"
#define SOURCECODE_BRIEF    "版本说明:WD-扬尘GPRSv1.1.0.0。\r\n"

#define SOURCECODE_VERSION_FRAME	1
#define SOURCECODE_VERSION_RELEASE	1
#define SOURCECODE_VERSION_BETA		0
#define SOURCECODE_VERSION_DEBUG	0



/*******************************************************************************
    MY ENUM
*******************************************************************************/
/****** PROTOCOL *****/
typedef enum {
    LSL_PROT_SMS = 1,
    LSL_PROT_NET = 2,
    LSL_PROT_UART = 3
} lsl_protocol_enum;


/****** GPRS ******/
/* oa_type.H Line635 -> soc event enum */

typedef enum {
    LSL_CON_MODE_DOMAIN  = 0,
    LSL_CON_MODE_IP      = 1
} lsl_connect_mode_enum;

/*******************************************************************************
    MY MACRO
*******************************************************************************/

#define MAX_GPRS_RX_PENDING_LEN  1024
#define MAX_GPRS_TX_PENDING_LEN  1024

#define SMS_RX_BUFFERLEN        256
#define SMS_TX_BUFFERLEN        256

/*******************************************************************************
    MY STRUCT
*******************************************************************************/

//  自定义回调   函数指针
//typedef void (*gprs_receive_cb_fun)(oa_uint8 sock_id,oa_uint8 *datas,oa_uint16 len);
typedef void (*client_receive_cb_fun)(oa_uint8 sock_id,oa_uint8 *datas,oa_uint16 len);



/**************↓ LSL FACTORY SETTING ↓***************/

#define USING_DEBUG_PROTOCOL    0

#define PROTOCOL_LIMIT          1

#define USER_COM_STR_LEN_MAX    256

/*--- PASSWORD ---*/
#define PASSWORD_LEN_MIN    6
#define PASSWORD_LEN_MAX    19

/*--- HEART PKT ---*/
#define HEART_TIME_MIN      30
#define HEART_TIME_MAX      4147200 // 4147200sec = 48 days
#define HEART_PKT_LEN_MAX   252

/*--- VIRTUAL ID ---*/
#define VIRTUAL_ID_LEN      8

/*--- PHONE NUM ---*/
#define PHONE_NUM_MAX       8

/*****************↑ END OF LSL FACTORY SETTING ↑*****/


/********************↓ LSL MCU ↓*********************/

#define GPIO_MODE_INPUT     0
#define GPIO_MODE_OUTPUT    1

#define GPIO_LOW            0
#define GPIO_HIGH           1

#define LSL_TIMER_485TX_ON      OA_TIMER_ID_10
#define LSL_TIMER_485TX_OFF     OA_TIMER_ID_11
#define LSL_TIMER_LINK_LED      OA_TIMER_ID_12
#define LSL_TIMER_RING_LED      OA_TIMER_ID_13
#define LSL_TIMER_HEART_PKT     OA_TIMER_ID_14
#define LSL_TIMER_FACTORY_SET   OA_TIMER_ID_15

/*******************************↑ END OF LSL MCU ↑***/



/*******************↓ LSL HARDWARE ↓*****************/

#define LINK_LED_IO         GPIO_P71
#define RING_LED_IO         GPIO_P56
#define CTRL_485_IO         GPIO_P55

#define GPIO_P37            4
#define GPIO_P55            61
#define GPIO_P56            40
#define GPIO_P71            42

/**************************↑ END OF LSL HARDWARE ↑***/


/*********************↓ LSL GPIO ↓*******************/

#define PIN_LINK_LED_HIGH()     oa_gpio_write(GPIO_HIGH,LINK_LED_IO)
#define PIN_LINK_LED_LOW()      oa_gpio_write(GPIO_LOW,LINK_LED_IO)

#define PIN_RING_LED_HIGH()     oa_gpio_write(GPIO_HIGH,RING_LED_IO)
#define PIN_RING_LED_LOW()      oa_gpio_write(GPIO_LOW,RING_LED_IO)

#define PIN_485CTRL_HIGH()      oa_gpio_write(GPIO_HIGH,CTRL_485_IO)
#define PIN_485CTRL_LOW()       oa_gpio_write(GPIO_LOW,CTRL_485_IO)

/******************************↑ END OF LSL GPIO ↑***/

/*******************↓ LSL PROTOCOL ↓*****************/
typedef enum
{
    LSL_SMS_CMD,
    LSL_SOC_CMD,
    LSL_UART_CMD
} lsl_cmd_enum;

typedef enum
{
    LSL_CMD_TRANS = 0,
    LSL_CMD_RIGHT,
    LSL_CMD_SAVE_ERROR,
    LSL_CMD_NOT_SUPPORT,
    LSL_CMD_FIN,
    LSL_CMD_RESERVED,
    
    LSL_CMD_NOT_EXIST,
    LSL_CMD_PASSWORD_ERR,
    LSL_CMD_WRONG,
    LSL_CMD_PHONE_NOT_BIND,
    LSL_CMD_PHONE_NOT_IN_TABLE,
    LSL_CMD_PHONE_TABLE_EMPTY,
    LSL_CMD_PHONE_TABLE_FULL,
    LSL_CMD_NEW_PSW_ILLEGAL,
    
    LSL_CMD_NOT_EXIST_WARNING,
    LSL_CMD_PASSWORD_ERR_WARNING,
    LSL_CMD_WRONG_WARNING,
    LSL_CMD_PHONE_NOT_BIND_WARNING,
    LSL_CMD_NEW_PSW_ILLEGAL_WARNING,
} lsl_cmd_state;

/**************************↑ END OF LSL PROTOCOL ↑***/


/*******************↓ LSL SETTING ↓******************/
//#define DOMAIN_NAME_MAX_LEN     128         /**/
//#define IP_ADDRESS_MAX_LEN      16          /**/

/***************************↑ END OF LSL SETTING ↑***/


/******************↓ LSL STRUCT ↓********************/
typedef struct _password_s
{
    oa_uint8    chr[PASSWORD_LEN_MAX];
    oa_uint8    len;
} PASSWORD_T;

typedef struct _heart_pkt_s
{
    oa_uint32   t;
    oa_uint8    chr[HEART_PKT_LEN_MAX];
} HEART_PKT_T;

/*--------------------------------------
    MODULE CONFIG
--------------------------------------*/
typedef struct _module_config_s
{   
    PASSWORD_T      psw_t;
    HEART_PKT_T     heart_t;
    oa_uint8        vir_id[VIRTUAL_ID_LEN];
    oa_uint8        phone[PHONE_NUM_MAX][12];
    oa_uint16   auto_upload_time;
    oa_uint16   RESERVED1;
} MODULE_CONFIG_T;
extern MODULE_CONFIG_T gModuleConfigT;


/*--------------------------------------
        GSM
--------------------------------------*/

typedef struct _sim_state_s
{
    oa_bool     sim_valid;
    oa_uint8    RESERVED1;
    oa_uint8    RESERVED2;
    oa_uint8    RESERVED3;  
} SIM_STATE_T;
extern SIM_STATE_T gSimStateT;

/*--------------------------------------
        SMS
--------------------------------------*/
typedef struct _sms_state_s
{
    oa_uint8    rx[SMS_RX_BUFFERLEN];
    oa_uint8    tx[SMS_TX_BUFFERLEN];
    oa_uint8    phone[12];
    oa_smsal_dcs_enum   sms_dcs;
    oa_uint8    RESERVED1;
    oa_uint8    RESERVED2;
    oa_uint8    RESERVED3;
} SMS_STATE_T;
extern SMS_STATE_T gSmsStateT;

/*--------------------------------------
        UART
--------------------------------------*/


typedef struct _uart_485_dat
{
    oa_uint8*   pDat;
    oa_uint32   len;
} UART_485_DAT;

extern UART_485_DAT gUart485DatT;



/*--------------------------------------
    MODULE STATE
--------------------------------------*/
typedef struct _module_state_s
{
    oa_uint32   tick;
    oa_uint32   try_connect_tick;
    oa_uint32   try_failed_tick;
    oa_uint8    user_com_str[USER_COM_STR_LEN_MAX];

    
} MODULE_STATE_T;
extern MODULE_STATE_T gModuleStateT;


///*--------------------------------------
//        PHONES TABLE
//--------------------------------------*/
//typedef struct _phone_info_s {
//    oa_uint8 index;
//    oa_uint8 notice;
//    oa_uint8 w1;
//    oa_uint8 w2;
//    oa_uint8 num[12];
//} PHONE_INFO_T;
//
//typedef struct _phone_tab_s {
//    PHONE_INFO_T    phone[16];
//} PHONE_TAB_T;
//extern PHONE_TAB_T gPhoneTabT;
//
///*--------------------------------------
//        DEBUG
//--------------------------------------*/
//#define GEOCODING_TEST  0x55
//#define GEOCONV_TEST    0x33
//#define DISTANCE_TEST   0xAA
//
//typedef struct _debug_variables_s
//{
//    oa_uint8    RESERVED1;  
//    oa_uint8    RESERVED2;
//    oa_uint8    RESERVED3;
//    oa_uint8    RESERVED4;
//
//    
//    oa_uint16   debug_port;
//    oa_uint8    RESERVED5;
//    oa_uint8    du_api_test;
//
//    oa_uint8    debug_ip[16];
//    oa_uint8    debug_domain_name[64];
//} DEBUG_VARIABLES_T;
//extern DEBUG_VARIABLES_T gDebugVariablesT;

/****************************↑ END OF LSL STRUCT ↑***/




/****************↓ LSL OPERATE MACRO ↓***************/
#define PHONE_CHECK_BINDING()   if(LSL_SMS_CMD == c)                            \
                                {                                               \
                                    if(OA_FALSE == lsl_check_phone_binding())   \
                                    {                                           \
                                        return LSL_CMD_PHONE_NOT_BIND_WARNING;  \
                                    }                                           \
                                }

#define PASSWORD_CHK()  if(oa_strncmp(psw,gModuleConfigT.psw_t.chr,gModuleConfigT.psw_t.len))   \
                        {                                                                       \
                            if(OA_TRUE == warn_msg) { return LSL_CMD_PASSWORD_ERR_WARNING; }    \
                            else{ return LSL_CMD_PASSWORD_ERR; }                                \
                        }                                                                       \
                        pd = psw + gModuleConfigT.psw_t.len;                                    \
                        if(':' != *pd)                                                          \
                        {                                                                       \
                            if(OA_TRUE == warn_msg) { return LSL_CMD_PASSWORD_ERR_WARNING; }    \
                            else{ return LSL_CMD_PASSWORD_ERR; }                                \
                        }                                                                       \
                        pd++;                                                                   \


#define USER_COM_STR_ACK(x) switch(c)                                                           \
                            {                                                                   \
                                case LSL_SMS_CMD:                                               \
                                    switch(x)                                                   \
                                    {                                                           \
                                        case OA_SMSAL_DEFAULT_DCS:                              \
                                            lsl_sms_send_7bit(gSmsStateT.phone,gModuleStateT.user_com_str,\
                                            oa_strlen((oa_char *)gModuleStateT.user_com_str));  \
                                            break;                                              \
                                        case OA_SMSAL_8BIT_DCS:                                 \
                                            lsl_sms_send_8bit(gSmsStateT.phone,gModuleStateT.user_com_str,\
                                            oa_strlen((oa_char *)gModuleStateT.user_com_str));  \
                                            break;                                              \
                                        case OA_SMSAL_UCS2_DCS:                                 \
                                            lsl_sms_send_ucs2(gSmsStateT.phone,gModuleStateT.user_com_str,\
                                            oa_strlen((oa_char *)gModuleStateT.user_com_str));  \
                                            break;                                              \
                                    }                                                           \
                                    return LSL_CMD_FIN;                                         \
                                case LSL_SOC_CMD:                                               \
                                    oa_write_buffer_noinit(gClientStateT.gprs_tx,gModuleStateT.user_com_str,\
                                    oa_strlen(gModuleStateT.user_com_str));                                 \
                                    lsl_soc_send_req();                                                     \
                                    return LSL_CMD_FIN;                                                     \
                                case LSL_UART_CMD:                                              \
                                    lsl_uart_485_tx(OA_UART2,gModuleStateT.user_com_str,        \
                                                    oa_strlen(gModuleStateT.user_com_str));     \
                                    return LSL_CMD_FIN;                                         \
                            } 
/*******************↑ END OF LSL OPERATE MACRO ↑*****/




/*********************↓ LSL APP ↓********************/

/*--- oa_app.c ---*/
extern int offline_cnt;



extern oa_uint16 crc16_ccitt(oa_uint8 *p, oa_uint16 len);



extern void lsl_factory_setting_timer_ind(void *p);
extern void my_process_service(void);

extern int lsl_uart_485_tx(oa_uart_enum P, oa_uint8 *q, oa_uint16 n);
extern lsl_cmd_state protocol_analyze(lsl_cmd_enum c,oa_uint8 *dat,oa_uint32 len);


/********************************↑ END OF LSL APP↑***/


/*CUSTOMER define */
#define OPENAT_CUSTOM_XXX  //XXX for customer

//#define OPENAT_SOC_DEMO    //if need use gprs,can open this macro for test 

#endif /*_OA_GLOBAL_H_*/

