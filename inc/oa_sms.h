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
 *   oa_sms.h
 *
 * Project:
 * --------
 *   OPEN AT project
 *
 * Description:
 * ------------
 *   This Module declares sms api in OPEN AT.
 *
 * Author:
 * -------
 *   simon.
 *
 ****************************************************************************/
#ifndef __OA_SMS_H__
#define __OA_SMS_H__


extern void lsl_sms_send_7bit(oa_uint8 *addr,oa_uint8 *p,oa_uint16 len);
extern void lsl_sms_send_8bit(oa_uint8 *addr,oa_uint8 *p,oa_uint16 len);
extern void lsl_sms_send_ucs2(oa_uint8 *addr,oa_uint8 *p,oa_uint16 len);
extern oa_bool lsl_check_phone_binding(void);



extern void oa_sms_init(void);
extern oa_bool lsl_check_phone_binding(void);
extern oa_bool oa_sms_rcv_ind_handler(oa_char * deliver_num, oa_char * timestamp, oa_uint8 * data,  oa_uint16 len, oa_smsal_dcs_enum dcs);

#endif /* __OA_SMS_H__ */
