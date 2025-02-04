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
/*! \file sgi_task.c
* \brief
* \author Lionel Gauthier
* \company Eurecom
* \email: lionel.gauthier@eurecom.fr
*/

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <net/if.h>

//-----------------------------------------------------------------------------
#include "assertions.h"
#include "mme_config.h"
#include "intertask_interface.h"
#include "sgi.h"

//-----------------------------------------------------------------------------
// NFVEPC Project-----------------------------------------------------------------
//-----------------------------------------------------------------------------
#include <netinet/in.h>
#include <linux/ioctl.h>
#define MAX_BS_AT_EPC 16
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
static void* sgi_task_thread(void *args_p);
static int sgi_create_endpoint_request(sgi_data_t *sgi_dataP, SGICreateEndpointReq *req_p);
static int sgi_update_endpoint_request(sgi_data_t *sgi_dataP, SGIUpdateEndpointReq *req_p);
static int sgi_init_phase2(sgi_data_t *sgi_data_pP);
//-----------------------------------------------------------------------------
static pthread_t fw_2_sgi_task_thread;



//-----------------------------------------------------------------------------
//nfvepc's Specific Modification
//-----------------------------------------------------------------------------
static pthread_t listen_task_thread;
//static pthread_t read_task_thread[MAX_BS_AT_EPC];
static pthread_t read_task_thread;

static pthread_t sample_message_task_thread;

static int clientNum = 0;
//struct sockaddr_in cli_addr[MAX_BS_AT_EPC];
static struct sockaddr_in cli_addr;
//int clilen[MAX_BS_AT_EPC];
//int newsockfd[MAX_BS_AT_EPC];
static int clilen;
static int newsockfd;
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
static void* sgi_task_thread(void *args_p)
//-----------------------------------------------------------------------------
{
    sgi_data_t *sgi_data_p;

    if (args_p == NULL) {
        return NULL;
    }

    itti_mark_task_ready(TASK_FW_IP);

    sgi_data_p = (sgi_data_t *)args_p;


    while(1) {
        /* Trying to fetch a message from the message queue.
         * If the queue is empty, this function will block till a
         * message is sent to the task.
         */
        MessageDef *received_message_p;
        itti_receive_msg(TASK_FW_IP, &received_message_p);
        DevAssert(received_message_p != NULL);
/*
 *  * Only for statistics 
 *   * NFVEPC Project
 *    */

struct timeval tim;
gettimeofday(&tim, NULL);
double t1=tim.tv_sec+(tim.tv_usec/1000000.0);              


  /*SGI_IF_DEBUG("-------------------------------\n" );
  SGI_IF_DEBUG("Current local time and date: %.6lf \n", t1 );
  SGI_IF_DEBUG("Received message as %d with size as: %d\n", ITTI_MSG_ID(received_message_p), sizeof( *received_message_p) );
  SGI_IF_DEBUG("GTPV1U_TUNNEL_DATA_IND:: %d, ACTIVATE_MESSAGE:: %d \n", GTPV1U_TUNNEL_DATA_IND, ACTIVATE_MESSAGE); */
  if (ITTI_MSG_ID(received_message_p) == 18)
  SGI_IF_DEBUG("ABCD %.6lf %s %d\n", t1, "GTPV1U_TUNNEL_DATA_IND", sizeof( *received_message_p));
  if (ITTI_MSG_ID(received_message_p) == 1)
  SGI_IF_DEBUG("ABCD %.6lf %s %d\n", t1, "ACTIVATE_MESSAGE", sizeof( *received_message_p));
//  SGI_IF_DEBUG("-------------------------------\n");
/*
 *  * Done with statistics
 *   */

        //SGI_IF_DEBUG("Cleare received_message_p \n");

        if (sgi_data_p->hss_associated) {
//            SGI_IF_DEBUG("Reached hss_associated \n");
//
            switch (ITTI_MSG_ID(received_message_p))
            {
                case GTPV1U_TUNNEL_DATA_IND: {
                    /* We received data from GTPV1_U incoming from an UE.
                     * Forward it host adapter.
                     */
                    Gtpv1uTunnelDataInd *data_ind_p;
                    data_ind_p = &received_message_p->ittiMsg.gtpv1uTunnelDataInd;
#ifdef SGI_SOCKET_RAW
                    sgi_send_data(&data_ind_p->buffer[data_ind_p->offset], data_ind_p->length, sgi_data_p, data_ind_p->local_S1u_teid);
#else
#ifdef SGI_SOCKET_DGRAM
                    sgi_dgram_send_data(&data_ind_p->buffer[data_ind_p->offset], data_ind_p->length, sgi_data_p, data_ind_p->local_S1u_teid);
#endif
#endif
                    /* Buffer is no longer needed, free it */
                    //itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), data_ind_p->buffer); //NFVEPC Project
                }
                break;
                case SGI_CREATE_ENDPOINT_REQUEST: {
                    SGICreateEndpointReq *req_p;
                    req_p = &received_message_p->ittiMsg.sgiCreateEndpointReq;
                    sgi_create_endpoint_request(sgi_data_p, req_p);
                }
                break;

                case SGI_UPDATE_ENDPOINT_REQUEST: {
                    SGIUpdateEndpointReq *req_p;
                    req_p = &received_message_p->ittiMsg.sgiUpdateEndpointReq;
                    sgi_update_endpoint_request(sgi_data_p, req_p);
                }
                break;

                case ACTIVATE_MESSAGE: {//NFVEPC Project
                    SGI_IF_DEBUG("receved ACTIVATE_MESSAGE");
                    sgi_data_p->hss_associated = 1;
#if !defined(ENABLE_USE_GTPU_IN_KERNEL)
                    SGI_IF_DEBUG("HSS ASSOCIATED, CAN START VLANS (bug in freediameter)");
                    sgi_init_phase2(sgi_data_p);
#endif
                } break; //NFVEPC Project ends

                default: {
                    SGI_IF_ERROR("Unkwnon message ID %d:%s\n",
                             ITTI_MSG_ID(received_message_p), ITTI_MSG_NAME(received_message_p));
                }
                break;
            }
        }else {
                    SGI_IF_DEBUG("HSS IS UNASSOCIATED");
            switch (ITTI_MSG_ID(received_message_p))
            {
                case ACTIVATE_MESSAGE: {
                    sgi_data_p->hss_associated = 1;
#if !defined(ENABLE_USE_GTPU_IN_KERNEL) //NFVEPC Project
//#if defined(ENABLE_USE_GTPU_IN_KERNEL)
                    SGI_IF_DEBUG("HSS ASSOCIATED, CAN START VLANS (bug in freediameter)");
                    sgi_init_phase2(sgi_data_p);
#endif
                } break;

                default: {
                    SGI_IF_ERROR("Unkwnon or ignored message ID %d:%s\n",
                             ITTI_MSG_ID(received_message_p), ITTI_MSG_NAME(received_message_p));
                }
                break;
            }
        }
        itti_free(ITTI_MSG_ORIGIN_ID(received_message_p), received_message_p);
        received_message_p = NULL;
    }
    return NULL;
}


