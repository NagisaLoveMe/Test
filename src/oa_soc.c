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
 *   oa_soc.c
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This module contains gprs socket connect for OPENAT.
 *
 * Author:
 * -------
 *   simon.
 *
 ****************************************************************************/
#include "oa_global.h"
#include "oa_type.h"
#include "oa_api.h"
#include "oa_soc.h"

#include "oa_setting.h"


/********************************** LSL SOC ***********************************/
static volatile oa_bool gp_result;

oa_uint32 hrt_dog_t = 0;
/********************************************************* END OF LSL SOC *****/

/*
* do not re-connect the network immediately after the old one closed.
* wait a period of time (for example, 5sec) to detach network,and then re-connect
*/
#define OA_GPRS_WAITING_RELEASING 5000 /*mSec*/

//Gprs heartbeat time, for keep gprs connecting, default 60s, if not use heartbeat, set OA_GPRS_HEARTBEAT_TIME = 0
#define OA_GPRS_HEARTBEAT_TIME  60

//Gprs rx/tx buffer length
#define OA_MAX_SOC_RCV_BUFLEN   30000
#define OA_MAX_SOC_SEND_BUFLEN  20480

oa_uint8 gprs_rx_buffer[OA_MAX_SOC_RCV_BUFLEN+1]={0}; // gprs接收数据缓冲区, 30K bytes
oa_uint8 gprs_tx_buffer[OA_MAX_SOC_SEND_BUFLEN+1]={0}; // gprs 发送缓冲区，gprs一次最大能发送1400 bytes

oa_soc_context g_soc_context = {0};
extern void oa_soc_set_apn_cb(oa_bool result);
extern void oa_soc_fill_addr_struct(void);
extern void oa_soc_notify_ind(void *inMsg);

void oa_soc_init(void)
{
    //init socket context
    g_soc_context.state = OA_SOC_STATE_OFFLINE;
    g_soc_context.socket_id = -1;
    g_soc_context.can_connect = OA_TRUE;
    g_soc_context.is_blocksend = OA_FALSE;
    g_soc_context.recon_counter = 0;
    
    /*create gprs tx ring buffers*/
    g_soc_context.gprs_tx = oa_create_ring_buffer_noinit(gprs_tx_buffer, sizeof(gprs_tx_buffer));

    //fill socket connect server address 
    oa_soc_fill_addr_struct();

    //register network APN, only set APN success,can connect network
    //oa_gprs_set_apn_req(&g_soc_context.apn_info, oa_soc_set_apn_cb);
    //get gprs APN information from nvram
    oa_memcpy(&g_soc_context.apn_info,oa_param_get_apninfo(),sizeof(oa_soc_apn_info_struct));
    //init APN information
    oa_gprs_apn_init(&g_soc_context.apn_info);

    //register socket network notify event callback function 
    oa_soc_notify_ind_register(oa_soc_notify_ind);
}

//Get and init socket address for connect request 
void oa_soc_fill_addr_struct(void)
{
    oa_bool ip_validity = OA_FALSE;
    oa_uint8 ip_addr_digit[20] = {0};

    //get socket connect address
    oa_memcpy(&g_soc_context.soc_addr,oa_param_get_soc_addr(),sizeof(oa_sockaddr_struct));

    //check the socket cnnect address is ip address or domain name
    oa_soc_ip_check((oa_char *)g_soc_context.soc_addr.addr,ip_addr_digit,&ip_validity);
    if(ip_validity)
        g_soc_context.addr_type = OA_IP_ADDRESS;
    else
        g_soc_context.addr_type = OA_DOMAIN_NAME;

}

void oa_soc_set_apn_cb(oa_bool result)
{
    OA_DEBUG_USER("%s:set apn ok! %s, %s, %s", __func__, g_soc_context.apn_info.apn, g_soc_context.apn_info.userName, g_soc_context.apn_info.password);
    g_soc_context.can_connect = OA_TRUE;
}

//Looped check the socket connect state. if offline and can connect, do connect request
void oa_soc_state_check(void)
{
    static oa_uint16 check_counter=0;
    if(g_soc_context.state == OA_SOC_STATE_OFFLINE)
    {
        oa_sockaddr_struct  * pSocAddr = oa_param_get_soc_addr();

        //if gprs recon_counter >=20, it's better to restart module, user also can close it
        if(g_soc_context.recon_counter>=20)
        {
            OA_DEBUG_USER("%s:restart module!",__func__);
            oa_module_restart( NULL );
            return;
        }

        //can connect and is valid sock_type, goto connect request
        if(g_soc_context.can_connect && (pSocAddr->sock_type<2))
        {
            oa_soc_connect_req();
            g_soc_context.recon_counter++;
        }
    }
    else if(g_soc_context.state == OA_SOC_STATE_ONLINE)
    {

        oa_int32 retransmit_times = oa_soc_tcp_retransmit_times(g_soc_context.socket_id);
        
        if(retransmit_times>=3)
        {/*warning: any data sent during a phone call, TCP layer will retransmit it
         *  that will cause a nonsensical counter by oa_tcpip_get_tcp_retransmit_times after a call
         */
            OA_DEBUG_USER("%s:retransmit too many times=%d!",__func__,retransmit_times);
            oa_soc_close_req( );
        }
        g_soc_context.recon_counter = 0;
    }
}

