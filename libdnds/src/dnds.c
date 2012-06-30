/*
 * dnds.c: Dynamic Network Directory Service Protocol API
 *
 * Copyright (C) 2010, 2011 Nicolas Bouliane
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>

#include "asn1/DNDSMessage.h"
#include "dnds.h"

/* TODO
 * check for DNDS_value_not_present (on id ?)
 */

// DNDSMessage
int DNDSMessage_new(DNDSMessage_t **msg)
{
	*msg = calloc(1, sizeof(DNDSMessage_t));

	if (*msg == NULL) {
		return DNDS_alloc_failed;
	}

	(*msg)->version = 1;

	return DNDS_success;
}

int DNDSMessage_del(DNDSMessage_t *msg)
{
	asn_DEF_DNDSMessage.free_struct(&asn_DEF_DNDSMessage, msg, 0);
	return DNDS_success;
}

int DNDSMessage_set_channel(DNDSMessage_t *msg, uint8_t channel)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	msg->channel = channel;

	return DNDS_success;
}

int DNDSMessage_get_channel(DNDSMessage_t *msg, uint8_t *channel)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	*channel = msg->channel;

	return DNDS_success;
}

int DNDSMessage_set_pdu(DNDSMessage_t *msg, pdu_PR pdu)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	msg->pdu.present = pdu;

	return DNDS_success;
}

int DNDSMessage_get_pdu(DNDSMessage_t *msg, pdu_PR *pdu)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	*pdu = msg->pdu.present;

	return DNDS_success;
}

int DNDSMessage_set_ethernet(DNDSMessage_t *msg, uint8_t *frame, size_t length)
{
	msg->pdu.choice.ethernet.buf = frame;
	msg->pdu.choice.ethernet.size = length;

	return 0;
}

int DNDSMessage_get_ethernet(DNDSMessage_t *msg, uint8_t **frame, size_t *length)
{
	*frame = msg->pdu.choice.ethernet.buf;
	*length = msg->pdu.choice.ethernet.size;

	return 0;
}

// DNMessage
int DNMessage_set_seqNumber(DNDSMessage_t *msg, uint32_t seqNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	msg->pdu.choice.dnm.seqNumber = seqNumber;

	return DNDS_success;
}

int DNMessage_get_seqNumber(DNDSMessage_t *msg, uint32_t *seqNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	*seqNumber = msg->pdu.choice.dnm.seqNumber;

	return DNDS_success;
}

int DNMessage_set_ackNumber(DNDSMessage_t *msg, uint32_t ackNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	msg->pdu.choice.dnm.ackNumber = ackNumber;

	return DNDS_success;
}

int DNMessage_get_ackNumber(DNDSMessage_t *msg, uint32_t *ackNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	*ackNumber = msg->pdu.choice.dnm.ackNumber;

	return DNDS_success;
}

int DNMessage_set_operation(DNDSMessage_t *msg, dnop_PR operation)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	msg->pdu.choice.dnm.dnop.present = operation;

	return DNDS_success;
}

int DNMessage_get_operation(DNDSMessage_t *msg, dnop_PR *operation)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	*operation = msg->pdu.choice.dnm.dnop.present;

	return DNDS_success;
}

// DSMessage
int DSMessage_set_seqNumber(DNDSMessage_t *msg, uint32_t seqNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	msg->pdu.choice.dsm.seqNumber = seqNumber;

	return DNDS_success;
}

int DSMessage_get_seqNumber(DNDSMessage_t *msg, uint32_t *seqNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	*seqNumber = msg->pdu.choice.dsm.seqNumber;

	return DNDS_success;
}

int DSMessage_set_ackNumber(DNDSMessage_t *msg, uint32_t ackNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	msg->pdu.choice.dsm.ackNumber = ackNumber;

	return DNDS_success;
}

int DSMessage_get_ackNumber(DNDSMessage_t *msg, uint32_t *ackNumber)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	*ackNumber = msg->pdu.choice.dsm.ackNumber;

	return DNDS_success;
}

int DSMessage_set_operation(DNDSMessage_t *msg, dsop_PR operation)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	msg->pdu.choice.dsm.dsop.present = operation;

	return DNDS_success;
}

int DSMessage_get_operation(DNDSMessage_t *msg, dsop_PR *operation)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	*operation = msg->pdu.choice.dsm.dsop.present;

	return DNDS_success;
}

// DNDSObject
int DNDSObject_set_objectType(DNDSObject_t *object, DNDSObject_PR type)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	object->present = type;

	return DNDS_success;
}

int DNDSObject_get_objectType(DNDSObject_t *object, DNDSObject_PR *objType)
{
	if (object == NULL || objType == NULL) {
		return DNDS_invalid_param;
	}

	*objType = object->present;

	return DNDS_success;
}

// PeerConnectInfo
int PeerConnectInfo_set_certName(DNDSMessage_t *msg, char *name, size_t length)
{
	if (msg == NULL || name == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_peerConnectInfo) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.certName.buf = (uint8_t *)strdup(name);
	msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.certName.size = length;

	return DNDS_success;
}

int PeerConnectInfo_get_certName(DNDSMessage_t *msg, char **name, size_t *length)
{
	if (msg == NULL || name == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_peerConnectInfo) {
		return DNDS_invalid_op;
	}

	*name = (char *)msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.certName.buf;
	*length = msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.certName.size;

	return DNDS_success;
}

int PeerConnectInfo_set_ipAddr(DNDSMessage_t *msg, char *ipAddress)
{
	if (msg == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_peerConnectInfo) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.ipAddr.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.ipAddr.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipAddress, msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.ipAddr.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.ipAddr.size = sizeof(struct in_addr);

	return DNDS_success;
}

int PeerConnectInfo_get_ipAddr(DNDSMessage_t *msg, char *ipAddress)
{
	if (msg == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_peerConnectInfo) {
		return DNDS_invalid_op;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.ipAddr.buf, ipAddress, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int PeerConnectInfo_set_state(DNDSMessage_t *msg, e_ConnectState state)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_peerConnectInfo) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.state = state;

	return DNDS_success;
}

int PeerConnectInfo_get_state(DNDSMessage_t *msg, e_ConnectState *state)
{
	if (msg == NULL || state == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_peerConnectInfo) {
		return DNDS_invalid_op;
	}

	*state = msg->pdu.choice.dsm.dsop.choice.peerConnectInfo.state;

	return DNDS_success;
}

// AddRequest
int AddRequest_set_objectType(DNDSMessage_t *msg, DNDSObject_PR objType, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_addRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.addRequest.present = objType;
	*object = &msg->pdu.choice.dsm.dsop.choice.addRequest;

	return DNDS_success;
}

int AddRequest_get_objectType(DNDSMessage_t *msg, DNDSObject_PR *objType)
{
	if (msg == NULL || objType == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_addRequest) {
		return DNDS_invalid_op;
	}

	*objType = msg->pdu.choice.dsm.dsop.choice.addRequest.present;

	return DNDS_success;
}

int AddRequest_get_object(DNDSMessage_t *msg, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_addRequest) {
		return DNDS_invalid_op;
	}

	*object = &msg->pdu.choice.dsm.dsop.choice.addRequest;

	return DNDS_success;
}

// AddResponse
int AddResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_addResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.addResponse = result;

	return DNDS_success;
}

int AddResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_addResponse) {
		return DNDS_invalid_op;
	}

	*result = msg->pdu.choice.dsm.dsop.choice.addResponse;

	return DNDS_success;
}

// AuthRequest
int AuthRequest_set_certName(DNDSMessage_t *msg, char *certName, size_t length)
{
	if (msg == NULL || certName == NULL) {
		return DNDS_invalid_param;
	}

	switch (msg->pdu.present) {
		case pdu_PR_dsm:

			if (msg->pdu.choice.dsm.dsop.present != dsop_PR_authRequest) {
				return DNDS_invalid_op;
			}

			msg->pdu.choice.dsm.dsop.choice.authRequest.certName.buf = (uint8_t *)strdup(certName);
			msg->pdu.choice.dsm.dsop.choice.authRequest.certName.size = length;
			break;

		case pdu_PR_dnm:

			if (msg->pdu.choice.dnm.dnop.present != dnop_PR_authRequest) {
				return DNDS_invalid_op;
			}

			msg->pdu.choice.dnm.dnop.choice.authRequest.certName.buf = (uint8_t *)strdup(certName);
			msg->pdu.choice.dnm.dnop.choice.authRequest.certName.size = length;
			break;

		default:
			return DNDS_invalid_pdu;
	}

	return DNDS_success;
}

int AuthRequest_get_certName(DNDSMessage_t *msg, char **certName, size_t *length)
{
	if (msg == NULL || certName == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	switch (msg->pdu.present) {
		case pdu_PR_dsm:

			if (msg->pdu.choice.dsm.dsop.present != dsop_PR_authRequest) {
				return DNDS_invalid_op;
			}

			*certName = (char *)msg->pdu.choice.dsm.dsop.choice.authRequest.certName.buf;
			*length = msg->pdu.choice.dsm.dsop.choice.authRequest.certName.size;
			break;

		case pdu_PR_dnm:

			if (msg->pdu.choice.dnm.dnop.present != dnop_PR_authRequest) {
				return DNDS_invalid_op;
			}

			*certName = (char *)msg->pdu.choice.dnm.dnop.choice.authRequest.certName.buf;
			*length = msg->pdu.choice.dnm.dnop.choice.authRequest.certName.size;
			break;

		default:
			return DNDS_invalid_op;
	}

	return DNDS_success;
}

// AuthResponse
int AuthResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	switch (msg->pdu.present) {
		case pdu_PR_dsm:

			if (msg->pdu.choice.dsm.dsop.present != dsop_PR_authResponse) {
				return DNDS_invalid_op;
			}

			msg->pdu.choice.dsm.dsop.choice.authResponse = result;
			break;

		case pdu_PR_dnm:

			if (msg->pdu.choice.dnm.dnop.present != dnop_PR_authResponse) {
				return DNDS_invalid_op;
			}

			msg->pdu.choice.dnm.dnop.choice.authResponse = result;
			break;

		default:
			return DNDS_invalid_pdu;
	}

	return DNDS_success;
}

int AuthResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	switch (msg->pdu.present) {
		case pdu_PR_dsm:

			if (msg->pdu.choice.dsm.dsop.present != dsop_PR_authResponse) {
				return DNDS_invalid_op;
			}

			*result = msg->pdu.choice.dsm.dsop.choice.authResponse;
			break;

		case pdu_PR_dnm:

			if (msg->pdu.choice.dnm.dnop.present != dnop_PR_authResponse) {
				return DNDS_invalid_op;
			}

			*result = msg->pdu.choice.dnm.dnop.choice.authResponse;
			break;

		default:
			return DNDS_invalid_pdu;
	}

	return DNDS_success;
}
// DelRequest
int DelRequest_set_objectType(DNDSMessage_t *msg, DNDSObject_PR objType, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_delRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.delRequest.present = objType;
	*object = &msg->pdu.choice.dsm.dsop.choice.delRequest;

	return DNDS_success;
}

int DelRequest_get_objectType(DNDSMessage_t *msg, DNDSObject_PR *objType)
{
	if (msg == NULL || objType == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_delRequest) {
		return DNDS_invalid_op;
	}

	*objType = msg->pdu.choice.dsm.dsop.choice.delRequest.present;

	return DNDS_success;
}

int DelRequest_get_object(DNDSMessage_t *msg, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_delRequest) {
		return DNDS_invalid_op;
	}

	*object = &msg->pdu.choice.dsm.dsop.choice.addRequest;

	return DNDS_success;
}

// DelResponse
int DelResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_delResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.delResponse = result;

	return DNDS_success;
}

int DelResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_delResponse) {
		return DNDS_invalid_op;
	}

	*result = msg->pdu.choice.dsm.dsop.choice.delResponse;

	return DNDS_success;
}
// ModifyRequest
int ModifyRequest_set_objectType(DNDSMessage_t *msg, DNDSObject_PR objType, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_modifyRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.modifyRequest.present = objType;
	*object = &msg->pdu.choice.dsm.dsop.choice.modifyRequest;

	return DNDS_success;
}

int ModifyRequest_get_objectType(DNDSMessage_t *msg, DNDSObject_PR *objType)
{
	if (msg == NULL || objType == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_modifyRequest) {
		return DNDS_invalid_op;
	}

	*objType = msg->pdu.choice.dsm.dsop.choice.modifyRequest.present;

	return DNDS_success;
}

int ModifyRequest_get_object(DNDSMessage_t *msg, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_modifyRequest) {
		return DNDS_invalid_op;
	}

	*object = &msg->pdu.choice.dsm.dsop.choice.modifyRequest;

	return DNDS_success;
}

// ModifyResponse
int ModifyResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_modifyResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.modifyResponse = result;

	return DNDS_success;
}

int ModifyResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_modifyResponse) {
		return DNDS_invalid_op;
	}

	*result = msg->pdu.choice.dsm.dsop.choice.modifyResponse;

	return DNDS_success;
}

