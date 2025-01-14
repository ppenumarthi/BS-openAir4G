/***************************************************************************
                          rrc_ue.h  -  description
                             -------------------
    begin                : Nov 7, 2001
    copyright            : (C) 2001, 2010 by Eurecom
    email                : Michelle.Wetterwald@eurecom.fr
 **************************************************************************
  Header associated with the rrc_ue.c file generated by Esterel Studio
 ***************************************************************************/
#include  "rrc_constant.h"
// redefined here to avoid confusion with RRC LITE value
#define IDLE RRC_CELL_IDLE

/* Generic function to encode RRC messages */
//void encode_message(int *Message_Id, int Message_Type);

int  INTEGER_COMBINE (int Id1, int Id2);

/* Selection of channels during RRC Connection SetUp
 */
void  Select_PRACH (void);
void  Select_Secondary_CCPCH (void);

/* Record of current RRC state - to be stored in: protocol_ms->rrc.protocol_state
 */
void  Set_State (int new_state, int forward);

/* release resources in RLC, MAC, PHY (no reply)
 */
void  release_radio_resources (void);

/* clear RRC variables: TRANSACTIONS, ESTABLISHED_SIGNALLING_CONNECTIONS, ESTABLISHED_RBs...
 */
void  clear_variables_rel (void);

/* clear RRC TRANSACTIONS IDENTIFIER...
 */
void  ue_clear_transaction (int Message_Type);
