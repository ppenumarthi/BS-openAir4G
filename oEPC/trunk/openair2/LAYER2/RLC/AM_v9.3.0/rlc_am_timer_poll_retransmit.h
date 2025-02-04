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
/*! \file rlc_am_timer_poll_retransmit.h
* \brief This file defines the prototypes of the functions manipulating the t-PollRetransmit timer.
* \author GAUTHIER Lionel
* \date 2010-2011
* \version
* \note
* \bug
* \warning
*/
/** @defgroup _rlc_am_timers_impl_ RLC AM Timers Reference Implementation
* @ingroup _rlc_am_internal_impl_
* @{
*/

#ifndef __RLC_AM_TIMER_POLL_RETRANSMIT_H__
#    define __RLC_AM_TIMER_POLL_RETRANSMIT_H__
#    ifdef RLC_AM_TIMER_POLL_RETRANSMIT_C
#        define private_rlc_am_timer_poll_retransmit(x)    x
#        define protected_rlc_am_timer_poll_retransmit(x)  x
#        define public_rlc_am_timer_poll_retransmit(x)     x
#    else
#        ifdef RLC_AM_MODULE
#            define private_rlc_am_timer_poll_retransmit(x)
#            define protected_rlc_am_timer_poll_retransmit(x)  extern x
#            define public_rlc_am_timer_poll_retransmit(x)     extern x
#        else
#            define private_rlc_am_timer_poll_retransmit(x)
#            define protected_rlc_am_timer_poll_retransmit(x)
#            define public_rlc_am_timer_poll_retransmit(x)     extern x
#        endif
#    endif

/*! \fn void rlc_am_check_timer_poll_retransmit(const protocol_ctxt_t* const ctxt_pP, rlc_am_entity_t* const rlc_pP)
* \brief      Check if timer Poll-retransmit has timed-out, if so it is stopped and has the status "timed-out".
* \param[in]  ctxt_pP           Running context.
* \param[in]  rlc_pP            RLC AM protocol instance pointer.

*/
protected_rlc_am_timer_poll_retransmit(void rlc_am_check_timer_poll_retransmit(
                const protocol_ctxt_t* const ctxt_pP,
                rlc_am_entity_t* const       rlc_pP));

/*! \fn int  rlc_am_is_timer_poll_retransmit_timed_out(const protocol_ctxt_t* const ctxt_pP, rlc_am_entity_t* const rlc_pP)
* \brief      Boolean function, check if timer Poll-retransmit has timed-out.
* \param[in]  ctxt_pP           Running context.
* \param[in]  rlc_pP            RLC AM protocol instance pointer.
* \return     1 if timer Poll-retransmit has timed-out, else 0.
*/
protected_rlc_am_timer_poll_retransmit(int  rlc_am_is_timer_poll_retransmit_timed_out(
                const protocol_ctxt_t* const ctxt_pP,
                rlc_am_entity_t* const       rlc_pP);)

/*! \fn void rlc_am_stop_and_reset_timer_poll_retransmit(const protocol_ctxt_t* const ctxt_pP, rlc_am_entity_t* const rlc_pP)
* \brief      Stop and reset the timer Poll-retransmit.
* \param[in]  ctxt_pP           Running context.
* \param[in]  rlc_pP            RLC AM protocol instance pointer.

*/
protected_rlc_am_timer_poll_retransmit(void rlc_am_stop_and_reset_timer_poll_retransmit(
                const protocol_ctxt_t* const ctxt_pP,
                rlc_am_entity_t* const       rlc_pP);)

/*! \fn void rlc_am_start_timer_poll_retransmit(const protocol_ctxt_t* const ctxt_pP, rlc_am_entity_t* const rlc_pP)
* \brief      Re-arm (based on RLC AM config parameter) and start timer Poll-retransmit.
* \param[in]  ctxt_pP           Running context.
* \param[in]  rlc_pP            RLC AM protocol instance pointer.
* \param[in]  frame             Frame index
*/
protected_rlc_am_timer_poll_retransmit(void rlc_am_start_timer_poll_retransmit(
                const protocol_ctxt_t* const ctxt_pP,
                rlc_am_entity_t* const       rlc_pP);)

/*! \fn void rlc_am_init_timer_poll_retransmit(const protocol_ctxt_t* const ctxt_pP, rlc_am_entity_t* const rlc_pP, const uint32_t time_outP)
* \brief      Initialize the timer Poll-retransmit with RLC AM time-out config parameter.
* \param[in]  ctxt_pP           Running context.
* \param[in]  rlc_pP            RLC AM protocol instance pointer.
* \param[in]  time_outP         Time-out in frame units.
*/
protected_rlc_am_timer_poll_retransmit(void rlc_am_init_timer_poll_retransmit(
                const protocol_ctxt_t* const ctxt_pP,
                rlc_am_entity_t* const       rlc_pP,
                const uint32_t               time_outP);)
/** @} */
#endif
