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
/*! \file gtpv1u_eNB.c
 * \brief
 * \author Sebastien ROUX, Lionel GAUTHIER
 * \version 1.0
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */
#include <stdio.h>
#include <errno.h>

#include "mme_config.h"

#include "assertions.h"
#include "intertask_interface.h"
#include "timer.h"

#include "gtpv1u.h"
#include "NwGtpv1u.h"
#include "NwGtpv1uMsg.h"
#include "NwGtpv1uPrivate.h"
#include "NwLog.h"
#include "gtpv1u_eNB_defs.h"
#include "gtpv1_u_messages_types.h"
#include "udp_eNB_task.h"
#include "UTIL/LOG/log.h"
#include "COMMON/platform_types.h"
#include "COMMON/platform_constants.h"
#include "UTIL/LOG/vcd_signal_dumper.h"


#undef GTP_DUMP_SOCKET

extern boolean_t pdcp_data_req(
                const protocol_ctxt_t* const  ctxt_pP,
                const srb_flag_t     srb_flagP,
                const rb_id_t        rb_idP,
                const mui_t          muiP,
                const confirm_t      confirmP,
                const sdu_size_t     sdu_buffer_sizeP,
                unsigned char *const sdu_buffer_pP,
                const pdcp_transmission_mode_t modeP);


static int
    gtpv1u_eNB_send_init_udp(
        uint16_t port_number);

NwGtpv1uRcT
    gtpv1u_eNB_log_request(
        NwGtpv1uLogMgrHandleT   hLogMgr,
        NwU32T                  logLevel,
        NwCharT                *file,
        NwU32T                  line,
        NwCharT                *logStr);

NwGtpv1uRcT
    gtpv1u_eNB_send_udp_msg(
        NwGtpv1uUdpHandleT      udpHandle,
        NwU8T                  *buffer,
        NwU32T                  buffer_len,
        NwU32T                  buffer_offset,
        NwU32T                  peerIpAddr,
        NwU16T                  peerPort);

NwGtpv1uRcT
    gtpv1u_eNB_process_stack_req(
        NwGtpv1uUlpHandleT hUlp,
        NwGtpv1uUlpApiT   *pUlpApi);

int
    data_recv_callback(
        uint16_t  portP,
        uint32_t  address,
        uint8_t  *buffer,
        uint32_t  length,
        void     *arg_p);
//int
//gtpv1u_create_tunnel_endpoint(
//    gtpv1u_data_t *gtpv1u_data_pP,
//    uint8_t        ue_idP,
//    uint8_t        rab_idP,
//    char          *sgw_ip_addr_pP,
//    uint16_t       portP);
static NwGtpv1uRcT
    gtpv1u_start_timer_wrapper(
    NwGtpv1uTimerMgrHandleT tmrMgrHandle,
    NwU32T                  timeoutSec,
    NwU32T                  timeoutUsec,
    NwU32T                  tmrType,
    void                   *timeoutArg,
    NwGtpv1uTimerHandleT   *hTmr);

static NwGtpv1uRcT
    gtpv1u_stop_timer_wrapper(
    NwGtpv1uTimerMgrHandleT     tmrMgrHandle,
    NwGtpv1uTimerHandleT         hTmr);

int
gtpv1u_initial_req(
    gtpv1u_data_t *gtpv1u_data_pP,
    teid_t         teidP,
    tcp_udp_port_t portP,
    uint32_t       address);

int
gtpv1u_new_data_req(
    uint8_t  enb_idP,
    uint8_t  ue_idP,
    uint8_t  rab_idP,
    uint8_t *buffer_pP,
    uint32_t buf_lenP,
    uint32_t buf_offsetP);

static int
gtpv1u_create_s1u_tunnel(
        gtpv1u_enb_create_tunnel_req_t *create_tunnel_req_pP);

static int
    gtpv1u_delete_s1u_tunnel(gtpv1u_enb_delete_tunnel_req_t *req_pP);

static int
    gtpv1u_eNB_init(void);

void *
    gtpv1u_eNB_task(void *args);

static gtpv1u_data_t gtpv1u_data_g;

#if defined(GTP_DUMP_SOCKET) && GTP_DUMP_SOCKET > 0
#include <linux/if.h>
static int           gtpv1u_dump_socket_g;

//-----------------------------------------------------------------------------
int gtpv1u_eNB_create_dump_socket(void) {
  struct ifreq ifr;
  int          hdrincl=1;

  if ((gtpv1u_dump_socket_g = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0 ) {
      LOG_E(GTPU, "Could not create dump socket %d:%s\n", errno, strerror(errno));
      return -1;
  }

  if (setsockopt(gtpv1u_dump_socket_g,
                  IPPROTO_IP,
                  IP_HDRINCL,
                  &hdrincl,
                  sizeof(hdrincl))==-1) {
      LOG_E(GTPU, "%s:%d set IP_HDRINCL %d:%s\n",
          __FILE__, __LINE__, errno, strerror(errno));
  }

  memset(&ifr, 0, sizeof(ifr));
  snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "lo");
  if (setsockopt(gtpv1u_dump_socket_g, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)) < 0)
  {
      LOG_E(GTPU, "%s:%d  setsockopt SO_BINDTODEVICE %d:%s\n",
          __FILE__, __LINE__, errno, strerror(errno));
      close(gtpv1u_dump_socket_g);
      return -1;
  }
}

//-----------------------------------------------------------------------------
static void gtpv1u_eNB_write_dump_socket(uint8_t *buffer_pP, uint32_t buffer_lengthP) {
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_LOOPBACK;

  if (sendto(gtpv1u_dump_socket_g, buffer_pP, (size_t)buffer_lengthP, 0, (struct sockaddr *)&sin, sizeof(struct sockaddr)) < 0)  {
      LOG_E(GTPU, "%s:%s:%d  sendto %d:%s\n",
          __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));
  }
}

#endif

