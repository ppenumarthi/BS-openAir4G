/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_PhyLayerParameters_v920_H_
#define	_PhyLayerParameters_v920_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum PhyLayerParameters_v920__enhancedDualLayerFDD_r9 {
	PhyLayerParameters_v920__enhancedDualLayerFDD_r9_supported	= 0
} e_PhyLayerParameters_v920__enhancedDualLayerFDD_r9;
typedef enum PhyLayerParameters_v920__enhancedDualLayerTDD_r9 {
	PhyLayerParameters_v920__enhancedDualLayerTDD_r9_supported	= 0
} e_PhyLayerParameters_v920__enhancedDualLayerTDD_r9;

/* PhyLayerParameters-v920 */
typedef struct PhyLayerParameters_v920 {
	long	*enhancedDualLayerFDD_r9	/* OPTIONAL */;
	long	*enhancedDualLayerTDD_r9	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PhyLayerParameters_v920_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_enhancedDualLayerFDD_r9_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_enhancedDualLayerTDD_r9_4;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_PhyLayerParameters_v920;

#ifdef __cplusplus
}
#endif

#endif	/* _PhyLayerParameters_v920_H_ */
#include <asn_internal.h>