//check current connect state. if state == offline, do socket connect request
void oa_soc_connect_req(void)
{
    oa_int16 ret=OA_SOC_SUCCESS;

    OA_DEBUG_USER("%s:state=%d", __func__,g_soc_context.state);

    if(g_soc_context.state == OA_SOC_STATE_ONLINE) return;

    if(g_soc_context.state == OA_SOC_STATE_OFFLINE)
    {
        //fill the socket address first
        oa_soc_fill_addr_struct( );
        
        if(g_soc_context.soc_addr.sock_type == OA_SOCK_STREAM 
        || g_soc_context.soc_addr.sock_type == OA_SOCK_DGRAM )
        {
            //create socket id firstly. socket id range(0-10), if ret id <0, it's error
            g_soc_context.socket_id = oa_soc_create(g_soc_context.soc_addr.sock_type, 0);
        }
        else
        {
            //invalid sock_type, return
            OA_DEBUG_USER("%s:sock_type invalid!", __func__);
            return;
        }
        
        if(g_soc_context.socket_id>=0)
        {
            OA_DEBUG_USER("%s:sock_id=%d create ok!", __func__,g_soc_context.socket_id);
        }
        
        OA_DEBUG_USER("Ready to connect:addr=%s,port=%d,soc_type=%d,addr_type=%d", 
                               g_soc_context.soc_addr.addr, 
                               g_soc_context.soc_addr.port, 
                               g_soc_context.soc_addr.sock_type,
                               g_soc_context.addr_type);
        
        //if create socket id success, goto socket connect request
        ret = oa_soc_connect(g_soc_context.socket_id, g_soc_context.addr_type, &g_soc_context.soc_addr);
        
        
        if(ret == OA_SOC_WOULDBLOCK)
        {  
            OA_DEBUG_USER("%s:sock_id=%d connect block waiting!",__func__,g_soc_context.socket_id);
            //wait for connect indication.will received asynchronous info in oa_soc_notify_ind
            g_soc_context.state = OA_SOC_STATE_CONNECT;
        }
        else if(ret == OA_SOC_SUCCESS)
        {
            OA_DEBUG_USER("%s:sock_id=%d connect ok!",__func__,g_soc_context.socket_id);
            //socket already connected
            oa_sleep(100); //just connected need delay 100ms then send gprs data, else may lost data
            g_soc_context.state = OA_SOC_STATE_ONLINE;
        
            oa_soc_start_heartbeat_timer();
        
            oa_soc_send_req();
        
        }
        else
        {
            OA_DEBUG_USER("%s:sock_id=%d connect fail ret=%d!",__func__,g_soc_context.socket_id,ret);
            oa_soc_close_req();
        }
    }
    
    return;
}

//Example: gprs send request demo, for user reference
void oa_soc_send_data_demo(void)
{
    oa_uint16 ret = 0;

    //first push gprs send data "gprs test data"  in gprs_tx buffer
    ret = oa_write_buffer_force_noinit(g_soc_context.gprs_tx, "gprs test data", oa_strlen("gprs test data"));  

    //then call send request
    oa_soc_send_req();
}

void oa_soc_send_req(void)
{
    oa_uint16 len;
    oa_int16 ret;

     /*if not online or sending state, return*/
    if(g_soc_context.state != OA_SOC_STATE_ONLINE
    || g_soc_context.is_blocksend == OA_TRUE)
        return;
    
    //once socket send, max len is 1024 bytes
    len = oa_query_buffer_noinit(
        g_soc_context.gprs_tx, 
        g_soc_context.gprs_tx_pending_data,
        OA_APP_GPRS_SENDING_SIZE);
    g_soc_context.gprs_tx_pending_size = len;
    
    if(len>0)
    {
        ret = oa_soc_send(g_soc_context.socket_id, g_soc_context.gprs_tx_pending_data, len);

        if(ret>=OA_SOC_SUCCESS)
        {
           OA_DEBUG_USER("%s:sock_id=%d send ok ret=%d\r\n",__func__,g_soc_context.socket_id,ret);

           //if send success,dummy read to delete
            len = oa_read_buffer_noinit(
                g_soc_context.gprs_tx, 
                g_soc_context.gprs_tx_pending_data,
                g_soc_context.gprs_tx_pending_size);
            g_soc_context.gprs_tx_pending_size = 0;

            //check gprs_tx buff and continue send gprs data
            oa_soc_send_req( );

            oa_soc_start_heartbeat_timer();

        }
        else if(ret==OA_SOC_WOULDBLOCK)
        {
            g_soc_context.is_blocksend = OA_TRUE;

            OA_DEBUG_USER("%s:sock_id=%d send block waiting!",__func__,g_soc_context.socket_id);
            //wait for sending result , care event OA_SOC_WRITE in callback function oa_soc_notify_ind
        }
        else
        {
            //ERROR!.
            OA_DEBUG_USER("%s:sock_id=%d send fail ret=%d!",__func__,g_soc_context.socket_id,ret);
            oa_soc_close_req( );
            return;
        }
    }
}


void oa_soc_can_reconnect_again(void* param)
{
    OA_DEBUG_USER("%s called", __func__);
    g_soc_context.can_connect = OA_TRUE;
}