//------------------------------------------------------
static int sgi_create_endpoint_request(sgi_data_t *sgi_dataP, SGICreateEndpointReq *req_p)
//------------------------------------------------------
{
    SGICreateEndpointResp                     *sgi_create_endpoint_resp_p;
    MessageDef                                *message_p;
    sgi_teid_mapping_t                        *mapping;

    SGI_IF_DEBUG("Rx IP_FW_CREATE_SGI_ENDPOINT_REQUEST, Context: S-GW S11 teid %u, S-GW S1U teid %u EPS bearer id %u\n",
        req_p->context_teid, req_p->sgw_S1u_teid, req_p->eps_bearer_id);

    message_p               = itti_alloc_new_message(TASK_FW_IP, SGI_CREATE_ENDPOINT_RESPONSE);
    if (message_p == NULL) {
        return -1;
    }
    sgi_create_endpoint_resp_p = &message_p->ittiMsg.sgiCreateEndpointResp;
    sgi_create_endpoint_resp_p->context_teid   = req_p->context_teid;
    sgi_create_endpoint_resp_p->sgw_S1u_teid   = req_p->sgw_S1u_teid;
    sgi_create_endpoint_resp_p->eps_bearer_id  = req_p->eps_bearer_id;
    sgi_create_endpoint_resp_p->pdn_type       = req_p->pdn_type;
    sgi_create_endpoint_resp_p->paa            = req_p->paa;
    sgi_create_endpoint_resp_p->status         = SGI_STATUS_OK;

    if (hashtable_is_key_exists(sgi_dataP->teid_mapping, req_p->sgw_S1u_teid) == HASH_TABLE_OK)
    {
        SGI_IF_ERROR("SGI_STATUS_ERROR_CONTEXT_ALREADY_EXIST Context: S11 teid %u\n", req_p->context_teid);
        sgi_create_endpoint_resp_p->status       = SGI_STATUS_ERROR_CONTEXT_ALREADY_EXIST;
    } else {
        mapping = malloc(sizeof(sgi_teid_mapping_t));
        if (mapping == NULL) {
            sgi_create_endpoint_resp_p->status       = SGI_STATUS_ERROR_NO_MEMORY_AVAILABLE;
        } else {
            memset(mapping, 0 , sizeof(sgi_teid_mapping_t));

            mapping->eps_bearer_id              = req_p->eps_bearer_id;
            mapping->enb_S1U_teid               = -1;
            if (hashtable_insert(sgi_dataP->teid_mapping, req_p->sgw_S1u_teid, mapping) != 0) {
                SGI_IF_ERROR("SGI_STATUS_ERROR_SYSTEM_FAILURE Context: S11 teid %u\n", req_p->context_teid);
                sgi_create_endpoint_resp_p->status  = SGI_STATUS_ERROR_SYSTEM_FAILURE;
                free(mapping);
            } else {
                SGI_IF_ERROR("SGI_STATUS_OK Context: S11 teid %u\n", req_p->context_teid);
                sgi_create_endpoint_resp_p->status  = SGI_STATUS_OK;
            }
    	}
    }
    return itti_send_msg_to_task(TASK_SPGW_APP, INSTANCE_DEFAULT, message_p);
}

