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
 *   oa_app.c
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This Module gives a sample app on openat framework.
 *
 * Author:
 * -------
 *   None!
 *
 ****************************************************************************/
#include "oa_global.h"
#include "oa_type.h"
#include "oa_llist.h"
#include "oa_api.h"
#include "oa_setting.h"
#include "oa_sms.h"
#include "oa_soc.h"



const oa_uint16 crc16_1021_tab[256] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};



oa_uint16 crc16_modbus(oa_uint8 *p,oa_uint16 len)
{
	oa_uint16 crc_value = 0xFFFF;
	oa_uint16 n;
	oa_uint16 i;
	oa_uint8  temp_value = 0x00;
	oa_uint8  j;
	n = len;
	for(i = 0;i < n;i++)
	{
		crc_value ^= *(p + i);
		for(j = 0;j < 8;j++)
		{
			if((crc_value & 0x0001) != 0)
			{
				crc_value = (crc_value >> 1) ^ 0xA001;
			}
			else
			{
				crc_value >>= 1;
			}
		}
	}
	temp_value = crc_value >> 8;
	crc_value = ((crc_value << 8) | temp_value) & 0xFFFF;
	return crc_value;
}

oa_uint16 crc16_ccitt(oa_uint8 *p, oa_uint16 len)
{
	oa_uint16 crc_value = 0;
	oa_uint16 n,i;
	oa_uint8 index;
	n = len;
	for(i = 0;i < n;i++)
	{
		index = (crc_value >> 8) ^ *(p + i);
		crc_value = ((crc_value << 8) ^ crc16_1021_tab[index]) & 0xFFFF;
	}
	return crc_value;
}


/********************************* SPECIAL USE ********************************/
static  oa_char     adc_buf[64] = {0};
static  oa_uint32   wait_485[64];

static  oa_char     connent_tx_hrtpkt = 0;
static  oa_uint32   adc_data;
static  oa_uint32   voltage_value = 0;
static  oa_uint32   current_value = 0;


static oa_bool      b_baud_set = OA_FALSE;
static oa_uint32    baud_temp;

static oa_uint8     wait_485_flag = 0;

static oa_uint8     error_to_reset_cnt = 0;
static oa_uint8     block_send_retry = 0;

static oa_uint8     imei_str[16] = { 0 };

static oa_bool      fixid_close_soc_flag = OA_FALSE;    // 是否因配置ID而关闭SOC重连
static oa_bool      cfg_tell_103 = OA_FALSE;            // 是否和103已经同步了配置

static oa_uint8		version_of_other_chip[16] = { 0 };

/******************************************************** END SPECIAL USE *****/



#define OA_APP_SCHEDULER_PERIOD 1000



/*------------------------------------------
    STRUCT
------------------------------------------*/
SIM_STATE_T     gSimStateT;
MODULE_CONFIG_T gModuleConfigT;
MODULE_STATE_T  gModuleStateT;
CLIENT_STATE_T  gClientStateT;
SMS_STATE_T     gSmsStateT;
UART_485_DAT    gUart485DatT;



/*****************************************************************/
/*********** Macro definition for NMEA data output.***************/
/***** Customer can enable/disable the macro definition.**********/ 
/*****************************************************************/
//#ifndef __OA_ONLY_VALID_NMEA_FOR_STATIC_DRIFT_TEST__ 
//#define __OA_ONLY_VALID_NMEA_FOR_STATIC_DRIFT_TEST__
//#endif

/*****************************************************************/
/*------------Customer define for Openat Version-----------------*/
/*****************************************************************/
oa_char OA_VERSION_NO[]="v6.1.5ep";

oa_char LSL_WEICHEN_COPYRIGHT[24] = "WEICHEN@COPYRIGHT";
/*****************************************************************
*-----------Customer define for trace and at cmd control-------- *
*1. g_oa_trace_level: which trace levels user care for;
*2. g_oa_trace_port : which port trace info will output;
*3. g_oa_ps_port     : at cmd control port, default is OA_UART1;
*4. g_oa_tst_port     : Catcher port, default is OA_UART3;
*NOTE: Be sure g_oa_ps_port with g_oa_tst_port is different;
*NOTE: Can modify but do not delete these define
******************************************************************/
oa_trace_level_enum     g_oa_trace_level = OA_TRACE_USER;    //Define debug information output level.
oa_trace_port_enum      g_oa_trace_port  = OA_TRACE_PORT_1; //Define debug information output port.
oa_uart_enum            g_oa_ps_port    = OA_UART1;              //Define the AT ps port, If set OA_UART_AT(virtual AT port) , can release UART1
oa_uart_enum            g_oa_tst_port    = OA_UART_NULL;      //Define the Catcher port,


/*****************************************************************/
/*-----------------End for customer define-----------------------*/
/*****************************************************************/

/*****************************************************************/
/*-----------------For uart port configure-----------------------*/
/*****************************************************************/  
oa_uart_struct g_uart1_port_setting = 
{
    115200,//9600,   /* baudrate */
    oa_uart_len_8,              /* dataBits; */
    oa_uart_sb_1,               /* stopBits; */
    oa_uart_pa_none,            /* parity; */
    oa_uart_fc_none,            /* flow control */
    0x11,               /* xonChar; */
    0x13,               /* xoffChar; */
    OA_FALSE
};
oa_uart_struct g_uart2_port_setting = 
{
    9600,//9600,   /* baudrate */
    oa_uart_len_8,              /* dataBits; */
    oa_uart_sb_1,               /* stopBits; */
    oa_uart_pa_none,            /* parity; */
    oa_uart_fc_none,            /* flow control */
    0x11,               /* xonChar; */
    0x13,               /* xoffChar; */
    OA_FALSE
};
oa_uart_struct g_uart3_port_setting = 
{
    115200,//9600,   /* baudrate */
    oa_uart_len_8,              /* dataBits; */
    oa_uart_sb_1,               /* stopBits; */
    oa_uart_pa_none,            /* parity; */
    oa_uart_fc_none,            /* flow control */
    0x11,               /* xonChar; */
    0x13,               /* xoffChar; */
    OA_FALSE
};

extern void oa_app_init(void);
extern void oa_app_at_rsp_recv(oa_uint16 len, oa_uint8 *pStr);


// on tone playing callback, do not delete
oa_bool oa_app_on_tone_play_req(oa_uint16 playtone)
{
    OA_DEBUG_USER("%s:%d", __func__,playtone);

    return OA_TRUE;
  
    switch (playtone)
    {
        case OA_MESSAGE_TONE:   // do not allow message tone
            return OA_FALSE;
        
        case OA_SMS_IN_CALL_TONE:  // do not allow message tone during call
            return OA_FALSE;
        
        case OA_INCOMING_CALL_TONE: // do not allow incoming call ringing
            return OA_FALSE;
          
        default:
            return OA_TRUE; // allow other tones playing
    }
}


void oa_app_power_shutdown(void* param)
{
    OA_DEBUG_USER("%s callled.", __func__);
    oa_power_shutdown(NULL);
}

void oa_press_key_0_handle(void *param)
{
  //oa_set_alm_data(0,1,15,16,0,0,0);
}

void oa_press_key_1_handle(void *param)
{
    //oa_uart_send_at_cmd_req((oa_uint8*)"ATA\r\n",oa_strlen("ATA\r\n"));
}
void oa_press_key_2_handle(void *param)
{
    //oa_uart_send_at_cmd_req((oa_uint8*)"ATA\r\n",oa_strlen("ATA\r\n"));
}
/*****************************************************************
*------oa_app_uart1_recv  
*          need oa_uart_open() and oa_uart_register_callback() firstly,
*          len max is OA_UART_BUFFER_SIZE, 1024
*****************************************************************/
void oa_app_uart1_recv( void * param, oa_uint32 len)
{
    char * pBuf = (char *)param;
  OA_DEBUG_USER("%s:len=%d,pBuf=%s", __func__,len, pBuf );
}

/*****************************************************************
*------oa_app_uart2_recv
*          need oa_uart_open() and oa_uart_register_callback() firstly, 
*          len max is OA_UART_BUFFER_SIZE, 1024
*****************************************************************/

//typedef enum
//{
//    LSL_CMD_TRANS = 0,
//    
//    LSL_CMD_RIGHT = 1,
//    
//    LSL_CMD_FIN = 2,
//    
//    LSL_CMD_NOT_EXIST,
//    LSL_CMD_PASSWORD_ERR,
//    LSL_CMD_WRONG,
//    LSL_CMD_PHONE_NOT_BIND,
//    
//    LSL_CMD_NOT_EXIST_WARNING,
//    LSL_CMD_PASSWORD_ERR_WARNING,
//    LSL_CMD_WRONG_WARNING,
//    LSL_CMD_PHONE_NOT_BIND_WARNING
//} lsl_cmd_state;


