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
/*****************************************************************************

Source      emm_send.h

Version     0.1

Date        2013/01/30

Product     NAS stack

Subsystem   EPS Mobility Management

Author      Frederic Maurel

Description Defines functions executed at the EMMAS Service Access
        Point to send EPS Mobility Management messages to the
        Access Stratum sublayer.

*****************************************************************************/
#ifndef __EMM_SEND_H__
#define __EMM_SEND_H__

#include "EmmStatus.h"

#include "DetachRequest.h"
#include "DetachAccept.h"

#ifdef NAS_UE
#include "AttachRequest.h"
#include "AttachComplete.h"
#include "TrackingAreaUpdateRequest.h"
#include "TrackingAreaUpdateComplete.h"
#include "ServiceRequest.h"
#include "ExtendedServiceRequest.h"
#include "GutiReallocationComplete.h"
#include "AuthenticationResponse.h"
#include "AuthenticationFailure.h"
#include "IdentityResponse.h"
#include "NASSecurityModeComplete.h"
#include "SecurityModeReject.h"
#include "UplinkNasTransport.h"
#endif

#ifdef NAS_MME
#include "AttachAccept.h"
#include "AttachReject.h"
#include "TrackingAreaUpdateAccept.h"
#include "TrackingAreaUpdateReject.h"
#include "ServiceReject.h"
#include "GutiReallocationCommand.h"
#include "AuthenticationRequest.h"
#include "AuthenticationReject.h"
#include "IdentityRequest.h"
#include "NASSecurityModeCommand.h"
#include "EmmInformation.h"
#include "DownlinkNasTransport.h"
#include "CsServiceNotification.h"
#endif

#include "emm_asDef.h"

/****************************************************************************/
/*********************  G L O B A L    C O N S T A N T S  *******************/
/****************************************************************************/

/****************************************************************************/
/************************  G L O B A L    T Y P E S  ************************/
/****************************************************************************/

/****************************************************************************/
/********************  G L O B A L    V A R I A B L E S  ********************/
/****************************************************************************/

/****************************************************************************/
/******************  E X P O R T E D    F U N C T I O N S  ******************/
/****************************************************************************/

/*
 * --------------------------------------------------------------------------
 * Functions executed by both the UE and the MME to send EMM messages
 * --------------------------------------------------------------------------
 */
int emm_send_status(const emm_as_status_t *, emm_status_msg *);

int emm_send_detach_accept(const emm_as_data_t *, detach_accept_msg *);

/*
 * --------------------------------------------------------------------------
 * Functions executed by the UE to send EMM messages to the network
 * --------------------------------------------------------------------------
 */
#ifdef NAS_UE
int emm_send_attach_request(const emm_as_establish_t *, attach_request_msg *);
int emm_send_attach_complete(const emm_as_data_t *, attach_complete_msg *);

int emm_send_initial_detach_request(const emm_as_establish_t *,
                                    detach_request_msg *);
int emm_send_detach_request(const emm_as_data_t *, detach_request_msg *);


int emm_send_initial_tau_request(const emm_as_establish_t *,
                                 tracking_area_update_request_msg *);

int emm_send_initial_sr_request(const emm_as_establish_t *,
                                service_request_msg *);

int emm_send_initial_extsr_request(const emm_as_establish_t *,
                                   extended_service_request_msg *);

int emm_send_identity_response(const emm_as_security_t *,
                               identity_response_msg *);
int emm_send_authentication_response(const emm_as_security_t *,
                                     authentication_response_msg *);
int emm_send_authentication_failure(const emm_as_security_t *,
                                    authentication_failure_msg *);
int emm_send_security_mode_complete(const emm_as_security_t *,
                                    security_mode_complete_msg *);
int emm_send_security_mode_reject(const emm_as_security_t *,
                                  security_mode_reject_msg *);
#endif

/*
 * --------------------------------------------------------------------------
 * Functions executed by the MME to send EMM messages to the UE
 * --------------------------------------------------------------------------
 */
#ifdef NAS_MME
int emm_send_attach_accept(const emm_as_establish_t *, attach_accept_msg *);
int emm_send_attach_reject(const emm_as_establish_t *, attach_reject_msg *);

int emm_send_tracking_area_update_reject(const emm_as_establish_t *msg,
                                         tracking_area_update_reject_msg *emm_msg);

int emm_send_identity_request(const emm_as_security_t *, identity_request_msg *);
int emm_send_authentication_request(const emm_as_security_t *,
                                    authentication_request_msg *);
int emm_send_authentication_reject(authentication_reject_msg *);
int emm_send_security_mode_command(const emm_as_security_t *,
                                   security_mode_command_msg *);
#endif

#endif /* __EMM_SEND_H__*/
