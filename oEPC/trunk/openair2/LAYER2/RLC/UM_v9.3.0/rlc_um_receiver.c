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
#define RLC_UM_MODULE
#define RLC_UM_RECEIVER_C
#include "platform_types.h"
//-----------------------------------------------------------------------------
#include "rlc.h"
#include "rlc_um.h"
#include "rlc_um_structs.h"
#include "rlc_primitives.h"
#include "mac_primitives.h"
#include "list.h"
#include "MAC_INTERFACE/extern.h"
#include "UTIL/LOG/log.h"

//#define DEBUG_RLC_UM_RX               1

//-----------------------------------------------------------------------------
void
rlc_um_display_rx_window(
                const protocol_ctxt_t* const ctxtP,
                rlc_um_entity_t * const rlc_pP
                )
//-----------------------------------------------------------------------------
{
    unsigned long sn = 0;
    unsigned long end_sn = 0;
    char          str[4];
    char          time_out_str[11];
    int           str_index;
    char          color[32];

    LOG_T(RLC, "\n");
    LOG_T(RLC, "+-------------------------------------------------------------------------------------------------------+");
    LOG_T(RLC, "\n");
    sprintf(time_out_str, "%010d", rlc_pP->t_reordering.frame_time_out);
    time_out_str[10] = 0;
    LOG_T(RLC, "| RLC UM RB %02d    VR(UR)=%03d    VR(UX)=%03d    VR(UH)=%03d    t-Reordering: %s %s %s             |",
          rlc_pP->rb_id, rlc_pP->vr_ur, rlc_pP->vr_ux, rlc_pP->vr_uh,
      (rlc_pP->t_reordering.running)?" ON":"OFF",
      (rlc_pP->t_reordering.running)?"Time-out frameP:":"               ",
      (rlc_pP->t_reordering.running)?time_out_str:"          ");
    LOG_T(RLC, "\n");
    LOG_T(RLC, "+------+------------------------------------------------------------------------------------------------+");
    LOG_T(RLC, "\n");
    LOG_T(RLC, "|      |00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 |");
    LOG_T(RLC, "\n");
    LOG_T(RLC, "+------+------------------------------------------------------------------------------------------------+");
    LOG_T(RLC, "\n");
    if (rlc_pP->rx_sn_length == 10) {
        end_sn = RLC_UM_SN_10_BITS_MODULO;
    } else {
        end_sn = RLC_UM_SN_5_BITS_MODULO;
    }


    for (sn = 0; sn < end_sn; sn++) {
        str[0]    = ' ';
        str[1]    = ' ';
        str[2]    = ' ';
        str[3]    = 0;
        str_index = 0;
        if ((sn % 32) == 0){
            if ((sn != 0)){
                LOG_T(RLC, "%s%s|", RLC_FG_COLOR_DEFAULT, RLC_NORMAL_VIDEO);
                LOG_T(RLC, "\n");
            }
            LOG_T(RLC, "%s%s| %04d |", RLC_FG_COLOR_DEFAULT, RLC_NORMAL_VIDEO, sn);
        }
        strcpy(color, RLC_FG_COLOR_DEFAULT);
        if (sn == rlc_pP->vr_ur) {
            str[str_index++] = 'R';
            strcpy(color, RLC_FG_COLOR_BLUE);
        }
        if (sn == rlc_pP->vr_ux) {
            str[str_index++] = 'X';
            strcpy(color, RLC_FG_COLOR_ORANGE);
        }
        if (sn == rlc_pP->vr_uh) {
            str[str_index++] = 'H';
            strcpy(color, RLC_FG_COLOR_RED);
        }

        if (rlc_um_get_pdu_from_dar_buffer(ctxtP, rlc_pP, sn)) {
            // test RLC_REVERSE_VIDEO
            if (str_index <= 2) str[str_index] = '.';
            LOG_T(RLC, "%s%s%s", color, RLC_REVERSE_VIDEO, str);
        } else {
            LOG_T(RLC, "%s%s%s", color, RLC_NORMAL_VIDEO, str);
        }
    }
    LOG_T(RLC, "%s%s|", RLC_FG_COLOR_DEFAULT, RLC_NORMAL_VIDEO);
    LOG_T(RLC, "\n");
    LOG_T(RLC, "+------+------------------------------------------------------------------------------------------------+");
    LOG_T(RLC, "\n");
}

//-----------------------------------------------------------------------------
void
rlc_um_receive (
                const protocol_ctxt_t* const ctxtP,
                rlc_um_entity_t * const rlc_pP,
                struct mac_data_ind data_indP)
{
//-----------------------------------------------------------------------------

    mem_block_t        *tb_p             = NULL;
    uint8_t               *first_byte_p     = NULL;
    uint16_t               tb_size_in_bytes = 0;

    while ((tb_p = list_remove_head (&data_indP.data))) {

        first_byte_p = ((struct mac_tb_ind *) (tb_p->data))->data_ptr;
        tb_size_in_bytes = ((struct mac_tb_ind *) (tb_p->data))->size;

        rlc_pP->stat_rx_data_bytes += tb_size_in_bytes;
        rlc_pP->stat_rx_data_pdu   += 1;

        if (tb_size_in_bytes > 0) {
            rlc_um_receive_process_dar (ctxtP, rlc_pP, tb_p, (rlc_um_pdu_sn_10_t *)first_byte_p, tb_size_in_bytes);
#if defined(DEBUG_RLC_UM_RX)
            LOG_D(RLC, "[FRAME %05u][%s][RLC_UM][MOD %u/%u][%s %u] VR(UR)=%03d VR(UX)=%03d VR(UH)=%03d\n",
                    frameP,
                    (rlc_pP->is_enb) ? "eNB" : "UE",
                    rlc_pP->enb_module_id,
                    rlc_pP->ue_module_id,
                    (rlc_pP->is_data_plane) ? "DRB" : "SRB",
                    rlc_pP->rb_id,
                    rlc_pP->vr_ur,
                    rlc_pP->vr_ux,
                    rlc_pP->vr_uh);
            //rlc_um_display_rx_window(rlc_pP); commented because bad display
#endif
        }
    }
}
