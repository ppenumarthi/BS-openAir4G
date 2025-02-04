/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_CounterCheckResponse_r8_IEs_H_
#define	_CounterCheckResponse_r8_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DRB-CountInfoList.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CounterCheckResponse_v8a0_IEs;

/* CounterCheckResponse-r8-IEs */
typedef struct CounterCheckResponse_r8_IEs {
	DRB_CountInfoList_t	 drb_CountInfoList;
	struct CounterCheckResponse_v8a0_IEs	*nonCriticalExtension	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CounterCheckResponse_r8_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CounterCheckResponse_r8_IEs;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CounterCheckResponse-v8a0-IEs.h"

#endif	/* _CounterCheckResponse_r8_IEs_H_ */
#include <asn_internal.h>
