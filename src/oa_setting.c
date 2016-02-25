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
 *   oa_setting.c
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This module contains setting routines for OPENAT.
 *
 * Author:
 * -------
 *   simon.
 *
 ****************************************************************************/
#include "oa_global.h"
#include "oa_type.h"
#include "oa_api.h"
#include "oa_setting.h"
#include "oa_soc.h"

#define OA_SETTING_NVRAM_IDX  0

/*********** settings**********/
//typedef struct
//{
//    oa_uint8 beg_flag; //default is 0xee,for check the soc parameter is valid
//    oa_uint8 dtuid[OA_DTU_UNIID_MAX_LEN+1];//Dtu ID, also for heartbeat if needed
//    oa_soc_apn_info_struct apn_info;  //APN information for gprs
//    oa_sockaddr_struct soc_addr; //socket connect address
//
//    /*------Beg customer add other setting parameter-------*/
//
//    /*------End customer add other setting parameter-------*/
//    oa_uint8 end_flag; //default is 0xee, for check the soc parameter is valid
//}oa_set_param_struct;
//
////default configure
//static oa_set_param_struct oa_set_default = 
//{
//    0xee, //beg_flag , for check the soc parameter is valid
//    {"MD_123456"},    //module id: 
//    {
//      {"cmnet"},{""},{""}
//    },//apn info
//    {
//       OA_SOCK_STREAM, //connct_type
//       12,                         //addr len
//       {"cmt.2233.org"},   //connect addr, IP address or Domain name
//       80 //port:2000
//    }, //socket connect address
//
//    /*------Beg customer add other setting parameter-------*/
//
//    /*------End customer add other setting parameter-------*/
//    0xee  //end_flag,for check the soc parameter is valid
//};
//static oa_set_param_struct g_set_param={0};


typedef struct _set_config_s
{
    oa_uint8                    beg_flag;                           //default is 0xee,for check the soc parameter is valid

    oa_uint8                    dtuid[OA_DTU_UNIID_MAX_LEN + 1];    //Dtu ID, also for heartbeat if needed
    oa_soc_apn_info_struct      apn_info;                           //APN information for gprs
    oa_sockaddr_struct          soc_addr;                           //socket connect address

    /*------Beg customer add other setting parameter-------*/
    CLIENT_PARAM_T              client_t;
//    oa_uint8    domain_name[DOMAIN_NAME_MAX_LEN];
//    oa_uint8    ip_address[IP_ADDRESS_MAX_LEN];
//    
//    oa_uint16   port;
//
//    lsl_connect_mode_enum    con_mode;
//    oa_uint8       RESERVED1;
    
    PASSWORD_T                  psw_t;
    
    HEART_PKT_T                 heart_t;
    
    oa_uint8                    vir_id[VIRTUAL_ID_LEN];
    
    oa_uint8                    phone[PHONE_NUM_MAX][12];
    
    oa_uart_struct              uart2_t;
    
    oa_uint16                   auto_upload_time;
    oa_uint16                   RESERVED1;
    /*------End customer add other setting parameter-------*/

    oa_uint8 end_flag; //default is 0xee, for check the soc parameter is valid
}SET_CONFIG_T;

//default configure
static SET_CONFIG_T DefaultConfigT = 
{
    //beg_flag , for check the soc parameter is valid
    0xee, 
    //module id:
    {"MD251_123456"},
    //APN information for gprs
    {
      {"cmnet"},{""},{""}
    },
    //apn info  --  oa_sockaddr_struct
    {
        OA_SOCK_STREAM, //connct_type
        12,                         //addr len
        {"cmt.2233.org"},   //connect addr, IP address or Domain name
        80 //port:2000
    }, //socket connect address

    /*------Beg customer add other setting parameter-------*/
    {
        // Domain name
        //{"NO_Using_Domain_Name"},
        {"NO_Using_Domain_Name"},
        // IP address
        {"255,255,255,255"},
        // Port number
        6000,
        
        // Server connect mode
        LSL_CON_MODE_DOMAIN,
        
        //RESERVED1
        0xFF
    },
    // password
    {
        {"12345678"},
        8
    },
    {
        120000,
        {"GPRS_DTU_HEART_PKT! "}
    },
    // vir_id
    { "0000" },
    // phone
    { "" },
    
    {
        9600,//9600,   /* baudrate */
        oa_uart_len_8,              /* dataBits; */
        oa_uart_sb_1,               /* stopBits; */
        oa_uart_pa_none,            /* parity; */
        oa_uart_fc_none,            /* flow control */
        0x11,               /* xonChar; */
        0x13,               /* xoffChar; */
        OA_FALSE
    },
    
    // oa_uint16                   auto_upload_time;
    0x0000,
    // oa_uint16                   RESERVED1;
    0xFFFF,
    /*------End customer add other setting parameter-------*/
    
    0xee  //end_flag,for check the soc parameter is valid
};

