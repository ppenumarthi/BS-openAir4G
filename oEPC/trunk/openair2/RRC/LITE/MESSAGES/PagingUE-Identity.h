/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_PagingUE_Identity_H_
#define	_PagingUE_Identity_H_


#include <asn_application.h>

/* Including external dependencies */
#include "S-TMSI.h"
#include "IMSI.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PagingUE_Identity_PR {
	PagingUE_Identity_PR_NOTHING,	/* No components present */
	PagingUE_Identity_PR_s_TMSI,
	PagingUE_Identity_PR_imsi,
	/* Extensions may appear below */
	
} PagingUE_Identity_PR;

/* PagingUE-Identity */
typedef struct PagingUE_Identity {
	PagingUE_Identity_PR present;
	union PagingUE_Identity_u {
		S_TMSI_t	 s_TMSI;
		IMSI_t	 imsi;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PagingUE_Identity_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PagingUE_Identity;

#ifdef __cplusplus
}
#endif

#endif	/* _PagingUE_Identity_H_ */
#include <asn_internal.h>