void oa_app_uart2_recv( void * param, oa_uint32 len)
{
    oa_uint8 * pBuf = (oa_uint8 *)param;
    oa_uint16 i;
    oa_uint16 payload_len;
    oa_uint16 crc16;
    oa_uint32 temp_u32;
    lsl_cmd_state   res;
    OA_DEBUG_USER("%s:len=%d,pBuf=%s", __func__,len, pBuf );
    
    //if(protocol_analyze(LSL_UART_CMD,pBuf,len)){ return; }
    res = protocol_analyze(LSL_UART_CMD,pBuf,len);
    
    switch(res)
    {
        case LSL_CMD_TRANS :
            break;
        case LSL_CMD_RIGHT :
            lsl_uart_485_tx(OA_UART2,"OK! ",4);
            return;
        case LSL_CMD_SAVE_ERROR :
            lsl_uart_485_tx(OA_UART2,"存储异常，恢复到出厂设置!",oa_strlen("存储异常，恢复到出厂设置!"));
            oa_timer_start(LSL_TIMER_FACTORY_SET,lsl_factory_setting_timer_ind,NULL,999);
            return;
        case LSL_CMD_FIN :
            return;
        case LSL_CMD_RESERVED:
            break;
        case LSL_CMD_NOT_EXIST :
        case LSL_CMD_PASSWORD_ERR :
        case LSL_CMD_WRONG :
        case LSL_CMD_PHONE_NOT_BIND :
        case LSL_CMD_NEW_PSW_ILLEGAL:
            break;
        
        case LSL_CMD_NOT_EXIST_WARNING :
            lsl_uart_485_tx(OA_UART2,"命令不存在!",oa_strlen("命令不存在!"));
            return;
        case LSL_CMD_PASSWORD_ERR_WARNING :
            lsl_uart_485_tx(OA_UART2,"密码错误!",oa_strlen("密码错误!"));
            return;
        case LSL_CMD_WRONG_WARNING :
            lsl_uart_485_tx(OA_UART2,"命令有误!",oa_strlen("命令有误!"));
            return;
        case LSL_CMD_PHONE_NOT_BIND_WARNING :
            lsl_uart_485_tx(OA_UART2,"手机未绑定!",oa_strlen("手机未绑定!"));
            return;
        case LSL_CMD_PHONE_NOT_IN_TABLE :
            lsl_uart_485_tx(OA_UART2,"手机列表无此号!",oa_strlen("手机列表无此号!"));
            return;
        case LSL_CMD_PHONE_TABLE_EMPTY :
            lsl_uart_485_tx(OA_UART2,"手机列表无号码!",oa_strlen("手机列表无号码!"));
            return;
        case LSL_CMD_PHONE_TABLE_FULL :
            lsl_uart_485_tx(OA_UART2,"手机列表已满!",oa_strlen("手机列表已满!"));
            return;
        case LSL_CMD_NEW_PSW_ILLEGAL_WARNING :
            lsl_uart_485_tx(OA_UART2,"密码应为字母或数字!",oa_strlen("密码应为字母或数字!"));
            return;
    }
    
//    if(*pBuf != 0xAA)
//    {
//        return;
//    }
//    
//    if(len < 34)
//    {
//        return;
//    }
    
    if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
    {
//        payload_len = (*(pBuf + 29) << 8) + *(pBuf + 30);
//        
//        temp_u32 = oa_strtoul(gModuleConfigT.vir_id,NULL,16);
//        for(i = 0;i < 4;i++)
//        {
//            *(pBuf + 11 + i) = (temp_u32 >> (24 - 8 * i)) & 0xFF; 
//        }
//        
//        crc16 = crc16_ccitt(pBuf,31 + payload_len);
//        
//        *(pBuf + 31 + payload_len) = (crc16 >> 8) & 0xFF;
//        *(pBuf + 32 + payload_len) = crc16 & 0xFF;
        
        oa_write_buffer_noinit(gClientStateT.gprs_tx,pBuf,len);
        //oa_soc_send(gClientStateT.sid_cse,pBuf,len);
        lsl_soc_send_req();
    }
}

/*****************************************************************
*------oa_app_uart3_recv
*          need oa_uart_open() and oa_uart_register_callback() firstly,  
*          len max is OA_UART_BUFFER_SIZE, 1024
*****************************************************************/
void oa_app_uart3_recv( void * param, oa_uint32 len)
{
    char * pBuf = (char *)param;
    OA_DEBUG_USER("%s:len=%d,pBuf=%s", __func__,len, pBuf );
}

/*****************************************************************/
/* Custom AT command implementation
   FORMAT: 
          [AT+OPENAT=xxx]
   EXAMPLE:
     input AT command string: [AT+OPENAT=GPRS:0,www.3322.org,2011#]
     string [GPRS:0,www.3322.org,2011#] will be received in this function*/
/*****************************************************************/
oa_bool oa_app_execute_custom_at_cmd(oa_char *pStr, oa_uint16 len)
{
    oa_bool ret = OA_FALSE;

    OA_DEBUG_USER("%s:cmd=%s,len=%d", __func__,pStr,len);
    
    //Custom AT commands parse and handle
    ret = oa_set_param_cmd_parse(pStr);

    return ret;
}

/*Main loop, entry function for normal scheduler and prompting*/
void oa_app_scheduler_entry(void* param)
{

    oa_timer_stop(OA_APP_SCHEDULER_ID);
    
    
    gModuleStateT.tick++;
    if(gModuleStateT.tick & 1)
    {
        //PIN_485CTRL_HIGH();
        oa_gpio_write(GPIO_HIGH,GPIO_P71);
    }
    else 
    {   
        //PIN_485CTRL_LOW();
        oa_gpio_write(GPIO_LOW,GPIO_P71);
    }
    if(oa_sim_network_is_valid() && oa_inital_csim_is_sms_ready())
    {
        OA_DEBUG_USER("SIM_Net_Work:Valid->gTick=%d\r\n", gModuleStateT.tick);
        if(gSimStateT.sim_valid == OA_FALSE)
        {
            OA_DEBUG("SIM init start...\r\n"); 
            oa_sms_init();              // oa_sms.C 
            gSimStateT.sim_valid = OA_TRUE;
            OA_DEBUG("SIM网络已联通......\r\n");
        }
//        //sim and network is invalid, can driver network event
//        if(!first_valid)
//        {
//            OA_DEBUG_USER("GSM network and sim init finished!");
//
//            /*sms init*/
//            oa_sms_init();
//#ifdef OPENAT_SOC_DEMO    //if need use gprs,can open this macro for test 
//            /*soc init*/
//            oa_soc_init();
//#endif
//            first_valid = OA_TRUE;
//        }
//
//#ifdef OPENAT_SOC_DEMO    //if need use gprs,can open this macro for test 
//        //gprs connect state check, if not online, do connect request, if online,do nothing
//        oa_soc_state_check();
//#endif
    }
    else
    {
        gSimStateT.sim_valid = OA_FALSE;
        lsl_soc_close_req();
        OA_DEBUG("SIM_Net_Work:Invalid!->gTick=%d\r\n",gModuleStateT.tick);
    }
    

    my_process_service();

    //start timer for loop main scheduler 
    oa_timer_start(OA_APP_SCHEDULER_ID, oa_app_scheduler_entry, NULL, OA_APP_SCHEDULER_PERIOD);
    return;
}

static oa_uint8 link_led;
static oa_uint8 ring_led;

void lsl_link_led_timer_ind(void *p)
{
    oa_uint32 t;
    oa_timer_stop(LSL_TIMER_LINK_LED);
    if(link_led++ & 1) { PIN_LINK_LED_LOW(); } else { PIN_LINK_LED_HIGH(); }
    if(OA_TRUE == gSimStateT.sim_valid) { t = 999;} else {t = 299;}
    oa_timer_start(LSL_TIMER_LINK_LED,lsl_link_led_timer_ind,NULL,t);
}

void lsl_ring_led_timer_ind(void *p)
{
    oa_uint32 t;
    oa_timer_stop(LSL_TIMER_RING_LED);
    if(cfg_tell_103 == OA_FALSE) {PIN_RING_LED_LOW();}
    else
    {
        if(ring_led++ & 1) { PIN_RING_LED_LOW(); } else { PIN_RING_LED_HIGH(); }
    }
    if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse) {t = 999;} else {t = 299;}
    oa_timer_start(LSL_TIMER_RING_LED,lsl_ring_led_timer_ind,NULL,t);
}


void lsl_factory_setting_timer_ind(void *p)
{
    OA_DEBUG_USER("%s:",__func__);
    oa_timer_stop(LSL_TIMER_FACTORY_SET);
    if(gClientStateT.sid_cse >= 0)
    {
        oa_soc_close(gClientStateT.sid_cse);    
    }
    oa_uart_close(OA_UART2);
    oa_app_init();
    return;
}

/*****************************************************************
*---openat user's app main entry. do not delete    
*****************************************************************/
void oa_app_main(void * param)
{     

	oa_app_init( );

    OA_DEBUG_USER(SOURCECODE_VERSION);
    OA_DEBUG_USER(SOURCECODE_BRIEF);
    
	/*Enter application scheduler loop*/
	oa_timer_start(OA_APP_SCHEDULER_ID, oa_app_scheduler_entry, NULL, 9999);
	
	oa_timer_start(LSL_TIMER_LINK_LED,lsl_link_led_timer_ind,NULL,299);
	oa_timer_start(LSL_TIMER_RING_LED,lsl_ring_led_timer_ind,NULL,299);
}