static SET_CONFIG_T         g_set_param = {0};

extern oa_uart_enum         g_oa_ps_port;

extern void oa_soc_set_apn_cb(oa_bool result);
void oa_set_param_init(void)
{
    //read soc parameter from NVRAM
    oa_memcpy(&g_set_param,oa_nvram_read_reserve(OA_SETTING_NVRAM_IDX),sizeof(SET_CONFIG_T));
    //check the soc parameter is valid or not, if is invalid, will init for default value
    if(g_set_param.beg_flag != 0xee || g_set_param.end_flag != 0xee)
    {
       //write the default soc parameter to NVRAM
       if(oa_nvram_write_reserve((oa_uint8 *)&DefaultConfigT,sizeof(SET_CONFIG_T),OA_SETTING_NVRAM_IDX))
       {
          oa_memcpy(&g_set_param,&DefaultConfigT,sizeof(SET_CONFIG_T));
          OA_DEBUG_USER("%s:g_set_param init OK!",__func__);    
       }
       else
       {
          OA_DEBUG_USER("%s:g_set_param init failed!",__func__);    
       }
    }
}

oa_bool oa_set_param_save(void)
{
    OA_DEBUG_USER("%s:",__func__);
    //check the soc parameter is valid or not, if is dirty, will reset module
    if(g_set_param.beg_flag != 0xee || g_set_param.end_flag != 0xee)
    {
        OA_DEBUG_USER("Soc setting save, parameter is dirty,reset target!");  
        //restore soc parameter from NVRAM
        oa_memcpy(&g_set_param,oa_nvram_read_reserve(OA_SETTING_NVRAM_IDX),sizeof(SET_CONFIG_T));
        return OA_FALSE;
    }

    if(OA_FALSE == oa_nvram_write_reserve((oa_uint8 *)&g_set_param,sizeof(SET_CONFIG_T),OA_SETTING_NVRAM_IDX))
    {
        OA_DEBUG_USER("Soc setting save failed!");    
        //restore soc parameter from NVRAM
        oa_memcpy(&g_set_param,oa_nvram_read_reserve(OA_SETTING_NVRAM_IDX),sizeof(SET_CONFIG_T));
        return OA_FALSE;
    }
    return OA_TRUE;
}

//check the soc parameter is valid or not, if is dirty, will be loaded from NVRAM
void oa_set_param_check(void)
{
    if(g_set_param.beg_flag == 0xee && g_set_param.end_flag == 0xee)
        return;

    //read soc parameter from NVRAM
    oa_memcpy(&g_set_param,oa_nvram_read_reserve(OA_SETTING_NVRAM_IDX),sizeof(SET_CONFIG_T));
}

