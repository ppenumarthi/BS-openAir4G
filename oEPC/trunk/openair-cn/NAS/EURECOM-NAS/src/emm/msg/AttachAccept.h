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

  Address      : Eurecom, Compus SophiaTech 450, route des chappes, 06451 Biot, France.

 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "EpsAttachResult.h"
#include "GprsTimer.h"
#include "TrackingAreaIdentityList.h"
#include "EsmMessageContainer.h"
#include "EpsMobileIdentity.h"
#include "LocationAreaIdentification.h"
#include "MobileIdentity.h"
#include "EmmCause.h"
#include "PlmnList.h"
#include "EmergencyNumberList.h"
#include "EpsNetworkFeatureSupport.h"
#include "AdditionalUpdateResult.h"

#ifndef ATTACH_ACCEPT_H_
#define ATTACH_ACCEPT_H_

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define ATTACH_ACCEPT_MINIMUM_LENGTH ( \
    EPS_ATTACH_RESULT_MINIMUM_LENGTH + \
    GPRS_TIMER_MINIMUM_LENGTH + \
    TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH + \
    ESM_MESSAGE_CONTAINER_MINIMUM_LENGTH )

/* Maximum length macro. Formed by maximum length of each field */
#define ATTACH_ACCEPT_MAXIMUM_LENGTH ( \
    EPS_ATTACH_RESULT_MAXIMUM_LENGTH + \
    GPRS_TIMER_MAXIMUM_LENGTH + \
    TRACKING_AREA_IDENTITY_LIST_MAXIMUM_LENGTH + \
    ESM_MESSAGE_CONTAINER_MAXIMUM_LENGTH + \
    EPS_MOBILE_IDENTITY_MAXIMUM_LENGTH + \
    LOCATION_AREA_IDENTIFICATION_MAXIMUM_LENGTH + \
    MOBILE_IDENTITY_MAXIMUM_LENGTH + \
    EMM_CAUSE_MAXIMUM_LENGTH + \
    GPRS_TIMER_MAXIMUM_LENGTH + \
    GPRS_TIMER_MAXIMUM_LENGTH + \
    PLMN_LIST_MAXIMUM_LENGTH + \
    EMERGENCY_NUMBER_LIST_MAXIMUM_LENGTH + \
    EPS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH + \
    ADDITIONAL_UPDATE_RESULT_MAXIMUM_LENGTH )

/* If an optional value is present and should be encoded, the corresponding
 * Bit mask should be set to 1.
 */
# define ATTACH_ACCEPT_GUTI_PRESENT                         (1<<0)
# define ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT (1<<1)
# define ATTACH_ACCEPT_MS_IDENTITY_PRESENT                  (1<<2)
# define ATTACH_ACCEPT_EMM_CAUSE_PRESENT                    (1<<3)
# define ATTACH_ACCEPT_T3402_VALUE_PRESENT                  (1<<4)
# define ATTACH_ACCEPT_T3423_VALUE_PRESENT                  (1<<5)
# define ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT             (1<<6)
# define ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT        (1<<7)
# define ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT  (1<<8)
# define ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT     (1<<9)

typedef enum attach_accept_iei_tag {
    ATTACH_ACCEPT_GUTI_IEI                          = 0x50, /* 0x50 = 80 */
    ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_IEI  = 0x13, /* 0x13 = 19 */
    ATTACH_ACCEPT_MS_IDENTITY_IEI                   = 0x23, /* 0x23 = 35 */
    ATTACH_ACCEPT_EMM_CAUSE_IEI                     = 0x53, /* 0x53 = 83 */
    ATTACH_ACCEPT_T3402_VALUE_IEI                   = 0x17, /* 0x17 = 23 */
    ATTACH_ACCEPT_T3423_VALUE_IEI                   = 0x59, /* 0x59 = 89 */
    ATTACH_ACCEPT_EQUIVALENT_PLMNS_IEI              = 0x4A, /* 0x4A = 74 */
    ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_IEI         = 0x34, /* 0x34 = 52 */
    ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_IEI   = 0x64, /* 0x64 = 100 */
    ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_IEI      = 0xF0, /* 0xF0 = 240 */
} attach_accept_iei;

/*
 * Message name: Attach accept
 * Description: This message is sent by the network to the UE to indicate that the corresponding attach request has been accepted. See table 8.2.1.1.
 * Significance: dual
 * Direction: network to UE
 */

typedef struct attach_accept_msg_tag {
    /* Mandatory fields */
    ProtocolDiscriminator       protocoldiscriminator:4;
    SecurityHeaderType          securityheadertype:4;
    MessageType                 messagetype;
    EpsAttachResult             epsattachresult;
    GprsTimer                   t3412value;
    TrackingAreaIdentityList    tailist;
    EsmMessageContainer         esmmessagecontainer;
    /* Optional fields */
    uint32_t                    presencemask;
    EpsMobileIdentity           guti;
    LocationAreaIdentification  locationareaidentification;
    MobileIdentity              msidentity;
    EmmCause                    emmcause;
    GprsTimer                   t3402value;
    GprsTimer                   t3423value;
    PlmnList                    equivalentplmns;
    EmergencyNumberList         emergencynumberlist;
    EpsNetworkFeatureSupport    epsnetworkfeaturesupport;
    AdditionalUpdateResult      additionalupdateresult;
} attach_accept_msg;

int decode_attach_accept(attach_accept_msg *attachaccept, uint8_t *buffer, uint32_t len);

int encode_attach_accept(attach_accept_msg *attachaccept, uint8_t *buffer, uint32_t len);

#endif /* ! defined(ATTACH_ACCEPT_H_) */

