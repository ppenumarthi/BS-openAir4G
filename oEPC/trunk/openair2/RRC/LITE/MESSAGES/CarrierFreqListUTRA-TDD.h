/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_CarrierFreqListUTRA_TDD_H_
#define	_CarrierFreqListUTRA_TDD_H_


#include <asn_application.h>

/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CarrierFreqUTRA_TDD;

/* CarrierFreqListUTRA-TDD */
typedef struct CarrierFreqListUTRA_TDD {
	A_SEQUENCE_OF(struct CarrierFreqUTRA_TDD) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CarrierFreqListUTRA_TDD_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CarrierFreqListUTRA_TDD;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CarrierFreqUTRA-TDD.h"

#endif	/* _CarrierFreqListUTRA_TDD_H_ */
#include <asn_internal.h>
