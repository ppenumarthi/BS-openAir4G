/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_SRB_ToAddModList_H_
#define	_SRB_ToAddModList_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SRB_ToAddMod;

/* SRB-ToAddModList */
typedef struct SRB_ToAddModList {
	A_SEQUENCE_OF(struct SRB_ToAddMod) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SRB_ToAddModList_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SRB_ToAddModList;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SRB-ToAddMod.h"

#endif	/* _SRB_ToAddModList_H_ */
#include <asn_internal.h>
