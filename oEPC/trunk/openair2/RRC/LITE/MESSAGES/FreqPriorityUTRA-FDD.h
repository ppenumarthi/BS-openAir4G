/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_FreqPriorityUTRA_FDD_H_
#define	_FreqPriorityUTRA_FDD_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ARFCN-ValueUTRA.h"
#include "CellReselectionPriority.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FreqPriorityUTRA-FDD */
typedef struct FreqPriorityUTRA_FDD {
	ARFCN_ValueUTRA_t	 carrierFreq;
	CellReselectionPriority_t	 cellReselectionPriority;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} FreqPriorityUTRA_FDD_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FreqPriorityUTRA_FDD;

#ifdef __cplusplus
}
#endif

#endif	/* _FreqPriorityUTRA_FDD_H_ */
#include <asn_internal.h>