/*****************************************************************
*        for EINT control
*****************************************************************/
#define EDGE_SENSITIVE OA_TRUE
#define LEVEL_SENSITIVE OA_FALSE
#define OA_CUR_TEST_EINT_NO 0/**/
//#define OA_GPIO_EINT_01 27
oa_bool sos_handle_polarity_0 = OA_TRUE;
/*do not modify it, eint configure mapping table*/
oa_uint8 eint_data_map_tb[]={3, 3, 'x',  3, 'x',  1, 'x',  3,  3 }; //'x'=not support
oa_uint8 eint_gpio_map_tb[]={0, 5, 'x', 11, 'x', 24, 'x', 56, 61};  //'x'=not support
oa_uint8 g_first_pullup_flag = OA_TRUE;

void oa_tst_eint_hisr(void)
{
    oa_uint8 eint_level;
    static oa_uint8 first_reverse = OA_FALSE;
    /*only a test trace, better not to print any traces in EINT HISR.*/
    //OA_DEBUG_USER("%s,oa_tst_eint_hisr called: %d", __func__,sos_handle_polarity_0);
    eint_level =oa_gpio_read(eint_gpio_map_tb[OA_CUR_TEST_EINT_NO]);
    if(eint_level == 0)
    {
      sos_handle_polarity_0 = 1;
      first_reverse = OA_TRUE;
    }
    else
    {
      sos_handle_polarity_0 = 0;
    }
    //sos_handle_polarity_0 = !sos_handle_polarity_0;
    if((first_reverse == OA_FALSE) 
      && (g_first_pullup_flag == OA_TRUE))
    {
      //dummy func
      OA_DEBUG_USER("dummy eint func for test");
    }
    else
    {
      //add eint handle func
      OA_DEBUG_USER("%s,eint_level:%d,soa_tst_eint_hisr polarity: %d", __func__,eint_level,sos_handle_polarity_0);
    }
    oa_eint_set_polarity(OA_CUR_TEST_EINT_NO, sos_handle_polarity_0);
}


/****************************************************************
*---user's app initalize and callback functions register
*****************************************************************/
void oa_app_init(void)
{
    oa_uint8 eint_level;
    oa_uint8   i;
    /***** NVRAM param init *****/
    oa_set_param_init();
    
    /***** PHONE TABLE *****/
    //phone_table_init();
    
    /***** KEY *****/
    //register keys handle callback functions if needed
    //oa_key_register(OA_KEY_POWER, OA_KEY_LONGPRESS, oa_app_power_shutdown);    
    //oa_key_register(OA_KEY_R0_C0, OA_KEY_DOWN, oa_press_key_0_handle);
    //oa_key_register(OA_KEY_R1_C1, OA_KEY_DOWN, oa_press_key_1_handle);
    //oa_key_register(OA_KEY_R2_C2, OA_KEY_DOWN, oa_press_key_2_handle);
    
    /***** GPIO *****/
    oa_gpio_init(GPIO_MODE_INPUT,GPIO_P37);     // P37: GPIO4   -> INPUT(AD CH4)
    
    oa_gpio_mode_setup(CTRL_485_IO,0);
    oa_gpio_init(GPIO_MODE_OUTPUT,CTRL_485_IO);    // P55: GPIO61  -> OUTPUT(485 CTRL)
    oa_gpio_mode_setup(LINK_LED_IO,0);
    oa_gpio_init(GPIO_MODE_OUTPUT,LINK_LED_IO);    // P71: GPIO42  -> OUTPUT(LINK LED)
    oa_gpio_mode_setup(RING_LED_IO,0);
    oa_gpio_init(GPIO_MODE_OUTPUT,RING_LED_IO);    // P56: GPIO40  -> OUTPUT(RING LED)
    PIN_485CTRL_HIGH();
    
    /***** AT CMD *****/
    //register AT cmd response callback
    oa_atf_rsp_callback_register(oa_app_at_rsp_recv);

    //register customer AT cmd(AT+OPENAT=XXXX) handle function
    oa_cust_cmd_register(oa_app_execute_custom_at_cmd);
    
    /*****************************************************************
    *        Begin for UARTs open and register
    *****************************************************************/
    /*--------Open UART3 if needed---------*/
    //OA user if needed use UART3, be sure g_oa_ps_port and g_oa_tst_port  not equal OA_UART3 firstly
#if 0    
    if(oa_uart_open(OA_UART3, &g_uart3_port_setting))
    {
        //UART3 open success
        //register UART3 received data callback function 
        oa_uart_register_callback(OA_UART3, oa_app_uart3_recv); 
        //set the time interval between uart data, for received data frame by frame,one frame max length is 1024 bytes
        oa_uart_set_read_delay(OA_UART3, 100);
    }
    else
    {
        //UART3 open failed
        OA_DEBUG_USER("%s:UART3 open failed!", __func__);
    }
#endif
    /*--------Open UART2 if needed---------*/
    //OA user if needed use UART2, be sure g_oa_ps_port and g_oa_tst_port  not equal OA_UART2 firstly
#if 1

    oa_memcpy(&g_uart2_port_setting,lsl_param_get_uart2(),sizeof(oa_uart_struct));

    if(oa_uart_open(OA_UART2, &g_uart2_port_setting))
    {
        //UART2 open success
        //register UART2 received data callback function 
        oa_uart_register_callback(OA_UART2, oa_app_uart2_recv); 
        //set the time interval between uart data, for received data frame by frame,one frame max length is 1024 bytes
        oa_uart_set_read_delay(OA_UART2, 100);
        OA_DEBUG_USER("UART2 [bps]      -> %d",g_uart2_port_setting.baud);
        OA_DEBUG_USER("UART2 [databits] -> %d",g_uart2_port_setting.dataBits);
        switch(g_uart2_port_setting.stopBits)
        {
            case oa_uart_sb_1:      OA_DEBUG_USER("UART2 [stopbits] -> 1");     break;
            case oa_uart_sb_2:      OA_DEBUG_USER("UART2 [stopbits] -> 2");     break;
            case oa_uart_sb_1_5:    OA_DEBUG_USER("UART2 [stopbits] -> 1.5");   break;
        }   
        switch(g_uart2_port_setting.parity)
        {
            case oa_uart_pa_none:   OA_DEBUG_USER("UART2 [parity]   -> NONE\r\n");  break;
            case oa_uart_pa_odd:    OA_DEBUG_USER("UART2 [parity]   -> ODD\r\n");   break;
            case oa_uart_pa_even:   OA_DEBUG_USER("UART2 [parity]   -> ENEN\r\n");  break;
            case oa_uart_pa_space:  OA_DEBUG_USER("UART2 [parity]   -> SPACE\r\n"); break;
        }
    }
    else
    {
        //UART2 open failed
        OA_DEBUG_USER("%s:UART2 open failed!!\r\n", __func__);
    }
#endif

    /*--------Open UART1 if needed,default marked it------------*/
#if 0 //OA user if needed use UART1, open the macro,be sure g_oa_ps_port and g_oa_tst_port  not equal OA_UART1 firstly
    if(oa_uart_open(OA_UART1, &g_uart1_port_setting))
    {
      //UART1 open success
      //register UART1 received data callback function 
      oa_uart_register_callback(OA_UART1, oa_app_uart1_recv); 
      //set the time interval between uart data, for received data frame by frame,one frame max length is 1024 bytes
      oa_uart_set_read_delay(OA_UART1, 100);
    }
    else
    {
      //UART1 open failed
      OA_DEBUG_USER("%s:UART1 open failed!!", __func__);
    }
#endif

    /*****************************************************************
    *        End for UARTs open and register
    *****************************************************************/
    
    
#if 0
    /*****************************************************************
    *        Demo for EINT control
    *****************************************************************/
    /*example to configurate EINT*/   
    oa_gpio_mode_setup(eint_gpio_map_tb[OA_CUR_TEST_EINT_NO], 0);    
    oa_gpio_init(0,eint_gpio_map_tb[OA_CUR_TEST_EINT_NO]);
    eint_level =oa_gpio_read(eint_gpio_map_tb[OA_CUR_TEST_EINT_NO]);
    if(eint_level == 0)//pulldown
    {
      g_first_pullup_flag = OA_FALSE;
      sos_handle_polarity_0 = 1;
    }
    else //pullup
    {
      g_first_pullup_flag = OA_TRUE;
      sos_handle_polarity_0 = 0;
    }    

    //OA_DEBUG_USER("sos_handle_polarity_0: %d", sos_handle_polarity_0);

    oa_gpio_mode_setup(eint_gpio_map_tb[OA_CUR_TEST_EINT_NO], eint_data_map_tb[OA_CUR_TEST_EINT_NO]);
    oa_gpio_init(0,eint_gpio_map_tb[OA_CUR_TEST_EINT_NO]);
    oa_eint_registration(OA_CUR_TEST_EINT_NO, OA_TRUE, sos_handle_polarity_0, oa_tst_eint_hisr, OA_TRUE);
    oa_eint_set_sensitivity(OA_CUR_TEST_EINT_NO, LEVEL_SENSITIVE);
    oa_eint_set_debounce(OA_CUR_TEST_EINT_NO, 80);
    /*****************************************************************
    *        End Demo for EINT control
    *****************************************************************/
#endif
    /*****************************************************************
    *        Demo for GPIO control
    *****************************************************************/
    //Example: Set Pin71(GPIO51) as GPIO mode and init to low level
    //oa_gpio_mode_setup(51, 0);/*set gpio as 0 mode(gpio mode)*/
    //oa_gpio_init(1,51);/*initial gpio as output*/
    //oa_gpio_write(0, 51); //init low level

    /*****************************************************************
    *        End Demo for GPIO control
    *****************************************************************/

    /***** SMS CALLBACK FUNCTION ****/
    //register sms receive callback function
    oa_sms_rcv_ind_register(oa_sms_rcv_ind_handler);

    
    oa_memcpy(&gModuleConfigT.heart_t,lsl_param_get_heart_pkt_info(),sizeof(HEART_PKT_T));
    oa_memcpy(&gModuleConfigT.psw_t,lsl_param_get_password(),sizeof(PASSWORD_T));
    oa_memcpy(&gClientStateT.client_t,lsl_param_get_client_info(),sizeof(CLIENT_PARAM_T));
    oa_memcpy(&gModuleConfigT.vir_id,lsl_param_get_virtual_id(),VIRTUAL_ID_LEN);
    oa_memcpy(&gModuleConfigT.phone,lsl_param_get_phone_table(),PHONE_NUM_MAX * 12);
    gModuleConfigT.auto_upload_time = lsl_param_get_auto_upload_time();
    
    OA_DEBUG_USER("\r\n********* PHONE TABLE *********\r\n");
    for(i = 0;i < PHONE_NUM_MAX;i++)
    {
        if(gModuleConfigT.phone[i][0] != 0)
        {
            OA_DEBUG_USER("PHONE[%d]:%s",i,gModuleConfigT.phone[i]);
        }
    }
    OA_DEBUG_USER("\r\n*******************************\r\n");
    
    gModuleStateT.try_failed_tick = 0xFFFFFFFF;
    gprs_init();
}

