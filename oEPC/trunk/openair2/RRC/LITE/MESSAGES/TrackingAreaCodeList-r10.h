/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_TrackingAreaCodeList_r10_H_
#define	_TrackingAreaCodeList_r10_H_


#include <asn_application.h>

/* Including external dependencies */
#include "TrackingAreaCode.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TrackingAreaCodeList-r10 */
typedef struct TrackingAreaCodeList_r10 {
	A_SEQUENCE_OF(TrackingAreaCode_t) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TrackingAreaCodeList_r10_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TrackingAreaCodeList_r10;

#ifdef __cplusplus
}
#endif

#endif	/* _TrackingAreaCodeList_r10_H_ */
#include <asn_internal.h>
