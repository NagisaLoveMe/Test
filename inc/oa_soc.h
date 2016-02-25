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
 *   oa_gprs.h
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This Module gives a sample app on open at framework.
 *
 * Author:
 * -------
 *   None!
 *
 ****************************************************************************/
#ifndef _OA_SOC_H_
#define _OA_SOC_H_

#include "oa_type.H"

#define OA_GPRS_BUFFER_SIZE 20480
#define OA_APP_GPRS_SENDING_SIZE 1024
#define OA_TIMER_FOR_HEARTBEAT OA_TIMER_ID_12


extern oa_uint32 hrt_dog_t;

typedef enum 
{
    OA_SOC_STATE_OFFLINE,
    OA_SOC_STATE_CONNECT,
    OA_SOC_STATE_ONLINE,
    OA_SOC_STATE_HALT
}oa_soc_state;

typedef struct
{
    oa_soc_state state;                             //socket connect current state
    oa_buffer *gprs_rx;
    oa_buffer *gprs_tx;
    oa_uint8 gprs_tx_pending_data[OA_APP_GPRS_SENDING_SIZE];
    oa_uint16 gprs_tx_pending_size;
    oa_uint8 gprs_rx_pending_data[OA_APP_GPRS_SENDING_SIZE];
    oa_uint16 gprs_rx_pending_size;
    
    oa_int8 socket_id;                  //socket connect id
    oa_uint32 recon_counter;
    oa_bool can_connect;
    oa_bool is_blocksend;               //socket is blocksend or not

    oa_soc_apn_info_struct apn_info;    //gprs APN information
    oa_sockaddr_struct soc_addr;        //socket connect server address
    oa_socket_addr_enum addr_type;      //socket connect server address type:IP Addror Domain Name

}oa_soc_context;
extern oa_soc_context g_soc_context;

extern void oa_soc_init(void);
extern void oa_soc_state_check(void);
extern void oa_soc_connect_req(void);
extern void oa_soc_close_req(void);
extern void oa_soc_send_req(void);
extern void oa_soc_gprs_recv(oa_uint8* data, oa_uint16 len);
extern void oa_soc_notify_ind(void *inMsg);
extern void oa_soc_start_heartbeat_timer(void);
extern void oa_soc_send_heartbeat_data(void* param);

oa_buffer *oa_create_ring_buffer_noinit(oa_uint8 *rawBuf, oa_uint32 maxLength);
void oa_free_buffer_noinit(oa_buffer *buf);
oa_uint32 oa_write_buffer_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len);
oa_uint32 oa_query_buffer_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len);
oa_uint32 oa_read_buffer_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len);
oa_uint32 oa_write_buffer_force_noinit(oa_buffer *buf, oa_uint8 *data, oa_uint32 len);






#define DOMAIN_NAME_MAX_LEN     128         /**/
#define IP_ADDRESS_MAX_LEN      16          /**/

typedef struct _s_client_param
{
    oa_uint8    domain_name[DOMAIN_NAME_MAX_LEN];
    oa_uint8    ip_address[IP_ADDRESS_MAX_LEN];
    
    oa_uint16   port;

    lsl_connect_mode_enum    con_mode;
    oa_uint8       RESERVED1;
} CLIENT_PARAM_T;

typedef struct _client_state_s {

    CLIENT_PARAM_T      client_t;
    
    oa_uint8            se_rx_pending[MAX_GPRS_RX_PENDING_LEN];
    oa_uint8            se_tx_pending[MAX_GPRS_TX_PENDING_LEN];
    
    oa_uint32           se_rx_pending_size;
    oa_uint32           se_tx_pending_size;
    
    oa_uint16           se_rx_length;
    oa_buffer           *gprs_rx;

    oa_uint16           se_tx_length;
    oa_buffer           *gprs_tx;
    
    oa_bool             is_blocksend;
    oa_uint8            RESERVED1;
    oa_uint8            RESERVED2;
    oa_uint8            RESERVED3;
    
    oa_uint16           se_rx_sta;
    oa_uint16           se_rx_end;
    
    oa_uint16           se_tx_sta;
    oa_uint16           se_tx_end;
    
    oa_int8             sid_cse;
    oa_bool             en_cse;
    oa_soc_state        st_cse;
    oa_uint8            connect_cnt;


    client_receive_cb_fun   client_recv_call_back;
    
} CLIENT_STATE_T;
extern CLIENT_STATE_T gClientStateT;


/*********************¡ý LSL APP ¡ý********************/

/*--- oa_soc.c ---*/
extern void lsl_soc_send_req(void);
extern void lsl_soc_close_req(void);
extern void heart_pkt_timer_ind(void);

extern void client_set_receive_callback(client_receive_cb_fun clientcf);
extern void lsl_client_gprs_datas_deal(oa_uint8 sock_id,oa_uint8 *datas,oa_uint16 len);
extern void lsl_soc_send_heartbeat_data(void);
extern void lsl_soc_send_data(void);

extern void lsl_debug_soc_send(oa_uint8 *pBuf,oa_uint32 len);
extern void gprs_init(void);
extern oa_bool gprs_connect(const char *apn,const char *address,oa_uint16 port,lsl_connect_mode_enum mode);
extern void lsl_soc_notify_ind(void *inMsg);
extern void gprs_set_apn_cb(oa_bool result);

/********************************¡ü END OF LSL APP¡ü***/



#endif/*_OA_SOC_H_*/