/*****************************************************************
*---Power on hardware init for customer, before openat app runing, do not delete            
*****************************************************************/
void oa_app_hw_init( void )
{
    
}

/*****************************************************************
*         AT cmd response received, after oa_uart_send_at_cmd_req or AT command indication
*****************************************************************/
void oa_app_at_rsp_recv(oa_uint16 len, oa_uint8 *pStr)
{
    oa_uint8 * pCmd=pStr;
    oa_uint16 cmdLen= len;

    //OA_DEBUG_USER("%s: len=%d,cmd=%s",__func__,cmdLen,pCmd);

    /*****************************************************************
    *        Call related AT commands handle framework, for reference
    *****************************************************************/
    if(oa_strncmp((oa_char *)pCmd,"\r\n+CLIP:",oa_strlen("\r\n+CLIP:"))==0)
    {
        //Incoming Call Ring  "+CLIP:"xxx",,,,,"
        oa_char *pParamBeg=NULL;    //pointer param string begin 
        oa_char *pParamEnd= NULL;   //pointer param string end
        oa_uint8 phone_no[21]={0};

        pParamBeg=oa_strchr((oa_char *)(pCmd+8), '"');
        pParamBeg = pParamBeg+1;
        pParamEnd=oa_strchr(pParamBeg, '"');

        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)<=20 && (pParamEnd-pParamBeg)>=3)
        {
          //Get incoming call phone number
          oa_memcpy(phone_no,pParamBeg,(pParamEnd-pParamBeg));
        }

        //If need answer incoming call, send AT command "ATA"
        //oa_uart_send_at_cmd_req((oa_uint8*)"ATA\r\n",oa_strlen("ATA\r\n"));

        //If need hangup incoming call, send AT command "ATH"
        //oa_uart_send_at_cmd_req((oa_uint8*)"ATH\r\n",oa_strlen("ATH\r\n"));

    }
    else if(oa_strncmp((oa_char *)pCmd,"\r\nNO CARRIER",oa_strlen("\r\nNO CARRIER"))==0)
    {
        //Incoming call or outgoing call be disconnected.
    }
    else if(oa_strncmp((oa_char *)pCmd,"\r\nRINGBACK",oa_strlen("\r\nRINGBACK"))==0)
    {
       //Outgoing call is Ringing
    }
    else if(oa_strncmp((oa_char *)pCmd,"\r\nVOICE",oa_strlen("\r\nVOICE"))==0)
    {
       //Outgoing call be connected
    }

    /*****************************************************************
    *        Other AT commands handle framework, for reference
    *****************************************************************/
    if(oa_strncmp((oa_char *)pCmd,"\r\n+REG:",oa_strlen("\r\n+REG:"))==0)
    {
       //If send "AT+REG?" for get current Lac and Cellid,will return string "+REG: n,<lac>,<cellid>"
    }
 }

/**************************************** LSL APP ***************************************/
int offline_cnt = 10;
oa_int32 retransmit_times;

void clear_hrt_dog_t(void)
{
    hrt_dog_t = 0;
}

void my_process_service(void)
{   
    oa_uint32 temp_u32;
    oa_uint16 crc16;
    oa_uint8  i;
    OA_DEBUG_USER("%s:",__func__);

    if(cfg_tell_103 == OA_FALSE)
    {
        if(fixid_close_soc_flag == OA_TRUE)
        {
            lsl_soc_close_req();
            fixid_close_soc_flag = OA_FALSE;
        }
        temp_u32 = oa_strtoul(gModuleConfigT.vir_id,NULL,16);
        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        gModuleStateT.user_com_str[0] = 0xAA;
        gModuleStateT.user_com_str[1] = 0xF9;
        gModuleStateT.user_com_str[2] = 0xDF;
        
        for(i = 0;i < 8;i++)
        {
            gModuleStateT.user_com_str[3 + i] = 0xFF;
        }
        gModuleStateT.user_com_str[11] = temp_u32 >> 24;
        gModuleStateT.user_com_str[12] = temp_u32 >> 16;
        gModuleStateT.user_com_str[13] = temp_u32 >> 8;
        gModuleStateT.user_com_str[14] = temp_u32;
        

        for(i = 0;i < 12;i++)
        {
            gModuleStateT.user_com_str[15 + i] = 0xFF;
        }
        
        gModuleStateT.user_com_str[27] = 0x01;
        gModuleStateT.user_com_str[28] = 0x03;
        
        // length
        gModuleStateT.user_com_str[29] = 0x00;
        gModuleStateT.user_com_str[30] = 0x06;
        
        gModuleStateT.user_com_str[31] = temp_u32 >> 24;
        gModuleStateT.user_com_str[32] = temp_u32 >> 16;
        gModuleStateT.user_com_str[33] = temp_u32 >> 8;
        gModuleStateT.user_com_str[34] = temp_u32;
        
        gModuleStateT.user_com_str[35] = gModuleConfigT.auto_upload_time >> 8;
        gModuleStateT.user_com_str[36] = gModuleConfigT.auto_upload_time & 0xFF;
        
        crc16 = crc16_ccitt(gModuleStateT.user_com_str,37);
        
        gModuleStateT.user_com_str[37] = crc16 >> 8;
        gModuleStateT.user_com_str[38] = crc16 & 0xFF;
        gModuleStateT.user_com_str[39] = 0xDD;
        
        lsl_uart_485_tx(OA_UART2,gModuleStateT.user_com_str,40);
        OA_DEBUG_USER("ID_CFG_CONTROLING id = %X,auto_upload_time = %d",temp_u32,gModuleConfigT.auto_upload_time);
        return;
    }
    
    if(OA_FALSE == gSimStateT.sim_valid)
    {
        return;
    }
    switch(gClientStateT.st_cse)
    {
        case OA_SOC_STATE_OFFLINE:
            OA_DEBUG_USER("[connect state] -> OFFLINE=%d",offline_cnt);
            if((gModuleStateT.try_failed_tick - gModuleStateT.try_connect_tick) < 5)
            {
                OA_DEBUG_USER("ERROR-CNT[%d]",error_to_reset_cnt);
                gModuleStateT.try_failed_tick = 0xFFFFFFFF;
                if(++error_to_reset_cnt >= 4)
                {
                    OA_DEBUG_USER("ERROR TO RESTART!!!!");
                    oa_module_restart(NULL);
                }
            }
            
            if(offline_cnt++ < 10) { return; }
            offline_cnt = 0;
            switch(gClientStateT.client_t.con_mode)
            {
                case LSL_CON_MODE_DOMAIN:
                    gprs_connect(NULL,gClientStateT.client_t.domain_name,gClientStateT.client_t.port,LSL_CON_MODE_DOMAIN);
                    gClientStateT.st_cse = OA_SOC_STATE_CONNECT;
                    gClientStateT.connect_cnt = 0;
                    break;
                case LSL_CON_MODE_IP:
                    gModuleStateT.try_connect_tick = gModuleStateT.tick;
                    gprs_connect(NULL,gClientStateT.client_t.ip_address,gClientStateT.client_t.port,LSL_CON_MODE_IP);
                    gClientStateT.st_cse = OA_SOC_STATE_CONNECT;
                    gClientStateT.connect_cnt = 0;
                    break;
            }
            break;
        case OA_SOC_STATE_CONNECT:
            OA_DEBUG_USER("[connect state] -> CONNECT = %d",gClientStateT.connect_cnt);
            if(120 == ++gClientStateT.connect_cnt)
            {
                OA_DEBUG_USER("CONNECT FAILED RETRY!");
                gClientStateT.connect_cnt = 0;
                oa_soc_close(gClientStateT.sid_cse);
                gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
            }
            break;
        case OA_SOC_STATE_ONLINE:
            OA_DEBUG_USER("[connect state] -> ONLINE");
            error_to_reset_cnt = 0;
            if(OA_TRUE == gClientStateT.is_blocksend)
            {
                OA_DEBUG_USER("BLOCKSEND !!!");
                if(++block_send_retry > 60)
                {
                    OA_DEBUG_USER("BLOCKSEND ERROR ,RESTART!!!!");
                    oa_module_restart(NULL);
                }
            }
            else
            {
                block_send_retry = 0;
            }
            
            retransmit_times = oa_soc_tcp_retransmit_times(g_soc_context.socket_id);
        
            if(retransmit_times>=3)
            {/*warning: any data sent during a phone call, TCP layer will retransmit it
             *  that will cause a nonsensical counter by oa_tcpip_get_tcp_retransmit_times after a call
             */
                OA_DEBUG_USER("%s:retransmit too many times=%d!",__func__,retransmit_times);
                lsl_soc_close_req( );
                gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
            }

            if(gModuleConfigT.heart_t.t)
            {
                temp_u32 = gModuleConfigT.heart_t.t * 2 + 30000;
                OA_DEBUG_USER("hrt_dog_t = %d of %d,",hrt_dog_t,temp_u32);
                hrt_dog_t += 1000;
                if(hrt_dog_t > temp_u32)
                {
                    OA_DEBUG_USER("hrt dog feel sad ~~o(>_<)o ~~");
                    lsl_soc_close_req( );
                }
            }
            break;
        case OA_SOC_STATE_HALT:
            OA_DEBUG_USER("[connect state] -> HALT");
            
            break; 
    }
}

