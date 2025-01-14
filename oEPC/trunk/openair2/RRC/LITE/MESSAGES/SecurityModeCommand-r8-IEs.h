/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_SecurityModeCommand_r8_IEs_H_
#define	_SecurityModeCommand_r8_IEs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SecurityConfigSMC.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SecurityModeCommand_v8a0_IEs;

/* SecurityModeCommand-r8-IEs */
typedef struct SecurityModeCommand_r8_IEs {
	SecurityConfigSMC_t	 securityConfigSMC;
	struct SecurityModeCommand_v8a0_IEs	*nonCriticalExtension	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SecurityModeCommand_r8_IEs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SecurityModeCommand_r8_IEs;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SecurityModeCommand-v8a0-IEs.h"

#endif	/* _SecurityModeCommand_r8_IEs_H_ */
#include <asn_internal.h>
