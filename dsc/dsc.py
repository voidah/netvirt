#!/usr/bin/env python

# Copyright (C) Nicolas Bouliane - 2012
# DSC> Directory service Client

import socket
import ssl
import pprint
import time
from dnds import *

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
ssl_sock = ssl.wrap_socket(sock,
                           server_side=False,
                           ca_certs="/etc/dnds/cert-demo/dsd_cert.pem",
                           certfile="/etc/dnds/cert-demo/dnd_cert.pem",
                           keyfile="/etc/dnds/cert-demo/dnd_privkey.pem",
                           ssl_version=ssl.PROTOCOL_TLSv1)

ssl_sock.connect(('127.0.0.1', 9091))

print repr(ssl_sock.getpeername())
print ssl_sock.cipher()
print pprint.pformat(ssl_sock.getpeercert())

msg = DNDSMessage()
msg.setComponentByName('version', '1')
msg.setComponentByName('channel', '0')

pdu = msg.setComponentByName('pdu').getComponentByName('pdu')
dsm = pdu.setComponentByName('dsm').getComponentByName('dsm')

dsm.setComponentByName('seqNumber', '1')
dsm.setComponentByName('ackNumber', '1')

dsop = dsm.setComponentByName('dsop').getComponentByName('dsop')

obj = dsop.setComponentByName('addRequest').getComponentByName('addRequest')
client = obj.setComponentByName('client').getComponentByName('client')

client.setComponentByName('id', '0')
client.setComponentByName('username', 'test-username')
client.setComponentByName('password', 'test-password')
client.setComponentByName('firstname', 'test-firstname')
client.setComponentByName('lastname', 'test-lastname')
client.setComponentByName('email', 'test-email')
client.setComponentByName('company', 'test-company')
client.setComponentByName('phone', 'test-phone')
client.setComponentByName('country', 'test-country')
client.setComponentByName('stateProvince', 'test-stateProvince')
client.setComponentByName('city', 'test-city')
client.setComponentByName('postalCode', 'test-postalCode')
client.setComponentByName('status', '0')

print(msg.prettyPrint())
print ssl_sock.write(encoder.encode(msg))

time.sleep(2)

msg = DNDSMessage()
msg.setComponentByName('version', '1')
msg.setComponentByName('channel', '0')
pdu = msg.setComponentByName('pdu').getComponentByName('pdu')
dsm = pdu.setComponentByName('dsm').getComponentByName('dsm')

dsm.setComponentByName('seqNumber', '1')
dsm.setComponentByName('ackNumber', '1')
dsop = dsm.setComponentByName('dsop').getComponentByName('dsop')

req = dsop.setComponentByName('searchRequest').getComponentByName('searchRequest')
req.setComponentByName('searchtype', 'object')

obj = req.setComponentByName('object').getComponentByName('object')
webcred = obj.setComponentByName('webcredential').getComponentByName('webcredential')

webcred.setComponentByName('clientId', '0')
webcred.setComponentByName('username', 'test-username')
webcred.setComponentByName('password', 'test-password')

print(msg.prettyPrint())
print ssl_sock.write(encoder.encode(msg))

data = ssl_sock.read()

substrate = data
a_msg, substrate = decoder.decode(substrate, asn1Spec=DNDSMessage())
print(a_msg.prettyPrint())

recv_pdu = a_msg.getComponentByName('pdu')
recv_dsm = recv_pdu.getComponentByName('dsm')
recv_dsop = recv_dsm.getComponentByName('dsop')
recv_req = recv_dsop.getComponentByName('searchResponse')
recv_objs = recv_req.getComponentByName('objects')

for idx in range(len(recv_objs)):
    recv_obj =  recv_objs.getComponentByPosition(idx)
    recv_web = recv_obj.getComponentByName('webcredential')
    recv_clientId = recv_web.getComponentByName('clientId')
    print "the client id is " + str(recv_clientId)


time.sleep(2)

msg = DNDSMessage()
msg.setComponentByName('version', '1')
msg.setComponentByName('channel', '0')

pdu = msg.setComponentByName('pdu').getComponentByName('pdu')
dsm = pdu.setComponentByName('dsm').getComponentByName('dsm')

dsm.setComponentByName('seqNumber', '1')
dsm.setComponentByName('ackNumber', '1')

dsop = dsm.setComponentByName('dsop').getComponentByName('dsop')

obj = dsop.setComponentByName('addRequest').getComponentByName('addRequest')
context = obj.setComponentByName('context').getComponentByName('context')

context.setComponentByName('clientId', str(recv_clientId))
context.setComponentByName('topology', 'mesh')
context.setComponentByName('description', 'home network1')
context.setComponentByName('network', '0x2c800000')
context.setComponentByName('netmask', '0xffffff00')

print(msg.prettyPrint())
print ssl_sock.write(encoder.encode(msg))

time.sleep(3)

ssl_sock.close()

"""
msg = DNDSMessage()
msg.setComponentByName('version', '1')
msg.setComponentByName('channel', '0')
pdu = msg.setComponentByName('pdu').getComponentByName('pdu')
dsm = pdu.setComponentByName('dsm').getComponentByName('dsm')

dsm.setComponentByName('seqNumber', '1')
dsm.setComponentByName('ackNumber', '1')
dsop = dsm.setComponentByName('dsop').getComponentByName('dsop')

req = dsop.setComponentByName('searchRequest').getComponentByName('searchRequest')
req.setComponentByName('searchtype', 'all')
req.setComponentByName('objectname', 'context')
"""

"""
f = open('dnds.ber', 'wb')
f.write(encoder.encode(msg))
f.close()

f = open('dnds.ber', 'rb')
substrate = f.read()
f.close()
my_msg, substrate = decoder.decode(substrate, asn1Spec=DNDSMessage())

print(my_msg.prettyPrint())
"""
