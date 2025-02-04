/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_RedirectedCarrierInfo_H_
#define	_RedirectedCarrierInfo_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ARFCN-ValueEUTRA.h"
#include "CarrierFreqsGERAN.h"
#include "ARFCN-ValueUTRA.h"
#include "CarrierFreqCDMA2000.h"
#include "CarrierFreqListUTRA-TDD-r10.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum RedirectedCarrierInfo_PR {
	RedirectedCarrierInfo_PR_NOTHING,	/* No components present */
	RedirectedCarrierInfo_PR_eutra,
	RedirectedCarrierInfo_PR_geran,
	RedirectedCarrierInfo_PR_utra_FDD,
	RedirectedCarrierInfo_PR_utra_TDD,
	RedirectedCarrierInfo_PR_cdma2000_HRPD,
	RedirectedCarrierInfo_PR_cdma2000_1xRTT,
	/* Extensions may appear below */
	RedirectedCarrierInfo_PR_utra_TDD_r10
} RedirectedCarrierInfo_PR;

/* RedirectedCarrierInfo */
typedef struct RedirectedCarrierInfo {
	RedirectedCarrierInfo_PR present;
	union RedirectedCarrierInfo_u {
		ARFCN_ValueEUTRA_t	 eutra;
		CarrierFreqsGERAN_t	 geran;
		ARFCN_ValueUTRA_t	 utra_FDD;
		ARFCN_ValueUTRA_t	 utra_TDD;
		CarrierFreqCDMA2000_t	 cdma2000_HRPD;
		CarrierFreqCDMA2000_t	 cdma2000_1xRTT;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		CarrierFreqListUTRA_TDD_r10_t	 utra_TDD_r10;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RedirectedCarrierInfo_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RedirectedCarrierInfo;

#ifdef __cplusplus
}
#endif

#endif	/* _RedirectedCarrierInfo_H_ */
#include <asn_internal.h>
