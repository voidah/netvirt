/*
 * Generated by asn1c-0.9.26 (http://lionet.info/asn1c)
 * From ASN.1 module "DNDS"
 * 	found in "dnds.asn1"
 * 	`asn1c -fnative-types`
 */

#ifndef	_Node_H_
#define	_Node_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeInteger.h>
#include <PrintableString.h>
#include <BIT_STRING.h>
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Node */
typedef struct Node {
	unsigned long	 contextId;
	PrintableString_t	*description	/* OPTIONAL */;
	PrintableString_t	*uuid	/* OPTIONAL */;
	PrintableString_t	*provCode	/* OPTIONAL */;
	PrintableString_t	*certificate	/* OPTIONAL */;
	BIT_STRING_t	*certificateKey	/* OPTIONAL */;
	PrintableString_t	*trustedCert	/* OPTIONAL */;
	OCTET_STRING_t	*ipAddress	/* OPTIONAL */;
	long	*status	/* OPTIONAL */;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Node_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_contextId_2;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_Node;

#ifdef __cplusplus
}
#endif

#endif	/* _Node_H_ */
#include <asn_internal.h>