/*parse the setting cmd by at command or sms command
PARAM:  * pCmdStr : the command input,  * cmd_ret: the return string for at print or sms send
RETURN: OA_TRUE is setting cmd , OA_FALSE  not setting cmd
FORMAT:
     [xxx:,,#] for parameter set
     [xxx?] for parameter query
EXAMPLE: 
1. set gprs connect address: [GPRS:0,www.3322.org,2011#]
2. query gprs connect address: [GPRS?]
2. set dtu heartbeat and baudrate: [DTUS:10,9600#]
*/
oa_bool oa_set_param_cmd_parse(char * pCmdStr)
{
    oa_char *pParamBeg=NULL;    //pointer param string begin 
    oa_char *pParamEnd= NULL;   //pointer param string end
    oa_bool ret= OA_FALSE;
    oa_char cmd_ret[100]={0};

    if(pCmdStr==NULL)
        return OA_FALSE;

    if(!oa_strncmp(pCmdStr, "DTUID:", 6))
    {
        //set dtu id format= <DTUID:[id]#>
        oa_uint16 idLen= 0;
        oa_uint8   idStr[OA_DTU_UNIID_MAX_LEN+1]={0};

        //+DTUID: for cmd return;
        oa_sprintf(cmd_ret, "+DTUID:");

        pParamBeg=pCmdStr+6;
        pParamEnd = oa_strchr(pParamBeg, '#');
        idLen = pParamEnd-pParamBeg; 

        if(idLen <= OA_DTU_UNIID_MAX_LEN)
        {
            if(idLen > 0)
            {
                oa_memcpy(idStr,pParamBeg,idLen);
                oa_param_set_dtu_id(idStr);
            }
            else
            {
                //if dtuid=null, set IMEI as dtuid
                oa_param_set_dtu_id(oa_get_imei());
            }
        }
        else
        {
            goto oa_error;
        }

    }
    else if(!oa_strncmp(pCmdStr, "DTUID?", 6))
    {
        //Query the dtu id 
        oa_sprintf(cmd_ret, "+DTUID:%s#", oa_param_get_dtu_id());
    }
    else if(!oa_strncmp(pCmdStr, "GPRS:", 5))
    {
        //Set the socket connect addr and port. format= <GPRS:[connect type],[ip addr],[ip port]#>
        oa_char portStr[8] = {0};
        oa_sockaddr_struct  soc_addr={0};

        //+GPRS: for cmd return;
        oa_sprintf(cmd_ret, "+GPRS:");

        //set sock_type
        pParamBeg=pCmdStr+5;
        pParamEnd = oa_strchr(pParamBeg, ',');
        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)==1)
        {
           soc_addr.sock_type = *pParamBeg-'0';;
        }
        else
        {
           goto oa_error;
        }

        //set sock ip addr
        pParamBeg=pParamEnd+1;
        pParamEnd = oa_strchr(pParamBeg, ',');
        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)<OA_MAX_SOCK_ADDR_LEN)
        {
           oa_memcpy(soc_addr.addr,pParamBeg,(pParamEnd-pParamBeg));   
           soc_addr.addr_len = (pParamEnd-pParamBeg);
        }
        else
        {
           goto oa_error;
        }

        //set sock ip port
        pParamBeg=pParamEnd+1;
        pParamEnd = oa_strchr(pParamBeg, '#');
        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)<6)
        {
           oa_memcpy(portStr,pParamBeg,(pParamEnd-pParamBeg));   
           soc_addr.port=oa_atoi(portStr);
        }
        else
        {
           goto oa_error;
        }

        if( soc_addr.sock_type<=2)
        {
           oa_param_set_soc_addr(&soc_addr);
           oa_soc_close_req( );
        }
        else
        {
           goto oa_error;
        }
    }
    else if(!oa_strncmp(pCmdStr, "GPRS?", 5))
    {
        //Query the GPRS param
        oa_sprintf(cmd_ret, "+GPRS:%d,%s,%d#", 
                                      g_set_param.soc_addr.sock_type,
                                      g_set_param.soc_addr.addr,
                                      g_set_param.soc_addr.port);
    }
    else if(!oa_strncmp(pCmdStr, "APN:", 4))
    {
        //set dtu apn format= <APN:[apnname],[username],[userpwd]#>
        oa_soc_apn_info_struct  apn_info={0};

        //+APN: for cmd return;
        oa_sprintf(cmd_ret, "+APN:");

        pParamBeg=pCmdStr+4;
        pParamEnd = oa_strchr(pParamBeg, ',');

        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)<=20)
        {
          //copy apn name
           oa_memcpy(apn_info.apn,pParamBeg,(pParamEnd-pParamBeg));
        }
        else
        {
           goto oa_error;
        }

        pParamBeg=pParamEnd+1;
        pParamEnd = oa_strchr(pParamBeg, ',');

        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)<=16)
        {
          //copy username
           oa_memcpy(apn_info.userName,pParamBeg,(pParamEnd-pParamBeg));
        }
        else
        {
           goto oa_error;
        }

        pParamBeg=pParamEnd+1;
        pParamEnd = oa_strchr(pParamBeg, '#');

        if(pParamBeg!=NULL && pParamEnd!=NULL && (pParamEnd-pParamBeg)<=16)
        {
          //copy username
           oa_memcpy(apn_info.password,pParamBeg,(pParamEnd-pParamBeg));
        }
        else
        {
           goto oa_error;
        }

        oa_param_set_apninfo(&apn_info);
        //update gprs APN information
        oa_memcpy(&g_soc_context.apn_info,oa_param_get_apninfo(),sizeof(oa_soc_apn_info_struct));
        oa_gprs_set_apn_req(&g_soc_context.apn_info, oa_soc_set_apn_cb);
        oa_soc_close_req( );
    }
    else if(!oa_strncmp(pCmdStr, "APN?", 4))
    {
        oa_soc_apn_info_struct  * pApnInfo=oa_param_get_apninfo();

        //query dtu apn
        oa_sprintf(cmd_ret, "+APN:%s,%s,%s#", pApnInfo->apn, pApnInfo->userName,pApnInfo->password);
    }
    else
    {
         //unknow at cmd,+UNKNOW: for cmd return;
         oa_sprintf(cmd_ret, "+UNKNOW:");
         goto oa_error;
    }

    oa_uart_write(g_oa_ps_port, (oa_uint8 *)cmd_ret, oa_strlen(cmd_ret));
    return OA_TRUE;

