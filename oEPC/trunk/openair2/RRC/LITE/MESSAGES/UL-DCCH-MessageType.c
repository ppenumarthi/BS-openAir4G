/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#include "UL-DCCH-MessageType.h"

static asn_per_constraints_t asn_PER_type_c1_constr_2 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 4,  4,  0,  15 }	/* (0..15) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_type_UL_DCCH_MessageType_constr_1 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_c1_2[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.csfbParametersRequestCDMA2000),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSFBParametersRequestCDMA2000,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"csfbParametersRequestCDMA2000"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.measurementReport),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MeasurementReport,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"measurementReport"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcConnectionReconfigurationComplete),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCConnectionReconfigurationComplete,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rrcConnectionReconfigurationComplete"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcConnectionReestablishmentComplete),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCConnectionReestablishmentComplete,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rrcConnectionReestablishmentComplete"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rrcConnectionSetupComplete),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RRCConnectionSetupComplete,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rrcConnectionSetupComplete"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.securityModeComplete),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityModeComplete,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"securityModeComplete"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.securityModeFailure),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SecurityModeFailure,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"securityModeFailure"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ueCapabilityInformation),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UECapabilityInformation,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ueCapabilityInformation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ulHandoverPreparationTransfer),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ULHandoverPreparationTransfer,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ulHandoverPreparationTransfer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ulInformationTransfer),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ULInformationTransfer,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ulInformationTransfer"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.counterCheckResponse),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CounterCheckResponse,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"counterCheckResponse"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.ueInformationResponse_r9),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UEInformationResponse_r9,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"ueInformationResponse-r9"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.proximityIndication_r9),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_ProximityIndication_r9,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"proximityIndication-r9"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.rnReconfigurationComplete_r10),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_RNReconfigurationComplete_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rnReconfigurationComplete-r10"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.mbmsCountingResponse_r10),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_MBMSCountingResponse_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"mbmsCountingResponse-r10"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType__c1, choice.interFreqRSTDMeasurementIndication_r10),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_InterFreqRSTDMeasurementIndication_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"interFreqRSTDMeasurementIndication-r10"
		},
};
static asn_TYPE_tag2member_t asn_MAP_c1_tag2el_2[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* csfbParametersRequestCDMA2000 at 114 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* measurementReport at 115 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* rrcConnectionReconfigurationComplete at 116 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* rrcConnectionReestablishmentComplete at 117 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* rrcConnectionSetupComplete at 118 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* securityModeComplete at 119 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* securityModeFailure at 120 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* ueCapabilityInformation at 121 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* ulHandoverPreparationTransfer at 122 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* ulInformationTransfer at 123 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* counterCheckResponse at 124 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* ueInformationResponse-r9 at 125 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* proximityIndication-r9 at 126 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* rnReconfigurationComplete-r10 at 127 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* mbmsCountingResponse-r10 at 128 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 } /* interFreqRSTDMeasurementIndication-r10 at 130 */
};
static asn_CHOICE_specifics_t asn_SPC_c1_specs_2 = {
	sizeof(struct UL_DCCH_MessageType__c1),
	offsetof(struct UL_DCCH_MessageType__c1, _asn_ctx),
	offsetof(struct UL_DCCH_MessageType__c1, present),
	sizeof(((struct UL_DCCH_MessageType__c1 *)0)->present),
	asn_MAP_c1_tag2el_2,
	16,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_c1_2 = {
	"c1",
	"c1",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_decode_aper,
	CHOICE_encode_aper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_type_c1_constr_2,
	asn_MBR_c1_2,
	16,	/* Elements count */
	&asn_SPC_c1_specs_2	/* Additional specs */
};

static ber_tlv_tag_t asn_DEF_messageClassExtension_tags_19[] = {
	(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_SEQUENCE_specifics_t asn_SPC_messageClassExtension_specs_19 = {
	sizeof(struct UL_DCCH_MessageType__messageClassExtension),
	offsetof(struct UL_DCCH_MessageType__messageClassExtension, _asn_ctx),
	0,	/* No top level tags */
	0,	/* No tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_messageClassExtension_19 = {
	"messageClassExtension",
	"messageClassExtension",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	SEQUENCE_decode_aper,
	SEQUENCE_encode_aper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_messageClassExtension_tags_19,
	sizeof(asn_DEF_messageClassExtension_tags_19)
		/sizeof(asn_DEF_messageClassExtension_tags_19[0]) - 1, /* 1 */
	asn_DEF_messageClassExtension_tags_19,	/* Same as above */
	sizeof(asn_DEF_messageClassExtension_tags_19)
		/sizeof(asn_DEF_messageClassExtension_tags_19[0]), /* 2 */
	0,	/* No PER visible constraints */
	0, 0,	/* No members */
	&asn_SPC_messageClassExtension_specs_19	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_UL_DCCH_MessageType_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType, choice.c1),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_c1_2,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"c1"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct UL_DCCH_MessageType, choice.messageClassExtension),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		0,
		&asn_DEF_messageClassExtension_19,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"messageClassExtension"
		},
};
static asn_TYPE_tag2member_t asn_MAP_UL_DCCH_MessageType_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* c1 at 114 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* messageClassExtension at 131 */
};
static asn_CHOICE_specifics_t asn_SPC_UL_DCCH_MessageType_specs_1 = {
	sizeof(struct UL_DCCH_MessageType),
	offsetof(struct UL_DCCH_MessageType, _asn_ctx),
	offsetof(struct UL_DCCH_MessageType, present),
	sizeof(((struct UL_DCCH_MessageType *)0)->present),
	asn_MAP_UL_DCCH_MessageType_tag2el_1,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_UL_DCCH_MessageType = {
	"UL-DCCH-MessageType",
	"UL-DCCH-MessageType",
	CHOICE_free,
	CHOICE_print,
	CHOICE_constraint,
	CHOICE_decode_ber,
	CHOICE_encode_der,
	CHOICE_decode_xer,
	CHOICE_encode_xer,
	CHOICE_decode_uper,
	CHOICE_encode_uper,
	CHOICE_decode_aper,
	CHOICE_encode_aper,
	CHOICE_outmost_tag,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	&asn_PER_type_UL_DCCH_MessageType_constr_1,
	asn_MBR_UL_DCCH_MessageType_1,
	2,	/* Elements count */
	&asn_SPC_UL_DCCH_MessageType_specs_1	/* Additional specs */
};

