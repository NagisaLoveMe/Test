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
 *   oa_sms.c
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This Module gives apis for sms operation.
 *
 * Author:
 * -------
 *   simon!
 *
 ****************************************************************************/
#include "oa_global.h"
#include "oa_type.h"
#include "oa_api.h"
#include "oa_sms.h"
#include "oa_llist.h"
#include "oa_setting.h"


//oa sms init after SIM ready and network registered
void oa_sms_init(void)
{
    //Delete SMS storage
    oa_uart_send_at_cmd_req((oa_uint8 *)"at+cmgd=0,4\r\n",oa_strlen("at+cmgd=0,4\r\n"));
}

/*SMS send request result callback function*/
void  oa_sms_send_req_cb(os_sms_result result)
{
    OA_DEBUG_USER("%s: result=%d",__func__,result);
    oa_memset(&gSmsStateT,0,sizeof(SMS_STATE_T));
}

/* Example 1:send 7bit sms content "123ABC" to address "13812345678" */
void os_sms_send_demo_7bit(void)
{
    oa_sms_send_req(oa_sms_send_req_cb,"13812345678", "123ABC", oa_strlen((oa_char *)"123ABC"),OA_SMSAL_DEFAULT_DCS);
}

/* Example 2:send 8bit sms content "0x01,0x02,0x03,0xAA,0xBB,0xCC" to address "13812345678" */
void os_sms_send_demo_8bit(void)
{
    oa_uint8 smsSendData[]={0x01,0x02,0x03,0xAA,0xBB,0xCC,0x00};
    oa_sms_send_req(oa_sms_send_req_cb,"13812345678",  (oa_char *)smsSendData, 6,OA_SMSAL_8BIT_DCS);
}

/* Example 3:send UCS2 sms content "测试ABC" to address "13812345678" */
void os_sms_send_demo_ucs2(void)
{
    oa_char sendUcs2Str[100]={0};

    /*First,convert GB2312 coding to UCS2 coding*/
    oa_chset_convert(OA_CHSET_GB2312,
                                   OA_CHSET_UCS2,
                                   "测试ABC",
                                  sendUcs2Str,
                                   20); 
    oa_sms_send_req(oa_sms_send_req_cb,"13812345678", sendUcs2Str, oa_wstrlen((oa_wchar *)sendUcs2Str),OA_SMSAL_UCS2_DCS);
}


void lsl_sms_send_7bit(oa_uint8 *addr,oa_uint8 *p,oa_uint16 len)
{
    oa_uint8 *q;
    oa_uint16 n;
    q = p;
    n = len;
    if(n > 160) { n = 160; }
    oa_sms_send_req(oa_sms_send_req_cb,(oa_char *)addr,(oa_char *)q,n,OA_SMSAL_DEFAULT_DCS);
}

void lsl_sms_send_8bit(oa_uint8 *addr,oa_uint8 *p,oa_uint16 len)
{
    oa_uint8 *q;
    oa_uint16 n;
    q = p;
    n = len;
    if(n > 140) { n = 140; }
    oa_sms_send_req(oa_sms_send_req_cb,(oa_char *)addr,(oa_char *)q,n,OA_SMSAL_8BIT_DCS);
}

void lsl_sms_send_ucs2(oa_uint8 *addr,oa_uint8 *p,oa_uint16 len)
{
    oa_uint8 *q;
    oa_char ucs2_str[140] = {0};
    q = p;
    oa_chset_convert(OA_CHSET_GB2312,OA_CHSET_UCS2,(oa_char *)q,ucs2_str,70); 
    oa_sms_send_req(oa_sms_send_req_cb,(oa_char *)addr,ucs2_str,oa_wstrlen((oa_wchar *)ucs2_str),OA_SMSAL_UCS2_DCS);
}

