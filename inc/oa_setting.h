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
 *   oa_setting.h
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This Module declares OPEN AT's setting function.
 *
 * Author:
 * -------
 *   simon.
 *
 ****************************************************************************/
#ifndef __OA_SETTING_H__
#define __OA_SETTING_H__

#include "oa_soc.h"

#define OA_DTU_UNIID_MAX_LEN 32
#define OA_DTU_UNIID_MIN_LEN 1

extern void oa_set_param_init(void);
extern oa_bool oa_set_param_save(void);
extern void oa_set_param_check(void);
extern oa_bool oa_set_param_cmd_parse(char * pCmdStr);

/*get function*/
extern oa_uint8 *oa_param_get_dtu_id(void);
extern oa_sockaddr_struct  *oa_param_get_soc_addr(void);
extern oa_soc_apn_info_struct *oa_param_get_apninfo(void);




/*set functions*/
extern oa_bool oa_param_set_dtu_id(oa_uint8 * id);
extern oa_bool oa_param_set_soc_addr(oa_sockaddr_struct * pData);
extern oa_bool oa_param_set_apninfo(oa_soc_apn_info_struct *pData);

/*Misc*/


/*************************** LSL SETTING **************************/

extern oa_bool lsl_param_set_psw(oa_uint8 *psw,oa_uint8 len);
extern oa_bool lsl_param_set_domain(oa_uint8 *domain,oa_uint16 len,oa_uint8 *port);
extern oa_bool lsl_param_set_ipaddr(oa_uint8 *ipaddr,oa_uint16 len,oa_uint8 *port);
extern oa_bool lsl_param_set_heart_pkt_info(oa_uint8* chr,oa_uint16 len,oa_uint32 t);
extern oa_bool lsl_param_set_virtual_id(oa_uint8 *id,oa_uint16 len);
extern oa_bool lsl_param_set_phone_table(oa_uint8 *num,oa_uint8 index);
extern oa_bool lsl_param_clr_phone_num(oa_uint8 index);
extern oa_bool lsl_param_set_uart2(oa_uart_struct* p);
extern oa_bool lsl_param_set_auto_upload_time(oa_uint16 time);

extern CLIENT_PARAM_T *lsl_param_get_client_info(void);
extern PASSWORD_T *lsl_param_get_password(void);
extern HEART_PKT_T *lsl_param_get_heart_pkt_info(void);
extern oa_uint8 *lsl_param_get_virtual_id(void);
extern oa_uint8 *lsl_param_get_phone_table(void);
extern oa_uart_struct* lsl_param_get_uart2(void);
extern oa_uint16 lsl_param_get_auto_upload_time(void);
/***************************************** END OF LSL SETTING *****/


#endif /* __OA_SETTING_H__ */