void oa_soc_close_req( void )
{
    oa_int8 ret = 0;

    g_soc_context.can_connect = OA_TRUE;

    g_soc_context.is_blocksend = OA_FALSE;

    if(g_soc_context.state == OA_SOC_STATE_OFFLINE) return;

    if(g_soc_context.socket_id>=0)
      ret = oa_soc_close(g_soc_context.socket_id);

    OA_DEBUG_USER("%s:socket_id=%d,ret=%d", __func__,g_soc_context.socket_id,ret);

    g_soc_context.state = OA_SOC_STATE_OFFLINE;

    g_soc_context.socket_id = -1;

	//oa_sleep(300);//need delete here.

    //do not reconnect immediately
	oa_evshed_start(OA_EVSHED_ID_0, oa_soc_can_reconnect_again, NULL, OA_GPRS_WAITING_RELEASING);
}

void oa_soc_gprs_recv(oa_uint8* data, oa_uint16 len)
{
    // received gprs data, 
//    OA_DEBUG_USER("%s:len=%d, data=%s!", __func__,len,data);
//
//	oa_uart_write(OA_UART2, (oa_uint8 *)data, len);

    OA_DEBUG_USER("%s:",__func__);
    OA_DEBUG_USER("LEN = %d,DATAS = %s",len,data);
    lsl_uart_485_tx(OA_UART2,data,len);
}

//Socket Notify Event indication handler framework
void oa_soc_notify_ind(void *inMsg)
{
    oa_int32 ret = 0;    
    oa_app_soc_notify_ind_struct *soc_notify = (oa_app_soc_notify_ind_struct*) inMsg;

    //if other application's socket id, ignore it.
    if(soc_notify->socket_id != g_soc_context.socket_id)
    {
       OA_DEBUG_USER("%s:sock_id=%d unknow, event_type=%d!",__func__,soc_notify->socket_id,soc_notify->event_type);    
       return;
    }
    
    switch (soc_notify->event_type)
    {
        case OA_SOC_WRITE:
        {
            /* Notify for send data*/
            if (soc_notify->result == OA_TRUE)
            {
                g_soc_context.is_blocksend = OA_FALSE;
                //resend the data ,else will lost data
                OA_DEBUG_USER("%s:sock_id=%d resend!",__func__,soc_notify->socket_id);
                oa_soc_send_req( );
            }
            else
            {
                OA_DEBUG_USER("%s:sock_id=%d send fail err=%d!",__func__,soc_notify->socket_id,soc_notify->error_cause);
                oa_soc_close_req( );
            }     
           
           break;
        }
            
        case OA_SOC_READ:
        {
          /* Notify for received data */
          if (soc_notify->result == OA_TRUE)
          {
            do
            {
                    oa_memset(gprs_rx_buffer, 0 , (OA_MAX_SOC_RCV_BUFLEN*sizeof(oa_uint8))); 

              //received gprs data, read data for protocol
              ret = oa_soc_recv(soc_notify->socket_id , (oa_uint8*)gprs_rx_buffer, OA_MAX_SOC_RCV_BUFLEN, 0);
              OA_DEBUG_USER("%s:sock_id=%d read ok ret=%d!",__func__,soc_notify->socket_id,ret);

              if(ret > 0)
              {   
                // read data length=ret, 
                oa_soc_gprs_recv((oa_uint8*)gprs_rx_buffer, ret);
              }
              else
              {
                //read data error or block
              }  
            }while(ret>0); //Make sure use  do{...}while  to read out all received data.
          }
          else
          {
            OA_DEBUG_USER("%s:sock_id=%d read fail err=%d!",__func__,soc_notify->socket_id,soc_notify->error_cause);
            oa_soc_close_req( );
          }     
          break;
        }
            
        case OA_SOC_CONNECT:
        {
            if (soc_notify->result == OA_TRUE)
            {
                OA_DEBUG_USER("%s:sock_id=%d connect ok!",__func__,soc_notify->socket_id);

                //if connect OK, goto query gprs_tx data and send
               oa_sleep(100); //just connected need delay 100ms then send gprs data, else may lost data

                g_soc_context.state = OA_SOC_STATE_ONLINE;
                oa_soc_send_req( );
            }
            else
            {
                OA_DEBUG_USER("%s:sock_id=%d connect fail err=%d!",__func__,soc_notify->socket_id,soc_notify->error_cause);
                oa_soc_close_req( );
            }
            break;
        }       
            
        case OA_SOC_CLOSE:
        {
            // the socket is closed by remote server, use soc_close to close local socketID, else will lead OA_SOC_LIMIT_RESOURCE

            OA_DEBUG_USER("%s:sock_id=%d close,error_cause=%d",__func__,soc_notify->socket_id,soc_notify->error_cause);

            oa_soc_close_req();

            break;
        }
            
        case OA_SOC_ACCEPT:
            break;
            
        default:
            break;
    }

    oa_soc_start_heartbeat_timer();
}

//begin a loop to send heartbeat data for keep gprs connecting
void oa_soc_start_heartbeat_timer(void)
{
    if(g_soc_context.state == OA_SOC_STATE_ONLINE 
      && OA_GPRS_HEARTBEAT_TIME>0)
    {
       oa_timer_start(OA_TIMER_FOR_HEARTBEAT, oa_soc_send_heartbeat_data, NULL, OA_GPRS_HEARTBEAT_TIME*1000);
    }
}

