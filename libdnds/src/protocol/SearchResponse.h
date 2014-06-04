/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "DNDS"
 * 	found in "dnds.asn1"
 * 	`asn1c -fnative-types`
 */

#ifndef	_SearchResponse_H_
#define	_SearchResponse_H_


#include <asn_application.h>

/* Including external dependencies */
#include "SearchType.h"
#include "DNDSResult.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct DNDSObject;

/* SearchResponse */
typedef struct SearchResponse {
	SearchType_t	 searchtype;
	DNDSResult_t	 dndsResult;
	struct objects {
		A_SEQUENCE_OF(struct DNDSObject) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} objects;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SearchResponse_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SearchResponse;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "DNDSObject.h"

#endif	/* _SearchResponse_H_ */
#include <asn_internal.h>
