/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_MobilityControlInfo_H_
#define	_MobilityControlInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PhysCellId.h"
#include "AdditionalSpectrumEmission.h"
#include <NativeEnumerated.h>
#include "C-RNTI.h"
#include "RadioResourceConfigCommon.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MobilityControlInfo__t304 {
	MobilityControlInfo__t304_ms50	= 0,
	MobilityControlInfo__t304_ms100	= 1,
	MobilityControlInfo__t304_ms150	= 2,
	MobilityControlInfo__t304_ms200	= 3,
	MobilityControlInfo__t304_ms500	= 4,
	MobilityControlInfo__t304_ms1000	= 5,
	MobilityControlInfo__t304_ms2000	= 6,
	MobilityControlInfo__t304_spare1	= 7
} e_MobilityControlInfo__t304;

/* Forward declarations */
struct CarrierFreqEUTRA;
struct CarrierBandwidthEUTRA;
struct RACH_ConfigDedicated;

/* MobilityControlInfo */
typedef struct MobilityControlInfo {
	PhysCellId_t	 targetPhysCellId;
	struct CarrierFreqEUTRA	*carrierFreq	/* OPTIONAL */;
	struct CarrierBandwidthEUTRA	*carrierBandwidth	/* OPTIONAL */;
	AdditionalSpectrumEmission_t	*additionalSpectrumEmission	/* OPTIONAL */;
	long	 t304;
	C_RNTI_t	 newUE_Identity;
	RadioResourceConfigCommon_t	 radioResourceConfigCommon;
	struct RACH_ConfigDedicated	*rach_ConfigDedicated	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MobilityControlInfo_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_t304_6;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_MobilityControlInfo;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CarrierFreqEUTRA.h"
#include "CarrierBandwidthEUTRA.h"
#include "RACH-ConfigDedicated.h"

#endif	/* _MobilityControlInfo_H_ */
#include <asn_internal.h>
