/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_SON_Parameters_r9_H_
#define	_SON_Parameters_r9_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SON_Parameters_r9__rach_Report_r9 {
	SON_Parameters_r9__rach_Report_r9_supported	= 0
} e_SON_Parameters_r9__rach_Report_r9;

/* SON-Parameters-r9 */
typedef struct SON_Parameters_r9 {
	long	*rach_Report_r9	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SON_Parameters_r9_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_rach_Report_r9_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_SON_Parameters_r9;

#ifdef __cplusplus
}
#endif

#endif	/* _SON_Parameters_r9_H_ */
#include <asn_internal.h>