//-----------------------------------------------------------------------------
static int gtpv1u_eNB_send_init_udp(uint16_t port_number)
{
    // Create and alloc new message
    MessageDef *message_p;
    struct in_addr addr;

    message_p = itti_alloc_new_message(TASK_GTPV1_U, UDP_INIT);
    if (message_p == NULL) {
        return -1;
    }

    UDP_INIT(message_p).port = port_number;
    //LG UDP_INIT(message_p).address = "0.0.0.0"; //ANY address

    addr.s_addr = gtpv1u_data_g.enb_ip_address_for_S1u_S12_S4_up;
    UDP_INIT(message_p).address = inet_ntoa(addr);
    LOG_D(GTPU, "Tx UDP_INIT IP addr %s\n", UDP_INIT(message_p).address);

    return itti_send_msg_to_task(TASK_UDP, INSTANCE_DEFAULT, message_p);
}

//-----------------------------------------------------------------------------
NwGtpv1uRcT gtpv1u_eNB_log_request(NwGtpv1uLogMgrHandleT hLogMgr,
                               NwU32T logLevel,
                               NwCharT *file,
                               NwU32T line,
                               NwCharT *logStr)
{
    LOG_D(GTPU, "%s\n", logStr);
    return NW_GTPV1U_OK;
}

//-----------------------------------------------------------------------------
NwGtpv1uRcT gtpv1u_eNB_send_udp_msg(
    NwGtpv1uUdpHandleT udpHandle,
    NwU8T *buffer,
    NwU32T buffer_len,
    NwU32T buffer_offset,
    NwU32T peerIpAddr,
    NwU16T peerPort)
{
    // Create and alloc new message
    MessageDef     *message_p       = NULL;
    udp_data_req_t *udp_data_req_p  = NULL;

    message_p = itti_alloc_new_message(TASK_GTPV1_U, UDP_DATA_REQ);

    if (message_p) {
#if defined(LOG_GTPU) && LOG_GTPU > 0
        LOG_D(GTPU, "Sending UDP_DATA_REQ length %u offset %u", buffer_len, buffer_offset);
#endif
        udp_data_req_p = &message_p->ittiMsg.udp_data_req;
        udp_data_req_p->peer_address  = peerIpAddr;
        udp_data_req_p->peer_port     = peerPort;
        udp_data_req_p->buffer        = buffer;
        udp_data_req_p->buffer_length = buffer_len;
        udp_data_req_p->buffer_offset = buffer_offset;
        return itti_send_msg_to_task(TASK_UDP, INSTANCE_DEFAULT, message_p);
    } else {
        return NW_GTPV1U_FAILURE;
    }
}


//-----------------------------------------------------------------------------
/* Callback called when a gtpv1u message arrived on UDP interface */
NwGtpv1uRcT gtpv1u_eNB_process_stack_req(
    NwGtpv1uUlpHandleT hUlp,
    NwGtpv1uUlpApiT   *pUlpApi)
{
    int                 result             = 0;
    teid_t              teid               = 0;
    hashtable_rc_t      hash_rc            = HASH_TABLE_KEY_NOT_EXISTS;
    gtpv1u_teid_data_t *gtpv1u_teid_data_p = NULL;
    protocol_ctxt_t     ctxt;

    switch(pUlpApi->apiType) {
            /* Here there are two type of messages handled:
             * - T-PDU
             * - END-MARKER
             */
        case NW_GTPV1U_ULP_API_RECV_TPDU: {
            uint8_t              buffer[4096];
            uint32_t             buffer_len;
            /* Nw-gptv1u stack has processed a PDU. we can schedule it to PDCP
             * for transmission.
             */
            teid = pUlpApi->apiInfo.recvMsgInfo.teid;
            if (NW_GTPV1U_OK != nwGtpv1uMsgGetTpdu(pUlpApi->apiInfo.recvMsgInfo.hMsg,
                                                   buffer, &buffer_len)) {
                LOG_E(GTPU, "Error while retrieving T-PDU");
            }
            itti_free(TASK_UDP, ((NwGtpv1uMsgT*)pUlpApi->apiInfo.recvMsgInfo.hMsg)->msgBuf);
#if defined(GTP_DUMP_SOCKET) && GTP_DUMP_SOCKET > 0
            gtpv1u_eNB_write_dump_socket(buffer,buffer_len);
#endif

            //-----------------------
            // GTPV1U->PDCP mapping
            //-----------------------
            hash_rc = hashtable_get(gtpv1u_data_g.teid_mapping, teid, (void**)&gtpv1u_teid_data_p);
            if (hash_rc == HASH_TABLE_OK) {
#if defined(LOG_GTPU) && LOG_GTPU > 0
                LOG_D(GTPU, "Received T-PDU from gtpv1u stack teid  %u size %d -> enb module id %u ue module id %u rab id %u\n",
                    teid,
                    buffer_len,
                    gtpv1u_teid_data_p->enb_id,
                    gtpv1u_teid_data_p->ue_id,
                    gtpv1u_teid_data_p->eps_bearer_id);
#endif

#warning "LG eps bearer mapping to DRB id to do (offset -4)"
                ctxt.enb_module_id = gtpv1u_teid_data_p->enb_id;
                ctxt.ue_module_id  = gtpv1u_teid_data_p->ue_id;
                ctxt.frame         = 0;
                ctxt.enb_flag      = ENB_FLAG_YES;

                result = pdcp_data_req(
                    &ctxt,
                    SRB_FLAG_NO,
                    (gtpv1u_teid_data_p->eps_bearer_id) ? gtpv1u_teid_data_p->eps_bearer_id - 4: 5-4,
                    0, // mui
                    FALSE, // confirm
                    buffer_len,
                    buffer,
                    PDCP_TRANSMISSION_MODE_DATA);
                AssertError (result == TRUE, return NW_GTPV1U_FAILURE ,"PDCP data request failed!\n");
            } else {
                LOG_E(GTPU, "Received T-PDU from gtpv1u stack teid %u unknown size %u", teid, buffer_len);
            }
        }
        break;
        default: {
            LOG_E(GTPU, "Received undefined UlpApi (%02x) from gtpv1u stack!\n",
                       pUlpApi->apiType);
        }
    }
    return NW_GTPV1U_OK;
}


