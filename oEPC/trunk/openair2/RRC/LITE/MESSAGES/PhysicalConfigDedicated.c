/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "EUTRA-RRC-Definitions"
 * 	found in "/home/nfvepc/oEPC/trunk/openair2/RRC/LITE/MESSAGES/asn1c/ASN1_files/EUTRA-RRC-Definitions.asn"
 * 	`asn1c -gen-PER -fcompound-names -fnative-types`
 */

#include "PhysicalConfigDedicated.h"

static asn_per_constraints_t asn_PER_type_antennaInfo_constr_10 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_per_constraints_t asn_PER_type_antennaInfo_r10_constr_17 GCC_NOTUSED = {
	{ APC_CONSTRAINED,	 1,  1,  0,  1 }	/* (0..1) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_TYPE_member_t asn_MBR_antennaInfo_10[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PhysicalConfigDedicated__antennaInfo, choice.explicitValue),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AntennaInfoDedicated,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"explicitValue"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PhysicalConfigDedicated__antennaInfo, choice.defaultValue),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"defaultValue"
		},
};
static asn_TYPE_tag2member_t asn_MAP_antennaInfo_tag2el_10[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* explicitValue at 2166 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* defaultValue at 2167 */
};
static asn_CHOICE_specifics_t asn_SPC_antennaInfo_specs_10 = {
	sizeof(struct PhysicalConfigDedicated__antennaInfo),
	offsetof(struct PhysicalConfigDedicated__antennaInfo, _asn_ctx),
	offsetof(struct PhysicalConfigDedicated__antennaInfo, present),
	sizeof(((struct PhysicalConfigDedicated__antennaInfo *)0)->present),
	asn_MAP_antennaInfo_tag2el_10,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_antennaInfo_10 = {
	"antennaInfo",
	"antennaInfo",
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
	&asn_PER_type_antennaInfo_constr_10,
	asn_MBR_antennaInfo_10,
	2,	/* Elements count */
	&asn_SPC_antennaInfo_specs_10	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_antennaInfo_r10_17[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct PhysicalConfigDedicated__antennaInfo_r10, choice.explicitValue_r10),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AntennaInfoDedicated_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"explicitValue-r10"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct PhysicalConfigDedicated__antennaInfo_r10, choice.defaultValue),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NULL,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"defaultValue"
		},
};
static asn_TYPE_tag2member_t asn_MAP_antennaInfo_r10_tag2el_17[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* explicitValue-r10 at 2175 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 } /* defaultValue at 2176 */
};
static asn_CHOICE_specifics_t asn_SPC_antennaInfo_r10_specs_17 = {
	sizeof(struct PhysicalConfigDedicated__antennaInfo_r10),
	offsetof(struct PhysicalConfigDedicated__antennaInfo_r10, _asn_ctx),
	offsetof(struct PhysicalConfigDedicated__antennaInfo_r10, present),
	sizeof(((struct PhysicalConfigDedicated__antennaInfo_r10 *)0)->present),
	asn_MAP_antennaInfo_r10_tag2el_17,
	2,	/* Count of tags in the map */
	0,
	-1	/* Extensions start */
};
static /* Use -fall-defs-global to expose */
asn_TYPE_descriptor_t asn_DEF_antennaInfo_r10_17 = {
	"antennaInfo-r10",
	"antennaInfo-r10",
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
	&asn_PER_type_antennaInfo_r10_constr_17,
	asn_MBR_antennaInfo_r10_17,
	2,	/* Elements count */
	&asn_SPC_antennaInfo_r10_specs_17	/* Additional specs */
};

