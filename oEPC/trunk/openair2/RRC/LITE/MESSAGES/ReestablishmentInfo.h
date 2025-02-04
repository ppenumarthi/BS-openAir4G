/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_ReestablishmentInfo_H_
#define	_ReestablishmentInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include "ShortMAC-I.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AdditionalReestabInfoList;

/* ReestablishmentInfo */
typedef struct ReestablishmentInfo {
	PhysCellId_t	 sourcePhysCellId;
	ShortMAC_I_t	 targetCellShortMAC_I;
	struct AdditionalReestabInfoList	*additionalReestabInfoList	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ReestablishmentInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReestablishmentInfo;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AdditionalReestabInfoList.h"

#endif	/* _ReestablishmentInfo_H_ */
#include <asn_internal.h>