//------------------------------------------------------
static int sgi_update_endpoint_request(sgi_data_t *sgi_dataP, SGIUpdateEndpointReq *req_p)
//------------------------------------------------------
{
    SGIUpdateEndpointResp                     *sgi_update_endpoint_resp_p = NULL;
    MessageDef                                *message_p = NULL;
    sgi_teid_mapping_t                        *mapping   = NULL;

    SGI_IF_DEBUG("Rx IP_FW_UPDATE_SGI_ENDPOINT_REQUEST, Context: S-GW S11 teid %u, S-GW S1U teid %u EPS bearer id %u\n",
        req_p->context_teid, req_p->sgw_S1u_teid, req_p->eps_bearer_id);

    message_p               = itti_alloc_new_message(TASK_FW_IP, SGI_UPDATE_ENDPOINT_RESPONSE);
    if (message_p == NULL) {
        return -1;
    }
    sgi_update_endpoint_resp_p = &message_p->ittiMsg.sgiUpdateEndpointResp;
    sgi_update_endpoint_resp_p->context_teid   = req_p->context_teid;
    sgi_update_endpoint_resp_p->sgw_S1u_teid   = req_p->sgw_S1u_teid;
    sgi_update_endpoint_resp_p->eps_bearer_id  = req_p->eps_bearer_id;
    sgi_update_endpoint_resp_p->enb_S1u_teid   = req_p->enb_S1u_teid;
    sgi_update_endpoint_resp_p->status         = SGI_STATUS_OK;

    if (hashtable_get(sgi_dataP->teid_mapping, req_p->sgw_S1u_teid, (void**)&mapping) == HASH_TABLE_OK)
    {
        mapping->enb_S1U_teid = req_p->enb_S1u_teid;
    } else {
        SGI_IF_ERROR("SGI_STATUS_ERROR_CONTEXT_NOT_FOUND Context: S11 teid %u\n", req_p->context_teid);
        sgi_update_endpoint_resp_p->status       = SGI_STATUS_ERROR_CONTEXT_NOT_FOUND;
    }
    return itti_send_msg_to_task(TASK_SPGW_APP, INSTANCE_DEFAULT, message_p);
}


//nfvepc's Specific Modification
//-----------------------------------------------------------------------------