// NetinfoRequest
int NetinfoRequest_set_ipLocal(DNDSMessage_t *msg, char *ipLocal)
{
	if (msg == NULL || ipLocal == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoRequest.ipLocal.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (msg->pdu.choice.dnm.dnop.choice.netinfoRequest.ipLocal.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipLocal, msg->pdu.choice.dnm.dnop.choice.netinfoRequest.ipLocal.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoRequest.ipLocal.size = sizeof(struct in_addr);

	return DNDS_success;
 }

int NetinfoRequest_get_ipLocal(DNDSMessage_t *msg, char *ipLocal)
{
	if (msg == NULL || ipLocal == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoRequest) {
		return DNDS_invalid_op;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, msg->pdu.choice.dnm.dnop.choice.netinfoRequest.ipLocal.buf, ipLocal, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int NetinfoRequest_set_macAddr(DNDSMessage_t *msg, uint8_t *macAddr)
{
	if (msg == NULL || macAddr == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoRequest.macAddr.buf = (uint8_t *)calloc(1, ETHER_ADDR_LEN);
	if (msg->pdu.choice.dnm.dnop.choice.netinfoRequest.macAddr.buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(msg->pdu.choice.dnm.dnop.choice.netinfoRequest.macAddr.buf, macAddr, ETHER_ADDR_LEN);
	msg->pdu.choice.dnm.dnop.choice.netinfoRequest.macAddr.size = ETHER_ADDR_LEN;

	return DNDS_success;
}

int NetinfoRequest_get_macAddr(DNDSMessage_t *msg, uint8_t *macAddr)
{
	if (msg == NULL || macAddr == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoRequest) {
		return DNDS_invalid_op;
	}

	memmove(macAddr, msg->pdu.choice.dnm.dnop.choice.netinfoRequest.macAddr.buf, ETHER_ADDR_LEN);

	return DNDS_success;
}

// NetinfoResponse
int NetinfoResponse_set_ipAddress(DNDSMessage_t *msg, char *ipAddress)
{
	if (msg == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoResponse.ipAddress.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (msg->pdu.choice.dnm.dnop.choice.netinfoResponse.ipAddress.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipAddress, msg->pdu.choice.dnm.dnop.choice.netinfoResponse.ipAddress.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoResponse.ipAddress.size = sizeof(struct in_addr);

	return DNDS_success;
}

int NetinfoResponse_get_ipAddress(DNDSMessage_t *msg, char *ipAddress)
{
	if (msg == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoResponse) {
		return DNDS_invalid_op;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, msg->pdu.choice.dnm.dnop.choice.netinfoResponse.ipAddress.buf, ipAddress, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int NetinfoResponse_set_netmask(DNDSMessage_t *msg, char *netmask)
{
	if (msg == NULL || netmask == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoResponse.netmask.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (msg->pdu.choice.dnm.dnop.choice.netinfoResponse.netmask.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, netmask, msg->pdu.choice.dnm.dnop.choice.netinfoResponse.netmask.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoResponse.netmask.size = sizeof(struct in_addr);

	return DNDS_success;
}

int NetinfoResponse_get_netmask(DNDSMessage_t *msg, char *netmask)
{
	if (msg == NULL || netmask == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoResponse) {
		return DNDS_invalid_op;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, msg->pdu.choice.dnm.dnop.choice.netinfoResponse.netmask.buf, netmask, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int NetinfoResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.netinfoResponse.result = result;

	return DNDS_success;
}

int NetinfoResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_netinfoResponse) {
		return DNDS_invalid_op;
	}

	*result = msg->pdu.choice.dnm.dnop.choice.netinfoResponse.result;

	return DNDS_success;
}

// ProvRequest
int ProvRequest_set_provCode(DNDSMessage_t *msg, char *provCode, size_t length)
{
	if (msg == NULL || provCode == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.provRequest.provCode = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (msg->pdu.choice.dnm.dnop.choice.provRequest.provCode == NULL) {
		return DNDS_alloc_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.provRequest.provCode->buf = (uint8_t *)strdup(provCode);
	msg->pdu.choice.dnm.dnop.choice.provRequest.provCode->size = length;

	return DNDS_success;
}

int ProvRequest_get_provCode(DNDSMessage_t *msg, char **provCode, size_t *length)
{
	if (msg == NULL || provCode == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provRequest) {
		return DNDS_invalid_op;
	}

	if (msg->pdu.choice.dnm.dnop.choice.provRequest.provCode == NULL) {
		return DNDS_value_not_present;
	}

	*provCode = (char *)msg->pdu.choice.dnm.dnop.choice.provRequest.provCode->buf;
	*length = msg->pdu.choice.dnm.dnop.choice.provRequest.provCode->size;

	return DNDS_success;
}

// ProvResponse
int ProvResponse_set_certificate(DNDSMessage_t *msg, char *certificate, size_t length)
{
	if (msg == NULL || certificate == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.provResponse.certificate = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (msg->pdu.choice.dnm.dnop.choice.provResponse.certificate == NULL) {
		return DNDS_alloc_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.provResponse.certificate->buf = (uint8_t *)strdup(certificate);
	msg->pdu.choice.dnm.dnop.choice.provResponse.certificate->size = length;

	return DNDS_success;
}

int ProvResponse_get_certificate(DNDSMessage_t *msg, char **certificate, size_t *length)
{
	if (msg == NULL || certificate == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

        if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provResponse) {
                return DNDS_invalid_op;
        }

	if (msg->pdu.choice.dnm.dnop.choice.provResponse.certificate == NULL) {
		return DNDS_value_not_present;
	}

	*certificate = (char *)msg->pdu.choice.dnm.dnop.choice.provResponse.certificate->buf;
	*length = msg->pdu.choice.dnm.dnop.choice.provResponse.certificate->size;

	return DNDS_success;
}

int ProvResponse_set_certificateKey(DNDSMessage_t *msg, char *certificateKey, size_t length)
{
	if (msg == NULL || certificateKey == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey == NULL) {
		return DNDS_alloc_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey->buf = (uint8_t *)strdup(certificateKey);
	msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey->size = length;

	return DNDS_success;
}

int ProvResponse_get_certificateKey(DNDSMessage_t *msg, char **certificateKey, size_t *length)
{
	if (msg == NULL || certificateKey == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

        if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provResponse) {
                return DNDS_invalid_op;
        }

	if (msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey == NULL) {
		return DNDS_value_not_present;
	}

	*certificateKey = (char *)msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey->buf;
	*length = msg->pdu.choice.dnm.dnop.choice.provResponse.certificateKey->size;

	return DNDS_success;
}

int ProvResponse_set_trustedCert(DNDSMessage_t *msg, char *trustedCert, size_t length)
{
	if (msg == NULL || trustedCert == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert == NULL) {
		return DNDS_alloc_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert->buf = (uint8_t *)strdup(trustedCert);
	msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert->size = length;

	return DNDS_success;

}

int ProvResponse_get_trustedCert(DNDSMessage_t *msg, char **trustedCert, size_t *length)
{
	if (msg == NULL || trustedCert == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

        if (msg->pdu.choice.dnm.dnop.present != dnop_PR_provResponse) {
                return DNDS_invalid_op;
        }

	if (msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert == NULL) {
		return DNDS_value_not_present;
	}

	*trustedCert = (char *)msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert->buf;
	*length = msg->pdu.choice.dnm.dnop.choice.provResponse.trustedCert->size;

	return DNDS_success;
}

// P2pRequest
int P2pRequest_set_macAddrDst(DNDSMessage_t *msg, uint8_t *macAddrDst)
{
	if (msg == NULL || macAddrDst == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pRequest.macAddrDst.buf = (uint8_t *)calloc(1, ETHER_ADDR_LEN);
	if (msg->pdu.choice.dnm.dnop.choice.p2pRequest.macAddrDst.buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(msg->pdu.choice.dnm.dnop.choice.p2pRequest.macAddrDst.buf, macAddrDst, ETHER_ADDR_LEN);
	msg->pdu.choice.dnm.dnop.choice.p2pRequest.macAddrDst.size = ETHER_ADDR_LEN;

	return DNDS_success;
}

int P2pRequest_get_macAddrDst(DNDSMessage_t *msg, uint8_t *macAddrDst)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	memmove(macAddrDst, msg->pdu.choice.dnm.dnop.choice.p2pRequest.macAddrDst.buf, ETHER_ADDR_LEN);

	return DNDS_success;
}

int P2pRequest_set_ipAddrDst(DNDSMessage_t *msg, char *ipAddrDst)
{
	if (msg == NULL || ipAddrDst == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pRequest.ipAddrDst.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (msg->pdu.choice.dnm.dnop.choice.p2pRequest.ipAddrDst.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipAddrDst, msg->pdu.choice.dnm.dnop.choice.p2pRequest.ipAddrDst.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pRequest.ipAddrDst.size = sizeof(struct in_addr);

	return DNDS_success;
}

int P2pRequest_get_ipAddrDst(DNDSMessage_t *msg, char *ipAddrDst)
{
	if (msg == NULL || ipAddrDst == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, msg->pdu.choice.dnm.dnop.choice.p2pRequest.ipAddrDst.buf, ipAddrDst, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int P2pRequest_set_port(DNDSMessage_t *msg, uint32_t port)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pRequest.port = port;

	return DNDS_success;
}

int P2pRequest_get_port(DNDSMessage_t *msg, uint32_t *port)
{
	if (msg == NULL || port == NULL) {
		return DNDS_invalid_param;
	}

	*port = msg->pdu.choice.dnm.dnop.choice.p2pRequest.port;

	return DNDS_success;
}

int P2pRequest_set_side(DNDSMessage_t *msg, e_P2pSide side)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pRequest.side = side;

	return DNDS_success;
}

int P2pRequest_get_side(DNDSMessage_t *msg, e_P2pSide *side)
{
	if (msg == NULL || side == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pRequest) {
		return DNDS_invalid_op;
	}

	*side = msg->pdu.choice.dnm.dnop.choice.p2pRequest.side;

	return DNDS_success;
}

// P2pResponse
int P2pResponse_set_macAddrDst(DNDSMessage_t *msg, uint8_t *macAddrDst)
{
	if (msg == NULL || macAddrDst == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pResponse.macAddrDst.buf = (uint8_t *)calloc(1, ETHER_ADDR_LEN);
	if (msg->pdu.choice.dnm.dnop.choice.p2pResponse.macAddrDst.buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(msg->pdu.choice.dnm.dnop.choice.p2pResponse.macAddrDst.buf, macAddrDst, ETHER_ADDR_LEN);
	msg->pdu.choice.dnm.dnop.choice.p2pResponse.macAddrDst.size = ETHER_ADDR_LEN;

	return DNDS_success;
}

int P2pResponse_get_macAddrDst(DNDSMessage_t *msg, uint8_t *macAddrDst)
{
	if (msg == NULL || macAddrDst == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pResponse) {
		return DNDS_invalid_op;
	}

	memmove(macAddrDst, msg->pdu.choice.dnm.dnop.choice.p2pResponse.macAddrDst.buf, ETHER_ADDR_LEN);

	return DNDS_success;
}

int P2pResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dnm.dnop.choice.p2pResponse.result = result;

	return DNDS_success;
}

int P2pResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dnm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dnm.dnop.present != dnop_PR_p2pResponse) {
		return DNDS_invalid_op;
	}

	*result = msg->pdu.choice.dnm.dnop.choice.p2pResponse.result;

	return DNDS_success;
}
// SearchRequest
int SearchRequest_set_searchType(DNDSMessage_t *msg, e_SearchType SearchType)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.searchRequest.searchtype = SearchType;

	return DNDS_success;
}

int SearchRequest_get_searchType(DNDSMessage_t *msg, e_SearchType *SearchType)
{
	if (msg == NULL || SearchType == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	*SearchType = msg->pdu.choice.dsm.dsop.choice.searchRequest.searchtype;

	return DNDS_success;
}

int SearchRequest_set_objectName(DNDSMessage_t *msg, e_ObjectName ObjectName)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.searchRequest.objectname = (ObjectName_t *)calloc(1, sizeof(ObjectName_t));
	if (msg->pdu.choice.dsm.dsop.choice.searchRequest.objectname == NULL) {
		return DNDS_alloc_failed;
	}

	*msg->pdu.choice.dsm.dsop.choice.searchRequest.objectname = (ObjectName_t)ObjectName;

	return DNDS_success;
}

int SearchRequest_get_objectName(DNDSMessage_t *msg, e_ObjectName *ObjectName)
{
	if (msg == NULL || ObjectName == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	if (msg->pdu.choice.dsm.dsop.choice.searchRequest.objectname == NULL) {
		*ObjectName = 0;
	}
	else {
		*ObjectName = (e_ObjectName)*msg->pdu.choice.dsm.dsop.choice.searchRequest.objectname;
	}

	return DNDS_success;
}

int SearchRequest_set_object(DNDSMessage_t *msg, DNDSObject_t *object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.searchRequest.object = object;

	return DNDS_success;
}

int SearchRequest_get_object(DNDSMessage_t *msg, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	*object = msg->pdu.choice.dsm.dsop.choice.searchRequest.object;

	return DNDS_success;
}

#if 0
int SearchRequest_set_objectType(DNDSMessage_t *msg, DNDSObject_PR objType, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	//msg->pdu.choice.dsm.dsop.choice.searchRequest.object.present = objType;
	//*object = &msg->pdu.choice.dsm.dsop.choice.searchRequest.object;

	return DNDS_success;
}

int SearchRequest_get_objectType(DNDSMessage_t *msg, DNDSObject_PR *objType)
{
	if (msg == NULL || objType == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	//*objType = msg->pdu.choice.dsm.dsop.choice.searchRequest.object.present;

	return DNDS_success;
}

int SearchRequest_get_object(DNDSMessage_t *msg, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchRequest) {
		return DNDS_invalid_op;
	}

	*object = &msg->pdu.choice.dsm.dsop.choice.searchRequest.object;

	return DNDS_success;
}
#endif

// SearchResponse

int SearchResponse_set_searchType(DNDSMessage_t *msg, e_SearchType SearchType)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.searchResponse.searchtype = SearchType;

	return DNDS_success;
}

int SearchResponse_get_searchType(DNDSMessage_t *msg, e_SearchType *SearchType)
{
	if (msg == NULL || SearchType == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	*SearchType = msg->pdu.choice.dsm.dsop.choice.searchResponse.searchtype;

	return DNDS_success;
}

int SearchResponse_set_result(DNDSMessage_t *msg, e_DNDSResult result)
{
	if (msg == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	msg->pdu.choice.dsm.dsop.choice.searchResponse.dndsResult = result;

	return DNDS_success;
}

int SearchResponse_get_result(DNDSMessage_t *msg, e_DNDSResult *result)
{
	if (msg == NULL || result == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	*result = msg->pdu.choice.dsm.dsop.choice.searchResponse.dndsResult;

	return DNDS_success;
}
int SearchResponse_add_object(DNDSMessage_t *msg, DNDSObject_t *object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	asn_set_add(&msg->pdu.choice.dsm.dsop.choice.searchResponse.objects.list, object);

	return DNDS_success;
}

int SearchResponse_get_object(DNDSMessage_t *msg, DNDSObject_t **object)
{
	if (msg == NULL || object == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	int count = msg->pdu.choice.dsm.dsop.choice.searchResponse.objects.list.count;
	if (count > 0) {

		*object = msg->pdu.choice.dsm.dsop.choice.searchResponse.objects.list.array[count-1];
		asn_set_del(&msg->pdu.choice.dsm.dsop.choice.searchResponse.objects.list, count-1, 0);
	}
	else {
		return DNDS_value_not_present;
	}

	return DNDS_success;
}

int SearchResponse_get_object_count(DNDSMessage_t *msg, uint32_t *count)
{
	if (msg == NULL || count == NULL) {
		return DNDS_invalid_param;
	}

	if (msg->pdu.present != pdu_PR_dsm) {
		return DNDS_invalid_pdu;
	}

	if (msg->pdu.choice.dsm.dsop.present != dsop_PR_searchResponse) {
		return DNDS_invalid_op;
	}

	*count = msg->pdu.choice.dsm.dsop.choice.searchResponse.objects.list.count;

	return DNDS_success;
}

// terminateRequest -- need nothing

// DNDS Objects
int DNDSObject_new(DNDSObject_t **object)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	*object = calloc(1, sizeof(DNDSObject_t));

	if (*object == NULL) {
		return DNDS_alloc_failed;
	}

	return DNDS_success;
}

// Acl
int Acl_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_acl) {
		return DNDS_invalid_object_type;
	}

	object->choice.acl.id = id;

	return DNDS_success;
}

int Acl_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_acl) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.acl.id;

	return DNDS_success;
}

int Acl_set_contextId(DNDSObject_t *object, uint32_t contextId)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_acl) {
		return DNDS_invalid_object_type;
	}

	object->choice.acl.contextId = contextId;

	return DNDS_success;
}

int Acl_get_contextId(DNDSObject_t *object, uint32_t *contextId)
{
	if (object == NULL || contextId == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_acl) {
		return DNDS_invalid_object_type;
	}

	*contextId = object->choice.acl.contextId;

	return DNDS_success;
}

int Acl_set_description(DNDSObject_t *object, char *description, size_t length)
{
	if (object == NULL || description == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_acl) {
		return DNDS_invalid_object_type;
	}

	object->choice.acl.description = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.acl.description == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.acl.description->buf = (uint8_t *)strdup(description);
	object->choice.acl.description->size = length;

	return DNDS_success;
}

int Acl_get_description(DNDSObject_t *object, char **description, size_t *length)
{
	if (object == NULL || description == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_acl) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.acl.description == NULL) {
		return DNDS_value_not_present;
	}

	if (object->choice.acl.description == NULL) {
		return DNDS_value_not_present;
	}

	*description = (char *)object->choice.acl.description->buf;
	*length = object->choice.acl.description->size;

	return DNDS_success;
}

// AclGroup
int AclGroup_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	object->choice.aclgroup.id = id;

	return DNDS_success;
}

int AclGroup_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.aclgroup.id;

	return DNDS_success;
}

int AclGroup_set_contextId(DNDSObject_t *object, uint32_t contextId)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	object->choice.aclgroup.contextId = contextId;

	return DNDS_success;
}

int AclGroup_get_contextId(DNDSObject_t *object, uint32_t *contextId)
{
	if (object == NULL || contextId == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	*contextId = object->choice.aclgroup.contextId;

	return DNDS_success;
}

int AclGroup_set_name(DNDSObject_t *object, char *name, size_t length)
{
	if (object == NULL || name == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	object->choice.aclgroup.name = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.aclgroup.name == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.aclgroup.name->buf = (uint8_t *)strdup(name);
	object->choice.aclgroup.name->size = length;

	return DNDS_success;
}

int AclGroup_get_name(DNDSObject_t *object, char **name, size_t *length)
{
	if (object == NULL || name == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.aclgroup.name == NULL) {
		return DNDS_value_not_present;
	}

	*name = (char *)object->choice.aclgroup.name->buf;
	*length = object->choice.aclgroup.name->size;

	return DNDS_success;
}

int AclGroup_set_description(DNDSObject_t *object, char *description, size_t length)
{
	if (object == NULL || description == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	object->choice.aclgroup.description = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.aclgroup.description == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.aclgroup.description->buf = (uint8_t *)strdup(description);
	object->choice.aclgroup.description->size = length;

	return DNDS_success;
}

int AclGroup_get_description(DNDSObject_t *object, char **description, size_t *length)
{
	if (object == NULL || description == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_aclgroup) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.aclgroup.description == NULL) {
		return DNDS_value_not_present;
	}

	*description = (char *)object->choice.aclgroup.description->buf;
	*length = object->choice.aclgroup.description->size;

	return DNDS_success;
}

// IpPool
int IpPool_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	object->choice.ippool.id = id;

	return DNDS_success;
}

int IpPool_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.ippool.id;

	return DNDS_success;
}

int IpPool_set_ipLocal(DNDSObject_t *object, char *ipLocal)
{
	if (object == NULL || ipLocal == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	object->choice.ippool.ipLocal = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.ippool.ipLocal == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.ippool.ipLocal->buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.ippool.ipLocal == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipLocal, object->choice.ippool.ipLocal->buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.ippool.ipLocal->size = sizeof(struct in_addr);

	return DNDS_success;
}

int IpPool_get_ipLocal(DNDSObject_t *object, char *ipLocal)
{
	if (object == NULL || ipLocal == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.ippool.ipLocal == NULL) {
		return DNDS_value_not_present;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.ippool.ipLocal->buf, ipLocal, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int IpPool_set_ipBegin(DNDSObject_t *object, char *ipBegin)
{
	if (object == NULL || ipBegin == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	object->choice.ippool.ipBegin = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.ippool.ipBegin == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.ippool.ipBegin->buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.ippool.ipBegin->buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipBegin, object->choice.ippool.ipBegin->buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.ippool.ipBegin->size = sizeof(struct in_addr);

	return DNDS_success;
}

int IpPool_get_ipBegin(DNDSObject_t *object, char *ipBegin)
{
	if (object == NULL || ipBegin == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.ippool.ipBegin == NULL) {
		return DNDS_value_not_present;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.ippool.ipBegin->buf, ipBegin, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int IpPool_set_ipEnd(DNDSObject_t *object, char *ipEnd)
{
	if (object == NULL || ipEnd == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	object->choice.ippool.ipEnd = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.ippool.ipEnd == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.ippool.ipEnd->buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.ippool.ipEnd->buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipEnd, object->choice.ippool.ipEnd->buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.ippool.ipEnd->size = sizeof(struct in_addr);

	return DNDS_success;
}

int IpPool_get_ipEnd(DNDSObject_t *object, char *ipEnd)
{
	if (object == NULL || ipEnd == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.ippool.ipEnd == NULL) {
		return DNDS_value_not_present;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.ippool.ipEnd->buf, ipEnd, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int IpPool_set_netmask(DNDSObject_t *object, char *netmask)
{
	if (object == NULL || netmask == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	object->choice.ippool.netmask = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.ippool.netmask == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.ippool.netmask->buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.ippool.netmask->buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, netmask, object->choice.ippool.netmask->buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.ippool.netmask->size = sizeof(struct in_addr);

	return DNDS_success;
}

int IpPool_get_netmask(DNDSObject_t *object, char *netmask)
{
	if (object == NULL || netmask == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_ippool) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.ippool.netmask == NULL) {
		return DNDS_value_not_present;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.ippool.netmask->buf, netmask, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

// Context
int Context_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.id = calloc(1, sizeof(uint32_t));
	if (object->choice.context.id == NULL) {
		return DNDS_alloc_failed;
	}

	*object->choice.context.id = id;

	return DNDS_success;
}

int Context_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.context.id == NULL) {
		return DNDS_value_not_present;
	}

	*id = (uint32_t)*object->choice.context.id;

	return DNDS_success;
}

int Context_set_clientId(DNDSObject_t *object, uint32_t clientId)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_alloc_failed;
	}

	object->choice.context.clientId = clientId;

	return DNDS_success;
}

int Context_get_clientId(DNDSObject_t *object, uint32_t *clientId)
{
	if (object == NULL || clientId == NULL) {
		return DNDS_invalid_param;
	}

	*clientId = (uint32_t)object->choice.context.clientId;

	return DNDS_success;
}

int Context_set_topology(DNDSObject_t *object, e_Topology topology)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.topology = topology;

	return DNDS_success;
}

int Context_get_topology(DNDSObject_t *object, e_Topology *topology)
{
	if (object == NULL || topology == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	*topology = object->choice.context.topology;

	return DNDS_success;
}

int Context_set_description(DNDSObject_t *object, char *description, size_t length)
{
	if (object == NULL || description == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.description = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.context.description == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.context.description->buf = (uint8_t *)strdup(description);
	object->choice.context.description->size = length;

	return DNDS_success;
}

int Context_get_description(DNDSObject_t *object, char **description, size_t *length)
{
	if (object == NULL || description == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.context.description == NULL) {
		return DNDS_value_not_present;
	}

	*description = (char *)object->choice.context.description->buf;
	*length = object->choice.context.description->size;

	return DNDS_success;
}

int Context_set_network(DNDSObject_t *object, char *network)
{
	if (object == NULL || network == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.network.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.context.network.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, network, object->choice.context.network.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.context.network.size = sizeof(struct in_addr);

	return DNDS_success;
}

int Context_get_network(DNDSObject_t *object, char *network)
{
	if (object == NULL || network == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.context.network.buf, network, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int Context_set_netmask(DNDSObject_t *object, char *netmask)
{
	if (object == NULL || netmask == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.netmask.buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.context.netmask.buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, netmask, object->choice.context.netmask.buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.context.netmask.size = sizeof(struct in_addr);

	return DNDS_success;
}

int Context_get_netmask(DNDSObject_t *object, char *netmask)
{
	if (object == NULL || netmask == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.context.netmask.buf, netmask, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int Context_set_serverCert(DNDSObject_t *object, char *serverCert, size_t length)
{
	if (object == NULL || serverCert == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.serverCert = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.context.serverCert == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.context.serverCert->buf = (uint8_t *)strdup(serverCert);
	object->choice.context.serverCert->size = length;

	return DNDS_success;
}

int Context_get_serverCert(DNDSObject_t *object, char **serverCert, size_t *length)
{
	if (object == NULL || serverCert == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.context.serverCert == NULL) {
		return DNDS_value_not_present;
	}

	if (object->choice.context.serverCert == NULL) {
		return DNDS_value_not_present;
	}

	*serverCert = (char *)object->choice.context.serverCert->buf;
	*length = object->choice.context.serverCert->size;

	return DNDS_success;
}

int Context_set_serverPrivkey(DNDSObject_t *object, char *serverPrivkey, size_t length)
{
	if (object == NULL || serverPrivkey == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.serverPrivkey = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.context.serverPrivkey == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.context.serverPrivkey->buf = (uint8_t *)strdup(serverPrivkey);
	object->choice.context.serverPrivkey->size = length;

	return DNDS_success;
}

int Context_get_serverPrivkey(DNDSObject_t *object, char **serverPrivkey, size_t *length)
{
	if (object == NULL || serverPrivkey == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.context.serverPrivkey == NULL) {
		return DNDS_value_not_present;
	}

	if (object->choice.context.serverPrivkey == NULL) {
		return DNDS_value_not_present;
	}

	*serverPrivkey = (char *)object->choice.context.serverPrivkey->buf;
	*length = object->choice.context.serverPrivkey->size;

	return DNDS_success;
}

int Context_set_trustedCert(DNDSObject_t *object, char *trustedCert, size_t length)
{
	if (object == NULL || trustedCert == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	object->choice.context.trustedCert = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.context.trustedCert == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.context.trustedCert->buf = (uint8_t *)strdup(trustedCert);
	object->choice.context.trustedCert->size = length;

	return DNDS_success;
}

int Context_get_trustedCert(DNDSObject_t *object, char **trustedCert, size_t *length)
{
	if (object == NULL || trustedCert == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_context) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.context.trustedCert == NULL) {
		return DNDS_value_not_present;
	}

	*trustedCert = (char *)object->choice.context.trustedCert->buf;
	*length = object->choice.context.trustedCert->size;

	return DNDS_success;
}

// Host
int Host_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.id = id;

	return DNDS_success;
}

int Host_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.host.id;

	return DNDS_success;
}

int Host_set_contextId(DNDSObject_t *object, uint32_t contextId)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.contextId = contextId;

	return DNDS_success;
}

int Host_get_contextId(DNDSObject_t *object, uint32_t *contextId)
{
	if (object == NULL || contextId == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	*contextId = object->choice.host.contextId;

	return DNDS_success;
}

int Host_set_peerId(DNDSObject_t *object, uint32_t peerId)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.peerId = peerId;

	return DNDS_success;
}

int Host_get_peerId(DNDSObject_t *object, uint32_t *peerId)
{
	if (object == NULL || peerId == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	*peerId = object->choice.host.peerId;

	return DNDS_success;
}

int Host_set_name(DNDSObject_t *object, char *name, size_t length)
{
	if (object == NULL || name == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.name = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.host.name == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.host.name->buf = (uint8_t *)strdup(name);
	object->choice.host.name->size = length;

	return DNDS_success;
}

int Host_get_name(DNDSObject_t *object, char **name, size_t *length)
{
	if (object == NULL || name == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.host.name == NULL) {
		return DNDS_value_not_present;
	}

	*name = (char *)object->choice.host.name->buf;
	*length = object->choice.host.name->size;

	return DNDS_success;
}

int Host_set_macAddress(DNDSObject_t *object, uint8_t *macAddress)
{
	if (object == NULL || macAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.macAddress = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.host.macAddress == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.host.macAddress->buf = (uint8_t *)calloc(1, ETHER_ADDR_LEN);
	if (object->choice.host.macAddress->buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(object->choice.host.macAddress->buf, macAddress, ETHER_ADDR_LEN);
	object->choice.host.macAddress->size = ETHER_ADDR_LEN;

	return DNDS_success;
}

int Host_get_macAddress(DNDSObject_t *object, uint8_t *macAddress)
{
	if (object == NULL || macAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.host.macAddress == NULL) {
		return DNDS_value_not_present;
	}

	memmove(macAddress, object->choice.host.macAddress->buf, ETHER_ADDR_LEN);

	return DNDS_success;
}

int Host_set_ipAddress(DNDSObject_t *object, char *ipAddress)
{
	if (object == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.ipAddress = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.host.ipAddress == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.host.ipAddress->buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.host.ipAddress->buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipAddress, object->choice.host.ipAddress->buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.host.ipAddress->size = sizeof(struct in_addr);

	return DNDS_success;
}

int Host_get_ipAddress(DNDSObject_t *object, char *ipAddress)
{
	if (object == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.host.ipAddress == NULL) {
		return DNDS_value_not_present;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.host.ipAddress->buf, ipAddress, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int Host_set_status(DNDSObject_t *object, uint8_t status)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	object->choice.host.status = (long *)calloc(1, sizeof(long));
	if (object->choice.host.status == NULL) {
		return DNDS_alloc_failed;
	}

	*object->choice.host.status = status;

	return DNDS_success;
}

int Host_get_status(DNDSObject_t *object, uint8_t *status)
{
	if (object == NULL || status == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_host) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.host.status == NULL) {
		return DNDS_value_not_present;
	}

	*status = *object->choice.host.status;

	return DNDS_success;
}

// Node
int Node_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	object->choice.node.id = id;

	return DNDS_success;
}

int Node_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.node.id;

	return DNDS_success;
}

int Node_set_name(DNDSObject_t *object, char *name, size_t length)
{
	if (object == NULL || name == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	object->choice.node.name = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.node.name == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.node.name->buf = (uint8_t *)strdup(name);
	object->choice.node.name->size = length;

	return DNDS_success;
}

int Node_get_name(DNDSObject_t *object, char **name, size_t *length)
{
	if (object == NULL || name == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.node.name == NULL) {
		return DNDS_value_not_present;
	}

	*name = (char *)object->choice.node.name->buf;
	*length = object->choice.node.name->size;

	return DNDS_success;
}

int Node_set_ipAddress(DNDSObject_t *object, char *ipAddress)
{
	if (object == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	object->choice.node.ipAddress = (OCTET_STRING_t *)calloc(1, sizeof(OCTET_STRING_t));
	if (object->choice.node.ipAddress == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.node.ipAddress->buf = (uint8_t *)calloc(1, sizeof(struct in_addr));
	if (object->choice.node.ipAddress->buf == NULL) {
		return DNDS_alloc_failed;
	}

	int ret;
	ret = inet_pton(AF_INET, ipAddress, object->choice.node.ipAddress->buf);
	if (ret != 1) {
		return DNDS_conversion_failed;
	}

	object->choice.node.ipAddress->size = sizeof(struct in_addr);

	return DNDS_success;
}

int Node_get_ipAddress(DNDSObject_t *object, char *ipAddress)
{
	if (object == NULL || ipAddress == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.node.ipAddress == NULL) {
		return DNDS_value_not_present;
	}

	const char *ret;
	ret = inet_ntop(AF_INET, object->choice.node.ipAddress->buf, ipAddress, INET_ADDRSTRLEN);
	if (ret == NULL) {
		return DNDS_conversion_failed;
	}

	return DNDS_success;
}

int Node_set_certificate(DNDSObject_t *object, char *certificate, size_t length)
{
	if (object == NULL || certificate == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	object->choice.node.certificate = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.node.certificate == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.node.certificate->buf = (uint8_t *)strdup(certificate);
	object->choice.node.certificate->size = length;

	return DNDS_success;
}

int Node_get_certificate(DNDSObject_t *object, char **certificate, size_t *length)
{
	if (object == NULL || certificate == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.node.certificate == NULL) {
		return DNDS_value_not_present;
	}

	*certificate = (char *)object->choice.node.certificate->buf;
	*length = object->choice.node.certificate->size;

	return DNDS_success;
}

int Node_set_certificateKey(DNDSObject_t *object, uint8_t *certificateKey, size_t length)
{
	if (object == NULL || certificateKey == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	object->choice.node.certificateKey = (BIT_STRING_t *)calloc(1, sizeof(BIT_STRING_t));
	if (object->choice.node.certificateKey == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.node.certificateKey->buf = (uint8_t *)calloc(1, length);
	if (object->choice.node.certificateKey->buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(object->choice.node.certificateKey->buf, certificateKey, length);
	object->choice.node.certificateKey->size = length;

	return DNDS_success;
}

int Node_get_certificateKey(DNDSObject_t *object, uint8_t **certificateKey, size_t *length)
{
	if (object == NULL || certificateKey == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.node.certificateKey == NULL) {
		return DNDS_value_not_present;
	}

	*certificateKey = object->choice.node.certificateKey->buf;
	*length = object->choice.node.certificateKey->size;

	return DNDS_success;
}

int Node_set_permission()
{
	return 0;
}

int Node_set_status(DNDSObject_t *object, uint8_t status)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	object->choice.node.status = (long *)calloc(1, sizeof(long));
	*object->choice.node.status = status;

	return DNDS_success;
}

int Node_get_status(DNDSObject_t *object, uint8_t *status)
{
	if (object == NULL || status == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_node) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.node.status == NULL) {
		return DNDS_value_not_present;
	}

	*status = *object->choice.node.status;

	return DNDS_success;
}

// Permission
int Permission_set_id()
{
	return 0;
}

int Permission_set_name()
{
	return 0;
}

int Permission_set_matrix()
{
	return 0;
}

// Peer
int Peer_set_contextId(DNDSObject_t *object, uint32_t contextId)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.contextId = contextId;

	return DNDS_success;
}

int Peer_get_contextId(DNDSObject_t *object, uint32_t *contextId)
{
	if (object == NULL || contextId == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	*contextId = object->choice.peer.contextId;

	return DNDS_success;
}

int Peer_set_description(DNDSObject_t *object, char *description, size_t length)
{
	if (object == NULL || description == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.description = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.peer.description == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.peer.description->buf = (uint8_t *)strdup(description);
	object->choice.peer.description->size = length;

	return DNDS_success;
}

int Peer_get_description(DNDSObject_t *object, char **description, size_t *length)
{
	if (object == NULL || description == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.peer.description == NULL) {
		return DNDS_value_not_present;
	}

	if (object->choice.peer.description == NULL) {
		return DNDS_value_not_present;
	}

	*description = (char *)object->choice.peer.description->buf;
	*length = object->choice.peer.description->size;

	return DNDS_success;
}

int Peer_set_provCode(DNDSObject_t *object, char *provCode, size_t length)
{
	if (object == NULL || provCode == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.provCode = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.peer.provCode == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.peer.provCode->buf = (uint8_t *)strdup(provCode);
	object->choice.peer.provCode->size = length;

	return DNDS_success;
}

int Peer_get_provCode(DNDSObject_t *object, char **provCode, size_t *length)
{
	if (object == NULL || provCode == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.peer.provCode == NULL) {
		return DNDS_value_not_present;
	}

	*provCode = object->choice.peer.provCode->buf;
	*length = object->choice.peer.provCode->size;

	return DNDS_success;
}

int Peer_set_certificate(DNDSObject_t *object, char *certificate, size_t length)
{
	if (object == NULL || certificate == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.certificate = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.peer.certificate == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.peer.certificate->buf = (uint8_t *)strdup(certificate);
	object->choice.peer.certificate->size = length;

	return DNDS_success;
}

int Peer_get_certificate(DNDSObject_t *object, char **certificate, size_t *length)
{
	if (object == NULL || certificate == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.peer.certificate == NULL) {
		return DNDS_value_not_present;
	}

	*certificate = (char *)object->choice.peer.certificate->buf;
	*length = object->choice.peer.certificate->size;

	return DNDS_success;
}

int Peer_set_certificateKey(DNDSObject_t *object, uint8_t *certificateKey, size_t length)
{
	if (object == NULL || certificateKey == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.certificateKey = (BIT_STRING_t *)calloc(1, sizeof(BIT_STRING_t));
	if (object->choice.peer.certificateKey == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.peer.certificateKey->buf = (uint8_t *)calloc(1, length);
	if (object->choice.peer.certificateKey->buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(object->choice.peer.certificateKey->buf, certificateKey, length);
	object->choice.peer.certificateKey->size = length;

	return DNDS_success;
}

int Peer_get_certificateKey(DNDSObject_t *object, uint8_t **certificateKey, size_t *length)
{
	if (object == NULL || certificateKey == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.peer.certificateKey == NULL) {
		return	DNDS_value_not_present;
	}

	*certificateKey = object->choice.peer.certificateKey->buf;
	*length = object->choice.peer.certificateKey->size;

	return DNDS_success;
}

int Peer_set_trustedCert(DNDSObject_t *object, uint8_t *trustedCert, size_t length)
{
	if (object == NULL || trustedCert == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.trustedCert = (BIT_STRING_t *)calloc(1, sizeof(BIT_STRING_t));
	if (object->choice.peer.trustedCert == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.peer.trustedCert->buf = (uint8_t *)calloc(1, length);
	if (object->choice.peer.trustedCert->buf == NULL) {
		return DNDS_alloc_failed;
	}

	memmove(object->choice.peer.trustedCert->buf, trustedCert, length);
	object->choice.peer.trustedCert->size = length;

	return DNDS_success;
}

int Peer_get_trustedCert(DNDSObject_t *object, uint8_t **trustedCert, size_t *length)
{
	if (object == NULL || trustedCert == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.peer.trustedCert == NULL) {
		return	DNDS_value_not_present;
	}

	*trustedCert = object->choice.peer.trustedCert->buf;
	*length = object->choice.peer.trustedCert->size;

	return DNDS_success;
}

int Peer_set_status(DNDSObject_t *object, uint8_t status)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	object->choice.peer.status = (long *)calloc(1, sizeof(long));
	if (object->choice.peer.status == NULL) {
		return DNDS_alloc_failed;
	}

	*object->choice.peer.status = status;

	return DNDS_success;
}

int Peer_get_status(DNDSObject_t *object, uint8_t *status)
{
	if (object == NULL || status == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_peer) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.peer.status == NULL) {
		return DNDS_value_not_present;
	}

	*status = *object->choice.peer.status;

	return DNDS_success;
}

// Client
int Client_set_id(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.id = id;

	return DNDS_success;
}

int Client_get_id(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.client.id;

	return DNDS_success;
}

int Client_set_username(DNDSObject_t *object, char *username, size_t length)
{
	if (object == NULL || username == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.username = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.username == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.username->buf = (uint8_t *)strdup(username);
	object->choice.client.username->size = length;

	return DNDS_success;
}

int Client_get_username(DNDSObject_t *object, char **username, size_t *length)
{
	if (object == NULL || username == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.username == NULL) {
		return DNDS_value_not_present;
	}

	*username = (char *)object->choice.client.username->buf;
	*length = object->choice.client.username->size;

	return DNDS_success;
}

int Client_set_password(DNDSObject_t *object, char *password, size_t length)
{
	if (object == NULL || password == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.password = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.password == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.password->buf = (uint8_t *)strdup(password);
	object->choice.client.password->size = length;

	return DNDS_success;
}

int Client_get_password(DNDSObject_t *object, char **password, size_t *length)
{
	if (object == NULL || password == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.password == NULL) {
		return DNDS_value_not_present;
	}

	*password = (char *)object->choice.client.password->buf;
	*length = object->choice.client.password->size;

	return DNDS_success;
}

int Client_set_firstname(DNDSObject_t *object, char *firstname, size_t length)
{
	if (object == NULL || firstname == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.firstname = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.firstname == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.firstname->buf = (uint8_t *)strdup(firstname);
	object->choice.client.firstname->size = length;

	return DNDS_success;
}

int Client_get_firstname(DNDSObject_t *object, char **firstname, size_t *length)
{
	if (object == NULL || firstname == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.firstname == NULL) {
		return DNDS_value_not_present;
	}

	*firstname = (char *)object->choice.client.firstname->buf;
	*length = object->choice.client.firstname->size;

	return DNDS_success;
}

int Client_set_lastname(DNDSObject_t *object, char *lastname, size_t length)
{
	if (object == NULL || lastname == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.lastname = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.lastname == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.lastname->buf = (uint8_t *)strdup(lastname);
	object->choice.client.lastname->size = length;

	return DNDS_success;
}

int Client_get_lastname(DNDSObject_t *object, char **lastname, size_t *length)
{
	if (object == NULL || lastname == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.lastname == NULL) {
		return DNDS_value_not_present;
	}

	*lastname = (char *)object->choice.client.lastname->buf;
	*length = object->choice.client.lastname->size;

	return DNDS_success;
}

int Client_set_email(DNDSObject_t *object, char *email, size_t length)
{
	if (object == NULL || email == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.email = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.email == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.email->buf = (uint8_t *)strdup(email);
	object->choice.client.email->size = length;

	return DNDS_success;
}

int Client_get_email(DNDSObject_t *object, char **email, size_t *length)
{
	if (object == NULL || email == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.email == NULL) {
		return DNDS_value_not_present;
	}

	*email = (char *)object->choice.client.email->buf;
	*length = object->choice.client.email->size;

	return DNDS_success;
}

int Client_set_company(DNDSObject_t *object, char *company, size_t length)
{
	if (object == NULL || company == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.company = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.company == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.company->buf = (uint8_t *)strdup(company);
	object->choice.client.company->size = length;

	return DNDS_success;
}

int Client_get_company(DNDSObject_t *object, char **company, size_t *length)
{
	if (object == NULL || company == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.company == NULL) {
		return DNDS_value_not_present;
	}

	*company = (char *)object->choice.client.company->buf;
	*length = object->choice.client.company->size;

	return DNDS_success;
}

int Client_set_phone(DNDSObject_t *object, char *phone, size_t length)
{
	if (object == NULL || phone == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.phone = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.phone == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.phone->buf = (uint8_t *)strdup(phone);
	object->choice.client.phone->size = length;

	return DNDS_success;
}

int Client_get_phone(DNDSObject_t *object, char **phone, size_t *length)
{
	if (object == NULL || phone == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.phone == NULL) {
		return DNDS_value_not_present;
	}

	*phone = (char *)object->choice.client.phone->buf;
	*length = object->choice.client.phone->size;

	return DNDS_success;
}

int Client_set_country(DNDSObject_t *object, char *country, size_t length)
{
	if (object == NULL || country == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.country = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.country == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.country->buf = (uint8_t *)strdup(country);
	object->choice.client.country->size = length;

	return DNDS_success;
}

int Client_get_country(DNDSObject_t *object, char **country, size_t *length)
{
	if (object == NULL || country == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.country == NULL) {
		return DNDS_value_not_present;
	}

	*country = (char *)object->choice.client.country->buf;
	*length = object->choice.client.country->size;

	return DNDS_success;
}

int Client_set_stateProvince(DNDSObject_t *object, char *stateProvince, size_t length)
{
	if (object == NULL || stateProvince == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.stateProvince = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.stateProvince == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.stateProvince->buf = (uint8_t *)strdup(stateProvince);
	object->choice.client.stateProvince->size = length;

	return DNDS_success;
}

int Client_get_stateProvince(DNDSObject_t *object, char **stateProvince, size_t *length)
{
	if (object == NULL || stateProvince == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.stateProvince == NULL) {
		return DNDS_value_not_present;
	}

	*stateProvince = (char *)object->choice.client.stateProvince->buf;
	*length = object->choice.client.stateProvince->size;

	return DNDS_success;
}

int Client_set_city(DNDSObject_t *object, char *city, size_t length)
{
	if (object == NULL || city == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.city = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.city == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.city->buf = (uint8_t *)strdup(city);
	object->choice.client.city->size = length;

	return DNDS_success;
}

int Client_get_city(DNDSObject_t *object, char **city, size_t *length)
{
	if (object == NULL || city == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.city == NULL) {
		return DNDS_value_not_present;
	}

	*city = (char *)object->choice.client.city->buf;
	*length = object->choice.client.city->size;

	return DNDS_success;
}

int Client_set_postalCode(DNDSObject_t *object, char *postalCode, size_t length)
{
	if (object == NULL || postalCode == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.postalCode = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.client.postalCode == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.client.postalCode->buf = (uint8_t *)strdup(postalCode);
	object->choice.client.postalCode->size = length;

	return DNDS_success;
}

int Client_get_postalCode(DNDSObject_t *object, char **postalCode, size_t *length)
{
	if (object == NULL || postalCode == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.postalCode == NULL) {
		return DNDS_value_not_present;
	}

	*postalCode = (char *)object->choice.client.stateProvince->buf;
	*length = object->choice.client.postalCode->size;

	return DNDS_success;
}

int Client_set_status(DNDSObject_t *object, uint8_t status)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	object->choice.client.status = (long *)calloc(1, sizeof(long));
	if (object->choice.client.status == NULL) {
		return DNDS_alloc_failed;
	}

	*object->choice.client.status = status;

	return DNDS_success;
}

int Client_get_status(DNDSObject_t *object, uint8_t *status)
{
	if (object == NULL || status == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_client) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.client.status == NULL) {
		return DNDS_value_not_present;
	}

	*status = *object->choice.client.status;

	return DNDS_success;
}

// WebCredential
int WebCredential_set_clientId(DNDSObject_t *object, uint32_t id)
{
	if (object == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_webcredential) {
		return DNDS_invalid_object_type;
	}

	object->choice.webcredential.clientId = id;

	return DNDS_success;
}

int WebCredential_get_clientId(DNDSObject_t *object, uint32_t *id)
{
	if (object == NULL || id == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_webcredential) {
		return DNDS_invalid_object_type;
	}

	*id = object->choice.webcredential.clientId;

	return DNDS_success;
}

int WebCredential_set_username(DNDSObject_t *object, char *username, size_t length)
{
	if (object == NULL || username == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_webcredential) {
		return DNDS_invalid_object_type;
	}

	object->choice.webcredential.username = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.webcredential.username == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.webcredential.username->buf = (uint8_t *)strdup(username);
	object->choice.webcredential.username->size = length;

	return DNDS_success;
}
int WebCredential_get_username(DNDSObject_t *object, char **username, size_t *length)
{
	if (object == NULL || username == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_webcredential) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.webcredential.username == NULL) {
		return DNDS_value_not_present;
	}

	*username = (char *)object->choice.webcredential.username->buf;
	*length = object->choice.webcredential.username->size;

	return DNDS_success;
}

int WebCredential_set_password(DNDSObject_t *object, char *password, size_t length)
{
	if (object == NULL || password == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_webcredential) {
		return DNDS_invalid_object_type;
	}

	object->choice.webcredential.password = (PrintableString_t *)calloc(1, sizeof(PrintableString_t));
	if (object->choice.webcredential.password == NULL) {
		return DNDS_alloc_failed;
	}

	object->choice.webcredential.password->buf = (uint8_t *)strdup(password);
	object->choice.webcredential.password->size = length;

	return DNDS_success;
}

int WebCredential_get_password(DNDSObject_t *object, char **password, size_t *length)
{
	if (object == NULL || password == NULL || length == NULL) {
		return DNDS_invalid_param;
	}

	if (object->present != DNDSObject_PR_webcredential) {
		return DNDS_invalid_object_type;
	}

	if (object->choice.webcredential.password == NULL) {
		return DNDS_value_not_present;
	}

	*password = (char *)object->choice.webcredential.password->buf;
	*length = object->choice.webcredential.password->size;

	return DNDS_success;
}

char *ObjectName_str(e_ObjectName objectname)
{
	switch (objectname) {
	case ObjectName_acl:
		return "acl";
	case ObjectName_aclgroup:
		return "aclgroup";
	case ObjectName_ippool:
		return "ippool";
	case ObjectName_context:
		return "context";
	case ObjectName_host:
		return "host";
	case ObjectName_node:
		return "node";
	case ObjectName_peer:
		return "peer";
	case ObjectName_permission:
		return "permission";
	case ObjectName_client:
		return "client";
	case ObjectName_webcredential:
		return "webcredential";
	}
	return "Unknown";
}

char *SearchType_str(e_SearchType searchtype)
{
	switch (searchtype) {
	case SearchType_all:
		return "all";
	case SearchType_sequence:
		return "by sequence";
	case SearchType_object:
		return "by object";
	}
	return "Unknown";
}

char *Topology_str(e_Topology topology)
{
	switch (topology) {
	case Topology_mesh:
		return "Mesh";
	case Topology_hubspoke:
		return "Hub and Spoke";
	case Topology_gateway:
		return "Gateway";
	}
	return "Unknown";
}

char *ConnectState_str(e_ConnectState state)
{
	switch (state) {
	case ConnectState_connected:
		return "Connected";
	case ConnectState_disconnected:
		return "Disconnected";
	}
	return "Unknown";
}

char *P2pSide_str(e_P2pSide side)
{
	switch (side) {
	case P2pSide_client:
		return "Client";
	case P2pSide_server:
		return "Server";
	}
	return "Unknown";
}

// DNDS API functions
char *DNDSResult_str(e_DNDSResult result)
{
	char *str = NULL;

	switch (result) {
		case DNDSResult_success:
			str = strdup("Success");
			break;

		case DNDSResult_operationError:
			str = strdup("Operation error");
			break;

		case DNDSResult_protocolError:
			str = strdup("Protocol error");
			break;

		case DNDSResult_noSuchObject:
			str = strdup("No such object");
			break;

		case DNDSResult_busy:
			str = strdup("Busy");
			break;

		case DNDSResult_secureStepUp:
			str = strdup("Secure step up");
			break;

		case DNDSResult_insufficientAccessRights:
			str = strdup("Insufficient access rights");
			break;
	}

	return str;

}
char *DNDS_strerror(DNDS_retcode_t retcode)
{
	char *strerror = NULL;

	switch (retcode) {
		case DNDS_success:
			strerror = strdup("Success");
			break;

		case DNDS_alloc_failed:
			strerror = strdup("Memory allocation failed");
			break;

		case DNDS_invalid_param:
			strerror = strdup("A parameter passed to the function is invalid");
			break;

		case DNDS_invalid_pdu:
			strerror = strdup("The PDU type is not set");
			break;

		case DNDS_invalid_op:
			strerror = strdup("The operation type is not set");

		case DNDS_invalid_object_type:
			strerror = strdup("The object type is not set");
			break;

		case DNDS_value_not_present:
			strerror = strdup("The value is not present");
			break;

		case DNDS_conversion_failed:
			strerror = strdup("Internal conversion failed");
			break;
	}

	return strerror;
}

// _printf functions usefull for debugging
void DNDSMessage_printf(DNDSMessage_t *msg)
{
	uint8_t channel;
	DNDSMessage_get_channel(msg, &channel);
	printf("DNDSMessage> channel: %i\n", channel);

	pdu_PR pdu;
	DNDSMessage_get_pdu(msg, &pdu);
	printf("DNDSMessage> pdu: %i\n", pdu);
}

void DNDSMessage_ethernet_printf(DNDSMessage_t *msg)
{
	uint8_t *frame;
	size_t length;

	DNDSMessage_get_ethernet(msg, &frame, &length);
	int i; for (i = 0; i < length; i++) { printf("%x", frame[i]); }; printf("\n");
}

void DSMessage_printf(DNDSMessage_t *msg)
{
	uint32_t seqNumber;
	DSMessage_get_seqNumber(msg, &seqNumber);
	printf("DSMessage> seqNumber: %i\n", seqNumber);

	uint32_t ackNumber;
	DSMessage_get_ackNumber(msg, &ackNumber);
	printf("DSMessage> ackNumber: %i\n", ackNumber);

	dsop_PR operation;
	DSMessage_get_operation(msg, &operation);
	printf("DSMessage> operation: %i\n", operation);
}

void DNMessage_printf(DNDSMessage_t *msg)
{
	uint32_t seqNumber;
	DNMessage_get_seqNumber(msg, &seqNumber);
	printf("DNMessage> seqNumber: %i\n", seqNumber);

	uint32_t ackNumber;
	DNMessage_get_ackNumber(msg, &ackNumber);
	printf("DNMessage> ackNumber: %i\n", ackNumber);

	dnop_PR operation;
	DNMessage_get_operation(msg, &operation);
	printf("DNMessage> operation: %i\n", operation);
}

void AddRequest_printf(DNDSMessage_t *msg)
{
	DNDSObject_PR objType;
	AddRequest_get_objectType(msg, &objType);
	printf("AddRequest> objType: %i\n", objType);
}

void AddResponse_printf(DNDSMessage_t *msg)
{
	e_DNDSResult result;
	AddResponse_get_result(msg, &result);
	printf("AddResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void PeerConnectInfo_printf(DNDSMessage_t *msg)
{
	int ret = 0;

	size_t length;
	char *certName;

	ret = PeerConnectInfo_get_certName(msg, &certName, &length);
	printf("PeerConnectInfo> certName(%i): %s\n", ret, certName);

	char ipAddress[INET_ADDRSTRLEN];
	ret = PeerConnectInfo_get_ipAddr(msg, ipAddress);
	printf("PeerConnectInfo> ipAddr(%i): %s\n", ret, ipAddress);

	e_ConnectState state;
	ret = PeerConnectInfo_get_state(msg, &state);
	printf("PeerConnectInfo> state(%i): %i :: %s\n", ret, state, ConnectState_str(state));
}

void P2pRequest_printf(DNDSMessage_t *msg)
{
	uint8_t macAddrDst[ETHER_ADDR_LEN];
	P2pRequest_get_macAddrDst(msg, macAddrDst);
	printf("P2pRequest> macAddrDst: %x:%x:%x:%x:%x:%x\n", macAddrDst[0],macAddrDst[1],macAddrDst[2],										macAddrDst[3],macAddrDst[4],macAddrDst[5]);

	char ipAddrDst[INET_ADDRSTRLEN];
	P2pRequest_get_ipAddrDst(msg, ipAddrDst);
	printf("P2pRequest> ipAddrDst: %s\n", ipAddrDst);

	uint32_t port;
	P2pRequest_get_port(msg, &port);
	printf("P2pRequest> port: %i\n", port);

	e_P2pSide side;
	P2pRequest_get_side(msg, &side);
	printf("P2pRequest> side: %i :: %s\n", side, P2pSide_str(side));
}

void P2pResponse_printf(DNDSMessage_t *msg)
{
	uint8_t macAddrDst[ETHER_ADDR_LEN];
	P2pResponse_get_macAddrDst(msg, macAddrDst);
	printf("P2pResponse> macAddrDst: %x:%x:%x:%x:%x:%x\n", macAddrDst[0],macAddrDst[1],macAddrDst[2],
								macAddrDst[3],macAddrDst[4],macAddrDst[5]);
	e_DNDSResult result;
	P2pResponse_get_result(msg, &result);
	printf("P2pResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void AuthRequest_printf(DNDSMessage_t *msg)
{
	size_t length;
	char *certName;
	AuthRequest_get_certName(msg, &certName, &length);
	printf("AuthRequest> certName: %s\n", certName);
}

void AuthResponse_printf(DNDSMessage_t *msg)
{
	e_DNDSResult result;
	AuthResponse_get_result(msg, &result);
	printf("AuthResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void DelRequest_printf(DNDSMessage_t *msg)
{
	DNDSObject_PR objType;
	DelRequest_get_objectType(msg, &objType);
	printf("DelRequest> objType: %i\n", objType);
}

void DelResponse_printf(DNDSMessage_t *msg)
{
	e_DNDSResult result;
	DelResponse_get_result(msg, &result);
	printf("DelResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void ModifyRequest_printf(DNDSMessage_t *msg)
{
	DNDSObject_PR objType;
	DelRequest_get_objectType(msg, &objType);
	printf("DelRequest> objType: %i\n", objType);
}

void ModifyResponse_printf(DNDSMessage_t *msg)
{
	e_DNDSResult result;
	ModifyResponse_get_result(msg, &result);
	printf("ModifyResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void NetinfoRequest_printf(DNDSMessage_t *msg)
{
	char ipLocal[INET_ADDRSTRLEN];
	NetinfoRequest_get_ipLocal(msg, ipLocal);
	printf("NetinfoRequest> ipLocal: %s\n", ipLocal);

	uint8_t macAddr[ETHER_ADDR_LEN];
	NetinfoRequest_get_macAddr(msg, macAddr);
	printf("NetinfoRequest> macAddr: %x:%x:%x:%x:%x:%x\n", macAddr[0],macAddr[1],macAddr[2],
								macAddr[3],macAddr[4],macAddr[5]);
}

void NetinfoResponse_printf(DNDSMessage_t *msg)
{
	char ipAddress[INET_ADDRSTRLEN];
	NetinfoResponse_get_ipAddress(msg, ipAddress);
	printf("NetinfoResponse> ipAddress: %s\n", ipAddress);

	char netmask[INET_ADDRSTRLEN];
	NetinfoResponse_get_netmask(msg, netmask);
	printf("NetinfoResponse> netmask: %s\n", netmask);

	e_DNDSResult result;
	NetinfoResponse_get_result(msg, &result);
	printf("NetinfoResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void SearchRequest_printf(DNDSMessage_t *msg)
{
	int ret = 0;

	e_SearchType SearchType;
	ret = SearchRequest_get_searchType(msg, &SearchType);
	printf("SearchRequest> searchType(%i): %s\n", ret, SearchType_str(SearchType));

	e_ObjectName ObjectName;
	SearchRequest_get_objectName(msg, &ObjectName);
	printf("SearchRequest> objectName(%i): %s\n", ret, ObjectName_str(ObjectName));

	DNDSObject_t *object = NULL;
	SearchRequest_get_object(msg, &object);
	DNDSObject_printf(object);
}

void SearchResponse_printf(DNDSMessage_t *msg)
{
	e_DNDSResult result;
	SearchResponse_get_result(msg, &result);
	printf("SearchResponse> result: %i :: %s\n", result, DNDSResult_str(result));
}

void Acl_printf(DNDSObject_t *object)
{
	uint32_t id;
	Acl_get_id(object, &id);
	printf("Acl> id: %i\n", id);

	uint32_t contextId;
	Acl_get_contextId(object, &contextId);
	printf("Acl> contextId: %i\n", contextId);

	char *description; size_t length;
	Acl_get_description(object, &description, &length);
	printf("Acl> description: %s\n", description);
}

void AclGroup_printf(DNDSObject_t *object)
{
	uint32_t id;
	AclGroup_get_id(object, &id);
	printf("AclGroup> id: %i\n", id);

	uint32_t contextId;
	AclGroup_get_contextId(object, &contextId);
	printf("AclGroup> contextId: %i\n", contextId);

	char *name; size_t length;
	AclGroup_get_name(object, &name, &length);
	printf("AclGroup> name: %s\n", name);

	char *description;
	AclGroup_get_description(object, &description, &length);
	printf("AclGroup> description: %s\n", description);
}

void IpPool_printf(DNDSObject_t *object)
{
	uint32_t id;
	IpPool_get_id(object, &id);
	printf("IpPool> id: %i\n", id);

	char ipLocal[INET_ADDRSTRLEN];
	IpPool_get_ipLocal(object, ipLocal);
	printf("IpPool> ipLocal: %s\n", ipLocal);

	char ipBegin[INET_ADDRSTRLEN];
	IpPool_get_ipBegin(object, ipBegin);
	printf("IpPool> ipBegin: %s\n", ipBegin);

	char ipEnd[INET_ADDRSTRLEN];
	IpPool_get_ipEnd(object, ipEnd);
	printf("IpPool> ipEnd: %s\n", ipEnd);

	char netmask[INET_ADDRSTRLEN];
	IpPool_get_netmask(object, netmask);
	printf("IpPool> netmask: %s\n", netmask);
}

void Context_printf(DNDSObject_t *object)
{
	int ret = 0;
	size_t length;

	uint32_t id;
	ret = Context_get_id(object, &id);
	printf("Context> id(%i): %i\n", ret, id);

	e_Topology topology;
        ret = Context_get_topology(object, &topology);
	printf("Context> topology(%i): %s\n", ret, Topology_str(topology));

	char *desc = NULL;
        ret = Context_get_description(object, &desc, &length);
	printf("Context> description(%i): %s\n", ret, desc);

	char network[INET_ADDRSTRLEN];
        ret = Context_get_network(object, network);
	printf("Context> network(%i): %s\n", ret, network);

	char netmask[INET_ADDRSTRLEN];
        ret = Context_get_netmask(object, netmask);
	printf("Context> netmask(%i): %s\n", ret, netmask);

	char *serverCert = NULL;
        ret = Context_get_serverCert(object, &serverCert, &length);
	printf("Context> serverCert(%i): %s\n", ret, serverCert);

	char *serverPrivkey = NULL;
        ret = Context_get_serverPrivkey(object, &serverPrivkey, &length);
	printf("Context> serverPrivkey(%i): %s\n", ret, serverPrivkey);

	char *trustedCert = NULL;
        ret = Context_get_trustedCert(object, &trustedCert, &length);
	printf("Context> trustedCert(%i): %s\n", ret, trustedCert);
}

void Host_printf(DNDSObject_t *object)
{
	uint32_t id;
	Host_get_id(object, &id);
	printf("Host> id: %i\n", id);

	uint32_t contextId;
	Host_get_contextId(object, &contextId);
	printf("Host> contextId: %i\n", contextId);

	uint32_t peerId;
	Host_get_peerId(object, &peerId);
	printf("Host> peerId: %i\n", peerId);

	char *name; size_t length;
	Host_get_name(object, &name, &length);
	printf("Host> name: %s\n", name);

	uint8_t macAddress[ETHER_ADDR_LEN];
	Host_get_macAddress(object, macAddress);
	printf("Host> macAddress: %x:%x:%x:%x:%x:%x\n", macAddress[0],macAddress[1],macAddress[2],
							macAddress[3],macAddress[4],macAddress[5]);
	char ipAddress[INET_ADDRSTRLEN];
	Host_get_ipAddress(object, ipAddress);
	printf("Host> ipAddress: %s\n", ipAddress);

	uint8_t status;
	Host_get_status(object, &status);
	printf("Host> status: %i\n", status);
}

void Node_printf(DNDSObject_t *object)
{
	uint32_t id;
	Node_get_id(object, &id);
	printf("Node> id: %i\n", id);

	char *name; size_t length;
	Node_get_name(object, &name, &length);
	printf("Node> name: %s\n", name);

	char ipAddress[INET_ADDRSTRLEN];
	Node_get_ipAddress(object, ipAddress);
	printf("Node> ipAddress: %s\n", ipAddress);

	char *certificate;
	Node_get_certificate(object, &certificate, &length);
	printf("Node> certificate: %s\n", certificate);

	uint8_t *certificateKey;
	Node_get_certificateKey(object, &certificateKey, &length);
	printf("Node> certficiateKey: ");
	int i; for (i = 0; i < length; i++) { printf("%x", certificateKey[i]); } printf("\n");

	uint8_t status;
	Node_get_status(object, &status);
	printf("Node> status: %i\n", status);
}

void Peer_printf(DNDSObject_t *object)
{
	uint32_t contextId;
	Peer_get_contextId(object, &contextId);
	printf("Peer> contextId: %i\n", contextId);

	char *certificate = NULL; size_t length = 0;
	Peer_get_certificate(object, &certificate, &length);
	printf("Peer> certficiate: %s\n", certificate);

	uint8_t *certificateKey = NULL;
	length = 0;
	Peer_get_certificateKey(object, &certificateKey, &length);
	printf("Peer> certificateKey: ");
	int i; for (i = 0; i < length; i++) { printf("%x", certificateKey[i]); }; printf("\n");

	uint8_t status;
	Peer_get_status(object, &status);
	printf("Peer> status: %i\n", status);
}

void Permission_printf(DNDSObject_t *object)
{
}

void WebCredential_printf(DNDSObject_t *object)
{
	size_t length;

	uint32_t id;
	WebCredential_get_clientId(object, &id);
	printf("WebCredential> id: %i\n", id);

	char *username;
	WebCredential_get_username(object, &username, &length);
	printf("WebCredential> username: %s\n", username);

	char *password;
	WebCredential_get_password(object, &password, &length);
	printf("WebCredential> password: %s\n", password);
}

void Client_printf(DNDSObject_t *object)
{
	size_t length;

	uint32_t id;
	Client_get_id(object, &id);
	printf("Client> id: %i\n", id);

	char *username;
	Client_get_username(object, &username, &length);
	printf("Client> username: %s\n", username);

	char *password;
	Client_get_password(object, &password, &length);
	printf("Client> password: %s\n", password);

	char *firstname;
	Client_get_firstname(object, &firstname, &length);
	printf("Client> firstname: %s\n", firstname);

	char *lastname;
	Client_get_lastname(object, &lastname, &length);
	printf("Client> lastname: %s\n", lastname);

	char *email;
	Client_get_email(object, &email, &length);
	printf("Client> email: %s\n", email);

	char *company;
	Client_get_company(object, &company, &length);
	printf("Client> company: %s\n", company);

	char *phone;
	Client_get_phone(object, &phone, &length);
	printf("Client> phone: %s\n", phone);

	char *country;
	Client_get_country(object, &country, &length);
	printf("Client> country: %s\n", country);

	char *stateProvince;
	Client_get_stateProvince(object, &stateProvince, &length);
	printf("Client> stateProvince: %s\n", stateProvince);

	char *city;
	Client_get_city(object, &city, &length);
	printf("Client> city: %s\n", city);

	char *postalCode;
	Client_get_postalCode(object, &postalCode, &length);
	printf("Client> postalCode: %s\n", postalCode);

	uint8_t status;
	Client_get_status(object, &status);
	printf("Client> status: %i\n", status);
}

void DNDSObject_printf(DNDSObject_t *obj)
{
	DNDSObject_PR objType;
	DNDSObject_get_objectType(obj, &objType);

	switch (objType) {

		case DNDSObject_PR_acl:
			Acl_printf(obj);
			break;

		case DNDSObject_PR_aclgroup:
			AclGroup_printf(obj);
			break;

		case DNDSObject_PR_ippool:
			IpPool_printf(obj);
			break;

		case DNDSObject_PR_context:
			Context_printf(obj);
			break;

		case DNDSObject_PR_host:
			Host_printf(obj);
			break;

		case DNDSObject_PR_node:
			Node_printf(obj);
			break;

		case DNDSObject_PR_peer:
			Peer_printf(obj);
			break;

		case DNDSObject_PR_permission:
			break;

		case DNDSObject_PR_client:
			Client_printf(obj);
			break;

		case DNDSObject_PR_webcredential:
			WebCredential_printf(obj);
			break;

		case DNDSObject_PR_NOTHING:
			break;
	}
}


