/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_CSFB_RegistrationParam1XRTT_v920_H_
#define	_CSFB_RegistrationParam1XRTT_v920_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CSFB_RegistrationParam1XRTT_v920__powerDownReg_r9 {
	CSFB_RegistrationParam1XRTT_v920__powerDownReg_r9_true	= 0
} e_CSFB_RegistrationParam1XRTT_v920__powerDownReg_r9;

/* CSFB-RegistrationParam1XRTT-v920 */
typedef struct CSFB_RegistrationParam1XRTT_v920 {
	long	 powerDownReg_r9;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CSFB_RegistrationParam1XRTT_v920_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_powerDownReg_r9_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_CSFB_RegistrationParam1XRTT_v920;

#ifdef __cplusplus
}
#endif

#endif	/* _CSFB_RegistrationParam1XRTT_v920_H_ */
#include <asn_internal.h>