//begin a loop to send heartbeat data for keep gprs connecting
void oa_soc_send_heartbeat_data(void* param)
{
     oa_int16 ret;

     /*if not online or sending state, return*/
     if(g_soc_context.state != OA_SOC_STATE_ONLINE
        || g_soc_context.is_blocksend == OA_TRUE)
        return;

     //send heartbeat package "gprs heartbeat data", user can modify it
     ret = oa_soc_send(g_soc_context.socket_id, "gprs heartbeat data", oa_strlen((oa_char*)"gprs heartbeat data"));

     OA_DEBUG_USER("%s: ret=%d!", __func__,ret);

     if(ret==OA_SOC_WOULDBLOCK)
     {
         //wait for sending result , care event OA_SOC_WRITE in callback function oa_soc_notify_ind
         g_soc_context.is_blocksend = OA_TRUE;        
     }

     //restart timer for loop send
     oa_soc_start_heartbeat_timer();
}

/************************************************************/
/********************** noinit buffer************************/
/************************************************************/
/*function to handle buffers */
//#define PTR_NOINIT_BUF_BEGIN 
oa_buffer *oa_create_ring_buffer_noinit(oa_uint8 *rawBuf, oa_uint32 maxLength)
{
    oa_buffer *temp_buf = (oa_buffer*)oa_sram_noinit_get_buf();

	//OA_DEBUG_USER("[GPRS] LOOK BEFORE %X:%X", temp_buf->buf, ((oa_uint8*)temp_buf + sizeof(oa_buffer)));
	
	if(temp_buf->buf != (oa_uint8*)((oa_uint8*)temp_buf + sizeof(oa_buffer)))//first use SRAM
	{
		oa_sram_noinit_reset();
	    temp_buf->buf = (oa_uint8*)((oa_uint8*)temp_buf + sizeof(oa_buffer));
	    temp_buf->len = 0;
	    temp_buf->offset = 0;
	    temp_buf->maxLength = maxLength;
		OA_DEBUG_USER("[GPRS] SRAM buffer first init! %X:%X\r\n", temp_buf->buf, ((oa_uint8*)temp_buf + sizeof(oa_buffer)));
	}
	else
	{
		OA_DEBUG_USER("[GPRS] SRAM buffer inited before. datalen=%d\r\n", temp_buf->len);
	}
	
    return temp_buf;
}

void oa_free_buffer_noinit(oa_buffer *buf)
{
}

oa_uint32 oa_write_buffer_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len)
{
    oa_uint32 i, p;

	oa_buffer *temp_buf = (oa_buffer*)oa_sram_noinit_get_buf();
	if(temp_buf->buf != (oa_uint8*)((oa_uint8*)temp_buf + sizeof(oa_buffer)))//first use SRAM
	{
		return (oa_uint32)0;
	}

    if(buf->len + len > buf->maxLength)
    {
        len = buf->maxLength - buf->len;
    }

    for(i = 0, p = buf->offset + buf->len; i < len; i++, p++)
    {
        if(p >= buf->maxLength)
        {
            p -= buf->maxLength;
        }

        buf->buf[p] = data[i];
    }
  
    buf->len += len;
  
    return len;
}

oa_uint32 oa_query_buffer_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len)
{
    oa_uint32 i, p;
	
	oa_buffer *temp_buf = (oa_buffer*)oa_sram_noinit_get_buf();
	
	if(temp_buf->buf != (oa_uint8*)((oa_uint8*)temp_buf + sizeof(oa_buffer)))//first use SRAM
	{
		return (oa_uint32)0;
	}

    if(buf->len < len)
    {
        len = buf->len;
    }
  
    for(i = 0, p = buf->offset; i < len; i++, p++)
    {
        if(p >= buf->maxLength)
        {
            p -= buf->maxLength;
        }

        data[i] = buf->buf[p];
    }

    //for query, don't update buffer control struct.
    //buf->len -= len;
    //buf->offset = p;  

    return len;
}

oa_uint32 oa_read_buffer_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len)
{
    oa_uint32 l;
	
	oa_buffer *temp_buf = (oa_buffer*)oa_sram_noinit_get_buf();
	if(temp_buf->buf != (oa_uint8*)((oa_uint8*)temp_buf + sizeof(oa_buffer)))//first use SRAM
	{
		return (oa_uint32)0;
	}
	
    l = oa_query_buffer_noinit(buf, data, len);
  
    buf->len -= l;
    buf->offset += l;
    if(buf->offset > buf->maxLength)
        buf->offset -= buf->maxLength;

    return l;
}

oa_uint32 oa_write_buffer_force_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len)
{
    oa_uint32 i, p, cut_off=0, real_len;

	oa_buffer *temp_buf = (oa_buffer*)oa_sram_noinit_get_buf();
	if(temp_buf->buf != (oa_uint8*)((oa_uint8*)temp_buf + sizeof(oa_buffer)))//first use SRAM
	{
		return (oa_uint32)0;
	}

	real_len = len;
 
    if(buf->len + len > buf->maxLength)
    {
		cut_off = len - (buf->maxLength - buf->len);
        len = buf->maxLength - buf->len;
    }

    for(i = 0, p = buf->offset + buf->len; i < len; i++, p++)
    {
        if(p >= buf->maxLength)
        {
            p -= buf->maxLength;
        }

        buf->buf[p] = data[i];
    }
	
	if(cut_off)
	{
		
	    for(i = len, p = buf->offset; i < real_len; i++, p++)
	    {
	        if(p >= buf->maxLength)
	        {
	            p -= buf->maxLength;
	        }

	        buf->buf[p] = data[i];
	    }

		buf->offset += cut_off;
		if(buf->offset >= buf->maxLength)
			buf->offset -= buf->maxLength;
	}
	
	buf->len += real_len;
	if(buf->len > buf->maxLength)
		buf->len = buf->maxLength;
  
    return real_len;
}

/********************************** LSL SOC ***********************************/


