/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
 *
 * Filename:
 * ---------
 * audcoeff.c
 *
 * Project:
 * --------
 *   MAUI Project
 *
 * Description:
 * ------------
 *   Default FIR Coefficients Table
 *
 * Author:
 * -------
 * -------
 *
 *==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/

/* for customer release */   
/******************** INPUT FIR ********************************/
#include "oa_type.h"


#define SPEECH_INPUT_FIR_COEFF_NORMAL_DEFAULT \
   { /* 0: Input FIR coefficients for 2G/3G Normal mode */ \
      32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_INPUT_FIR_COEFF_HEADSET_DEFAULT \
   { /* 1: Input FIR coefficients for 2G/3G/VoIP Headset mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_INPUT_FIR_COEFF_HANDFREE_DEFAULT \
   { /* 2: Input FIR coefficients for 2G/3G Handfree mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_INPUT_FIR_COEFF_BT_DEFAULT \
   { /* 3: Input FIR coefficients for 2G/3G/VoIP BT mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_INPUT_FIR_COEFF_VOIP_NORMAL_DEFAULT \
   { /* 4: Input FIR coefficients for VoIP Normal mode */ \
      506,  -287,   250,  -214,   566, \
      221,    76,  1036,  -329,  1006, \
      574,  -224,  -589,  2436, -1079, \
     1366, -3116,  2142, -3633,  3436, \
   -11302, 20674, 20674,-11302,  3436, \
    -3633,  2142, -3116,  1366, -1079, \
     2436,  -589,  -224,   574,  1006, \
     -329,  1036,    76,   221,   566, \
     -214,   250,  -287,   506,     0 \
   }

#define SPEECH_INPUT_FIR_COEFF_VOIP_HANDFREE_DEFAULT \
   { /* 5: Input FIR coefficients for VoIP Handfree mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_INPUT_FIR_COEFF_DEFAULT \
{ \
    SPEECH_INPUT_FIR_COEFF_NORMAL_DEFAULT, \
    SPEECH_INPUT_FIR_COEFF_HEADSET_DEFAULT, \
    SPEECH_INPUT_FIR_COEFF_HANDFREE_DEFAULT, \
    SPEECH_INPUT_FIR_COEFF_BT_DEFAULT, \
    SPEECH_INPUT_FIR_COEFF_VOIP_NORMAL_DEFAULT, \
    SPEECH_INPUT_FIR_COEFF_VOIP_HANDFREE_DEFAULT \
}

/******************** OUTPUT FIR ********************************/

#define SPEECH_OUTPUT_FIR_COEFF_NORMAL_DEFAULT \
   { /* 0: Output FIR coefficients for 2G/3G Normal mode */ \
      32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_OUTPUT_FIR_COEFF_HEADSET_DEFAULT \
   { /* 1: Output FIR coefficients for 2G/3G/VoIP Headset mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_OUTPUT_FIR_COEFF_HANDFREE_DEFAULT \
   { /* 2: Output FIR coefficients for 2G/3G Handfree mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_OUTPUT_FIR_COEFF_BT_DEFAULT \
   { /* 3: Output FIR coefficients for 2G/3G/VoIP BT mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_OUTPUT_FIR_COEFF_VOIP_NORMAL_DEFAULT \
   { /* 4: Output FIR coefficients for VoIP Normal mode */ \
      173,  -287,   196,  -405,  -496, \
     -204,  -721,    85,  -772,   -72, \
     -112, -1557,  -215,   413,   191, \
     1049,   820,   766,  -805,  9594, \
   -16362, 20674, 20674,-16362,  9594, \
     -805,   766,   820,  1049,   191, \
      413,  -215, -1557,  -112,   -72, \
     -772,    85,  -721,  -204,  -496, \
     -405,   196,  -287,   173,     0 \
   }

#define SPEECH_OUTPUT_FIR_COEFF_VOIP_HANDFREE_DEFAULT \
   { /* 5: Output FIR coefficients for VoIP Handfree mode */ \
    32767,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0, \
        0,     0,     0,     0,     0  \
   }

#define SPEECH_OUTPUT_FIR_COEFF_DEFAULT \
{ \
    SPEECH_OUTPUT_FIR_COEFF_NORMAL_DEFAULT, \
    SPEECH_OUTPUT_FIR_COEFF_HEADSET_DEFAULT, \
    SPEECH_OUTPUT_FIR_COEFF_HANDFREE_DEFAULT, \
    SPEECH_OUTPUT_FIR_COEFF_BT_DEFAULT, \
    SPEECH_OUTPUT_FIR_COEFF_VOIP_NORMAL_DEFAULT, \
    SPEECH_OUTPUT_FIR_COEFF_VOIP_HANDFREE_DEFAULT \
}

const signed short Speech_Input_FIR_Coeff[6][45] = SPEECH_INPUT_FIR_COEFF_DEFAULT;
const signed short Speech_Output_FIR_Coeff[6][45] = SPEECH_OUTPUT_FIR_COEFF_DEFAULT;



#define MICROPHONE_VOLUME     176
#define SIDE_TONE_VOLUME      0

#define GAIN_NOR_MED_VOL_MAX 144
#define GAIN_HED_MED_VOL_MAX 112  /*Headset*/
#define GAIN_HND_MED_VOL_MAX 144  /* Handfree*/ 
#define GAIN_TVO_VOL_MAX (176-24)

#define GAIN_NOR_MED_VOL_STEP 4 /* 1=0.5dB as unit */
#define GAIN_HED_MED_VOL_STEP 4 /* 1=0.5dB as unit */
#define GAIN_HND_MED_VOL_STEP 4 /* 1=0.5dB as unit */
#define GAIN_TVO_VOL_STEP 4 /* 1=0.5dB as unit */

#define GAIN_TVO_VOL0			4   
#define GAIN_TVO_VOL1			61
#define GAIN_TVO_VOL2			94
#define GAIN_TVO_VOL3			127
#define GAIN_TVO_VOL4			164
#define GAIN_TVO_VOL5			180
#define GAIN_TVO_VOL6			196

#define GAIN_NOR_CTN_VOL    3
#define GAIN_NOR_CTN_VOL0		127
#define GAIN_NOR_CTN_VOL1		127
#define GAIN_NOR_CTN_VOL2		127
#define GAIN_NOR_CTN_VOL3		127
#define GAIN_NOR_CTN_VOL4		127
#define GAIN_NOR_CTN_VOL5		127
#define GAIN_NOR_CTN_VOL6		127

#define GAIN_NOR_KEY_VOL    3
#define GAIN_NOR_KEY_VOL0			16
#define GAIN_NOR_KEY_VOL1			32
#define GAIN_NOR_KEY_VOL2			48
#define GAIN_NOR_KEY_VOL3			64
#define GAIN_NOR_KEY_VOL4			80
#define GAIN_NOR_KEY_VOL5			96
#define GAIN_NOR_KEY_VOL6			112

#define GAIN_NOR_MIC_VOL    3
#define GAIN_NOR_MIC_VOL0	240
#define GAIN_NOR_MIC_VOL1	240
#define GAIN_NOR_MIC_VOL2	240
#define GAIN_NOR_MIC_VOL3	168
#define GAIN_NOR_MIC_VOL4	240
#define GAIN_NOR_MIC_VOL5	240
#define GAIN_NOR_MIC_VOL6	240

#define GAIN_NOR_GMI_VOL    3
#define GAIN_NOR_GMI_VOL0		48
#define GAIN_NOR_GMI_VOL1		64
#define GAIN_NOR_GMI_VOL2		80
#define GAIN_NOR_GMI_VOL3		96
#define GAIN_NOR_GMI_VOL4		112
#define GAIN_NOR_GMI_VOL5		128
#define GAIN_NOR_GMI_VOL6		144

#define GAIN_NOR_SPH_VOL    3
#define GAIN_NOR_SPH_VOL0		80
#define GAIN_NOR_SPH_VOL1		96
#define GAIN_NOR_SPH_VOL2	  112	
#define GAIN_NOR_SPH_VOL3		128
#define GAIN_NOR_SPH_VOL4		144
#define GAIN_NOR_SPH_VOL5		160
#define GAIN_NOR_SPH_VOL6		176

#define GAIN_NOR_SID_VOL    3
#define GAIN_NOR_SID_VOL0			SIDE_TONE_VOLUME
#define GAIN_NOR_SID_VOL1			SIDE_TONE_VOLUME
#define GAIN_NOR_SID_VOL2			SIDE_TONE_VOLUME
#define GAIN_NOR_SID_VOL3			SIDE_TONE_VOLUME
#define GAIN_NOR_SID_VOL4			SIDE_TONE_VOLUME
#define GAIN_NOR_SID_VOL5			SIDE_TONE_VOLUME
#define GAIN_NOR_SID_VOL6			SIDE_TONE_VOLUME

#define GAIN_NOR_MED_VOL    3
#define GAIN_NOR_MED_VOL0			48
#define GAIN_NOR_MED_VOL1			64
#define GAIN_NOR_MED_VOL2			80
#define GAIN_NOR_MED_VOL3			96
#define GAIN_NOR_MED_VOL4			112
#define GAIN_NOR_MED_VOL5			128
#define GAIN_NOR_MED_VOL6			144

/* define Gain For Headset */
/* speaker, microphone, loudspeaker, buzzer */
#define GAIN_HED_CTN_VOL    3
#define GAIN_HED_CTN_VOL0		4
#define GAIN_HED_CTN_VOL1		8
#define GAIN_HED_CTN_VOL2		16
#define GAIN_HED_CTN_VOL3		32
#define GAIN_HED_CTN_VOL4		64
#define GAIN_HED_CTN_VOL5		96
#define GAIN_HED_CTN_VOL6		127

#define GAIN_HED_KEY_VOL    3
#define GAIN_HED_KEY_VOL0		4
#define GAIN_HED_KEY_VOL1		8
#define GAIN_HED_KEY_VOL2		12
#define GAIN_HED_KEY_VOL3		16
#define GAIN_HED_KEY_VOL4		20
#define GAIN_HED_KEY_VOL5		24
#define GAIN_HED_KEY_VOL6		28

#define GAIN_HED_MIC_VOL		3
#define GAIN_HED_MIC_VOL0		180
#define GAIN_HED_MIC_VOL1		180
#define GAIN_HED_MIC_VOL2		180
#define GAIN_HED_MIC_VOL3		160
#define GAIN_HED_MIC_VOL4		180
#define GAIN_HED_MIC_VOL5		196
#define GAIN_HED_MIC_VOL6		212

#define GAIN_HED_GMI_VOL		3
#define GAIN_HED_GMI_VOL0		16
#define GAIN_HED_GMI_VOL1		32
#define GAIN_HED_GMI_VOL2		48
#define GAIN_HED_GMI_VOL3		64
#define GAIN_HED_GMI_VOL4		80
#define GAIN_HED_GMI_VOL5		96
#define GAIN_HED_GMI_VOL6		112

#define GAIN_HED_SPH_VOL    3
#define GAIN_HED_SPH_VOL0		16
#define GAIN_HED_SPH_VOL1		32
#define GAIN_HED_SPH_VOL2		48
#define GAIN_HED_SPH_VOL3		64
#define GAIN_HED_SPH_VOL4		80
#define GAIN_HED_SPH_VOL5		96
#define GAIN_HED_SPH_VOL6		112

#define GAIN_HED_SID_VOL    3
#define GAIN_HED_SID_VOL0		SIDE_TONE_VOLUME
#define GAIN_HED_SID_VOL1		SIDE_TONE_VOLUME
#define GAIN_HED_SID_VOL2		SIDE_TONE_VOLUME
#define GAIN_HED_SID_VOL3		SIDE_TONE_VOLUME
#define GAIN_HED_SID_VOL4		SIDE_TONE_VOLUME
#define GAIN_HED_SID_VOL5		SIDE_TONE_VOLUME
#define GAIN_HED_SID_VOL6		SIDE_TONE_VOLUME

#define GAIN_HED_MED_VOL    3
#define GAIN_HED_MED_VOL0			16
#define GAIN_HED_MED_VOL1			32
#define GAIN_HED_MED_VOL2			48
#define GAIN_HED_MED_VOL3			64
#define GAIN_HED_MED_VOL4			80
#define GAIN_HED_MED_VOL5			96
#define GAIN_HED_MED_VOL6			112


/* define Gain For Handfree */
/* speaker, microphone, loudspeaker, buzzer */
#define GAIN_HND_CTN_VOL    3
#define GAIN_HND_CTN_VOL0			127
#define GAIN_HND_CTN_VOL1			127
#define GAIN_HND_CTN_VOL2			127
#define GAIN_HND_CTN_VOL3			127
#define GAIN_HND_CTN_VOL4			127
#define GAIN_HND_CTN_VOL5			127
#define GAIN_HND_CTN_VOL6			127

#define GAIN_HND_KEY_VOL    3
#define GAIN_HND_KEY_VOL0		16
#define GAIN_HND_KEY_VOL1		28
#define GAIN_HND_KEY_VOL2		40
#define GAIN_HND_KEY_VOL3		52
#define GAIN_HND_KEY_VOL4		64
#define GAIN_HND_KEY_VOL5		76
#define GAIN_HND_KEY_VOL6		88

#define GAIN_HND_MIC_VOL		3
#define GAIN_HND_MIC_VOL0		180
#define GAIN_HND_MIC_VOL1		180
#define GAIN_HND_MIC_VOL2		180
#define GAIN_HND_MIC_VOL3		180
#define GAIN_HND_MIC_VOL4		180
#define GAIN_HND_MIC_VOL5		196
#define GAIN_HND_MIC_VOL6		212

#define GAIN_HND_GMI_VOL		3
#define GAIN_HND_GMI_VOL0			48
#define GAIN_HND_GMI_VOL1			64
#define GAIN_HND_GMI_VOL2			80
#define GAIN_HND_GMI_VOL3			96
#define GAIN_HND_GMI_VOL4			112
#define GAIN_HND_GMI_VOL5			128
#define GAIN_HND_GMI_VOL6			144

#define GAIN_HND_SPH_VOL    3
#define GAIN_HND_SPH_VOL0			16
#define GAIN_HND_SPH_VOL1			32
#define GAIN_HND_SPH_VOL2			48
#define GAIN_HND_SPH_VOL3			64
#define GAIN_HND_SPH_VOL4			80
#define GAIN_HND_SPH_VOL5			96
#define GAIN_HND_SPH_VOL6			112

#define GAIN_HND_SID_VOL    3
#define GAIN_HND_SID_VOL0		SIDE_TONE_VOLUME
#define GAIN_HND_SID_VOL1		SIDE_TONE_VOLUME
#define GAIN_HND_SID_VOL2		SIDE_TONE_VOLUME
#define GAIN_HND_SID_VOL3		SIDE_TONE_VOLUME
#define GAIN_HND_SID_VOL4		SIDE_TONE_VOLUME
#define GAIN_HND_SID_VOL5		SIDE_TONE_VOLUME
#define GAIN_HND_SID_VOL6		SIDE_TONE_VOLUME

#define GAIN_HND_MED_VOL    3
#define GAIN_HND_MED_VOL0			48
#define GAIN_HND_MED_VOL1			64
#define GAIN_HND_MED_VOL2			80
#define GAIN_HND_MED_VOL3			96
#define GAIN_HND_MED_VOL4			112
#define GAIN_HND_MED_VOL5			128
#define GAIN_HND_MED_VOL6			144

oa_uint8 const NVRAM_EF_CUST_ACOUSTIC_DATA_DEFAULT[] = {
     /* Normal: Call Tone, Keypad Tone, Microphone, GMI Tone, Speech Tone, Side Tone */ 
      GAIN_NOR_CTN_VOL0,GAIN_NOR_CTN_VOL1, GAIN_NOR_CTN_VOL2, GAIN_NOR_CTN_VOL3, GAIN_NOR_CTN_VOL4, GAIN_NOR_CTN_VOL5, GAIN_NOR_CTN_VOL6,
      GAIN_NOR_KEY_VOL0, GAIN_NOR_KEY_VOL1, GAIN_NOR_KEY_VOL2, GAIN_NOR_KEY_VOL3, GAIN_NOR_KEY_VOL4, GAIN_NOR_KEY_VOL5, GAIN_NOR_KEY_VOL6, 
      GAIN_NOR_MIC_VOL0, GAIN_NOR_MIC_VOL1, GAIN_NOR_MIC_VOL2, GAIN_NOR_MIC_VOL3, GAIN_NOR_MIC_VOL4, GAIN_NOR_MIC_VOL5, GAIN_NOR_MIC_VOL6, 
      GAIN_NOR_GMI_VOL0, GAIN_NOR_GMI_VOL1, GAIN_NOR_GMI_VOL2, GAIN_NOR_GMI_VOL3, GAIN_NOR_GMI_VOL4, GAIN_NOR_GMI_VOL5, GAIN_NOR_GMI_VOL6, 
      GAIN_NOR_SPH_VOL0, GAIN_NOR_SPH_VOL1, GAIN_NOR_SPH_VOL2, GAIN_NOR_SPH_VOL3, GAIN_NOR_SPH_VOL4, GAIN_NOR_SPH_VOL5, GAIN_NOR_SPH_VOL6, 
      GAIN_NOR_SID_VOL0, GAIN_NOR_SID_VOL1, GAIN_NOR_SID_VOL2, GAIN_NOR_SID_VOL3, GAIN_NOR_SID_VOL4, GAIN_NOR_SID_VOL5, GAIN_NOR_SID_VOL6, 
      GAIN_NOR_MED_VOL0,GAIN_NOR_MED_VOL1, GAIN_NOR_MED_VOL2, GAIN_NOR_MED_VOL3, GAIN_NOR_MED_VOL4, GAIN_NOR_MED_VOL5, GAIN_NOR_MED_VOL6
   , 
     /* Handset: Call Tone, Keypad Tone, Microphone, GMI Tone, Speech Tone */ 
      GAIN_HED_CTN_VOL0, GAIN_HED_CTN_VOL1, GAIN_HED_CTN_VOL2, GAIN_HED_CTN_VOL3, GAIN_HED_CTN_VOL4, GAIN_HED_CTN_VOL5, GAIN_HED_CTN_VOL6, 
      GAIN_HED_KEY_VOL0, GAIN_HED_KEY_VOL1, GAIN_HED_KEY_VOL2, GAIN_HED_KEY_VOL3, GAIN_HED_KEY_VOL4, GAIN_HED_KEY_VOL5, GAIN_HED_KEY_VOL6, 
      GAIN_HED_MIC_VOL0, GAIN_HED_MIC_VOL1, GAIN_HED_MIC_VOL2, GAIN_HED_MIC_VOL3, GAIN_HED_MIC_VOL4, GAIN_HED_MIC_VOL5, GAIN_HED_MIC_VOL6, 
      GAIN_HED_GMI_VOL0, GAIN_HED_GMI_VOL1, GAIN_HED_GMI_VOL2, GAIN_HED_GMI_VOL3, GAIN_HED_GMI_VOL4, GAIN_HED_GMI_VOL5, GAIN_HED_GMI_VOL6, 
      GAIN_HED_SPH_VOL0, GAIN_HED_SPH_VOL1, GAIN_HED_SPH_VOL2, GAIN_HED_SPH_VOL3, GAIN_HED_SPH_VOL4, GAIN_HED_SPH_VOL5, GAIN_HED_SPH_VOL6, 
      GAIN_HED_SID_VOL0, GAIN_HED_SID_VOL1, GAIN_HED_SID_VOL2, GAIN_HED_SID_VOL3, GAIN_HED_SID_VOL4, GAIN_HED_SID_VOL5, GAIN_HED_SID_VOL6, 
      GAIN_HED_MED_VOL0,GAIN_HED_MED_VOL1, GAIN_HED_MED_VOL2, GAIN_HED_MED_VOL3, GAIN_HED_MED_VOL4, GAIN_HED_MED_VOL5, GAIN_HED_MED_VOL6
   , 
     /* Handfree: Call Tone, Keypad Tone, Microphone, GMI Tone, Speech Tone */  
     GAIN_HND_CTN_VOL0, GAIN_HND_CTN_VOL1, GAIN_HND_CTN_VOL2, GAIN_HND_CTN_VOL3, GAIN_HND_CTN_VOL4, GAIN_HND_CTN_VOL5, GAIN_HND_CTN_VOL6, 
     GAIN_HND_KEY_VOL0,  GAIN_HND_KEY_VOL1, GAIN_HND_KEY_VOL2, GAIN_HND_KEY_VOL3, GAIN_HND_KEY_VOL4, GAIN_HND_KEY_VOL5, GAIN_HND_KEY_VOL6, 
     GAIN_HND_MIC_VOL0,  GAIN_HND_MIC_VOL1, GAIN_HND_MIC_VOL2, GAIN_HND_MIC_VOL3, GAIN_HND_MIC_VOL4, GAIN_HND_MIC_VOL5, GAIN_HND_MIC_VOL6, 
     GAIN_HND_GMI_VOL0,  GAIN_HND_GMI_VOL1, GAIN_HND_GMI_VOL2, GAIN_HND_GMI_VOL3, GAIN_HND_GMI_VOL4, GAIN_HND_GMI_VOL5, GAIN_HND_GMI_VOL6, 
     GAIN_HND_SPH_VOL0,  GAIN_HND_SPH_VOL1, GAIN_HND_SPH_VOL2, GAIN_HND_SPH_VOL3, GAIN_HND_SPH_VOL4, GAIN_HND_SPH_VOL5, GAIN_HND_SPH_VOL6, 
     GAIN_HND_SID_VOL0,  GAIN_HND_SID_VOL1, GAIN_HND_SID_VOL2, GAIN_HND_SID_VOL3, GAIN_HND_SID_VOL4, GAIN_HND_SID_VOL5, GAIN_HND_SID_VOL6, 
     GAIN_HND_MED_VOL0, GAIN_HND_MED_VOL1, GAIN_HND_MED_VOL2, GAIN_HND_MED_VOL3, GAIN_HND_MED_VOL4, GAIN_HND_MED_VOL5, GAIN_HND_MED_VOL6
   ,  
   /* Normal volume: CTN, SPK, MIC, BUZ, SPH, SID, MED */
   GAIN_NOR_CTN_VOL, GAIN_NOR_KEY_VOL, GAIN_NOR_MIC_VOL, GAIN_NOR_GMI_VOL, GAIN_NOR_SPH_VOL, GAIN_NOR_SID_VOL, GAIN_NOR_MED_VOL,
   /* Headset volume: CTN, SPK, MIC, BUZ, SPH, SID, MED */
   GAIN_HED_CTN_VOL, GAIN_HED_KEY_VOL, GAIN_HED_MIC_VOL, GAIN_HED_GMI_VOL, GAIN_HED_SPH_VOL, GAIN_HED_SID_VOL, GAIN_HED_MED_VOL,
   /* Handfree volume: CTN, SPK, MIC, BUZ, SPH, SID, MED */
   GAIN_HND_CTN_VOL, GAIN_HND_KEY_VOL, GAIN_HND_MIC_VOL, GAIN_HND_GMI_VOL, GAIN_HND_SPH_VOL, GAIN_HND_SID_VOL, GAIN_HND_MED_VOL
   
   /* Normal, Headset, Handfree mode melody volume gains */
   ,GAIN_NOR_MED_VOL_MAX, GAIN_HED_MED_VOL_MAX, GAIN_HND_MED_VOL_MAX,GAIN_TVO_VOL_MAX,
   /* Normal, Headset, Handfree mode melody volume gain steps */
   GAIN_NOR_MED_VOL_STEP, GAIN_HED_MED_VOL_STEP, GAIN_HND_MED_VOL_STEP,GAIN_TVO_VOL_STEP,
   /* TV out volume gains*/
   GAIN_TVO_VOL0, GAIN_TVO_VOL1, GAIN_TVO_VOL2, GAIN_TVO_VOL3, GAIN_TVO_VOL4, GAIN_TVO_VOL5, GAIN_TVO_VOL6
};



