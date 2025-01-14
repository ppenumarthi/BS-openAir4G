/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#ifndef	_CarrierFreqCDMA2000_H_
#define	_CarrierFreqCDMA2000_H_


#include <asn_application.h>

/* Including external dependencies */
#include "BandclassCDMA2000.h"
#include "ARFCN-ValueCDMA2000.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CarrierFreqCDMA2000 */
typedef struct CarrierFreqCDMA2000 {
	BandclassCDMA2000_t	 bandClass;
	ARFCN_ValueCDMA2000_t	 arfcn;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CarrierFreqCDMA2000_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CarrierFreqCDMA2000;

#ifdef __cplusplus
}
#endif

#endif	/* _CarrierFreqCDMA2000_H_ */
#include <asn_internal.h>