//-----------------------------------------------------------------------------
int data_recv_callback(uint16_t  portP,
                       uint32_t  address,
                       uint8_t  *buffer,
                       uint32_t  length,
                       void     *arg_p)
{
    gtpv1u_data_t        *gtpv1u_data_p;

    if (arg_p == NULL) {
        return -1;
    }

    gtpv1u_data_p = (gtpv1u_data_t *)arg_p;

    return nwGtpv1uProcessUdpReq(gtpv1u_data_p->gtpv1u_stack,
           buffer,
           length,
           portP,
           address);
}

//int
//gtpv1u_create_tunnel_endpoint(
//    gtpv1u_data_t *gtpv1u_data_pP,
//    uint8_t        ue_idP,
//    uint8_t        rab_idP,
//    char          *sgw_ip_addr_pP,
//    uint16_t       portP)
//{
//    uint32_t                     teid;
//    uint8_t                      max_attempt = 100;
//    NwGtpv1uRcT                  rc          = NW_GTPV1U_FAILURE;
//    NwGtpv1uUlpApiT              ulp_req;
//    struct gtpv1u_ue_data_s     *new_ue_p    = NULL;
//    struct gtpv1u_bearer_s      *bearer_p    = NULL;
//    hashtable_rc_t               hash_rc     = HASH_TABLE_KEY_NOT_EXISTS;;
//
//    if (rab_idP > GTPV1U_MAX_BEARERS_PER_UE) {
//        LOG_E(GTPU, "Could not use rab_id %d > max %d\n",
//              rab_idP, GTPV1U_MAX_BEARERS_PER_UE);
//        return -1;
//    }
//
//
//    if ((hash_rc = hashtable_get(gtpv1u_data_pP->ue_mapping, (uint64_t)ue_idP, (void**)&new_ue_p)) == HASH_TABLE_OK) {
//        /* A context for this UE already exist in the tree, use it */
//        /* We check that the tunnel is not already configured */
//        if (new_ue_p->bearers[rab_idP].state != BEARER_DOWN) {
//            LOG_E(GTPU, "Cannot create new end-point over already existing tunnel\n");
//            return -1;
//        }
//    } else {
//        /* Context doesn't exist, create it */
//        if (rab_idP != 0) {
//            /* UE should first establish Default bearer before trying to setup
//             * additional bearers.
//             */
//            LOG_E(GTPU, "UE context is not known and rab_id != 0\n");
//            return -1;
//        }
//        new_ue_p = calloc(1, sizeof(struct gtpv1u_ue_data_s));
//        new_ue_p->ue_id = ue_idP;
//
//        hash_rc = hashtable_insert(gtpv1u_data_pP->ue_mapping, (uint64_t)ue_idP, new_ue_p);
//
//        if ((hash_rc != HASH_TABLE_OK) && (hash_rc != HASH_TABLE_INSERT_OVERWRITTEN_DATA)) {
//            LOG_E(GTPU, "Failed to insert new UE context\n");
//            free(new_ue_p);
//            return -1;
//        }
//    }
//
//    bearer_p = &new_ue_p->bearers[rab_idP];
//
//    /* Configure the bearer */
//    bearer_p->state       = BEARER_IN_CONFIG;
//    bearer_p->sgw_ip_addr = inet_addr(sgw_ip_addr_pP);
//    bearer_p->port        = portP;
//
//    /* Create the new stack api request */
//    memset(&ulp_req, 0, sizeof(NwGtpv1uUlpApiT));
//    ulp_req.apiType = NW_GTPV1U_ULP_API_CREATE_TUNNEL_ENDPOINT;
//
//    /* Try to create new tunnel-endpoint.
//     * If teid generated is already present in the stack, just peek another random
//     * teid. This could be ok for small number of tunnel but more errors could be
//     * thrown if we reached high number of tunnels.
//     * TODO: find a solution for teid
//     */
//    do {
//        /* Request for a new random TEID */
//        teid = gtpv1u_new_teid();
//        ulp_req.apiInfo.createTunnelEndPointInfo.teid = teid;
//
//        rc = nwGtpv1uProcessUlpReq(gtpv1u_data_pP->gtpv1u_stack, &ulp_req);
//
//        if (rc == NW_GTPV1U_OK) {
////             LOG_D(GTPU, "Successfully created new tunnel endpoint for teid 0x%x\n",
////                   teid);
//            bearer_p->teid_eNB = teid;
////             gtpv1u_initial_req(gtpv1u_data_pP, teid, GTPV1U_UDP_PORT,
////                                inet_addr("192.168.56.101"));
//            LOG_I(GTPU, "Created eNB tunnel endpoint %u for ue id %u, rab id %u\n", teid, ue_idP, rab_idP);
//            return 0;
//        } else {
//            LOG_W(GTPU, "Teid %u already in use... %s\n",
//                  teid, (max_attempt > 1) ? "Trying another one" : "Last chance");
//        }
//    } while(max_attempt-- && rc != NW_GTPV1U_OK);
//
//    bearer_p->state = BEARER_DOWN;
//    LOG_I(GTPU, "Failed to created eNB tunnel endpoint %u for ue id %u, rab id %u, bearer down\n", teid, ue_idP, rab_idP);
//
//    return -1;
//}


//-----------------------------------------------------------------------------
static NwGtpv1uRcT gtpv1u_start_timer_wrapper(
    NwGtpv1uTimerMgrHandleT tmrMgrHandle,
    NwU32T                  timeoutSec,
    NwU32T                  timeoutUsec,
    NwU32T                  tmrType,
    void                   *timeoutArg,
    NwGtpv1uTimerHandleT   *hTmr)
{

    NwGtpv1uRcT rc = NW_GTPV1U_OK;
    long        timer_id;

    if (tmrType == NW_GTPV1U_TMR_TYPE_ONE_SHOT) {
        timer_setup(timeoutSec,
                    timeoutUsec,
                    TASK_GTPV1_U,
                    INSTANCE_DEFAULT,
                    TIMER_ONE_SHOT,
                    timeoutArg,
                    &timer_id);
    } else {
        timer_setup(timeoutSec,
                    timeoutUsec,
                    TASK_GTPV1_U,
                    INSTANCE_DEFAULT,
                    TIMER_PERIODIC,
                    timeoutArg,
                    &timer_id);
    }

    return rc;
}


