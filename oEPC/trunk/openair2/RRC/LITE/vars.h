/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
    included in this distribution in the file called "COPYING". If not,
    see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@eurecom.fr

  Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE

*******************************************************************************/

/*! \file vars.hles
* \brief rrc variables 
* \author Raymond Knopp and Navid Nikaein
* \date 2013
* \version 1.0 
* \company Eurecom
* \email: navid.nikaein@eurecom.fr
*/ 


#ifndef __OPENAIR_RRC_VARS_H__
#define __OPENAIR_RRC_VARS_H__
#include "defs.h"
#include "LAYER2/RLC/rlc.h"
#include "COMMON/mac_rrc_primitives.h"
#include "LAYER2/MAC/defs.h"

eNB_RRC_INST *eNB_rrc_inst;
UE_RRC_INST *UE_rrc_inst;
//RRC_XFACE *Rrc_xface;
#ifndef USER_MODE
//MAC_RLC_XFACE *Mac_rlc_xface;
#ifndef NO_RRM
int S_rrc= RRC2RRM_FIFO;
#endif //NO_RRM
//int R_rrc= RRM2RRC_FIFO;
#else
#include "LAYER2/MAC/extern.h"
#ifndef NO_RRM
sock_rrm_t S_rrc;
#endif 
#endif

#ifndef NO_RRM
#ifndef USER_MODE
char *Header_buf;
char *Data;
unsigned short Header_read_idx,Data_read_idx,Header_size;
#endif
unsigned short Data_to_read;
#endif //NO_RRM

#define MAX_U32 0xFFFFFFFF

uint8_t DRB2LCHAN[8];

long logicalChannelGroup0 = 0;
long  logicalChannelSR_Mask_r9=0;

struct LogicalChannelConfig__ul_SpecificParameters LCSRB1 =  {1,
							      LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_infinity,
							      0,
							      &logicalChannelGroup0};
struct LogicalChannelConfig__ul_SpecificParameters LCSRB2 =  {3,
							      LogicalChannelConfig__ul_SpecificParameters__prioritisedBitRate_infinity,
							      0,
							      &logicalChannelGroup0};


// These are the default SRB configurations from 36.331 (Chapter 9, p. 176-179 in v8.6)
LogicalChannelConfig_t  SRB1_logicalChannelConfig_defaultValue = {&LCSRB1
#ifdef Rel10
								  ,
								  &logicalChannelSR_Mask_r9
#endif
                                                                 };

LogicalChannelConfig_t SRB2_logicalChannelConfig_defaultValue = {&LCSRB2
#ifdef Rel10
								 ,
								 &logicalChannelSR_Mask_r9
#endif
                                                                };

//CONSTANTS
rlc_info_t Rlc_info_um,Rlc_info_am_config;
uint16_t RACH_FREQ_ALLOC;
//uint8_t NB_RACH;
LCHAN_DESC BCCH_LCHAN_DESC,CCCH_LCHAN_DESC,DCCH_LCHAN_DESC,DTCH_DL_LCHAN_DESC,DTCH_UL_LCHAN_DESC;
MAC_MEAS_T BCCH_MEAS_TRIGGER,CCCH_MEAS_TRIGGER,DCCH_MEAS_TRIGGER,DTCH_MEAS_TRIGGER;
MAC_AVG_T BCCH_MEAS_AVG, CCCH_MEAS_AVG,DCCH_MEAS_AVG, DTCH_MEAS_AVG;

// timers 
uint16_t T300[8] = {100,200,300,400,600,1000,1500,2000};
uint16_t T310[8] = {0,50,100,200,500,1000,2000};
uint16_t N310[8] = {1,2,3,4,6,8,10,20};
uint16_t N311[8] = {1,2,3,4,6,8,10,20};
uint32_t T304[8] = {50,100,150,200,500,1000,2000,MAX_U32};

// TimeToTrigger enum mapping table (36.331 TimeToTrigger IE)
uint32_t timeToTrigger_ms[16] = {0,40,64,80,100,128,160,256,320,480,512,640,1024,1280,2560,5120};

/* 36.133 Section 9.1.4 RSRP Measurement Report Mapping, Table: 9.1.4-1 */
float RSRP_meas_mapping[98] = {
		-140,
		-139,
		-138,
		-137,
		-136,
		-135,
		-134,
		-133,
		-132,
		-131,
		-130,
		-129,
		-128,
		-127,
		-126,
		-125,
		-124,
		-123,
		-122,
		-121,
		-120,
		-119,
		-118,
		-117,
		-116,
		-115,
		-114,
		-113,
		-112,
		-111,
		-110,
		-109,
		-108,
		-107,
		-106,
		-105,
		-104,
		-103,
		-102,
		-101,
		-100,
		-99,
		-98,
		-97,
		-96,
		-95,
		-94,
		-93,
		-92,
		-91,
		-90,
		-89,
		-88,
		-87,
		-86,
		-85,
		-84,
		-83,
		-82,
		-81,
		-80,
		-79,
		-78,
		-77,
		-76,
		-75,
		-74,
		-73,
		-72,
		-71,
		-70,
		-69,
		-68,
		-67,
		-66,
		-65,
		-64,
		-63,
		-62,
		-61,
		-60,
		-59,
		-58,
		-57,
		-56,
		-55,
		-54,
		-53,
		-52,
		-51,
		-50,
		-49,
		-48,
		-47,
		-46,
		-45,
		-44,
		-43
};

float RSRQ_meas_mapping[35] = {
	-19,
	-18.5,
	-18,
	-17.5,
	-17,
	-16.5,
	-16,
	-15.5,
	-15,
	-14.5,
	-14,
	-13.5,
	-13,
	-12.5,
	-12,
	-11.5,
	-11,
	-10.5,
	-10,
	-9.5,
	-9,
	-8.5,
	-8,
	-7.5,
	-7,
	-6.5,
	-6,
	-5.5,
	-5,
	-4.5,
	-4,
	-3.5,
	-3,
	-2.5,
	-2
};

#endif
