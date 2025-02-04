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
#include "AttachAccept.h"

int decode_attach_accept(attach_accept_msg *attach_accept, uint8_t *buffer, uint32_t len)
{
    uint32_t decoded = 0;
    int decoded_result = 0;

    // Check if we got a NULL pointer and if buffer length is >= minimum length expected for the message.
    CHECK_PDU_POINTER_AND_LENGTH_DECODER(buffer, ATTACH_ACCEPT_MINIMUM_LENGTH, len);

    /* Decoding mandatory fields */
    if ((decoded_result = decode_u8_eps_attach_result(&attach_accept->epsattachresult, 0, *(buffer + decoded), len - decoded)) < 0)
        return decoded_result;

    decoded++;
    if ((decoded_result = decode_gprs_timer(&attach_accept->t3412value, 0, buffer + decoded, len - decoded)) < 0)
        return decoded_result;
    else
        decoded += decoded_result;

    if ((decoded_result = decode_tracking_area_identity_list(&attach_accept->tailist, 0, buffer + decoded, len - decoded)) < 0)
        return decoded_result;
    else
        decoded += decoded_result;

    if ((decoded_result = decode_esm_message_container(&attach_accept->esmmessagecontainer, 0, buffer + decoded, len - decoded)) < 0)
        return decoded_result;
    else
        decoded += decoded_result;

    /* Decoding optional fields */
    while(len - decoded > 0)
    {
        uint8_t ieiDecoded = *(buffer + decoded);
        /* Type | value iei are below 0x80 so just return the first 4 bits */
        if (ieiDecoded >= 0x80)
            ieiDecoded = ieiDecoded & 0xf0;
        switch(ieiDecoded)
        {
            case ATTACH_ACCEPT_GUTI_IEI:
                if ((decoded_result =
                    decode_eps_mobile_identity(&attach_accept->guti,
                    ATTACH_ACCEPT_GUTI_IEI, buffer + decoded, len - decoded))
                    <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_GUTI_PRESENT;
                break;
            case ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_IEI:
                if ((decoded_result =
                    decode_location_area_identification(&attach_accept->locationareaidentification,
                    ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_IEI, buffer +
                    decoded, len - decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT;
                break;
            case ATTACH_ACCEPT_MS_IDENTITY_IEI:
                if ((decoded_result =
                    decode_mobile_identity(&attach_accept->msidentity,
                    ATTACH_ACCEPT_MS_IDENTITY_IEI, buffer + decoded, len -
                    decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_MS_IDENTITY_PRESENT;
                break;
            case ATTACH_ACCEPT_EMM_CAUSE_IEI:
                if ((decoded_result = decode_emm_cause(&attach_accept->emmcause,
                    ATTACH_ACCEPT_EMM_CAUSE_IEI, buffer + decoded, len -
                    decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_EMM_CAUSE_PRESENT;
                break;
            case ATTACH_ACCEPT_T3402_VALUE_IEI:
                if ((decoded_result =
                    decode_gprs_timer(&attach_accept->t3402value,
                    ATTACH_ACCEPT_T3402_VALUE_IEI, buffer + decoded, len -
                    decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_T3402_VALUE_PRESENT;
                break;
            case ATTACH_ACCEPT_T3423_VALUE_IEI:
                if ((decoded_result =
                    decode_gprs_timer(&attach_accept->t3423value,
                    ATTACH_ACCEPT_T3423_VALUE_IEI, buffer + decoded, len -
                    decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_T3423_VALUE_PRESENT;
                break;
            case ATTACH_ACCEPT_EQUIVALENT_PLMNS_IEI:
                if ((decoded_result =
                    decode_plmn_list(&attach_accept->equivalentplmns,
                    ATTACH_ACCEPT_EQUIVALENT_PLMNS_IEI, buffer + decoded, len -
                    decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT;
                break;
            case ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_IEI:
                if ((decoded_result =
                    decode_emergency_number_list(&attach_accept->emergencynumberlist,
                    ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_IEI, buffer + decoded,
                    len - decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT;
                break;
            case ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_IEI:
                if ((decoded_result =
                    decode_eps_network_feature_support(&attach_accept->epsnetworkfeaturesupport,
                    ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_IEI, buffer +
                    decoded, len - decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
                break;
            case ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_IEI:
                if ((decoded_result =
                    decode_additional_update_result(&attach_accept->additionalupdateresult,
                    ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_IEI, buffer +
                    decoded, len - decoded)) <= 0)
                    return decoded_result;
                decoded += decoded_result;
                /* Set corresponding mask to 1 in presencemask */
                attach_accept->presencemask |= ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT;
                break;
            default:
                errorCodeDecoder = TLV_DECODE_UNEXPECTED_IEI;
                return TLV_DECODE_UNEXPECTED_IEI;
        }
    }
    return decoded;
}

int encode_attach_accept(attach_accept_msg *attach_accept, uint8_t *buffer, uint32_t len)
{
    int encoded = 0;
    int encode_result = 0;

    LOG_FUNC_IN;

    /* Checking IEI and pointer */
    CHECK_PDU_POINTER_AND_LENGTH_ENCODER(buffer, ATTACH_ACCEPT_MINIMUM_LENGTH, len);

    *(buffer + encoded) = (encode_u8_eps_attach_result(&attach_accept->epsattachresult) & 0x0f);
    encoded++;
#warning "LG TEST override t3412value"
    attach_accept->t3412value.unit = GPRS_TIMER_UNIT_360S;
    attach_accept->t3412value.timervalue = 10;
    if ((encode_result = encode_gprs_timer(&attach_accept->t3412value, 0, buffer
         + encoded, len - encoded)) < 0) {        //Return in case of error
        LOG_TRACE(WARNING, "Failed encode_gprs_timer");
        LOG_FUNC_RETURN(encode_result);
    } else
        encoded += encode_result;

    if ((encode_result =
         encode_tracking_area_identity_list(&attach_accept->tailist, 0, buffer
         + encoded, len - encoded)) < 0) {       //Return in case of error
        LOG_TRACE(WARNING, "Failed encode_tracking_area_identity_list");
        LOG_FUNC_RETURN(encode_result);
    } else
        encoded += encode_result;

    if ((encode_result =
         encode_esm_message_container(&attach_accept->esmmessagecontainer, 0,
         buffer + encoded, len - encoded)) < 0) {       //Return in case of error
        LOG_TRACE(WARNING, "Failed encode_esm_message_container");
        LOG_FUNC_RETURN(encode_result);
    } else
        encoded += encode_result;

    if ((attach_accept->presencemask & ATTACH_ACCEPT_GUTI_PRESENT)
        == ATTACH_ACCEPT_GUTI_PRESENT)
    {
        if ((encode_result = encode_eps_mobile_identity(&attach_accept->guti,
             ATTACH_ACCEPT_GUTI_IEI, buffer + encoded, len - encoded)) < 0) {
            // Return in case of error
            LOG_TRACE(WARNING, "Failed encode_eps_mobile_identity");
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT)
        == ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT)
    {
        if ((encode_result =
             encode_location_area_identification(&attach_accept->locationareaidentification,
             ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_IEI, buffer + encoded,
             len - encoded)) < 0) {
            LOG_TRACE(WARNING, "Failed encode_location_area_identification");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_MS_IDENTITY_PRESENT)
        == ATTACH_ACCEPT_MS_IDENTITY_PRESENT)
    {
        if ((encode_result = encode_mobile_identity(&attach_accept->msidentity,
             ATTACH_ACCEPT_MS_IDENTITY_IEI, buffer + encoded, len - encoded)) <
             0) {
            LOG_TRACE(WARNING, "Failed encode_mobile_identity");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_EMM_CAUSE_PRESENT)
        == ATTACH_ACCEPT_EMM_CAUSE_PRESENT)
    {
        if ((encode_result = encode_emm_cause(&attach_accept->emmcause,
             ATTACH_ACCEPT_EMM_CAUSE_IEI, buffer + encoded, len - encoded)) <
             0) {
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

#warning "LG TEST override t3402value"
    attach_accept->presencemask |= ATTACH_ACCEPT_T3402_VALUE_PRESENT;
    attach_accept->t3402value.unit = GPRS_TIMER_UNIT_60S;
    attach_accept->t3402value.timervalue = 12;
    if ((attach_accept->presencemask & ATTACH_ACCEPT_T3402_VALUE_PRESENT)
        == ATTACH_ACCEPT_T3402_VALUE_PRESENT)
    {
        if ((encode_result = encode_gprs_timer(&attach_accept->t3402value,
             ATTACH_ACCEPT_T3402_VALUE_IEI, buffer + encoded, len - encoded)) <
             0) {
            LOG_TRACE(WARNING, "Failed encode_gprs_timer");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_T3423_VALUE_PRESENT)
        == ATTACH_ACCEPT_T3423_VALUE_PRESENT)
    {
        if ((encode_result = encode_gprs_timer(&attach_accept->t3423value,
             ATTACH_ACCEPT_T3423_VALUE_IEI, buffer + encoded, len - encoded)) <
             0) {
            LOG_TRACE(WARNING, "Failed encode_gprs_timer");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT)
        == ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT)
    {
        if ((encode_result = encode_plmn_list(&attach_accept->equivalentplmns,
             ATTACH_ACCEPT_EQUIVALENT_PLMNS_IEI, buffer + encoded, len -
             encoded)) < 0) {
            LOG_TRACE(WARNING, "Failed encode_plmn_list");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT)
        == ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT)
    {
        if ((encode_result =
             encode_emergency_number_list(&attach_accept->emergencynumberlist,
             ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_IEI, buffer + encoded, len -
             encoded)) < 0) {
            LOG_TRACE(WARNING, "Failed encode_emergency_number_list");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT)
        == ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT)
    {
        if ((encode_result =
             encode_eps_network_feature_support(&attach_accept->epsnetworkfeaturesupport,
             ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_IEI, buffer + encoded,
             len - encoded)) < 0) {
            LOG_TRACE(WARNING, "Failed encode_eps_network_feature_support");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    if ((attach_accept->presencemask & ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT)
        == ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT)
    {
        if ((encode_result =
             encode_additional_update_result(&attach_accept->additionalupdateresult,
             ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_IEI, buffer + encoded, len
             - encoded)) < 0) {
            LOG_TRACE(WARNING, "Failed encode_additional_update_result");
            // Return in case of error
            LOG_FUNC_RETURN(encode_result);
        } else
            encoded += encode_result;
    }

    LOG_FUNC_RETURN(encoded);
}