//-----------------------------------------------------------------------------
static NwGtpv1uRcT
gtpv1u_stop_timer_wrapper(
    NwGtpv1uTimerMgrHandleT tmrMgrHandle,
    NwGtpv1uTimerHandleT hTmr)
{

    NwGtpv1uRcT rc = NW_GTPV1U_OK;

    return rc;
}


//-----------------------------------------------------------------------------
int
gtpv1u_initial_req(
    gtpv1u_data_t *gtpv1u_data_pP,
    teid_t         teidP,
    tcp_udp_port_t portP,
    uint32_t       address)
{
    NwGtpv1uUlpApiT ulp_req;
    NwGtpv1uRcT     rc = NW_GTPV1U_FAILURE;

    memset(&ulp_req, 0, sizeof(NwGtpv1uUlpApiT));

    ulp_req.apiType = NW_GTPV1U_ULP_API_INITIAL_REQ;
    ulp_req.apiInfo.initialReqInfo.teid     = teidP;
    ulp_req.apiInfo.initialReqInfo.peerPort = portP;
    ulp_req.apiInfo.initialReqInfo.peerIp   = address;

    rc = nwGtpv1uProcessUlpReq(gtpv1u_data_pP->gtpv1u_stack, &ulp_req);
    if (rc == NW_GTPV1U_OK) {
        LOG_D(GTPU, "Successfully sent initial req for teid %u\n", teidP);
    } else {
        LOG_W(GTPU, "Could not send initial req for teid %u\n", teidP);
    }
    return (rc == NW_GTPV1U_OK) ? 0 : -1;
}

//-----------------------------------------------------------------------------
int
gtpv1u_new_data_req(
    uint8_t  enb_idP,
    uint8_t  ue_idP,
    uint8_t  rab_idP,
    uint8_t *buffer_pP,
    uint32_t buf_lenP,
    uint32_t buf_offsetP
    )
{

    NwGtpv1uUlpApiT          stack_req;
    NwGtpv1uRcT              rc            = NW_GTPV1U_FAILURE;
    struct gtpv1u_ue_data_s  ue;
    struct gtpv1u_ue_data_s *ue_inst_p     = NULL;
    struct gtpv1u_bearer_s  *bearer_p      = NULL;
    hashtable_rc_t           hash_rc       = HASH_TABLE_KEY_NOT_EXISTS;;
    gtpv1u_data_t           *gtpv1u_data_p = NULL;

    memset(&ue, 0, sizeof(struct gtpv1u_ue_data_s));
    ue.ue_id = ue_idP;

    AssertFatal(enb_idP >=0, "Bad parameter enb module id %u\n", enb_idP);
    AssertFatal((rab_idP - GTPV1U_BEARER_OFFSET)< GTPV1U_MAX_BEARERS_ID, "Bad parameter rab id %u\n", rab_idP);
    AssertFatal((rab_idP - GTPV1U_BEARER_OFFSET) >= 0 , "Bad parameter rab id %u\n", rab_idP);

    gtpv1u_data_p = &gtpv1u_data_g;
    /* Check that UE context is present in ue map. */
    hash_rc = hashtable_get(gtpv1u_data_p->ue_mapping, (uint64_t)ue_idP, (void**)&ue_inst_p);

    if (hash_rc ==  HASH_TABLE_KEY_NOT_EXISTS ) {
        LOG_E(GTPU, "[UE %d] Trying to send data on non-existing UE context\n", ue_idP);
        return -1;
    }

    bearer_p = &ue_inst_p->bearers[rab_idP - GTPV1U_BEARER_OFFSET];

    /* Ensure the bearer in ready.
     * TODO: handle the cases where the bearer is in HANDOVER state.
     * In such case packets should be placed in FIFO.
     */
    if (bearer_p->state != BEARER_UP) {
        LOG_W(GTPU, "Trying to send data over bearer with state(%u) != BEARER_UP\n",
              bearer_p->state);
#warning  LG: HACK WHILE WAITING FOR NAS, normally return -1
        if (bearer_p->state != BEARER_IN_CONFIG)
            return -1;
    }

    memset(&stack_req, 0, sizeof(NwGtpv1uUlpApiT));

    stack_req.apiType                   = NW_GTPV1U_ULP_API_SEND_TPDU;
    stack_req.apiInfo.sendtoInfo.teid   = bearer_p->teid_sgw;
    stack_req.apiInfo.sendtoInfo.ipAddr = bearer_p->sgw_ip_addr;

    LOG_D(GTPU, "TX TO TEID %u addr 0x%x\n",bearer_p->teid_sgw, bearer_p->sgw_ip_addr);
    rc = nwGtpv1uGpduMsgNew(gtpv1u_data_p->gtpv1u_stack,
            bearer_p->teid_sgw,
            NW_FALSE,
            gtpv1u_data_p->seq_num++,
            buffer_pP,
            buf_lenP,
            buf_offsetP,
            &(stack_req.apiInfo.sendtoInfo.hMsg));

    if (rc != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uGpduMsgNew failed: 0x%x\n", rc);
        return -1;
    }
    rc = nwGtpv1uProcessUlpReq(gtpv1u_data_p->gtpv1u_stack,
                               &stack_req);
    if (rc != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uProcessUlpReq failed: 0x%x\n", rc);
        return -1;
    }
    rc = nwGtpv1uMsgDelete(gtpv1u_data_p->gtpv1u_stack,
                           stack_req.apiInfo.sendtoInfo.hMsg);
    if (rc != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uMsgDelete failed: 0x%x\n", rc);
        return -1;
    }
    LOG_D(GTPU, "%s() return code OK\n", __FUNCTION__);
    return 0;
}