static void 
   send_sample_message()
{
    SGI_IF_DEBUG("In send_sample function\n");
      //while(1) 
      //{
    MessageDef *msg_p           = NULL;

    Gtpv1uCreateTunnelReq *create_tunnel_req_pP;
//        gtpv1u_enb_create_tunnel_req_t *create_tunnel_req_pP;

        //msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_CREATE_TUNNEL_REQ);
        // -- Latermsg_p = itti_alloc_new_message(15, GTPV1U_CREATE_TUNNEL_REQ); //TASK_ENB_APP = 15 from logs
    msg_p = itti_alloc_new_message(15, GTPV1U_CREATE_TUNNEL_RESP); //TASK_ENB_APP = 15 from logs

        //msg_p->ittiMsgHeader.messageId = GTPV1U_CREATE_TUNNEL_REQ; //UDP_DATA_IND;
    msg_p->ittiMsgHeader.messageId = GTPV1U_CREATE_TUNNEL_RESP; //UDP_DATA_IND;
    msg_p->ittiMsgHeader.originTaskId = 15;
    msg_p->ittiMsgHeader.destinationTaskId = TASK_GTPV1_U;
    msg_p->ittiMsgHeader.ittiMsgSize= 42; 

	//Sample values
    //msg_p->ittiMsg.gtpv1uCreateTunnelReq.S1u_teid      = 1;
    //msg_p->ittiMsg.gtpv1uCreateTunnelReq.context_teid  = 1;
    //msg_p->ittiMsg.gtpv1uCreateTunnelReq.eps_bearer_id = 1; 

    msg_p->ittiMsg.gtpv1uCreateTunnelResp.S1u_teid      = 1;
    msg_p->ittiMsg.gtpv1uCreateTunnelResp.context_teid  = 1;
    msg_p->ittiMsg.gtpv1uCreateTunnelResp.eps_bearer_id = 1; 

    itti_send_msg_to_task(TASK_SPGW_APP, INSTANCE_DEFAULT, msg_p);
    //itti_send_msg_to_task(TASK_GTPV1_U, INSTANCE_DEFAULT, msg_p);
      //}

    return;
}

static void
    read_from_socket(void *sock)
{
         int n;
         int sockNum = *(int*)sock;
         char buffer[4388];
         //n = read(newsockfd[sockNum],buffer,4388);
         //
	 //int flags = 1; 
	 //ioctl(newsockfd, FIOBIO, &flags);
	 //fcntl(fd, F_SETFL, flags | O_NONBLOCK);
         while(1) {
		//SGI_IF_DEBUG(" Listening \n");
    	         n = read(newsockfd,buffer,4388);
        	 if (n > 0) {
                 //SGI_IF_DEBUG("here is the message: %s\n",buffer);
                 //printf("the message is %s\n", buffer);
	         }




	 //MessageDef *msg_p           = (MessageDef *)buffer;
//	 MessageDef *msg_p           =  *buffer;
	// msg_p = itti_alloc_new_message(TASK_ENB_APP, GTPV1U_CREATE_TUNNEL_REQ);
 	//itti_send_msg_to_task(TASK_SPGW_APP, INSTANCE_DEFAULT, buffer);
		MessageDef *msg_p           = (struct MessageDef *)buffer;
 	//msg_p->ittiMsgHeader.messageId = GTPV1U_TUNNEL_DATA_IND;
	//itti_send_msg_to_task(TASK_FW_IP, INSTANCE_DEFAULT, buffer);

/*
 *  * Only for statistics 
 *   * NFVEPC Project
 *    */
struct timeval tim;
gettimeofday(&tim, NULL);
double t1=tim.tv_sec+(tim.tv_usec/1000000.0);              
  /*SGI_IF_DEBUG( "-------------------------------\n" );
  SGI_IF_DEBUG( "Current local time and date: %.6lf \n", t1 );
  SGI_IF_DEBUG( "Received message GTPV1_U_DATA_IND at SGI interface \n"); */
  SGI_IF_DEBUG( "ABCD %.6lf GTPV1_U_DATA_IND %d\n", t1, sizeof(*msg_p));
//  SGI_IF_DEBUG( "-------------------------------\n");
/*
 *  * Done with statistics
 *   */
		MessageDef *message_p = itti_alloc_new_message(TASK_GTPV1_U, GTPV1U_TUNNEL_DATA_IND);
	        if (message_p == NULL) {
	             return -1; 
        	}
	
		Gtpv1uTunnelDataInd *data_ind_p = NULL; 
        	data_ind_p                       = &message_p->ittiMsg.gtpv1uTunnelDataInd;

	        data_ind_p->length               = msg_p->ittiMsg.gtpv1uTunnelDataReq.length;
        //memcpy(data_ind_p->buffer, msg_p->ittiMsg.gtpv1uTunnelDataReq.buffer, data_ind_p->length);
        	data_ind_p->buffer		= msg_p->ittiMsg.gtpv1uTunnelDataReq.buffer;

	        data_ind_p->offset               = 0;
        //data_ind_p->local_S1u_teid       = pUlpApi->apiInfo.recvMsgInfo.teid;
		data_ind_p->local_S1u_teid       = 1;
	//data_ind_p->S1u_enb_teid	 = 1;

		itti_send_msg_to_task(TASK_FW_IP, INSTANCE_DEFAULT, message_p);
	}
}


