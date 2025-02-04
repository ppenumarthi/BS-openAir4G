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
#include <string.h>
#include <stdint.h>


#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "GutiReallocationComplete.h"

int decode_guti_reallocation_complete(guti_reallocation_complete_msg *guti_reallocation_complete, uint8_t *buffer, uint32_t len)
{
    uint32_t decoded = 0;
    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
    CHECK_PDU_POINTER_AND_LENGTH_DECODER(buffer, GUTI_REALLOCATION_COMPLETE_MINIMUM_LENGTH, len);

    /* Decoding mandatory fields */
    return decoded;
}

int encode_guti_reallocation_complete(guti_reallocation_complete_msg *guti_reallocation_complete, uint8_t *buffer, uint32_t len)
{
    int encoded = 0;
    /* Checking IEI and pointer */
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER(buffer, GUTI_REALLOCATION_COMPLETE_MINIMUM_LENGTH, len);

    return encoded;
}

