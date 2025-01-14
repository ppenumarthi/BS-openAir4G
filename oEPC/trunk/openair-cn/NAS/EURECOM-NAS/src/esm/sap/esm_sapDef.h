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
Source      esm_sapDef.h

Version     0.1

Date        2012/11/21

Product     NAS stack

Subsystem   EPS Session Management

Author      Frederic Maurel

Description Defines the ESM Service Access Point that provides EPS
        bearer context handling and resources allocation procedures.

*****************************************************************************/

#include "emmData.h"

#ifndef __ESM_SAPDEF_H__
#define __ESM_SAPDEF_H__

#include "OctetString.h"

/****************************************************************************/
/*********************  G L O B A L    C O N S T A N T S  *******************/
/****************************************************************************/

/*
 * EPS Session Management primitives
 * ---------------------------------
 */
typedef enum {
    ESM_START = 0,
    /* Procedures related to EPS bearer contexts (initiated by the network) */
    ESM_DEFAULT_EPS_BEARER_CONTEXT_ACTIVATE_REQ,
    ESM_DEFAULT_EPS_BEARER_CONTEXT_ACTIVATE_CNF,
    ESM_DEFAULT_EPS_BEARER_CONTEXT_ACTIVATE_REJ,
    ESM_DEDICATED_EPS_BEARER_CONTEXT_ACTIVATE_REQ,
    ESM_DEDICATED_EPS_BEARER_CONTEXT_ACTIVATE_CNF,
    ESM_DEDICATED_EPS_BEARER_CONTEXT_ACTIVATE_REJ,
    ESM_EPS_BEARER_CONTEXT_MODIFY_REQ,
    ESM_EPS_BEARER_CONTEXT_MODIFY_CNF,
    ESM_EPS_BEARER_CONTEXT_MODIFY_REJ,
    ESM_EPS_BEARER_CONTEXT_DEACTIVATE_REQ,
    ESM_EPS_BEARER_CONTEXT_DEACTIVATE_CNF,
    /* Transaction related procedures (initiated by the UE) */
    ESM_PDN_CONNECTIVITY_REQ,
    ESM_PDN_CONNECTIVITY_REJ,
    ESM_PDN_DISCONNECT_REQ,
    ESM_PDN_DISCONNECT_REJ,
    ESM_BEARER_RESOURCE_ALLOCATE_REQ,
    ESM_BEARER_RESOURCE_ALLOCATE_REJ,
    ESM_BEARER_RESOURCE_MODIFY_REQ,
    ESM_BEARER_RESOURCE_MODIFY_REJ,
    /* ESM data indication ("raw" ESM message) */
    ESM_UNITDATA_IND,
    ESM_END
} esm_primitive_t;

/****************************************************************************/
/************************  G L O B A L    T Y P E S  ************************/
/****************************************************************************/

/*
 * Error code returned upon processing ESM-SAP primitive
 */
typedef enum {
    ESM_SAP_SUCCESS = 1, /* ESM-SAP primitive succeed           */
    ESM_SAP_DISCARDED,   /* ESM-SAP primitive failed, the caller should
              * ignore the error                */
    ESM_SAP_FAILED   /* ESM-SAP primitive failed, the caller should
              * take specific action and state transition may
              * occurs                  */
} esm_sap_error_t;

/*
 * ESM primitive for activate EPS default bearer context procedure
 * ---------------------------------------------------------------
 */
typedef struct {
} esm_activate_eps_default_bearer_context_t;

/*
 * ESM primitive for PDN connectivity procedure
 * --------------------------------------------
 */
typedef struct {
    int cid;        /* PDN connection local identifier      */
    int is_defined; /* Indicates whether a PDN context has been defined
             * for the specified APN            */
    int pdn_type;   /* PDN address type (IPv4, IPv6, IPv4v6)    */
    const char *apn;    /* PDN's Access Point Name          */
    int is_emergency;   /* Indicates whether the PDN context has been
             * defined to establish connection for emergency
             * bearer services              */
} esm_pdn_connectivity_t;

/*
 * ESM primitive for PDN disconnect procedure
 * ------------------------------------------
 */
typedef struct {
    int cid;        /* PDN connection local identifier      */
} esm_pdn_disconnect_t;

/*
 * ESM primitive for deactivate EPS bearer context procedure
 * ---------------------------------------------------------
 */
typedef struct {
#define ESM_SAP_ALL_EBI     0xff
    unsigned int ebi;   /* EPS bearer identity of the EPS bearer context
             * to be deactivated                */
} esm_eps_bearer_context_deactivate_t;

/*
 * ------------------------------
 * Structure of ESM-SAP primitive
 * ------------------------------
 */
typedef union {
    esm_pdn_connectivity_t pdn_connect;
    esm_pdn_disconnect_t pdn_disconnect;
    esm_eps_bearer_context_deactivate_t eps_bearer_context_deactivate;
} esm_sap_data_t;

typedef struct {
    esm_primitive_t primitive;  /* ESM-SAP primitive to process     */
    int is_standalone;      /* Indicates whether the ESM message handled
                 * within this primitive has to be sent/received
                 * standalone or together within an EMM related
                 * message              */
#if defined(NAS_MME)
    emm_data_context_t *ctx;   /* UE context                       */
#endif
    unsigned int ueid;         /* Local UE identifier              */
    esm_sap_error_t err;       /* ESM-SAP error code               */
    const OctetString *recv;   /* Encoded ESM message received     */
    OctetString send;          /* Encoded ESM message to be sent   */
    esm_sap_data_t data;       /* ESM message data parameters      */
} esm_sap_t;

/****************************************************************************/
/********************  G L O B A L    V A R I A B L E S  ********************/
/****************************************************************************/

/****************************************************************************/
/******************  E X P O R T E D    F U N C T I O N S  ******************/
/****************************************************************************/

#endif /* __ESM_SAPDEF_H__*/