static void listen_on_server()
{
	int sockfd, portno;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP packets
	//sockfd = socket(AF_INET, SOCK_DGRAM, 0); //UDP packets
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	portno = 2152;
	serv_addr.sin_family = AF_INET;
	inet_aton("192.168.1.2", &serv_addr.sin_addr);
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		SGI_IF_ERROR("Error on binding  \n");

	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while(1) {
		//newsockfd[clientNum] = accept(sockfd, (struct sockaddr *) &cli_addr[clientNum], &clilen[clientNum]);
		//clilen[clientNum] = sizeof(cli_addr[clientNum]);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		//if (newsockfd[clientNum] < 0) {
		if (newsockfd < 0) {
			SGI_IF_ERROR("Error on accept  \n");
		}
		//if( pthread_create( &read_task_thread[clientNum] , NULL , read_from_socket, (void*) clientNum) < 0)
		if( pthread_create( &read_task_thread , NULL , read_from_socket, (void*) clientNum) < 0)
        	{
		SGI_IF_ERROR("Can not create a new thread for reading packets for the newly attached BS\n");
		exit(0);
        	}
		clientNum++;
	}
}

/*
 *
void
   listen_on_socket()
{

	//FILE *fp;
	//fp = fopen("/tmp/test.txt", "a");

	int sockfd, newsockfd, portno, clilen, n;
	//char buffer[256];
	char buffer[4388];
	//void *buffer;
	struct sockaddr_in serv_addr, cli_addr;

	//sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP packets
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); //UDP packets
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = 2152;
	serv_addr.sin_family = AF_INET;
	inet_aton("192.168.1.2", &serv_addr.sin_addr);
        serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
    	    SGI_IF_ERROR("Error on binding  \n");
	while(1) 
	{
        listen(sockfd,5);
	clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
    	    SGI_IF_ERROR("Error on accept  \n"); break;
	}
	bzero(buffer,4388);
        n = read(newsockfd,buffer,4388);

	//MessageDef *msg_p	= itti_alloc_new_message(15, 19);
	//msg_p	= (MessageDef *)buffer;	
        if (n > 0) {
    		SGI_IF_DEBUG("here is the message: %s\n",buffer);
		printf("the message is %s\n", buffer);
	}
	else
		break;
	}

	//fclose(fp);

	return;
}
*/
//-----------------------------------------------------------------------------





//-----------------------------------------------------------------------------
int sgi_init(const pgw_config_t *pgw_config_p)
//-----------------------------------------------------------------------------
{

    volatile sgi_data_t *sgi_data_p;
    volatile int *sample_data_p; // never used by the thread, NFVEPC Project
    int len;
    int i;

    SGI_IF_DEBUG("Initializing FW_IP task interface\n");
    //SGI_IF_INFO("Initializing FW_IP task interface\n");

    sgi_data_p = malloc(sizeof(sgi_data_t));
    memset((void *)sgi_data_p, 0, sizeof(sgi_data_t));
    for (i=0; i < SGI_MAX_EPS_BEARERS_PER_USER; i++) {
        sgi_data_p->sd[i] = -1;
    }

    //SGI_IF_INFO("First FOR loop\n");
    sgi_data_p->thread_started = 0;
    pthread_mutex_init (&sgi_data_p->thread_started_mutex, NULL);

    sgi_data_p->teid_mapping = hashtable_create (SGI_MAX_EPS_BEARERS, NULL, NULL);
    if (sgi_data_p->teid_mapping == NULL) {
        SGI_IF_ERROR("Failed to create SGI hashtable teid_mapping\n");
        return -1;
    }

    sgi_data_p->addr_v4_mapping = hashtable_create (SGI_MAX_SERVED_USERS_PER_PGW, NULL, NULL);
    if (sgi_data_p->addr_v4_mapping == NULL) {
        SGI_IF_ERROR("Failed to create SGI hashtable addr_v4_mapping\n");
        return -1;
    }

    sgi_data_p->addr_v6_mapping = obj_hashtable_create (SGI_MAX_SERVED_USERS_PER_PGW, NULL, NULL, NULL);
    if (sgi_data_p->addr_v6_mapping == NULL) {
        SGI_IF_ERROR("Failed to create SGI hashtable addr_v6_mapping\n");
        return -1;
    }


    len = strlen(pgw_config_p->ipv4.pgw_interface_name_for_SGI);
    sgi_data_p->interface_name = calloc(len + 1, sizeof(char));
    memcpy(sgi_data_p->interface_name, pgw_config_p->ipv4.pgw_interface_name_for_SGI, len);
    sgi_data_p->interface_name[len] = '\0';
    sgi_data_p->ipv4_addr = pgw_config_p->ipv4.pgw_ipv4_address_for_SGI;

    SGI_IF_DEBUG("About to clla sgi_task_thread pthread_create: ");
    if (pthread_create(&fw_2_sgi_task_thread, NULL, &sgi_task_thread, (void *)sgi_data_p) < 0) {
        SGI_IF_ERROR("sgi_task_thread pthread_create: %s", strerror(errno));
        return -1;
    }
    SGI_IF_DEBUG("succesfully created a process with FW_IP using sgi_task_thread pthread_create: ");


   //Putting NFVEPC Project here
   //listen_on_socket();
   // if (pthread_create(&listen_task_thread, NULL, &listen_on_socket, (void *)sample_data_p) < 0) {
    if (pthread_create(&listen_task_thread, NULL, &listen_on_server, (void *)sample_data_p) < 0) {
        SGI_IF_ERROR("listen_task_thread pthread_create: %s", strerror(errno));
        return -1;
    }
    SGI_IF_DEBUG("succesfully created a process to listen on SGI interface using listen_task_thread pthread_create: ");

    /*if (pthread_create(&sample_message_task_thread, NULL, &send_sample_message, (void *)sample_data_p) < 0) {
        SGI_IF_ERROR("send_sample_message pthread_create: %s", strerror(errno));
        return -1;
    }
    SGI_IF_DEBUG("succesfully created a process to send sample message send_sample_message pthread_create: "); */

    return 0;
}