/************** DESIGN **************/






/*******************************************************************************
*   Function name:      client_set_receive_callback
*   Descriptions:       set user client receive callback function
*   Parameters:         client_receive_cb_fun   clientcf
*   Returned value:     NONE
*******************************************************************************/
void client_set_receive_callback(client_receive_cb_fun clientcf) {
    
    OA_DEBUG_USER("Function -> client_set_receive_callback ...");
    if(clientcf != NULL) {
        gClientStateT.client_recv_call_back = clientcf;
    }
}

void lsl_client_gprs_datas_deal(oa_uint8 sock_id,oa_uint8 *datas,oa_uint16 len)
{
    lsl_cmd_state   res;
    
    OA_DEBUG_USER("%s:",__func__);
    //OA_DEBUG_USER("LEN = %d,DATAS = %s",len,datas);
    res = protocol_analyze(LSL_SOC_CMD,datas,len);
    
    switch(res)
    {
        case LSL_CMD_TRANS :
            break;
        case LSL_CMD_RIGHT :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"OK! ",oa_strlen("OK! "));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_SAVE_ERROR :
            //lsl_uart_485_tx(OA_UART2,"存储异常，恢复到出厂设置!",oa_strlen("存储异常，恢复到出厂设置!"));
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"存储异常，恢复到出厂设置!",oa_strlen("存储异常，恢复到出厂设置!"));
                lsl_soc_send_req();
            }
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
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"存储异常，恢复到出厂设置!",oa_strlen("存储异常，恢复到出厂设置!"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_PASSWORD_ERR_WARNING :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"密码错误",oa_strlen("密码错误"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_WRONG_WARNING :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"命令有误",oa_strlen("命令有误"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_PHONE_NOT_BIND_WARNING :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"手机未绑定",oa_strlen("手机未绑定"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_PHONE_NOT_IN_TABLE :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"手机列表无此号!",oa_strlen("手机列表无此号!"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_PHONE_TABLE_EMPTY :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"手机列表无号码!",oa_strlen("手机列表无号码!"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_PHONE_TABLE_FULL :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"手机列表已满!",oa_strlen("手机列表已满!"));
                lsl_soc_send_req();
            }
            return;
        case LSL_CMD_NEW_PSW_ILLEGAL_WARNING :
            if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
            {
                oa_write_buffer_noinit(gClientStateT.gprs_tx,"密码应为字母或数字",oa_strlen("密码应为字母或数字"));
                lsl_soc_send_req();
            }
            return;

    }
    
    lsl_uart_485_tx(OA_UART2,datas,len);
}

//begin a loop to send heartbeat data for keep gprs connecting

static oa_uint8 protocol_heart_pkt[34] = 
{
    //3
    0xAA,0xF9,0x1F,
    //8
    0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,
    //4 NODEID
    0xFF,0xFF,0xFF,0xFF,
    //12
    0x4D,0x6F,0x64,0x75,0x6C,0x65,0x53,0x74,0x72,0x69,0x6E,0x67,
    //4
    0x01,0x00,0x00,0x00,
    //2 CRC
    0xFF,0xFF,
    //1
    0xDD
};

void lsl_soc_send_heartbeat_data(void)
{
    oa_int16 ret;
    oa_uint16 i;
    oa_uint16 crc16;
    oa_uint32 temp_u32;

    /*if not online or sending state, return*/
    if(gClientStateT.st_cse != OA_SOC_STATE_ONLINE
    || gClientStateT.is_blocksend == OA_TRUE)
    {
        return;
    }
     //send heartbeat package "gprs heartbeat data", user can modify it
    //ret = oa_soc_send(gClientStateT.sid_cse,gModuleConfigT.heart_t.chr,oa_strlen(gModuleConfigT.heart_t.chr));
    
    //OA_DEBUG_USER("%s: ret=%d!", __func__,ret);

    temp_u32 = oa_strtoul(gModuleConfigT.vir_id,NULL,16);
    for(i = 0;i < 4;i++)
    {
        protocol_heart_pkt[11 + i] = (temp_u32 >> (24 - 8 * i)) & 0xFF; 
    }
    
    crc16 = crc16_ccitt(protocol_heart_pkt,31);
    
    protocol_heart_pkt[31] = (crc16 >> 8) & 0xFF;
    protocol_heart_pkt[32] = crc16 & 0xFF;


    oa_write_buffer_noinit(gClientStateT.gprs_tx,protocol_heart_pkt,34);
    lsl_soc_send_req();

    if(ret == OA_SOC_WOULDBLOCK)
    {
        //wait for sending result , care event OA_SOC_WRITE in callback function oa_soc_notify_ind
        gClientStateT.is_blocksend = OA_TRUE;        
    }

    //restart timer for loop send
    //oa_soc_start_heartbeat_timer();
}


void lsl_soc_send_data(void)
{
    oa_uint16 ret = 0;

    //first push gprs send data "gprs test data"  in gprs_tx buffer
    //ret = oa_write_buffer_force_noinit(g_soc_context.gprs_tx, "gprs test data", oa_strlen("gprs test data"));  

    //then call send request
    lsl_soc_send_req();
}

void lsl_soc_send_req(void)
{
    oa_uint16 len;
    oa_int16 ret;

     /*if not online or sending state, return*/
    if((gClientStateT.st_cse != OA_SOC_STATE_ONLINE) 
        || (gClientStateT.is_blocksend == OA_TRUE))
    {
        return;
    }
    
    //once socket send, max len is 1024 bytes
    len = oa_query_buffer_noinit(
        gClientStateT.gprs_tx, 
        gClientStateT.se_tx_pending,
        MAX_GPRS_TX_PENDING_LEN);
    gClientStateT.se_tx_pending_size = len;
    
    if(len > 0)
    {
        ret = oa_soc_send(gClientStateT.sid_cse,gClientStateT.se_tx_pending,len);

        if(ret >= OA_SOC_SUCCESS)
        {
           OA_DEBUG_USER("%s:sock_id=%d send ok ret=%d\r\n",__func__,gClientStateT.sid_cse,ret);

           //if send success,dummy read to delete
            len = oa_read_buffer_noinit(
                gClientStateT.gprs_tx, 
                gClientStateT.se_tx_pending,
                gClientStateT.se_tx_pending_size);
            gClientStateT.se_tx_pending_size = 0;

            //check gprs_tx buff and continue send gprs data
            lsl_soc_send_req();

        }
        else if(ret == OA_SOC_WOULDBLOCK)
        {
            gClientStateT.is_blocksend = OA_TRUE;

            OA_DEBUG_USER("%s:sock_id=%d send block waiting!",__func__,gClientStateT.sid_cse);
            //wait for sending result , care event OA_SOC_WRITE in callback function oa_soc_notify_ind
        }
        else
        {
            //ERROR!.
            OA_DEBUG_USER("%s:sock_id=%d send fail ret=%d!",__func__,gClientStateT.sid_cse,ret);
            lsl_soc_close_req();
            return;
        }
    }
}

void lsl_soc_close_req(void)
{
    oa_int8 ret = 0;

    gClientStateT.is_blocksend = OA_FALSE;
    
    if(gClientStateT.st_cse == OA_SOC_STATE_OFFLINE) { return; }

    if(gClientStateT.sid_cse >= 0)
    {
        ret = oa_soc_close(gClientStateT.sid_cse);
    }
    OA_DEBUG_USER("%s:socket_id=%d,ret=%d", __func__,gClientStateT.sid_cse,ret);

    gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;

    gClientStateT.sid_cse = -1;
    
    hrt_dog_t = 0;
}

void lsl_debug_soc_send(oa_uint8 *pBuf,oa_uint32 len)
{
    if(OA_SOC_STATE_ONLINE == gClientStateT.st_cse)
    {
        oa_soc_send(gClientStateT.sid_cse,pBuf,len);
    }
}

//void lsl_debug_soc_close_req(void)
//{
//    if(gClientStateT.sid_cse >= 0)
//    {
//        oa_soc_close(gClientStateT.sid_cse);
//        gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
//        offline_cnt = 0;
//    }
//}

void heart_pkt_timer_ind(void)
{
    oa_timer_stop(LSL_TIMER_HEART_PKT);
    //lsl_debug_soc_send(gModuleConfigT.heart_t.chr,oa_strlen(gModuleConfigT.heart_t.chr));
    lsl_soc_send_heartbeat_data();
    if(gModuleConfigT.heart_t.t >= HEART_TIME_MIN)
    {
        oa_timer_start(LSL_TIMER_HEART_PKT,heart_pkt_timer_ind,NULL,gModuleConfigT.heart_t.t);
    }
}



void gprs_init(void)
{
    OA_DEBUG_USER("%s:",__func__);
    
    OA_DEBUG_USER("Domain:->%s",gClientStateT.client_t.domain_name);
    OA_DEBUG_USER("IP Addr:->%s",gClientStateT.client_t.ip_address);
    OA_DEBUG_USER("Port Num:->%d",gClientStateT.client_t.port);
    if(LSL_CON_MODE_DOMAIN == gClientStateT.client_t.con_mode)
    {
        OA_DEBUG_USER("Connect Mode:-> Domain Mode\r\n");
    }
    else
    {
        OA_DEBUG_USER("Connect Mode:-> IP Mode\r\n");
    }
    
    gClientStateT.en_cse = OA_TRUE;
    gClientStateT.sid_cse = -1;
    gClientStateT.is_blocksend = OA_FALSE;
    gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
    
    
    //gClientStateT.client_t.con_mode = LSL_CON_MODE_DOMAIN;
    //gClientStateT.client_recv_call_back = NULL;
    //oa_memset(gClientStateT.se_rx_buffer,0,MAX_GPRS_RX_BUFFER_LEN);
    
    gClientStateT.gprs_tx = oa_create_ring_buffer_noinit(gprs_tx_buffer, sizeof(gprs_tx_buffer));
    //gClientStateT.gprs_tx = oa_create_ring_buffer_noinit(gprs_tx_buffer, sizeof(gprs_tx_buffer));
    //gClientStateT.gprs_tx = oa_create_ring_buffer_noinit(gprs_tx_buffer, sizeof(gprs_tx_buffer));
    
    oa_soc_notify_ind_register(lsl_soc_notify_ind);
    client_set_receive_callback(lsl_client_gprs_datas_deal);    // 使用自定义回调，用于与服务器连接
}


/*******************************************************
*   Function name:  gprs_connect
*   Descriptions:   GPRS connect server
*
*   parameters:     const char  *apn    point of an apn struct      // 接入点
*                   const char  *address     point of a ip address  // IP地址 or 域名
*                   oa_uint16   port    the port number to connect  // 端口号
*
*   Returned value: oa_bool             1.  oa_true
*                                       2.  oa_false
********************************************************/
oa_bool gprs_connect(const char *apn,const char *address,oa_uint16 port,lsl_connect_mode_enum mode)
{
    /*************oa_soc_apn_info_struct****************
    //network apn info struct
    typedef struct 
    {
        oa_uint8 apn[OA_MAX_TCPIP_APN_LEN];
        oa_uint8 userName[OA_MAX_TCPIP_USER_NAME_LEN];
        oa_uint8 password[OA_MAX_TCPIP_PASSWORD_LEN];
    }oa_soc_apn_info_struct;
    ***************************************************/
	oa_soc_apn_info_struct apn_info = {"cmnet","",""};  // [6.2.2] The structure describes the gprs network APN information 
	
	/****************oa_sockaddr_struct*****************
	typedef struct 
    {
        oa_socket_type_enum	sock_type;
        oa_int16	addr_len;
        oa_uint8	addr[OA_MAX_SOCK_ADDR_LEN];
        oa_uint16	port;
    } oa_sockaddr_struct;
    ***************************************************/
    oa_sockaddr_struct     soc_addr_info;               // [6.2.1] The structure describes a socket address information
    
    oa_uint8 err;

    OA_DEBUG_USER("%s",__func__);
	
	if(apn != NULL)
	{
		oa_strcpy(apn_info.apn,apn);
	}
	//gp_result = OA_FALSE;
	//oa_gprs_set_apn_req(&apn_info, gprs_set_apn_cb);    // [6.3.1] set APN information for GPRS connection
	//OA_DEBUG_USER("gprs apn set over!\r\n");
	//if(gp_result == OA_FALSE)
	//{
		//return OA_FALSE;
	//}
	
	oa_gprs_apn_init(&apn_info);
	
	OA_DEBUG_USER("gprs apn set OK\r\n");

    gClientStateT.sid_cse = oa_soc_create(OA_SOCK_STREAM, 0);
    if(gClientStateT.sid_cse < 0)
    {
        OA_DEBUG_USER("ERROR SID TO RESTART!!!!");
        oa_module_restart(NULL);
    }
    soc_addr_info.sock_type = OA_SOCK_STREAM;
    soc_addr_info.addr_len = oa_strlen(address);
    oa_strcpy(soc_addr_info.addr, address);
    soc_addr_info.port = port;

    OA_DEBUG_USER("------------- USER_SERVER ---------------");
    OA_DEBUG_USER("gClientStateT.sid_cse = %d",gClientStateT.sid_cse);
    OA_DEBUG_USER("soc_addr_info.addr_len = %d",soc_addr_info.addr_len);
    OA_DEBUG_USER("soc_addr_info.addr = %s",soc_addr_info.addr);
    OA_DEBUG_USER("soc_addr_info.port = %d",soc_addr_info.port);

    if (LSL_CON_MODE_DOMAIN == mode) 
    { 
        OA_DEBUG_USER("Domain name is %s,port is %d",soc_addr_info.addr,soc_addr_info.port);
        err = oa_soc_connect(gClientStateT.sid_cse,OA_DOMAIN_NAME, &soc_addr_info);
        // [6.3.6] This function is used to connect to a server whose address is specified by *addr. 
        // Address Support: IP_ADDRESS and DOMAIN_NAME
    }
    else { // mode == 0  -->  IP_MODE
        OA_DEBUG_USER("ip is %s,port is %d",soc_addr_info.addr,soc_addr_info.port);
        err = oa_soc_connect(gClientStateT.sid_cse,OA_IP_ADDRESS, &soc_addr_info);
    }
	
	OA_DEBUG_USER("gprs connect result is %d\r\n",err);

	if(err != OA_SOC_SUCCESS)
		return OA_FALSE;
	
	return OA_TRUE;
	
}

void lsl_soc_notify_ind(void *inMsg)
{
    oa_int32 ret = 0;    
    /**************oa_app_soc_notify_ind_struct********************
    typedef struct
    {
       oa_uint8    ref_count;
       oa_uint16   msg_len;	
       oa_int8     socket_id;   //  socket ID 
       oa_uint8    event_type;  //  soc_event_enum 
       oa_bool     result;      //
       oa_int8     error_cause; //  used only when EVENT is close/connect refer oa_soc_error_enum
       oa_int32    detail_cause;//  refer to ps_cause_enum if error_cause is
                                    SOC_BEARER_FAIL 
    } oa_app_soc_notify_ind_struct;
    **************************************************************/
    oa_app_soc_notify_ind_struct *soc_notify = (oa_app_soc_notify_ind_struct*)inMsg;

    OA_DEBUG_USER("%s:",__func__);
    
    switch(soc_notify->event_type) // [6.1.3] oa_soc_event_enum
    {
        case OA_SOC_WRITE:
        {
            OA_DEBUG_USER("gprs_soc_notify_ind: OA_SOC_WRITE!\r\n");
            
            /* Notify for send data*/
            if (soc_notify->result == OA_TRUE)
            {
                gClientStateT.is_blocksend = OA_FALSE;
                //resend the data ,else will lost data
                OA_DEBUG_USER("%s:sock_id=%d resend!",__func__,soc_notify->socket_id);
                lsl_soc_send_req();
            }
            else
            {
                OA_DEBUG_USER("%s:sock_id=%d send fail err=%d!",__func__,soc_notify->socket_id,soc_notify->error_cause);
                lsl_soc_close_req();
            }
            
            break;   
        }
        case OA_SOC_READ:
        {
            OA_DEBUG_USER("gprs_soc_notify_ind: OA_SOC_READ!\r\n");
            if (soc_notify->socket_id == gClientStateT.sid_cse)
            {
                OA_DEBUG_USER("User Server -> Read data!");
                
                
                if (soc_notify->result == OA_TRUE)
                {
                    hrt_dog_t = 0;
                    //do
                    //{
                        oa_memset(gprs_rx_buffer, 0 , (OA_MAX_SOC_RCV_BUFLEN*sizeof(oa_uint8))); 
        
                        //received gprs data, read data for protocol
                        ret = oa_soc_recv(soc_notify->socket_id , (oa_uint8*)gprs_rx_buffer, OA_MAX_SOC_RCV_BUFLEN, 0);
                        //OA_DEBUG_USER("%s:sock_id=%d read ok ret=%d!",__func__,soc_notify->socket_id,ret);
                        
                        if(ret > 0)
                        {
                            // read data length=ret, 
                            //oa_soc_gprs_recv((oa_uint8*)gprs_rx_buffer, ret);
                            OA_DEBUG_USER("LEN = %d,DATAS = %s",ret,gprs_rx_buffer);
                            //lsl_uart_485_tx(OA_UART2,gprs_rx_buffer,ret);
                            if(gClientStateT.client_recv_call_back != NULL)
                            {
                                gClientStateT.client_recv_call_back(soc_notify->socket_id,gprs_rx_buffer,ret);
                            }
                        }
                        else
                        {
                            //read data error or block
                        }  
                    //}while(ret>0); //Make sure use  do{...}while  to read out all received data.
                }
                else
                {
                    OA_DEBUG_USER("%s:sock_id=%d read fail err=%d!",__func__,soc_notify->socket_id,soc_notify->error_cause);
                    lsl_soc_close_req();
                }
                break;
            }
        }
        case OA_SOC_ACCEPT:
        {
            OA_DEBUG_USER("gprs_soc_notify_ind: OA_SOC_ACCEPT!\r\n");
            break;
        }
        case OA_SOC_CONNECT:
        {
            if (soc_notify->result == OA_TRUE)
            {
                OA_DEBUG_USER("gprs_soc_notify_ind: OA_SOC_CONNECT! -> Soc ID = %d\r\n",soc_notify->socket_id);
                hrt_dog_t = 0;
                oa_sleep(100); //just connected need delay 100ms then send gprs data, else may lost data
                if (soc_notify->socket_id == gClientStateT.sid_cse)
                {
                    OA_DEBUG_USER("User Server -> SERVER ONLINE!");
                    gClientStateT.st_cse = OA_SOC_STATE_ONLINE;
                    if(gModuleConfigT.heart_t.t >= HEART_TIME_MIN)
                    {
                        //oa_timer_start(LSL_TIMER_HEART_PKT,heart_pkt_timer_ind,NULL,gModuleConfigT.heart_t.t);
                        oa_timer_start(LSL_TIMER_HEART_PKT,heart_pkt_timer_ind,NULL,1);
                    }
                }
                else
                {
                    OA_DEBUG_USER("Unknow Server -> connect to unknow server!############################\r\n");
                    OA_DEBUG_USER("Socket ID = %d",soc_notify->socket_id);
                    gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
                    oa_soc_close(soc_notify->socket_id);
                }
            }
            else
            {
                if (soc_notify->socket_id == gClientStateT.sid_cse)
                {
                    OA_DEBUG_USER("User Server -> SERVER CONNECT FAIL!\r\n");
                    gModuleStateT.try_failed_tick = gModuleStateT.tick;
                    gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
                    oa_soc_close(soc_notify->socket_id);
                }
                else
                {
                    OA_DEBUG_USER("Unknow Server -> CONNECT FAIL!\r\n");
                    gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
                    oa_soc_close(soc_notify->socket_id);
                }
                
                lsl_soc_close_req();
            }
            break;
        }       
        case OA_SOC_CLOSE:
        {
            OA_DEBUG_USER("gprs_soc_notify_ind: OA_SOC_CLOSE!\r\n");
			OA_DEBUG_USER("soc_notify->socket_id = %d",soc_notify->socket_id);
			if (soc_notify->socket_id == gClientStateT.sid_cse)
			{
			    OA_DEBUG_USER("Client SOC[CLOSE] --> ");
                gClientStateT.st_cse = OA_SOC_STATE_OFFLINE;
                oa_timer_stop(LSL_TIMER_HEART_PKT);
                
                if (soc_notify->result == OA_TRUE)
				{
				    
				}
				else
				{
					if(OA_SOC_CONNRESET == soc_notify->error_cause)
					{
						/*If the socket is closed by remote server, use soc_close to close local socketID*/
                        OA_DEBUG_USER("oa_soc_close(%d)",soc_notify->socket_id);
						oa_soc_close(soc_notify->socket_id);
	            		break;
	        		}
				}
            }
            break;
        }
        default:
            break;
    }
    
    //oa_soc_start_heartbeat_timer();
}

/***********************************************
*   Function name:      gprs_set_apn_cb
*   Descriptions:       
*   parameters:         oa_bool     result
*   Returned value:     NONE
************************************************/
void gprs_set_apn_cb(oa_bool result)
{
    OA_DEBUG_USER("%s",__func__);
    OA_DEBUG_USER("gprs_set_apn_cb : look result is %d\r\n",result);
    gp_result = result;
}

/********************************************************* END OF LSL SOC *****/