//-----------------------------------------------------------------------------
static int
gtpv1u_create_s1u_tunnel(
    gtpv1u_enb_create_tunnel_req_t *create_tunnel_req_pP)
{
    /* Create a new nw-gtpv1-u stack req using API */
    NwGtpv1uUlpApiT          stack_req;
    NwGtpv1uRcT              rc                   = NW_GTPV1U_FAILURE;
    /* Local tunnel end-point identifier */
    teid_t                   s1u_teid             = 0;
    gtpv1u_teid_data_t      *gtpv1u_teid_data_p   = NULL;
    gtpv1u_ue_data_t        *gtpv1u_ue_data_p     = NULL;
    MessageDef              *message_p            = NULL;
    hashtable_rc_t           hash_rc              = HASH_TABLE_KEY_NOT_EXISTS;
    int                      i;
    ebi_t                    eps_bearer_id        = 0;
    int                      ipv4_addr            = 0;
    int                      ip_offset            = 0;
    in_addr_t                in_addr;
    int                      addrs_length_in_bytes= 0;

    message_p = itti_alloc_new_message(TASK_GTPV1_U, GTPV1U_ENB_CREATE_TUNNEL_RESP);
    GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).ue_index    = create_tunnel_req_pP->ue_index;
    GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).status      = 0;
    GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).num_tunnels = 0;

    for (i = 0; i < create_tunnel_req_pP->num_tunnels; i++) {
        ip_offset               = 0;
        eps_bearer_id = create_tunnel_req_pP->eps_bearer_id[i];
        LOG_D(GTPU, "Rx GTPV1U_ENB_CREATE_TUNNEL_REQ ue_index %u eps bearer id %u\n",
            create_tunnel_req_pP->ue_index, eps_bearer_id);
        memset(&stack_req, 0, sizeof(NwGtpv1uUlpApiT));

        stack_req.apiType = NW_GTPV1U_ULP_API_CREATE_TUNNEL_ENDPOINT;

        do {
            s1u_teid = gtpv1u_new_teid();
            LOG_D(GTPU, "gtpv1u_create_s1u_tunnel() 0x%x %u(dec)\n", s1u_teid, s1u_teid);
            stack_req.apiInfo.createTunnelEndPointInfo.teid          = s1u_teid;
            stack_req.apiInfo.createTunnelEndPointInfo.hUlpSession   = 0;
            stack_req.apiInfo.createTunnelEndPointInfo.hStackSession = 0;

            rc = nwGtpv1uProcessUlpReq(gtpv1u_data_g.gtpv1u_stack, &stack_req);
            LOG_D(GTPU, ".\n");
        } while (rc != NW_GTPV1U_OK);

        //-----------------------
        // PDCP->GTPV1U mapping
        //-----------------------
        hash_rc = hashtable_get(gtpv1u_data_g.ue_mapping, create_tunnel_req_pP->ue_index, (void **)&gtpv1u_ue_data_p);
        if ((hash_rc == HASH_TABLE_KEY_NOT_EXISTS) || (hash_rc == HASH_TABLE_OK)) {

            if (hash_rc == HASH_TABLE_KEY_NOT_EXISTS) {
                gtpv1u_ue_data_p = calloc (1, sizeof(gtpv1u_ue_data_t));
                hash_rc = hashtable_insert(gtpv1u_data_g.ue_mapping, create_tunnel_req_pP->ue_index, gtpv1u_ue_data_p);
                AssertFatal(hash_rc == HASH_TABLE_OK, "Error inserting ue_mapping in GTPV1U hashtable");
            }
            gtpv1u_ue_data_p->ue_id       = create_tunnel_req_pP->ue_index;
                gtpv1u_ue_data_p->instance_id = 0; // TO DO
              memcpy(&GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).enb_addr.buffer,
                      &gtpv1u_data_g.enb_ip_address_for_S1u_S12_S4_up,
                      sizeof (in_addr_t));
              GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).enb_addr.length = sizeof (in_addr_t);

              //addrs_length_in_bytes = create_tunnel_req_pP->sgw_addr[i].length / 8; -- NFVEPC Project, as value is not being correctly set
              addrs_length_in_bytes = 4;
              AssertFatal((addrs_length_in_bytes == 4) ||
                      (addrs_length_in_bytes == 16) ||
                      (addrs_length_in_bytes == 20),
                      "Bad transport layer address length %d (bits) %d (bytes)",
                      create_tunnel_req_pP->sgw_addr[i].length, addrs_length_in_bytes);

            if ((addrs_length_in_bytes == 4) ||
                (addrs_length_in_bytes == 20)) {
                in_addr = *((in_addr_t*)create_tunnel_req_pP->sgw_addr[i].buffer);
                ip_offset = 4;
                gtpv1u_ue_data_p->bearers[eps_bearer_id - GTPV1U_BEARER_OFFSET].sgw_ip_addr = in_addr;
            }
            if ((addrs_length_in_bytes == 16) ||
                (addrs_length_in_bytes == 20)) {
                memcpy(gtpv1u_ue_data_p->bearers[eps_bearer_id - GTPV1U_BEARER_OFFSET].sgw_ip6_addr.s6_addr,
                        &create_tunnel_req_pP->sgw_addr[i].buffer[ip_offset],
                        16);
            }
            gtpv1u_ue_data_p->bearers[eps_bearer_id - GTPV1U_BEARER_OFFSET].state    = BEARER_IN_CONFIG;
            gtpv1u_ue_data_p->bearers[eps_bearer_id - GTPV1U_BEARER_OFFSET].teid_eNB = s1u_teid;
            gtpv1u_ue_data_p->bearers[eps_bearer_id - GTPV1U_BEARER_OFFSET].teid_sgw = create_tunnel_req_pP->sgw_S1u_teid[i];
            GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).enb_S1u_teid[i] = s1u_teid;

        } else {
            GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).enb_S1u_teid[i] = 0;
            GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).status         = 0xFF;
        }
        GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).eps_bearer_id[i] = eps_bearer_id;
        GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).num_tunnels      += 1;

        //-----------------------
        // GTPV1U->PDCP mapping
        //-----------------------
        hash_rc = hashtable_get(gtpv1u_data_g.teid_mapping, s1u_teid, (void**)&gtpv1u_teid_data_p);
        if (hash_rc == HASH_TABLE_KEY_NOT_EXISTS) {
            gtpv1u_teid_data_p = calloc (1, sizeof(gtpv1u_teid_data_t));
            gtpv1u_teid_data_p->enb_id        = 0; // TO DO
            gtpv1u_teid_data_p->ue_id         = create_tunnel_req_pP->ue_index;
            gtpv1u_teid_data_p->eps_bearer_id = eps_bearer_id;
            hash_rc = hashtable_insert(gtpv1u_data_g.teid_mapping, s1u_teid, gtpv1u_teid_data_p);
            AssertFatal(hash_rc == HASH_TABLE_OK, "Error inserting teid mapping in GTPV1U hashtable");
        } else {
            GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).enb_S1u_teid[i] = 0;
            GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).status         = 0xFF;
        }
    }
    LOG_D(GTPU, "Tx GTPV1U_ENB_CREATE_TUNNEL_RESP ue_index %u status %d\n",
        create_tunnel_req_pP->ue_index,
        GTPV1U_ENB_CREATE_TUNNEL_RESP(message_p).status);
    return itti_send_msg_to_task(TASK_RRC_ENB, INSTANCE_DEFAULT, message_p);
}