int sgi_init_phase2(sgi_data_t *sgi_data_pP) {
    int i;

    if (strcmp(sgi_data_pP->interface_name, PGW_CONFIG_STRING_INTERFACE_DISABLED) != 0) {
        sgi_data_pP->interface_index = if_nametoindex(sgi_data_pP->interface_name);

        if (sgi_create_sockets(sgi_data_pP) < 0) {
            SGI_IF_ERROR("Could not create socket, leaving thread %s\n", __FUNCTION__);
            free(sgi_data_pP);
            return -1;
        }
    } else {
        SGI_IF_WARNING("SGI interface disabled by config file\n");
    }


    if (strcmp(sgi_data_pP->interface_name, PGW_CONFIG_STRING_INTERFACE_DISABLED) != 0) {
#ifdef ENABLE_USE_PCAP_FOR_SGI
        if (pthread_create(&sgi_data_pP->capture_on_sgi_if_thread, NULL, &sgi_pcap_fw_2_gtpv1u_thread, (void *)sgi_data_pP) < 0) {
            SGI_IF_ERROR("sgi_pcap_fw_2_gtpv1u_thread pthread_create: %s", strerror(errno));
            return -1;
        }
#endif
#ifdef ENABLE_USE_NETFILTER_FOR_SGI
        if (pthread_create(&sgi_data_pP->capture_on_sgi_if_thread, NULL, &sgi_nf_fw_2_gtpv1u_thread, (void *)sgi_data_pP) < 0) {
            SGI_IF_ERROR("sgi_nf_fw_2_gtpv1u_thread pthread_create: %s", strerror(errno));
            return -1;
        }
#endif
//#if 0
#ifdef ENABLE_USE_RAW_SOCKET_FOR_SGI
        for (i=0; i < SGI_MAX_EPS_BEARERS_PER_USER; i++) {
            sgi_read_thread_args_t *args_p = malloc(sizeof(sgi_read_thread_args_t));
            args_p->sgi_data      = sgi_data_pP;
            args_p->socket_index  = i;
            if (pthread_create(&sgi_data_pP->capture_on_sgi_if_thread, NULL, &sgi_sock_raw_fw_2_gtpv1u_thread, (void *)args_p) < 0) {
                SGI_IF_ERROR("sgi_sock_raw_fw_2_gtpv1u_thread pthread_create: %s", strerror(errno));
                return -1;
            }
        }
#endif
//#endif
    }

    while (sgi_data_pP->thread_started != SGI_MAX_EPS_BEARERS_PER_USER ) {
        usleep(1000);
    }
    /*SGI_IF_DEBUG("ARP RESOLVING ROUTER...\n");
    while (sgi_data_p->hw_address_of_router_captured == 0) {
        sgi_send_arp_request(sgi_data_p, "router.eur");
        sleep(2);
    }*/
    SGI_IF_DEBUG("Initializing FW_IP task interface: DONE\n");
    return 0;
}