oa_error:
    oa_uart_write(g_oa_ps_port,  (oa_uint8 *)cmd_ret, oa_strlen(cmd_ret));
    return OA_FALSE;
}

/*get functions*/
oa_uint8 *oa_param_get_dtu_id(void)
{
    oa_set_param_check();
    return g_set_param.dtuid;
}
oa_sockaddr_struct  *oa_param_get_soc_addr(void)
{
    oa_set_param_check();
    return &g_set_param.soc_addr;
}

oa_soc_apn_info_struct *oa_param_get_apninfo(void)
{
    oa_set_param_check();
    return &g_set_param.apn_info;
}

/*set functions*/
oa_bool oa_param_set_dtu_id(oa_uint8 * id)
{
    oa_bool ret = OA_FALSE;
    oa_uint16 id_len=oa_strlen((oa_char *)id);

    if(id_len<=OA_DTU_UNIID_MAX_LEN)
    {  
      oa_memset(g_set_param.dtuid, 0x00, OA_DTU_UNIID_MAX_LEN);
      oa_memcpy(g_set_param.dtuid, id, id_len);

      ret = oa_set_param_save();
    }
  
    return ret;
}

oa_bool oa_param_set_soc_addr(oa_sockaddr_struct * pData)
{
    oa_bool ret = OA_FALSE;

    oa_memcpy(&g_set_param.soc_addr,pData,sizeof(oa_sockaddr_struct));
    ret = oa_set_param_save();

    return ret;
}

oa_bool oa_param_set_apninfo(oa_soc_apn_info_struct *pData)
{
    oa_bool ret = OA_FALSE;

    oa_memcpy(&g_set_param.apn_info,pData,sizeof(oa_soc_apn_info_struct));
    ret = oa_set_param_save();

    return ret;
}

/*********************** LSL APP ******************/

/*--- SET PARAM ---*/

oa_bool lsl_param_set_psw(oa_uint8 *psw,oa_uint8 len)
{
    oa_bool res = OA_FALSE;
    oa_uint8    *q;
    oa_uint8    n;
    q = psw;
    n = len;
    
    oa_memset(g_set_param.psw_t.chr,0, PASSWORD_LEN_MAX);
    oa_memcpy(g_set_param.psw_t.chr, q, n);
    g_set_param.psw_t.len = n;
    
    res = oa_set_param_save();

    return res;
}

oa_bool lsl_param_set_domain(oa_uint8 *domain,oa_uint16 len,oa_uint8 *port)
{
    oa_bool res = OA_FALSE;
    oa_uint8    *dns,*pot;
    oa_uint16   n;
    dns = domain;
    n = len;
    pot = port;
    
    oa_memset(g_set_param.client_t.domain_name,0,DOMAIN_NAME_MAX_LEN);
    oa_memcpy(g_set_param.client_t.domain_name,dns,n);
    g_set_param.client_t.port = oa_atoi(pot);
    g_set_param.client_t.con_mode = LSL_CON_MODE_DOMAIN;
    
    res = oa_set_param_save();
    
    return res;
}