//-----------------------------------------------------------------------------
static int gtpv1u_delete_s1u_tunnel(gtpv1u_enb_delete_tunnel_req_t *req_pP)
{
    NwGtpv1uUlpApiT          stack_req;
    NwGtpv1uRcT              rc                   = NW_GTPV1U_FAILURE;
    MessageDef              *message_p = NULL;
    gtpv1u_ue_data_t        *gtpv1u_ue_data_p     = NULL;
    hashtable_rc_t           hash_rc              = HASH_TABLE_KEY_NOT_EXISTS;
    teid_t                   teid_eNB             = 0;
    int                      erab_index           = 0;

    message_p = itti_alloc_new_message(TASK_GTPV1_U, GTPV1U_ENB_DELETE_TUNNEL_RESP);

    GTPV1U_ENB_DELETE_TUNNEL_RESP(message_p).ue_index     = req_pP->ue_index;
    GTPV1U_ENB_DELETE_TUNNEL_RESP(message_p).status       = 0;


    hash_rc = hashtable_get(gtpv1u_data_g.ue_mapping, req_pP->ue_index, (void**)&gtpv1u_ue_data_p);
    if (hash_rc == HASH_TABLE_OK) {

        for (erab_index = 0; erab_index < req_pP->num_erab; erab_index++) {
            teid_eNB = gtpv1u_ue_data_p->bearers[req_pP->eps_bearer_id[erab_index] - GTPV1U_BEARER_OFFSET].teid_eNB;
            LOG_D(GTPU, "Rx GTPV1U_ENB_DELETE_TUNNEL user index %u eNB S1U teid %u eps bearer id %u\n",
                  req_pP->ue_index, teid_eNB, req_pP->eps_bearer_id[erab_index]);

            {
                memset(&stack_req, 0, sizeof(NwGtpv1uUlpApiT));
                stack_req.apiType = NW_GTPV1U_ULP_API_DESTROY_TUNNEL_ENDPOINT;
                LOG_D(GTPU, "gtpv1u_delete_s1u_tunnel erab %u  %u\n",
                      req_pP->eps_bearer_id[erab_index],
                      teid_eNB);
                stack_req.apiInfo.createTunnelEndPointInfo.teid          = teid_eNB;
                stack_req.apiInfo.createTunnelEndPointInfo.hUlpSession   = 0;
                stack_req.apiInfo.createTunnelEndPointInfo.hStackSession = 0;

                rc = nwGtpv1uProcessUlpReq(gtpv1u_data_g.gtpv1u_stack, &stack_req);
                LOG_D(GTPU, ".\n");
            }
            if (rc != NW_GTPV1U_OK) {
                GTPV1U_ENB_DELETE_TUNNEL_RESP(message_p).status       |= 0xFF;
                LOG_E(GTPU, "NW_GTPV1U_ULP_API_DESTROY_TUNNEL_ENDPOINT failed");
            }

            //-----------------------
            // PDCP->GTPV1U mapping
            //-----------------------
            gtpv1u_ue_data_p->bearers[req_pP->eps_bearer_id[erab_index] - GTPV1U_BEARER_OFFSET].state       = BEARER_DOWN;
            gtpv1u_ue_data_p->bearers[req_pP->eps_bearer_id[erab_index] - GTPV1U_BEARER_OFFSET].teid_eNB    = 0;
            gtpv1u_ue_data_p->bearers[req_pP->eps_bearer_id[erab_index] - GTPV1U_BEARER_OFFSET].teid_sgw    = 0;
            gtpv1u_ue_data_p->bearers[req_pP->eps_bearer_id[erab_index] - GTPV1U_BEARER_OFFSET].sgw_ip_addr = 0;
            gtpv1u_ue_data_p->num_bearers -= 1;

            if (gtpv1u_ue_data_p->num_bearers == 0) {
                hash_rc = hashtable_remove(gtpv1u_data_g.ue_mapping, req_pP->ue_index);
                LOG_D(GTPU, "Removed user index %u,no more bearers configured\n", req_pP->ue_index);
            }

            //-----------------------
            // GTPV1U->PDCP mapping
            //-----------------------
            hash_rc = hashtable_remove(gtpv1u_data_g.teid_mapping, teid_eNB);
            if (hash_rc != HASH_TABLE_OK) {
                LOG_D(GTPU, "Removed user index %u , enb S1U teid %u not found\n", req_pP->ue_index, teid_eNB);
            }
        }
    }// else silently do nothing


    LOG_D(GTPU, "Tx GTPV1U_ENB_DELETE_TUNNEL_RESP user index %u eNB S1U teid %u status %u\n",
        GTPV1U_ENB_DELETE_TUNNEL_RESP(message_p).ue_index,
        GTPV1U_ENB_DELETE_TUNNEL_RESP(message_p).enb_S1u_teid,
        GTPV1U_ENB_DELETE_TUNNEL_RESP(message_p).status);
    return itti_send_msg_to_task(TASK_RRC_ENB, INSTANCE_DEFAULT, message_p);
}