void lsl_timer_485tx_off_ind(void *p)
{
    OA_DEBUG_USER("%s:<<<%d",__func__,--wait_485_flag);
    oa_timer_stop(LSL_TIMER_485TX_OFF);
    if(wait_485_flag)
    {
        oa_timer_start(LSL_TIMER_485TX_OFF, lsl_timer_485tx_off_ind, (void *)0, wait_485[wait_485_flag]);
    }
    else
    {
        PIN_485CTRL_HIGH();
    }
    
    if(OA_TRUE == b_baud_set)
    {
        g_uart2_port_setting.baud = baud_temp;
        
        oa_uart_close(OA_UART2);
        
        if(oa_uart_open(OA_UART2, &g_uart2_port_setting))
        {
            //UART2 open success
            //register UART2 received data callback function 
            oa_uart_register_callback(OA_UART2, oa_app_uart2_recv); 
            //set the time interval between uart data, for received data frame by frame,one frame max length is 1024 bytes
            oa_uart_set_read_delay(OA_UART2, 100);
            OA_DEBUG_USER("UART2 [bps]      -> %d",g_uart2_port_setting.baud);
            OA_DEBUG_USER("UART2 [databits] -> %d",g_uart2_port_setting.dataBits);
            switch(g_uart2_port_setting.stopBits)
            {
                case oa_uart_sb_1:      OA_DEBUG_USER("UART2 [stopbits] -> 1");     break;
                case oa_uart_sb_2:      OA_DEBUG_USER("UART2 [stopbits] -> 2");     break;
                case oa_uart_sb_1_5:    OA_DEBUG_USER("UART2 [stopbits] -> 1.5");   break;
            }   
            switch(g_uart2_port_setting.parity)
            {
                case oa_uart_pa_none:   OA_DEBUG_USER("UART2 [parity]   -> NONE\r\n");  break;
                case oa_uart_pa_odd:    OA_DEBUG_USER("UART2 [parity]   -> ODD\r\n");   break;
                case oa_uart_pa_even:   OA_DEBUG_USER("UART2 [parity]   -> ENEN\r\n");  break;
                case oa_uart_pa_space:  OA_DEBUG_USER("UART2 [parity]   -> SPACE\r\n"); break;
            }
        }
        else
        {
            //UART2 open failed
            OA_DEBUG_USER("%s:UART2 open failed!!\r\n", __func__);
        }
        b_baud_set = OA_FALSE;
    }
    
}

void lsl_timer_485tx_on_ind(void *p)
{
//    oa_uint32 t;
//    OA_DEBUG_USER("%s:",__func__);
//    //OA_DEBUG_USER("LEN=%d,DAT=%s,*dat=0x%08X",gUart485DatT.len,gUart485DatT.pDat,gUart485DatT.pDat);
//    oa_timer_stop(LSL_TIMER_485TX_ON);
//    PIN_485CTRL_LOW();
//    oa_uart_write(OA_UART2,gUart485DatT.pDat,gUart485DatT.len);
//    switch(g_uart2_port_setting.baud)
//    {
//        case 2400:      t = gUart485DatT.len * 25 / 6 + 10;     break;
//        case 4800:      t = gUart485DatT.len * 25 / 12 + 10;    break;
//        case 9600:      t = gUart485DatT.len * 25 / 24 + 10;    break;
//        case 19200:     t = gUart485DatT.len * 25 / 48 + 10;    break;
//        case 38400:     t = gUart485DatT.len * 25 / 96 + 10;    break;
//        case 57600:     t = gUart485DatT.len * 25 / 144 + 10;   break;
//        case 115200:    t = gUart485DatT.len * 25 / 288 + 10;   break;
//        default:        t = gUart485DatT.len * 25 / 24 + 10;    break;  // UART PARAM ERROR
//    }
//    oa_timer_start(LSL_TIMER_485TX_OFF, lsl_timer_485tx_off_ind, (void *)0, t);
}

int lsl_uart_485_tx(oa_uart_enum P, oa_uint8 *q, oa_uint16 n)
{
    oa_uint32 t;
    gUart485DatT.pDat = q;
    gUart485DatT.len = n;
    OA_DEBUG_USER("%s:>>>%d",__func__,wait_485_flag);

    PIN_485CTRL_LOW();
    oa_uart_write(OA_UART2,gUart485DatT.pDat,gUart485DatT.len);

    switch(g_uart2_port_setting.baud)
    {
        case 2400:      wait_485[wait_485_flag] = gUart485DatT.len * 25 / 6 + 10;     break;
        case 4800:      wait_485[wait_485_flag] = gUart485DatT.len * 25 / 12 + 10;    break;
        case 9600:      wait_485[wait_485_flag] = gUart485DatT.len * 25 / 24 + 10;    break;
        case 19200:     wait_485[wait_485_flag] = gUart485DatT.len * 25 / 48 + 10;    break;
        case 38400:     wait_485[wait_485_flag] = gUart485DatT.len * 25 / 96 + 10;    break;
        case 57600:     wait_485[wait_485_flag] = gUart485DatT.len * 25 / 144 + 10;   break;
        case 115200:    wait_485[wait_485_flag] = gUart485DatT.len * 25 / 288 + 10;   break;
        default:        wait_485[wait_485_flag] = gUart485DatT.len * 25 / 24 + 10;    break;  // UART PARAM ERROR
    }
    
    if(wait_485_flag == 0)
    {
        oa_timer_start(LSL_TIMER_485TX_OFF, lsl_timer_485tx_off_ind, (void *)0, wait_485[0]);
    }
    wait_485_flag++;
    
    return 0;
}