static asn_TYPE_member_t asn_MBR_PhysicalConfigDedicated_1[] = {
	{ ATF_POINTER, 23, offsetof(struct PhysicalConfigDedicated, pdsch_ConfigDedicated),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PDSCH_ConfigDedicated,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pdsch-ConfigDedicated"
		},
	{ ATF_POINTER, 22, offsetof(struct PhysicalConfigDedicated, pucch_ConfigDedicated),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_ConfigDedicated,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pucch-ConfigDedicated"
		},
	{ ATF_POINTER, 21, offsetof(struct PhysicalConfigDedicated, pusch_ConfigDedicated),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUSCH_ConfigDedicated,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pusch-ConfigDedicated"
		},
	{ ATF_POINTER, 20, offsetof(struct PhysicalConfigDedicated, uplinkPowerControlDedicated),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UplinkPowerControlDedicated,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"uplinkPowerControlDedicated"
		},
	{ ATF_POINTER, 19, offsetof(struct PhysicalConfigDedicated, tpc_PDCCH_ConfigPUCCH),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_TPC_PDCCH_Config,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tpc-PDCCH-ConfigPUCCH"
		},
	{ ATF_POINTER, 18, offsetof(struct PhysicalConfigDedicated, tpc_PDCCH_ConfigPUSCH),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_TPC_PDCCH_Config,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"tpc-PDCCH-ConfigPUSCH"
		},
	{ ATF_POINTER, 17, offsetof(struct PhysicalConfigDedicated, cqi_ReportConfig),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CQI_ReportConfig,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"cqi-ReportConfig"
		},
	{ ATF_POINTER, 16, offsetof(struct PhysicalConfigDedicated, soundingRS_UL_ConfigDedicated),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SoundingRS_UL_ConfigDedicated,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"soundingRS-UL-ConfigDedicated"
		},
	{ ATF_POINTER, 15, offsetof(struct PhysicalConfigDedicated, antennaInfo),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_antennaInfo_10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"antennaInfo"
		},
	{ ATF_POINTER, 14, offsetof(struct PhysicalConfigDedicated, schedulingRequestConfig),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SchedulingRequestConfig,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"schedulingRequestConfig"
		},
	{ ATF_POINTER, 13, offsetof(struct PhysicalConfigDedicated, cqi_ReportConfig_v920),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CQI_ReportConfig_v920,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"cqi-ReportConfig-v920"
		},
	{ ATF_POINTER, 12, offsetof(struct PhysicalConfigDedicated, antennaInfo_v920),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AntennaInfoDedicated_v920,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"antennaInfo-v920"
		},
	{ ATF_POINTER, 11, offsetof(struct PhysicalConfigDedicated, antennaInfo_r10),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_antennaInfo_r10_17,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"antennaInfo-r10"
		},
	{ ATF_POINTER, 10, offsetof(struct PhysicalConfigDedicated, antennaInfoUL_r10),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AntennaInfoUL_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"antennaInfoUL-r10"
		},
	{ ATF_POINTER, 9, offsetof(struct PhysicalConfigDedicated, cif_Presence_r10),
		(ASN_TAG_CLASS_CONTEXT | (14 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"cif-Presence-r10"
		},
	{ ATF_POINTER, 8, offsetof(struct PhysicalConfigDedicated, cqi_ReportConfig_r10),
		(ASN_TAG_CLASS_CONTEXT | (15 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CQI_ReportConfig_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"cqi-ReportConfig-r10"
		},
	{ ATF_POINTER, 7, offsetof(struct PhysicalConfigDedicated, csi_RS_Config_r10),
		(ASN_TAG_CLASS_CONTEXT | (16 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CSI_RS_Config_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"csi-RS-Config-r10"
		},
	{ ATF_POINTER, 6, offsetof(struct PhysicalConfigDedicated, pucch_ConfigDedicated_v1020),
		(ASN_TAG_CLASS_CONTEXT | (17 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUCCH_ConfigDedicated_v1020,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pucch-ConfigDedicated-v1020"
		},
	{ ATF_POINTER, 5, offsetof(struct PhysicalConfigDedicated, pusch_ConfigDedicated_v1020),
		(ASN_TAG_CLASS_CONTEXT | (18 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PUSCH_ConfigDedicated_v1020,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"pusch-ConfigDedicated-v1020"
		},
	{ ATF_POINTER, 4, offsetof(struct PhysicalConfigDedicated, schedulingRequestConfig_v1020),
		(ASN_TAG_CLASS_CONTEXT | (19 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SchedulingRequestConfig_v1020,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"schedulingRequestConfig-v1020"
		},
	{ ATF_POINTER, 3, offsetof(struct PhysicalConfigDedicated, soundingRS_UL_ConfigDedicated_v1020),
		(ASN_TAG_CLASS_CONTEXT | (20 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SoundingRS_UL_ConfigDedicated_v1020,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"soundingRS-UL-ConfigDedicated-v1020"
		},
	{ ATF_POINTER, 2, offsetof(struct PhysicalConfigDedicated, soundingRS_UL_ConfigDedicatedAperiodic_r10),
		(ASN_TAG_CLASS_CONTEXT | (21 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_SoundingRS_UL_ConfigDedicatedAperiodic_r10,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"soundingRS-UL-ConfigDedicatedAperiodic-r10"
		},
	{ ATF_POINTER, 1, offsetof(struct PhysicalConfigDedicated, uplinkPowerControlDedicated_v1020),
		(ASN_TAG_CLASS_CONTEXT | (22 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_UplinkPowerControlDedicated_v1020,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"uplinkPowerControlDedicated-v1020"
		},
};
static int asn_MAP_PhysicalConfigDedicated_oms_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };
static ber_tlv_tag_t asn_DEF_PhysicalConfigDedicated_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_PhysicalConfigDedicated_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* pdsch-ConfigDedicated at 2157 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* pucch-ConfigDedicated at 2158 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* pusch-ConfigDedicated at 2159 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* uplinkPowerControlDedicated at 2160 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* tpc-PDCCH-ConfigPUCCH at 2161 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* tpc-PDCCH-ConfigPUSCH at 2162 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* cqi-ReportConfig at 2163 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* soundingRS-UL-ConfigDedicated at 2164 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* antennaInfo at 2166 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* schedulingRequestConfig at 2169 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* cqi-ReportConfig-v920 at 2171 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* antennaInfo-v920 at 2172 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* antennaInfo-r10 at 2175 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 }, /* antennaInfoUL-r10 at 2178 */
    { (ASN_TAG_CLASS_CONTEXT | (14 << 2)), 14, 0, 0 }, /* cif-Presence-r10 at 2179 */
    { (ASN_TAG_CLASS_CONTEXT | (15 << 2)), 15, 0, 0 }, /* cqi-ReportConfig-r10 at 2180 */
    { (ASN_TAG_CLASS_CONTEXT | (16 << 2)), 16, 0, 0 }, /* csi-RS-Config-r10 at 2181 */
    { (ASN_TAG_CLASS_CONTEXT | (17 << 2)), 17, 0, 0 }, /* pucch-ConfigDedicated-v1020 at 2182 */
    { (ASN_TAG_CLASS_CONTEXT | (18 << 2)), 18, 0, 0 }, /* pusch-ConfigDedicated-v1020 at 2183 */
    { (ASN_TAG_CLASS_CONTEXT | (19 << 2)), 19, 0, 0 }, /* schedulingRequestConfig-v1020 at 2184 */
    { (ASN_TAG_CLASS_CONTEXT | (20 << 2)), 20, 0, 0 }, /* soundingRS-UL-ConfigDedicated-v1020 at 2186 */
    { (ASN_TAG_CLASS_CONTEXT | (21 << 2)), 21, 0, 0 }, /* soundingRS-UL-ConfigDedicatedAperiodic-r10 at 2188 */
    { (ASN_TAG_CLASS_CONTEXT | (22 << 2)), 22, 0, 0 } /* uplinkPowerControlDedicated-v1020 at 2189 */
};
static asn_SEQUENCE_specifics_t asn_SPC_PhysicalConfigDedicated_specs_1 = {
	sizeof(struct PhysicalConfigDedicated),
	offsetof(struct PhysicalConfigDedicated, _asn_ctx),
	asn_MAP_PhysicalConfigDedicated_tag2el_1,
	23,	/* Count of tags in the map */
	asn_MAP_PhysicalConfigDedicated_oms_1,	/* Optional members */
	10, 13,	/* Root/Additions */
	9,	/* Start extensions */
	24	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_PhysicalConfigDedicated = {
	"PhysicalConfigDedicated",
	"PhysicalConfigDedicated",
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
	asn_DEF_PhysicalConfigDedicated_tags_1,
	sizeof(asn_DEF_PhysicalConfigDedicated_tags_1)
		/sizeof(asn_DEF_PhysicalConfigDedicated_tags_1[0]), /* 1 */
	asn_DEF_PhysicalConfigDedicated_tags_1,	/* Same as above */
	sizeof(asn_DEF_PhysicalConfigDedicated_tags_1)
		/sizeof(asn_DEF_PhysicalConfigDedicated_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_PhysicalConfigDedicated_1,
	23,	/* Elements count */
	&asn_SPC_PhysicalConfigDedicated_specs_1	/* Additional specs */
};