oa_bool lsl_param_set_ipaddr(oa_uint8 *ipaddr,oa_uint16 len,oa_uint8 *port)
{
    oa_bool res = OA_FALSE;
    oa_uint8    *ip,*pot;
    oa_uint16   n;
    ip = ipaddr;
    n = len;
    pot = port;
    
    oa_memset(g_set_param.client_t.ip_address,0,IP_ADDRESS_MAX_LEN);
    oa_memcpy(g_set_param.client_t.ip_address,ip,n);
    g_set_param.client_t.port = oa_atoi(pot);
    g_set_param.client_t.con_mode = LSL_CON_MODE_IP;
    
    res = oa_set_param_save();
    
    return res;
}

oa_bool lsl_param_set_heart_pkt_info(oa_uint8* chr,oa_uint16 len,oa_uint32 t)
{
    oa_bool res = OA_FALSE;
    
    oa_uint8    *c;
    oa_uint16   n;
    oa_uint32   tm;
    c = chr;
    n = len;
    tm = t;
    oa_memset(g_set_param.heart_t.chr,0,HEART_PKT_LEN_MAX);
    oa_memcpy(g_set_param.heart_t.chr,c,n);
    g_set_param.heart_t.t = tm;
    
    res = oa_set_param_save();
    
    return res;
}

oa_bool lsl_param_set_virtual_id(oa_uint8 *id,oa_uint16 len)
{
    oa_bool res = OA_FALSE;
    oa_uint8    *q;
    oa_uint16   n;
    q = id;
    n = len;
    oa_memset(g_set_param.vir_id,0,VIRTUAL_ID_LEN);
    oa_memcpy(g_set_param.vir_id,q,n);
    
    res = oa_set_param_save();
    
    return res;
}

oa_bool lsl_param_set_phone_table(oa_uint8 *num,oa_uint8 index)
{
    oa_bool res = OA_FALSE;
    oa_uint8    *q;
    oa_uint8    indx;
    q = num;
    indx = index;
    oa_memset(g_set_param.phone[indx],0,12);
    oa_memcpy(g_set_param.phone[indx],q,11);
    res = oa_set_param_save();
    return res;
}

oa_bool lsl_param_clr_phone_num(oa_uint8 index)
{
    oa_bool res = OA_FALSE;
    oa_uint8    indx;
    indx = index;
    oa_memset(g_set_param.phone[indx],0,12);
    res = oa_set_param_save();
    return res;
}

oa_bool lsl_param_set_uart2(oa_uart_struct* p)
{
    oa_bool res = OA_FALSE;
    oa_memcpy(&g_set_param.uart2_t,(oa_uint8*)p,sizeof(oa_uart_struct));
    
    res = oa_set_param_save();
    
    return res;
}

oa_bool lsl_param_set_auto_upload_time(oa_uint16 time)
{
    oa_bool res = OA_FALSE;
    g_set_param.auto_upload_time = time;
    res = oa_set_param_save();
    return res;
}

/*--- GET PARAM ---*/

CLIENT_PARAM_T *lsl_param_get_client_info(void)
{
    oa_set_param_check();
    return (&g_set_param.client_t);
}

PASSWORD_T *lsl_param_get_password(void)
{
    oa_set_param_check();
    return (&g_set_param.psw_t);
}

HEART_PKT_T *lsl_param_get_heart_pkt_info(void)
{
    oa_set_param_check();
    return (&g_set_param.heart_t);
}

oa_uint8 *lsl_param_get_virtual_id(void)
{
    oa_set_param_check();
    return (g_set_param.vir_id);
}

oa_uint8 *lsl_param_get_phone_table(void)
{
    oa_set_param_check();
    return (g_set_param.phone[0]);
}

oa_uart_struct* lsl_param_get_uart2(void)
{
    oa_set_param_check();
    return (&g_set_param.uart2_t);
}

oa_uint16 lsl_param_get_auto_upload_time(void)
{
    oa_set_param_check();
    return g_set_param.auto_upload_time;
}

/***************************** END OF LSL APP *****/

//#endif