//-----------------------------------------------------------------------------
static int gtpv1u_eNB_init(void)
{
  int                     ret;
  NwGtpv1uRcT             rc = NW_GTPV1U_FAILURE;
  NwGtpv1uUlpEntityT      ulp;
  NwGtpv1uUdpEntityT      udp;
  NwGtpv1uLogMgrEntityT   log;
  NwGtpv1uTimerMgrEntityT tmr;
  Enb_properties_t       *enb_properties_p  = NULL;

    enb_properties_p = enb_config_get()->properties[0];


    memset(&gtpv1u_data_g, 0, sizeof(gtpv1u_data_g));

    LOG_I(GTPU, "Initializing GTPU stack %p\n",&gtpv1u_data_g);
    //gtpv1u_data_g.gtpv1u_stack;
    /* Initialize UE hashtable */
    gtpv1u_data_g.ue_mapping      = hashtable_create (32, NULL, NULL);
    AssertFatal(gtpv1u_data_g.ue_mapping != NULL, " ERROR Initializing TASK_GTPV1_U task interface: in hashtable_create returned %p\n", gtpv1u_data_g.ue_mapping);
    gtpv1u_data_g.teid_mapping    = hashtable_create (256, NULL, NULL);
    AssertFatal(gtpv1u_data_g.teid_mapping != NULL, " ERROR Initializing TASK_GTPV1_U task interface: in hashtable_create\n");
    gtpv1u_data_g.enb_ip_address_for_S1u_S12_S4_up         = enb_properties_p->enb_ipv4_address_for_S1U;
    gtpv1u_data_g.ip_addr         = NULL;
    gtpv1u_data_g.enb_port_for_S1u_S12_S4_up = enb_properties_p->enb_port_for_S1U;
    //gtpv1u_data_g.udp_data;
    gtpv1u_data_g.seq_num         = 50;
    gtpv1u_data_g.restart_counter = 0;

    /* Initializing GTPv1-U stack */
    if ((rc = nwGtpv1uInitialize(&gtpv1u_data_g.gtpv1u_stack)) != NW_GTPV1U_OK) {
        LOG_E(GTPU, "Failed to setup nwGtpv1u stack %x\n", rc);
        return -1;
    }

    if ((rc = nwGtpv1uSetLogLevel(gtpv1u_data_g.gtpv1u_stack,
                                  NW_LOG_LEVEL_DEBG)) != NW_GTPV1U_OK) {
        LOG_E(GTPU, "Failed to setup loglevel for stack %x\n", rc);
        return -1;
    }


    /* Set the ULP API callback. Called once message have been processed by the
     * nw-gtpv1u stack.
     */
    ulp.ulpReqCallback = gtpv1u_eNB_process_stack_req;

    if ((rc = nwGtpv1uSetUlpEntity(gtpv1u_data_g.gtpv1u_stack, &ulp)) != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uSetUlpEntity: %x", rc);
        return -1;
    }

    /* nw-gtpv1u stack requires an udp callback to send data over UDP.
     * We provide a wrapper to UDP task.
     */
    udp.udpDataReqCallback = gtpv1u_eNB_send_udp_msg;

    if ((rc = nwGtpv1uSetUdpEntity(gtpv1u_data_g.gtpv1u_stack, &udp)) != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uSetUdpEntity: %x", rc);
        return -1;
    }

    log.logReqCallback = gtpv1u_eNB_log_request;

    if ((rc = nwGtpv1uSetLogMgrEntity(gtpv1u_data_g.gtpv1u_stack, &log)) != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uSetLogMgrEntity: %x", rc);
        return -1;
    }

    /* Timer interface is more complicated as both wrappers doesn't send a message
     * to the timer task but call the timer API functions start/stop timer.
     */
    tmr.tmrMgrHandle     = 0;
    tmr.tmrStartCallback = gtpv1u_start_timer_wrapper;
    tmr.tmrStopCallback  = gtpv1u_stop_timer_wrapper;

    if ((rc = nwGtpv1uSetTimerMgrEntity(gtpv1u_data_g.gtpv1u_stack, &tmr)) != NW_GTPV1U_OK) {
        LOG_E(GTPU, "nwGtpv1uSetTimerMgrEntity: %x", rc);
        return -1;
    }

#if defined(GTP_DUMP_SOCKET) && GTP_DUMP_SOCKET > 0
    if ((ret = gtpv1u_eNB_create_dump_socket()) < 0) {
        return -1;
    }
#endif
    ret = gtpv1u_eNB_send_init_udp(gtpv1u_data_g.enb_port_for_S1u_S12_S4_up);
    if (ret < 0) {
        return ret;
    }

    LOG_D(GTPU, "Initializing GTPV1U interface for eNB: DONE\n");
    return 0;
}


