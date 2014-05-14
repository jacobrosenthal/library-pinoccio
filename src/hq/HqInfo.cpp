/**************************************************************************\
* Pinoccio Library                                                         *
* https://github.com/Pinoccio/library-pinoccio                             *
* Copyright (c) 2012-2014, Pinoccio Inc. All rights reserved.              *
* ------------------------------------------------------------------------ *
*  This program is free software; you can redistribute it and/or modify it *
*  under the terms of the BSD License as described in license.txt.         *
\**************************************************************************/
#include "HqHandler.h"

// Uncomment this in order to use TLS
//#define USE_TLS

// Warning: Setting an IP address here, or setting a _shorter_ hostname
// is not possible due to a bug in the Gainspan firmware... e.g. if I
// set google.com here, the gainspan module will try to connect to
// "google.compinocc.io"...
const char HqHandler::host[] = "pool.base.pinocc.io";

#ifndef USE_TLS
// 22757 for TLS, 22756 for plain
const uint16_t HqHandler::port = 22756;
const uint8_t HqHandler::cacert[] = {};
const size_t HqHandler::cacert_len = 0;
#else
// 22757 for TLS, 22756 for plain
const uint16_t HqHandler::port = 22757;

// CA certificate that signed the server certificate.
//  - Using the server certificate here doesn't work, only the CA that
//    signed it is checked (except self-signed certificates where the
//    CA and server certificates are the same, though this was not
//    tested).
//  - No checks of the server certificate (like hostname) are done,
//    other than to confirm that it was indeed signed by the right CA.
//    This means that if you use a server certificate signed by a
//    commercial CA, _any_ other certificate signed by the same CA
//    will also pass the check, which is probably not what you want...
//  - This should be a certificate in (binary) DER format. To convert
//    it to something that can be pasted below, you can use the
//    `xxd -i` command, which should be available on Linux and MacOS X.
//
const uint8_t HqHandler::cacert[] = {
  // This is the Pinoccio HQ CA certificate:
  //
  //
  //    Data:
  //        Version: 3 (0x2)
  //        Serial Number: 9502755412364719963 (0x83e091b3256b775b)
  //    Signature Algorithm: sha1WithRSAEncryption
  //        Issuer: C=US, ST=NV, L=Reno, O=Pinoccio, OU=Carlo, CN=carlo.pinocc.io/emailAddress=cert@pinocc.io
  //        Validity
  //            Not Before: Jan 24 16:43:46 2014 GMT
  //            Not After : May 27 16:43:46 3013 GMT
  //        Subject: C=US, ST=NV, L=Reno, O=Pinoccio, OU=Carlo, CN=carlo.pinocc.io/emailAddress=cert@pinocc.io
  0x30, 0x82, 0x03, 0xe1, 0x30, 0x82, 0x02, 0xc9, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x09, 0x00, 0x83, 0xe0, 0x91, 0xb3, 0x25, 0x6b, 0x77, 0x5b,
  0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01,
  0x05, 0x05, 0x00, 0x30, 0x81, 0x85, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
  0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x0b, 0x30, 0x09, 0x06,
  0x03, 0x55, 0x04, 0x08, 0x0c, 0x02, 0x4e, 0x56, 0x31, 0x0d, 0x30, 0x0b,
  0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x04, 0x52, 0x65, 0x6e, 0x6f, 0x31,
  0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x08, 0x50, 0x69,
  0x6e, 0x6f, 0x63, 0x63, 0x69, 0x6f, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03,
  0x55, 0x04, 0x0b, 0x0c, 0x05, 0x43, 0x61, 0x72, 0x6c, 0x6f, 0x31, 0x18,
  0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x0f, 0x63, 0x61, 0x72,
  0x6c, 0x6f, 0x2e, 0x70, 0x69, 0x6e, 0x6f, 0x63, 0x63, 0x2e, 0x69, 0x6f,
  0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
  0x01, 0x09, 0x01, 0x16, 0x0e, 0x63, 0x65, 0x72, 0x74, 0x40, 0x70, 0x69,
  0x6e, 0x6f, 0x63, 0x63, 0x2e, 0x69, 0x6f, 0x30, 0x20, 0x17, 0x0d, 0x31,
  0x34, 0x30, 0x31, 0x32, 0x34, 0x31, 0x36, 0x34, 0x33, 0x34, 0x36, 0x5a,
  0x18, 0x0f, 0x33, 0x30, 0x31, 0x33, 0x30, 0x35, 0x32, 0x37, 0x31, 0x36,
  0x34, 0x33, 0x34, 0x36, 0x5a, 0x30, 0x81, 0x85, 0x31, 0x0b, 0x30, 0x09,
  0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x0b, 0x30,
  0x09, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c, 0x02, 0x4e, 0x56, 0x31, 0x0d,
  0x30, 0x0b, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x04, 0x52, 0x65, 0x6e,
  0x6f, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x08,
  0x50, 0x69, 0x6e, 0x6f, 0x63, 0x63, 0x69, 0x6f, 0x31, 0x0e, 0x30, 0x0c,
  0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x05, 0x43, 0x61, 0x72, 0x6c, 0x6f,
  0x31, 0x18, 0x30, 0x16, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x0f, 0x63,
  0x61, 0x72, 0x6c, 0x6f, 0x2e, 0x70, 0x69, 0x6e, 0x6f, 0x63, 0x63, 0x2e,
  0x69, 0x6f, 0x31, 0x1d, 0x30, 0x1b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x0e, 0x63, 0x65, 0x72, 0x74, 0x40,
  0x70, 0x69, 0x6e, 0x6f, 0x63, 0x63, 0x2e, 0x69, 0x6f, 0x30, 0x82, 0x01,
  0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
  0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01,
  0x0a, 0x02, 0x82, 0x01, 0x01, 0x00, 0xb1, 0x82, 0xbc, 0x69, 0x39, 0x8f,
  0xad, 0x58, 0x13, 0x75, 0x18, 0x77, 0x4e, 0x2a, 0x4a, 0xd9, 0xe8, 0x95,
  0xbe, 0xbc, 0xd1, 0x1d, 0xfd, 0xfe, 0xb4, 0x14, 0x5d, 0x0d, 0x17, 0x03,
  0x86, 0xcc, 0x0d, 0xdb, 0x13, 0xab, 0x68, 0xc6, 0x98, 0x0d, 0x2b, 0xf7,
  0x7e, 0x86, 0x03, 0xf0, 0xd2, 0xce, 0x60, 0xa5, 0xae, 0xfe, 0x68, 0xab,
  0x50, 0x62, 0xd8, 0x3c, 0x0d, 0x56, 0x4b, 0xb6, 0x26, 0x3c, 0x70, 0x89,
  0x28, 0x2c, 0xcc, 0xbb, 0x15, 0x4f, 0x95, 0x1c, 0x84, 0x6a, 0xae, 0x5e,
  0x13, 0xdc, 0x70, 0x4b, 0x18, 0x58, 0xad, 0xed, 0xe7, 0xbb, 0x17, 0x60,
  0x02, 0xb2, 0x91, 0x18, 0xf8, 0x1f, 0x2f, 0x66, 0x34, 0x4a, 0x49, 0xd0,
  0x21, 0xb4, 0x02, 0xde, 0x3a, 0x69, 0x33, 0x0f, 0xd0, 0x06, 0x5b, 0xb0,
  0x6a, 0xd8, 0xb2, 0x67, 0xd6, 0x1c, 0xec, 0xaa, 0x6c, 0xba, 0x64, 0x30,
  0x42, 0xa2, 0x44, 0x08, 0xad, 0x15, 0x76, 0x15, 0xb6, 0xd3, 0x3b, 0x7b,
  0x29, 0x22, 0xe8, 0xcd, 0x3c, 0x91, 0x81, 0x7f, 0x4c, 0x25, 0x02, 0xae,
  0xf5, 0x12, 0x7c, 0x07, 0xd0, 0x6e, 0xe8, 0xc2, 0x31, 0x43, 0xd1, 0x6d,
  0x27, 0x91, 0x76, 0xfb, 0x3b, 0xfa, 0x4f, 0x40, 0x39, 0xce, 0xb5, 0x76,
  0x6b, 0x5c, 0xa8, 0x0e, 0x71, 0xae, 0x71, 0x79, 0x57, 0x4d, 0x97, 0x1f,
  0xda, 0xc8, 0x59, 0x9e, 0x01, 0xeb, 0x0d, 0xb2, 0xf4, 0xdb, 0x22, 0xe7,
  0xee, 0xb1, 0x70, 0xdc, 0x88, 0xeb, 0x31, 0xa1, 0xa2, 0x7c, 0x59, 0xa8,
  0xf7, 0xc2, 0x84, 0x75, 0x31, 0xc2, 0xe0, 0xde, 0x4f, 0xcf, 0xbf, 0x9e,
  0x55, 0x6f, 0x39, 0xf3, 0x0c, 0xa4, 0x23, 0x0e, 0x3c, 0xc9, 0xec, 0x3b,
  0xc2, 0x70, 0x3f, 0x05, 0xf6, 0xc8, 0xb3, 0xf5, 0xc9, 0x58, 0x22, 0x15,
  0x5b, 0x23, 0xb3, 0xf9, 0x14, 0x27, 0x1d, 0x62, 0xab, 0xfb, 0x02, 0x03,
  0x01, 0x00, 0x01, 0xa3, 0x50, 0x30, 0x4e, 0x30, 0x1d, 0x06, 0x03, 0x55,
  0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x59, 0x8c, 0x65, 0x60, 0xe2, 0x97,
  0x89, 0x86, 0x4c, 0xe3, 0xca, 0x63, 0xa2, 0xf8, 0x0c, 0x3e, 0xe8, 0x85,
  0x23, 0xdc, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30,
  0x16, 0x80, 0x14, 0x59, 0x8c, 0x65, 0x60, 0xe2, 0x97, 0x89, 0x86, 0x4c,
  0xe3, 0xca, 0x63, 0xa2, 0xf8, 0x0c, 0x3e, 0xe8, 0x85, 0x23, 0xdc, 0x30,
  0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01,
  0xff, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01,
  0x01, 0x05, 0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x58, 0x02, 0x54,
  0x88, 0x0f, 0xdf, 0xd3, 0x4c, 0x80, 0xcb, 0x00, 0x06, 0x20, 0xa1, 0x07,
  0xf4, 0x5f, 0x4a, 0xde, 0x86, 0x0a, 0x50, 0x58, 0x94, 0xe0, 0x1c, 0xb9,
  0x36, 0xec, 0xa3, 0x5a, 0x8c, 0xe0, 0xf1, 0xbe, 0xe1, 0x84, 0x40, 0x46,
  0x17, 0x0f, 0xd5, 0xce, 0xa9, 0xfa, 0x40, 0xe6, 0xde, 0x61, 0x8a, 0x13,
  0x9a, 0xe2, 0xa7, 0x7a, 0xfe, 0x49, 0x47, 0xab, 0x66, 0x68, 0x63, 0x38,
  0x82, 0x97, 0xa9, 0x3c, 0x23, 0x77, 0x2b, 0xd8, 0x5a, 0xc2, 0x9c, 0x54,
  0x34, 0x44, 0x0d, 0xb3, 0x1f, 0xc7, 0xc9, 0xa6, 0x83, 0x3f, 0x93, 0x16,
  0xff, 0xf8, 0xb0, 0xb6, 0x22, 0x2c, 0x46, 0x5f, 0x69, 0x05, 0x0e, 0xdd,
  0x53, 0x4d, 0xfb, 0x80, 0x87, 0xd5, 0x5d, 0xac, 0x3e, 0xd1, 0x64, 0xab,
  0x93, 0x30, 0xfc, 0xba, 0xd4, 0x7f, 0x77, 0x0f, 0xa8, 0xfa, 0xfd, 0x48,
  0xf7, 0x6a, 0xbe, 0xf4, 0xd5, 0x4c, 0xca, 0xd2, 0xce, 0x7e, 0x61, 0x55,
  0x9b, 0xf5, 0x7c, 0xa6, 0x02, 0xff, 0xc6, 0xf2, 0x20, 0x7c, 0x93, 0x96,
  0xf9, 0xe1, 0xfb, 0x74, 0xb7, 0xbf, 0xa4, 0xdb, 0xe5, 0xb3, 0x5f, 0x24,
  0x88, 0x32, 0x53, 0xa4, 0x9f, 0xec, 0x54, 0x03, 0x27, 0x56, 0xa3, 0x7e,
  0xbd, 0xb3, 0x73, 0xeb, 0xf4, 0x0d, 0x45, 0xb3, 0xdc, 0xf8, 0x1f, 0x80,
  0x30, 0x83, 0x33, 0xa2, 0xf6, 0x78, 0xf0, 0xe2, 0x06, 0x97, 0xf2, 0x4b,
  0x5e, 0x9d, 0x80, 0x20, 0x92, 0xc0, 0x7f, 0xdf, 0x19, 0x7d, 0x7d, 0xd1,
  0x03, 0x76, 0xda, 0x46, 0x36, 0xe0, 0x91, 0xc5, 0x74, 0xd0, 0xdb, 0x9e,
  0x47, 0xbf, 0x55, 0x4e, 0xc8, 0xd4, 0x63, 0x3c, 0x78, 0xe2, 0xea, 0xf5,
  0x27, 0x16, 0xab, 0xde, 0x47, 0xd9, 0x7e, 0x93, 0xdb, 0x93, 0x20, 0xa9,
  0x22, 0xbd, 0xc0, 0xe3, 0x32, 0xa8, 0xa2, 0xa9, 0x75, 0xfb, 0x5a, 0x6e,
  0xff
};
const size_t HqHandler::cacert_len = sizeof(cacert);
#endif