oa_bool lsl_check_phone_binding(void)
{
    oa_bool res = OA_FALSE;
    oa_uint8 i;
    for(i = 0;i < PHONE_NUM_MAX;i++)
    {
        if(gModuleConfigT.phone[i][0] != 0) // now place not empty
        {
            if(!oa_strncmp(gSmsStateT.phone,gModuleConfigT.phone[i],11)) // the phone num has existed
            {
                res = OA_TRUE;
            }
        }
    }
    return res;
}

 /*****************************************************************/
 /*------------------sms received indication handler refer framework
 PARAM:
 deliver_num: SMS MO address
 timestamp: sms timestamp
                  timestamp[0]= nYear;
                  timestamp[1]= nMonth;
                  timestamp[2]= nDay;
                  timestamp[3]= nHour;
                  timestamp[4]= nMin;
                  timestamp[5]= nSec;

 data: sms text data
 len: sms data length,
        if dcs=7-bit or 8-bit, is the bytes length,
        if dcs=UCS2,is the unicode length
 dcs: refer oa_smsal_dcs_enum
 RETURN:
   OA_TRUE: the sms be handled by openat user, and will be be saved
   OA_FALSE:the sms is not openat user cared
******************************************************************/
oa_bool oa_sms_rcv_ind_handler(oa_char * deliver_num, oa_char * timestamp, oa_uint8 * data, oa_uint16 len, oa_smsal_dcs_enum dcs)
{
    lsl_cmd_state   res;
    
    OA_DEBUG_USER("SMS received:deliver_num=%s,timestamp=%d-%d-%d-%d:%d:%d,dcs=%d,len=%d,data=%s",\
    deliver_num,*(timestamp+0),*(timestamp+1),*(timestamp+2),*(timestamp+3),*(timestamp+4),*(timestamp+5),dcs,len,data);
    
    if(oa_strncmp(deliver_num,"+86",3)) {return OA_TRUE;}
        
    oa_memset(gSmsStateT.phone,0,12);
    oa_memcpy(gSmsStateT.phone,deliver_num + 3,11);
    
    oa_memset(gSmsStateT.rx,0,SMS_RX_BUFFERLEN);

    if(dcs == OA_SMSAL_DEFAULT_DCS)
    {
    	/*handle ascii sms text.*/
        OA_DEBUG_USER("OA_SMSAL_DEFAULT_DCS >>>\r\n");
        gSmsStateT.sms_dcs = OA_SMSAL_DEFAULT_DCS;
        //oa_chset_convert(OA_CHSET_UCS2,OA_CHSET_GB2312,data,gSmsStateT.rx,len);
        oa_memcpy(gSmsStateT.rx,data,len);
    }
    else if(dcs == OA_SMSAL_UCS2_DCS)
    {
    	/*handle unicode sms text.*/
    	OA_DEBUG_USER("OA_SMSAL_UCS2_DCS >>>\r\n");
    	gSmsStateT.sms_dcs = OA_SMSAL_UCS2_DCS;
    	oa_chset_convert(OA_CHSET_UCS2,OA_CHSET_GB2312,data,gSmsStateT.rx,len*2);
    }
  	else
    {
	    /*handle 8-bit sms text.*/
	    OA_DEBUG_USER("ELSE >>>\r\n");
	    gSmsStateT.sms_dcs = OA_SMSAL_8BIT_DCS;
  	}
  	
  	OA_DEBUG_USER("\r\n========SMS========\r\nPHONE: %s\r\nDAT=%s",gSmsStateT.phone,gSmsStateT.rx);
  	
  	res = protocol_analyze(LSL_SMS_CMD,gSmsStateT.rx,len);
  	
  	switch(res)
  	{
  	    case LSL_CMD_TRANS :
            break;
        case LSL_CMD_RIGHT :
            lsl_sms_send_7bit(gSmsStateT.phone, "OK!", oa_strlen((oa_char *)"OK!"));
            return OA_TRUE;
        case LSL_CMD_SAVE_ERROR :
            OA_DEBUG_USER("SMS TO USER -> 存储异常，恢复到出厂设置!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "存储异常，恢复到出厂设置!",\
                    oa_strlen((oa_char *)"存储异常，恢复到出厂设置!"));
            oa_memset(gModuleStateT.user_com_str,0,USER_COM_STR_LEN_MAX);
            oa_timer_start(LSL_TIMER_FACTORY_SET,lsl_factory_setting_timer_ind,NULL,999);
            return OA_TRUE;
        case LSL_CMD_FIN :
            return OA_TRUE;
        case LSL_CMD_RESERVED:
            break;
        case LSL_CMD_NOT_EXIST :
        case LSL_CMD_PASSWORD_ERR :
        case LSL_CMD_WRONG :
        case LSL_CMD_PHONE_NOT_BIND :
        case LSL_CMD_NEW_PSW_ILLEGAL:
            break;
        
        case LSL_CMD_NOT_EXIST_WARNING :
            OA_DEBUG_USER("SMS TO USER -> 命令不存在!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "命令不存在!", \
                    oa_strlen((oa_char *)"命令不存在!"));
            return OA_TRUE;
        case LSL_CMD_PASSWORD_ERR_WARNING :
            OA_DEBUG_USER("SMS TO USER -> 密码错误!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "密码错误!",\
                    oa_strlen((oa_char *)"密码错误!"));
            return OA_TRUE;
        case LSL_CMD_WRONG_WARNING :
            OA_DEBUG_USER("SMS TO USER -> 命令有误!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "命令有误!",\
                    oa_strlen((oa_char *)"命令有误!"));
            return OA_TRUE;
        case LSL_CMD_PHONE_NOT_BIND_WARNING :
            OA_DEBUG_USER("SMS TO USER -> 手机未绑定!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "手机未绑定!",\
                    oa_strlen((oa_char *)"手机未绑定!"));
            return OA_TRUE;
        case LSL_CMD_PHONE_NOT_IN_TABLE :
            OA_DEBUG_USER("SMS TO USER -> 手机列表无此号!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "手机列表无此号!",\
                    oa_strlen((oa_char *)"手机列表无此号!"));
            return OA_TRUE;
        case LSL_CMD_PHONE_TABLE_EMPTY :
            OA_DEBUG_USER("SMS TO USER -> 手机列表无号码!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "手机列表无号码!",\
                    oa_strlen((oa_char *)"手机列表无号码!"));
            return OA_TRUE;
        case LSL_CMD_PHONE_TABLE_FULL :
            OA_DEBUG_USER("SMS TO USER -> 手机列表已满!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "手机列表已满!",\
                    oa_strlen((oa_char *)"手机列表已满!"));
            return OA_TRUE;
        case LSL_CMD_NEW_PSW_ILLEGAL_WARNING :
            OA_DEBUG_USER("SMS TO USER -> 密码应为字母或数字!");
            lsl_sms_send_ucs2(gSmsStateT.phone, "密码应为字母或数字!",\
                    oa_strlen((oa_char *)"密码应为字母或数字!"));
            return OA_TRUE;
  	}
  	
  	lsl_uart_485_tx(OA_UART2,gSmsStateT.rx,len);
  	
	return OA_TRUE;/*delete current SMS from memory*/
	//return OA_FALSE;/*do not delete this SMS, user can handle it*/
   /*Warning: if user return OA_FALSE to save current sms in memory, 
      he should pay attention to the memory capacity. 
      Please use at+cpms to confirm current memory capacity and, when the memory is full, 
      user will receive AT command response "+OPENAT: SMS FULL" from PS port 
      and system will clean all the SMS in order to receive more SMS.*/
}
/************************/