//-----------------------------------------------------------------------------
void *gtpv1u_eNB_task(void *args)
{
    int rc = 0;
    rc = gtpv1u_eNB_init();
    AssertFatal(rc == 0, "gtpv1u_eNB_init Failed");
    itti_mark_task_ready(TASK_GTPV1_U);
    while(1) {
        /* Trying to fetch a message from the message queue.
         * If the queue is empty, this function will block till a
         * message is sent to the task.
         */
        MessageDef *received_message_p = NULL;
        itti_receive_msg(TASK_GTPV1_U, &received_message_p);
        vcd_signal_dumper_dump_function_by_name(VCD_SIGNAL_DUMPER_FUNCTIONS_GTPV1U_ENB_TASK, VCD_FUNCTION_IN);
        DevAssert(received_message_p != NULL);

        LOG_I(GTPU,"GTPV1U_ENB_CREATE_TUNNEL_REQ: %d \n", GTPV1U_ENB_CREATE_TUNNEL_REQ);
        switch (ITTI_MSG_ID(received_message_p))
        {
            case GTPV1U_ENB_CREATE_TUNNEL_REQ: {
    		LOG_D(GTPU, "Received Message as GTPV1U_ENB_CREATE_TUNNEL_REQ:: \n");
                gtpv1u_create_s1u_tunnel(&received_message_p->ittiMsg.Gtpv1uCreateTunnelReq);
            }
            break;

            case GTPV1U_ENB_DELETE_TUNNEL_REQ: {
    		LOG_D(GTPU, "Received Message as GTPV1U_ENB_DELETE_TUNNEL_REQ:: \n");
                gtpv1u_delete_s1u_tunnel(&received_message_p->ittiMsg.Gtpv1uDeleteTunnelReq);
            }
            break;

            // DATA COMING FROM UDP
            case UDP_DATA_IND: {
    		LOG_D(GTPU, "Received Message as UDP_DATA_IND:: \n");
                udp_data_ind_t *udp_data_ind_p;
                udp_data_ind_p = &received_message_p->ittiMsg.udp_data_ind;
                nwGtpv1uProcessUdpReq(gtpv1u_data_g.gtpv1u_stack,
                                      udp_data_ind_p->buffer,
                                      udp_data_ind_p->buffer_length,
                                      udp_data_ind_p->peer_port,
                                      udp_data_ind_p->peer_address);
                //itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), udp_data_ind_p->buffer);
            }
            break;

            // DATA TO BE SENT TO UDP
            case GTPV1U_ENB_TUNNEL_DATA_REQ: {
    		LOG_D(GTPU, "Received Message as GTPV1U_ENB_TUNNEL_DATA_REQ: \n");
            	gtpv1u_enb_tunnel_data_req_t *data_req_p           = NULL;
                NwGtpv1uUlpApiT          stack_req;
                NwGtpv1uRcT                   rc                   = NW_GTPV1U_FAILURE;
                hashtable_rc_t                hash_rc              = HASH_TABLE_KEY_NOT_EXISTS;
                gtpv1u_ue_data_t             *gtpv1u_ue_data_p     = NULL;
                teid_t                        enb_s1u_teid         = 0;
                teid_t                        sgw_s1u_teid         = 0;

                vcd_signal_dumper_dump_function_by_name(VCD_SIGNAL_DUMPER_FUNCTIONS_GTPV1U_PROCESS_TUNNEL_DATA_REQ, VCD_FUNCTION_IN);
                data_req_p = &GTPV1U_ENB_TUNNEL_DATA_REQ(received_message_p);
                //ipv4_send_data(ipv4_data_p->sd, data_ind_p->buffer, data_ind_p->length);

#if defined(GTP_DUMP_SOCKET) && GTP_DUMP_SOCKET > 0
                gtpv1u_eNB_write_dump_socket(&data_req_p->buffer[data_req_p->offset],data_req_p->length);
#endif
                memset(&stack_req, 0, sizeof(NwGtpv1uUlpApiT));

                hash_rc = hashtable_get(gtpv1u_data_g.ue_mapping, (uint64_t)data_req_p->ue_index, (void**)&gtpv1u_ue_data_p);

                if (hash_rc == HASH_TABLE_KEY_NOT_EXISTS) {
                    LOG_E(GTPU, "nwGtpv1uProcessUlpReq failed: while getting ue_index %u in hashtable ue_mapping\n", data_req_p->ue_index);
                } else {
                    if ((data_req_p->rab_id >= GTPV1U_BEARER_OFFSET) && (data_req_p->rab_id <= max_val_DRB_Identity)) {
                        enb_s1u_teid                        = gtpv1u_ue_data_p->bearers[data_req_p->rab_id - GTPV1U_BEARER_OFFSET].teid_eNB;
                        sgw_s1u_teid                        = gtpv1u_ue_data_p->bearers[data_req_p->rab_id - GTPV1U_BEARER_OFFSET].teid_sgw;
                        stack_req.apiType                   = NW_GTPV1U_ULP_API_SEND_TPDU;
                        stack_req.apiInfo.sendtoInfo.teid   = sgw_s1u_teid;
                        stack_req.apiInfo.sendtoInfo.ipAddr = gtpv1u_ue_data_p->bearers[data_req_p->rab_id - GTPV1U_BEARER_OFFSET].sgw_ip_addr;

                        rc = nwGtpv1uGpduMsgNew(
                                 gtpv1u_data_g.gtpv1u_stack,
                                 sgw_s1u_teid,
                                 NW_FALSE,
                                 gtpv1u_data_g.seq_num++,
                                 data_req_p->buffer,
                                 data_req_p->length,
                                 data_req_p->offset,
                                 &(stack_req.apiInfo.sendtoInfo.hMsg));

                        if (rc != NW_GTPV1U_OK) {
                            LOG_E(GTPU, "nwGtpv1uGpduMsgNew failed: 0x%x\n", rc);
                        } else {
                            rc = nwGtpv1uProcessUlpReq(gtpv1u_data_g.gtpv1u_stack, &stack_req);
                            if (rc != NW_GTPV1U_OK) {
                                LOG_E(GTPU, "nwGtpv1uProcessUlpReq failed: 0x%x\n", rc);
                            }
                            rc = nwGtpv1uMsgDelete(gtpv1u_data_g.gtpv1u_stack,
                                    stack_req.apiInfo.sendtoInfo.hMsg);
                            if (rc != NW_GTPV1U_OK) {
                                LOG_E(GTPU, "nwGtpv1uMsgDelete failed: 0x%x\n", rc);
                            }
                        }
                    }
                }
                vcd_signal_dumper_dump_function_by_name(VCD_SIGNAL_DUMPER_FUNCTIONS_GTPV1U_PROCESS_TUNNEL_DATA_REQ, VCD_FUNCTION_OUT);
                /* Buffer still needed, do not free it */
                //itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), data_req_p->buffer);
            }
            break;

            case TERMINATE_MESSAGE: {
                if (gtpv1u_data_g.ue_mapping != NULL) {
                    hashtable_destroy (gtpv1u_data_g.ue_mapping);
                }
                if (gtpv1u_data_g.teid_mapping != NULL) {
                    hashtable_destroy (gtpv1u_data_g.teid_mapping);
                }
                itti_exit_task();
            } break;

            case TIMER_HAS_EXPIRED:
                nwGtpv1uProcessTimeout(&received_message_p->ittiMsg.timer_has_expired.arg);
                break;

            default: {
                LOG_E(GTPU, "Unkwnon message ID %d:%s\n",
                           ITTI_MSG_ID(received_message_p),
                           ITTI_MSG_NAME(received_message_p));
            }
            break;
        }
        rc = itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), received_message_p);
        AssertFatal(rc == EXIT_SUCCESS, "Failed to free memory (%d)!\n", rc);
        received_message_p = NULL;
        vcd_signal_dumper_dump_function_by_name(VCD_SIGNAL_DUMPER_FUNCTIONS_GTPV1U_ENB_TASK, VCD_FUNCTION_OUT);
    }
    return NULL;
}