lsl_cmd_state protocol_analyze(lsl_cmd_enum c,oa_uint8 *dat,oa_uint32 len)
{
    oa_uint8    *q,*psw,*pd,*pp,*pj;
    oa_uint16   i,j,k;
    oa_uint8    temp_u8;
    oa_uint16   temp_u16;
    oa_uint32   temp_u32;
    oa_uint8    temp_str[16] = { 0 };
    oa_uint8    temp_spk_hex[16] = { 0 };
    oa_uint8    temp_spk[32] = { 0 };
    oa_bool     warn_msg;
    oa_bool     what;
    OA_DEBUG_USER("%s:",__func__);
    
    q = dat;
#if USING_DEBUG_PROTOCOL > 0
    if(!oa_strncmp(q,"DEBUG_READ",10))   // 读取电流和开关量
    {
        read_adc_value_api(4);
        return 128;
    }
#endif  
    
    if(c == LSL_SOC_CMD)
    {
        OA_DEBUG_USER("*q     = 0x%X",*q);
        OA_DEBUG_USER("*(q+1) = 0x%X",*(q+1));
        OA_DEBUG_USER("*(q+2) = 0x%X",*(q+2));
    }   

	// 自动上报时间配置
    if((*q == 0xAA) && (*(q + 1) == 0xFC)  && (*(q + 2) == 0x17) && (*(q + 29) == 0x00) && (*(q + 30) == 0x02) && (len >= 36))
    {
        temp_u16 = crc16_ccitt(q,33);
        
        if((*(q + 33) == temp_u16 >> 8) && (*(q + 34) == (temp_u16 & 0xFF)) && (*(q + 35) == 0xDD))
        {
            temp_u16 = (*(q + 31) << 8) + *(q + 32);
            
            OA_DEBUG_USER("AUTO_UPLOAD_TIME_SAVE = %d",temp_u16);
            if(OA_FALSE == lsl_param_set_auto_upload_time(temp_u16))
            {
                return LSL_CMD_SAVE_ERROR;
            }
            gModuleConfigT.auto_upload_time = lsl_param_get_auto_upload_time();
            OA_DEBUG_USER("AUTO_UPLOAD_TIME = %d",temp_u16);
            
            q += 36;
            len -= 36;
            
            temp_u32 = oa_strtoul(gModuleConfigT.vir_id,NULL,16);
            oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
            gModuleStateT.user_com_str[0] = 0xAA;
            gModuleStateT.user_com_str[1] = 0xFD;
            gModuleStateT.user_com_str[2] = 0x17;
            
            for(i = 0;i < 8;i++)
            {
                gModuleStateT.user_com_str[3 + i] = 0x30 + i;
            }
            
            gModuleStateT.user_com_str[11] = temp_u32 >> 24;
            gModuleStateT.user_com_str[12] = temp_u32 >> 16;
            gModuleStateT.user_com_str[13] = temp_u32 >> 8;
            gModuleStateT.user_com_str[14] = temp_u32;
            
            
//            for(i = 0;i < 12;i++)
//            {
//                gModuleStateT.user_com_str[15 + i] = 0xFF;
//            }
            oa_strncpy(&gModuleStateT.user_com_str[15],"ModuleString",oa_strlen("ModuleString"));
            
            gModuleStateT.user_com_str[27] = 0x03;
            gModuleStateT.user_com_str[28] = 0x00;
            
            // length
            gModuleStateT.user_com_str[29] = 0x00;
            gModuleStateT.user_com_str[30] = 0x02;
            
            gModuleStateT.user_com_str[31] = gModuleConfigT.auto_upload_time >> 8;
            gModuleStateT.user_com_str[32] = gModuleConfigT.auto_upload_time & 0xFF;
            
            temp_u16 = crc16_ccitt(gModuleStateT.user_com_str,33);
            
            gModuleStateT.user_com_str[33] = temp_u16 >> 8;
            gModuleStateT.user_com_str[34] = temp_u16 & 0xFF;
            gModuleStateT.user_com_str[35] = 0xDD;
            
            oa_write_buffer_noinit(gClientStateT.gprs_tx,gModuleStateT.user_com_str,36);
            //oa_soc_send(gClientStateT.sid_cse,pBuf,len);
            lsl_soc_send_req();
            
            cfg_tell_103 = OA_FALSE;
        }
    }
    
    if(len <= 11 ) {return LSL_CMD_TRANS;}   // XXX=123456:...# > 12
    
    if(!oa_strncmp((q + 4),"?=",2)) { psw = q + 6; warn_msg = OA_TRUE; }
    else { psw = q + 4; warn_msg = OA_FALSE; }

    if (!oa_strncmp(q, "CMD=", 4))
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
    }
    else if (!oa_strncmp(q, "PKT=", 4))     // 读取心跳包内容
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        if(oa_strncmp(pd,"HeartInfo?",10))
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        
        if(gModuleConfigT.heart_t.t)
        {
            oa_sprintf(gModuleStateT.user_com_str,"PKT=%s,Time=%ds;",
            gModuleConfigT.heart_t.chr,gModuleConfigT.heart_t.t / 1000);
        }
        else
        {
            oa_sprintf(gModuleStateT.user_com_str,"Heart PKT Closed;");
        }
        
        USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
    }
    else if (!oa_strncmp(q, "HRT=", 4))     // 设置心跳包
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        pj = oa_strchr(pd,'#');
        pp = oa_strchr(pd,'@');
        
        j = pj - pp;
        k = pp - pd;
        
        OA_DEBUG_USER("pd=%s,j=%d,k=%d",pd,j,k);
        
        if((j >= HEART_PKT_LEN_MAX) || (j <= 1) || (k < 1) || (k > 7) || (!pp) || (!pj))
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        for(i = 0;i < k;i++)
        {
            if(0 == ((*(pd + i) <= '9') && (*(pd + i) >= '0')) )
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
        }
        
        *pp = 0;*pj = 0;
        pp++;
        
        temp_u32 = oa_atoi(pd);
        
        if(temp_u32 > HEART_TIME_MAX)
        {
            temp_u32 = HEART_TIME_MAX;
        }
        else if(0 == temp_u32)
        {
            
        }
        else if(temp_u32 < (HEART_TIME_MIN))
        {
            temp_u32 = HEART_TIME_MIN;
        }
        
        temp_u32 *= 1000;
        
        if(OA_FALSE == lsl_param_set_heart_pkt_info(pp,j - 1,temp_u32))
        {
            return LSL_CMD_SAVE_ERROR;
        }
        
        oa_timer_stop(LSL_TIMER_HEART_PKT);
        
        oa_memcpy(&gModuleConfigT.heart_t,lsl_param_get_heart_pkt_info(),sizeof(HEART_PKT_T));
        
        if(gModuleConfigT.heart_t.t >= HEART_TIME_MIN)
        {
            oa_timer_start(LSL_TIMER_HEART_PKT,heart_pkt_timer_ind,NULL,gModuleConfigT.heart_t.t);
        }
    }
    else if (!oa_strncmp(q, "PWC=", 4))     // 更改操作密码     (3/3-WR)
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        pj = oa_strchr(pd,'#');
        j = pj - pd;
        if((!pj) || (j < PASSWORD_LEN_MIN) || (j >= PASSWORD_LEN_MAX))
        { 
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        for(i = 0;i < j;i++)
        {
            if( 0 == ( ((*(pd + i) <= 'Z')&&(*(pd + i) >= 'A')) ||      \
                       ((*(pd + i) <= 'z')&&(*(pd + i) >= 'a')) ||      \
                       ((*(pd + i) <= '9')&&(*(pd + i) >= '0')) ) )
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_NEW_PSW_ILLEGAL_WARNING; }
                else{ return LSL_CMD_NEW_PSW_ILLEGAL; }
            }
        }
        if(OA_FALSE == lsl_param_set_psw(pd,j))
        {
            return LSL_CMD_SAVE_ERROR;
        }
        oa_memcpy(&gModuleConfigT.psw_t,lsl_param_get_password(),sizeof(PASSWORD_T));
    }
    else if (!oa_strncmp(q, "CIP=", 4))     // 清除域名、IP、端口 (RESERVED)
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        return LSL_CMD_RESERVED;
        switch(c)
        {
            case LSL_SMS_CMD:       //  ADD CODE
                break;
            case LSL_SOC_CMD:       //  ADD CODE
                break;
            case LSL_UART_CMD:
                break;
        }
    }
    else if (!oa_strncmp(q, "NET=", 4))     //  更改数据中心 IP 地址及端口号 (1/3-WR)
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        pp = oa_strchr(pd,',');
        if(!pp)
        {
            pp = oa_strchr(pd,'，');
        }
        
        pj = oa_strchr(pd,'#');
        
        j = pp - pd;
        k = pj - pp;
        
        if((j > 15) || (k <= 1) || (!pp) || (!pj))
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        for(i = 0;i < j;i++)
        {
            if(0 == (((*(pd + i) <= '9') && (*(pd + i) >= '0')) || (*(pd + i) == '.')) )
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
        }
        
        for(i = 1;i < k;i++)
        {
            if( 0 == ((*(pp + i) <= '9') && (*(pp + i) >= '0')) )
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
        }
        
        *pp = 0;    *pj = 0;
        pp++;
        
        if(OA_FALSE == lsl_param_set_ipaddr(pd,j,pp))
        {
            return LSL_CMD_SAVE_ERROR;
        }
        
        lsl_soc_close_req();
        
        oa_memcpy(&gClientStateT.client_t,lsl_param_get_client_info(),sizeof(CLIENT_PARAM_T));
    }
    else if (!oa_strncmp(q, "DNS=", 4))     // 更改数据中心域名及端口号 (1/3-WR)
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        pp = oa_strchr(pd,',');
        if(!pp)
        {
            pp = oa_strchr(pd,'，');
        }
        
        pj = oa_strchr(pd,'#');
        
        j = pp - pd;
        k = pj - pp;
        
        for(i = 1;i < k;i++)
        {
            if( 0 == ( (*(pp + i) <= '9')&&(*(pp + i) >= '0'))  )
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
        }
        
        if((!pp) || (!pj) || (j <= 1) || (k <= 1))
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        *pp = 0;    *pj = 0;
        pp++;
        
        if(OA_FALSE == lsl_param_set_domain(pd,j,pp))
        {
            return LSL_CMD_SAVE_ERROR;
        }
        
        lsl_soc_close_req();
        
        oa_memcpy(&gClientStateT.client_t,lsl_param_get_client_info(),sizeof(CLIENT_PARAM_T));

    }
    else if (!oa_strncmp(q, "BPS=", 4))     // 更改串口速率     (1/3-WR)
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        if(!oa_strncmp(pd,"2400#",5))
        {
            temp_u32 = 2400;
        }
        else if(!oa_strncmp(pd,"4800#",5))
        {
            temp_u32 = 4800;
        }
        else if(!oa_strncmp(pd,"9600#",5))
        {
            temp_u32 = 9600;
        }
        else if(!oa_strncmp(pd,"14400#",6))
        {
            temp_u32 = 14400;
        }
        else if(!oa_strncmp(pd,"19200#",6))
        {
            temp_u32 = 19200;
        }
        else if(!oa_strncmp(pd,"38400#",6))
        {
            temp_u32 = 38400;
        }
        else if(!oa_strncmp(pd,"57600#",6))
        {
            temp_u32 = 57600;
        }
        else if(!oa_strncmp(pd,"115200#",7))
        {
            temp_u32 = 115200;
        }
        else
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        baud_temp = temp_u32;
        
        if(OA_FALSE == lsl_param_set_uart2(&g_uart2_port_setting))
        {
            baud_temp = 9600;
            return LSL_CMD_SAVE_ERROR;
        }
        
        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        oa_sprintf(gModuleStateT.user_com_str,"BPS->%d;",baud_temp);
        
        switch(c)
        {
            case LSL_SMS_CMD:
            case LSL_SOC_CMD:
                
                g_uart2_port_setting.baud = baud_temp;
        
                oa_uart_close(OA_UART2);
                
                if(oa_uart_open(OA_UART2, &g_uart2_port_setting))
                {
                    //UART2 open success
                    //register UART2 received data callback function 
                    oa_uart_register_callback(OA_UART2, oa_app_uart2_recv); 
                    //set the time interval between uart data, for received data frame by frame,one frame max length is 1024 bytes
                    oa_uart_set_read_delay(OA_UART2, 100);
                    OA_DEBUG_USER("UART2 [bps]      -> %d",g_uart2_port_setting.baud);
                    OA_DEBUG_USER("UART2 [databits] -> %d",g_uart2_port_setting.dataBits);
                    switch(g_uart2_port_setting.stopBits)
                    {
                        case oa_uart_sb_1:      OA_DEBUG_USER("UART2 [stopbits] -> 1");     break;
                        case oa_uart_sb_2:      OA_DEBUG_USER("UART2 [stopbits] -> 2");     break;
                        case oa_uart_sb_1_5:    OA_DEBUG_USER("UART2 [stopbits] -> 1.5");   break;
                    }   
                    switch(g_uart2_port_setting.parity)
                    {
                        case oa_uart_pa_none:   OA_DEBUG_USER("UART2 [parity]   -> NONE\r\n");  break;
                        case oa_uart_pa_odd:    OA_DEBUG_USER("UART2 [parity]   -> ODD\r\n");   break;
                        case oa_uart_pa_even:   OA_DEBUG_USER("UART2 [parity]   -> ENEN\r\n");  break;
                        case oa_uart_pa_space:  OA_DEBUG_USER("UART2 [parity]   -> SPACE\r\n"); break;
                    }
                }
                else
                {
                    //UART2 open failed
                    OA_DEBUG_USER("%s:UART2 open failed!!\r\n", __func__);
                }
                
                break;
            case LSL_UART_CMD:
                b_baud_set = OA_TRUE;
                break;
        }
        USER_COM_STR_ACK(OA_SMSAL_DEFAULT_DCS);
    }
    else if (!oa_strncmp(q, "DNB=", 4))     // 解绑手机
    {
        
#if PROTOCOL_LIMIT > 0
        if(LSL_SOC_CMD == c)  { return LSL_CMD_RESERVED; }
        if(LSL_UART_CMD == c) { return LSL_CMD_RESERVED; }
#endif
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        
        pj = oa_strchr(pd,'#');
        j = pj - pd;
        
        if((!pj) || ((j != 4) && (j != 11)))
        { 
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else { return LSL_CMD_WRONG; }
        }
            
        if(4 == j)
        {
            if(oa_strncmp(pd,"THIS",4)) // len = 4, not "THIS" string
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
            else
            {
                for(i = 0;i < PHONE_NUM_MAX;i++)
                {
                    if(gModuleConfigT.phone[i][0] != 0) // now place not empty
                    {
                        if(!oa_strncmp(gSmsStateT.phone,gModuleConfigT.phone[i],11)) // the phone num has existed
                        {
                            if(OA_FALSE == lsl_param_clr_phone_num(i))
                            {
                                return LSL_CMD_SAVE_ERROR;
                            }
                            oa_memcpy(&gModuleConfigT.phone,lsl_param_get_phone_table(),PHONE_NUM_MAX * 12);
                            return LSL_CMD_RIGHT;
                        }
                    }
                }
            }
        }
        else // len = 11
        {
            for(i = 0;i < j;i++)
            {
                if( 0 == ( (*(pd + i) <= '9') && (*(pd + i) >= '0')) )
                {
                    if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                    else{ return LSL_CMD_WRONG; }
                }
            }
            for(i = 0;i < PHONE_NUM_MAX;i++)
            {
                if(gModuleConfigT.phone[i][0] != 0) // now place not empty
                {
                    if(!oa_strncmp(pd,gModuleConfigT.phone[i],11)) // the phone num has existed
                    {
                        if(OA_FALSE == lsl_param_clr_phone_num(i))
                        {
                            return LSL_CMD_SAVE_ERROR;
                        }
                        oa_memcpy(&gModuleConfigT.phone,lsl_param_get_phone_table(),PHONE_NUM_MAX * 12);
                        return LSL_CMD_RIGHT;
                    }
                }
            }
        }
        return LSL_CMD_PHONE_NOT_IN_TABLE;
    }
    else if (!oa_strncmp(q, "NBD=", 4))     // 设定第一个手机（RESERVED）
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        return LSL_CMD_RESERVED;
        
        switch(c)
        {
            case LSL_SMS_CMD:       //  ADD CODE
                break;
            case LSL_SOC_CMD:       //  ADD CODE
                break;
            case LSL_UART_CMD:
                break;
        }
    }
    else if (!oa_strncmp(q, "BND=", 4))     // 绑定手机
    {
        
#if PROTOCOL_LIMIT > 0
        if(LSL_SOC_CMD == c)  { return LSL_CMD_RESERVED; }
        if(LSL_UART_CMD == c) { return LSL_CMD_RESERVED; }
#endif
        PASSWORD_CHK();
        //PHONE_CHECK_BINDING();
        pj = oa_strchr(pd,'#');
        j = pj - pd;
        
        if((!pj) || ((j != 4) && (j != 11)))
        { 
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else { return LSL_CMD_WRONG; }
        }
            
        if(4 == j)
        {
            if(oa_strncmp(pd,"THIS",4)) // len = 4, not "THIS" string
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
            else
            {
                temp_u8 = 0xFF;
        
                for(i = 0;i < PHONE_NUM_MAX;i++)
                {
                    if(gModuleConfigT.phone[i][0] != 0) // now place not empty
                    {
                        if(!oa_strncmp(gSmsStateT.phone,gModuleConfigT.phone[i],11)) // the phone num has existed
                        {
                            return LSL_CMD_RIGHT;
                        }
                    }
                    else // empty
                    {
                        if(i <= temp_u8 ) {temp_u8 = i;}
                    }
                }
                
                if(temp_u8 > (PHONE_NUM_MAX - 1))
                {
                    return LSL_CMD_PHONE_TABLE_FULL;
                }
                
                if(OA_FALSE == lsl_param_set_phone_table(gSmsStateT.phone,temp_u8))
                {
                    return LSL_CMD_SAVE_ERROR;
                }
            }
        }
        else // len = 11
        {
            for(i = 0;i < j;i++)
            {
                if( 0 == ( (*(pd + i) <= '9') && (*(pd + i) >= '0')) )
                {
                    if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                    else{ return LSL_CMD_WRONG; }
                }
            }
            
            temp_u8 = 0xFF;
        
            for(i = 0;i < PHONE_NUM_MAX;i++)
            {
                if(gModuleConfigT.phone[i][0] != 0) // now place not empty
                {
                    if(!oa_strncmp(pd,gModuleConfigT.phone[i],11)) // the phone num has existed
                    {
                        return LSL_CMD_RIGHT;
                    }
                }
                else // empty
                {
                    if(i <= temp_u8 ) {temp_u8 = i;}
                }
            }
            
            if(temp_u8 > (PHONE_NUM_MAX - 1))
            {
                return LSL_CMD_PHONE_TABLE_FULL;
            }
            
            if(OA_FALSE == lsl_param_set_phone_table(pd,temp_u8))
            {
                return LSL_CMD_SAVE_ERROR;
            }
            
        }

        
        oa_memcpy(&gModuleConfigT.phone,lsl_param_get_phone_table(),PHONE_NUM_MAX * 12);
        
    }
    else if (!oa_strncmp(q, "ASK=", 4))     // 绑定手机查询
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        if(oa_strncmp(pd,"MOBILE?",7))
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        oa_memcpy(&gModuleConfigT.phone,lsl_param_get_phone_table(),PHONE_NUM_MAX * 12);

        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        
        temp_u8 = 0;
        
        for(i = 0;i < PHONE_NUM_MAX;i++)
        {
            if(gModuleConfigT.phone[i][0] != 0) // now place not empty
            {
                oa_memset(temp_str,0,16);
                oa_sprintf(temp_str,"(%d)%s;",++temp_u8,gModuleConfigT.phone[i]);
                oa_strcat(gModuleStateT.user_com_str,temp_str);
            }
        }
        
        if(0 == temp_u8)
        {
            return LSL_CMD_PHONE_TABLE_EMPTY;
        }
        
        USER_COM_STR_ACK(OA_SMSAL_DEFAULT_DCS);
    }
    else if (!oa_strncmp(q, "GET=", 4))     // 读取配置
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        if(oa_strncmp(pd,"CFG?",4)) 
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        
        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        
        if(LSL_CON_MODE_DOMAIN == gClientStateT.client_t.con_mode)
        {
            oa_sprintf(gModuleStateT.user_com_str,"DNS=%s,PORT=%d;",
                       gClientStateT.client_t.domain_name,gClientStateT.client_t.port);
        }
        else
        {
            oa_sprintf(gModuleStateT.user_com_str,"IP=%s,PORT=%d;",
                       gClientStateT.client_t.ip_address,gClientStateT.client_t.port);
        }
        
        
        USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
        
    }
    else if (!oa_strncmp(q, "SID=", 4))     // ID号配置
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        pj = oa_strchr(pd,'#');
        j = pj - pd;
        
        if((!pj) || (j != VIRTUAL_ID_LEN))
        { 
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else { return LSL_CMD_WRONG; }
        }

        for(i = 0;i < j;i++)
        {
            if( 0 == ( ((*(pd + i) <= 'F') && (*(pd + i) >= 'A')) ||    \
                       ((*(pd + i) <= 'f') && (*(pd + i) >= 'a')) ||    \
                       ((*(pd + i) <= '9') && (*(pd + i) >= '0')) ) )
            {
                if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
                else{ return LSL_CMD_WRONG; }
            }
        }
        
        if(OA_FALSE == lsl_param_set_virtual_id(pd,j))
        {
            return LSL_CMD_SAVE_ERROR;
        }
        oa_memcpy(&gModuleConfigT.vir_id,lsl_param_get_virtual_id(),VIRTUAL_ID_LEN);
        
        cfg_tell_103 = OA_FALSE;
        fixid_close_soc_flag = OA_TRUE;
    }
    else if (!oa_strncmp(q, "IDR=", 4))     // ID号读取 (1/3-WR)
    {
        PASSWORD_CHK();
        PHONE_CHECK_BINDING();
        if(oa_strncmp(pd,"NUM?",4)) 
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
        temp_u32 = oa_strtoul(gModuleConfigT.vir_id,NULL,16);
        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        oa_sprintf(gModuleStateT.user_com_str,"ID=0x%08X(十进制:%u);",temp_u32,temp_u32);
        
        USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
    }
        else if (!oa_strncmp(q, "@WEI&CHEN@IMEI#", 15))
    {
        oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        oa_memcpy(gModuleStateT.user_com_str,oa_get_imei(),15);
        oa_strcat(gModuleStateT.user_com_str,"Q");
        USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
    }
    else if (!oa_strncmp(q, "@WEI&CHEN@SPKEY#" , 16))
    {
        pd = q + 16;

        oa_memset(imei_str,0,16);
        oa_memcpy(imei_str,oa_get_imei(),15);
        OA_DEBUG_USER("IMEI=%s\r\nHEX=%X\r\n",imei_str,imei_str);
        for(i = 0;i < 15;i++)
        {
            temp_spk_hex[i] = imei_str[i] ^ LSL_WEICHEN_COPYRIGHT[i];
            if((i == 3) || (i == 7) || (i == 11))
            {
                temp_u32 = (temp_spk_hex[i - 3] << 24) + (temp_spk_hex[i - 2] << 16) \
                            + (temp_spk_hex[i - 1] << 8) + temp_spk_hex[i];
                oa_memset(temp_str,0,16);
                oa_sprintf(temp_str,"%04X",temp_u32);
                oa_strcat(temp_spk,temp_str);
            }
            else if(i == 14)
            {
                temp_u32 = (temp_spk_hex[i - 2] << 16) + (temp_spk_hex[i - 1] << 8) \
                + temp_spk_hex[i];
                oa_memset(temp_str,0,16);
                oa_sprintf(temp_str,"%03X",temp_u32);
                oa_strcat(temp_spk,temp_str);
            }
        }
        OA_DEBUG_USER("SPK=%s",temp_spk);
        if(!oa_strncmp(pd,temp_spk,30))
        {
            oa_memcpy(&gModuleConfigT.psw_t,lsl_param_get_password(),sizeof(PASSWORD_T));
            oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
            oa_strcpy(gModuleStateT.user_com_str,gModuleConfigT.psw_t.chr);
            
            USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
        }
        else
        {
            if(OA_TRUE == warn_msg) { return LSL_CMD_WRONG_WARNING; }
            else{ return LSL_CMD_WRONG; }
        }
    }
