/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_VarMeasReport_H_
#define	_VarMeasReport_H_


#include <asn_application.h>

/* Including external dependencies */
#include "MeasId.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CellsTriggeredList;

/* VarMeasReport */
typedef struct VarMeasReport {
	MeasId_t	 measId;
	struct CellsTriggeredList	*cellsTriggeredList	/* OPTIONAL */;
	long	 numberOfReportsSent;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VarMeasReport_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VarMeasReport;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CellsTriggeredList.h"

#endif	/* _VarMeasReport_H_ */
#include <asn_internal.h>