//    else if (!oa_strncmp(q, "STM32F103_ID_GET;", 17))
//    {
//        cfg_tell_103 = OA_TRUE;
//        return LSL_CMD_FIN;
//    }
    else if (!oa_strncmp(q, "STM32F103_CFG_ASK;", 18))
    {
        cfg_tell_103 = OA_FALSE;
        return LSL_CMD_FIN;
    }
    else if (!oa_strncmp(q, "STM32F103_CFG_GET;", 18))
    {
    	q += 18;
    	if(!oa_strncmp(q + 16,"VER",3))
    	{
        	cfg_tell_103 = OA_TRUE;
			oa_memcpy(version_of_other_chip,q,8);
			version_of_other_chip[8] = SOURCECODE_VERSION_FRAME;
			version_of_other_chip[9] = SOURCECODE_VERSION_RELEASE;
			version_of_other_chip[10] = SOURCECODE_VERSION_BETA;
			version_of_other_chip[11] = SOURCECODE_VERSION_DEBUG;
    	}
        return LSL_CMD_FIN;
    }
	else if(!oa_strncmp(q,"THISVERSION?",12))
	{
		if(cfg_tell_103 == OA_FALSE)
		{
			oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        	oa_sprintf(gModuleStateT.user_com_str,"Please Wait and Try Again.");
			USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
		}
		else
		{
			oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
        	oa_sprintf(gModuleStateT.user_com_str,"CHIP For Device:%d.%d.%d.%d;CHIP For Transmit:%d.%d.%d.%d;CHIP For GPRS:%d.%d.%d.%d",
				version_of_other_chip[0],version_of_other_chip[1],version_of_other_chip[2],version_of_other_chip[3],
				version_of_other_chip[4],version_of_other_chip[5],version_of_other_chip[6],version_of_other_chip[7],
				version_of_other_chip[8],version_of_other_chip[9],version_of_other_chip[10],version_of_other_chip[11]);
			USER_COM_STR_ACK(OA_SMSAL_UCS2_DCS);
		}
		return LSL_CMD_FIN;
	}
    else
    {
        return LSL_CMD_TRANS;
    }
    return LSL_CMD_RIGHT;
}

/********************************************************************* END OF LSL APP ***/



/*Warning: NEVER, NEVER, NEVER Modify the compile options!!!*/
#pragma arm section code = "OA_USER_ENTRY"
oa_bool oa_user_dll_entry_dispatch_to_sys(oa_user_dll_dispatch_sys_struct *param)
{
    param->verSr = OA_VERSION_NO;
    param->main_entry = oa_app_main;
    param->hw_init = oa_app_hw_init;
    
    /*if do not want to check, put zero(0x00) here*/
    param->api_check_sum = 0;

    param->trace_level = g_oa_trace_level;
    param->trace_port = g_oa_trace_port;
    param->ps_port = g_oa_ps_port;
    param->tst_port = g_oa_tst_port;
}
#pragma arm section code


