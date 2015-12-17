/*
 * Copyright (C) 2012 Swift Navigation Inc.
 * Contact: Colin Beighley <colinbeighley@gmail.com>
 *
 * This source is subject to the license found in the file 'LICENSE' which must
 * be be distributed together with this source. All other rights reserved.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <stdlib.h>
#include <assert.h>

#include "prns.h"

static const u8 gps_ca_codes[][128];
static const u8 sbas_ca_codes[][128];

/** \defgroup prns Spreading Codes
 *
 * Pesudo-random numbers (PRNs) used in the Direct-Sequence Spread Spectrum
 * modulation of GNSS signals.
 *
 * \{ */

/** Returns the C/A code PRN corresponding to a given sid. The 1023 chip
 * PRN is packed one chip per bit into an array of 128 8-bit words. The final
 * 1024th bit is zero for all PRNs.
 *
 * \param sid Signal ID.
 *
 * \return A pointer to an array containing the C/A code PRN corresponding to
 *         the PRN number <code>prn</code>, packed one bit per chip.
 */
const u8* ca_code(gnss_signal_t sid)
{
  assert(sid.band == BAND_L1);

  switch(sid.constellation) {
    case CONSTELLATION_GPS:
      assert(sid.sat - GPS_FIRST_PRN < NUM_SATS_GPS);
      return gps_ca_codes[sid.sat - GPS_FIRST_PRN];
    case CONSTELLATION_SBAS:
      assert(sid.sat - SBAS_FIRST_PRN < NUM_SATS_SBAS);
      return sbas_ca_codes[sid.sat - SBAS_FIRST_PRN];
    default:
      assert("unsupported constellation");
  }
  return NULL;
}

inline s8 get_chip(u8* code, u32 chip_num)
{
  u32 byte = chip_num / 8;
  u8 bit = 7 - chip_num % 8;

  return ((code[byte] >> bit) & 1) ? -1 : 1;
}

/** \} */

/* {
 *   {prn0[0-7], ... , prn0[1016-1023]},
 *   ... ,
 *   {prn31[0-7], ... , prn31[1016-1023]}
 * }
 * Where 1024'th index is 0 for all.
 */
static const u8 gps_ca_codes[NUM_SATS_GPS][128] = {
  {0x37,0xC6,0xB6,0x1A,0xEC,0x15,0x2E,0xEA,0xA6,0xE1,0x60,0x48,0xC8,0x35,0x5E,0xFF,
   0x15,0xBB,0x21,0xF0,0xA3,0x30,0x9F,0xD0,0xC1,0x59,0xD2,0x39,0x0A,0xEA,0x7D,0xFE,
   0xFC,0xE2,0x7E,0x39,0x00,0x58,0xB4,0x9E,0xA9,0xD8,0xD2,0x27,0x11,0x0F,0x27,0x9B,
   0x6F,0x92,0xD2,0x1D,0x1F,0xAD,0x81,0xF5,0x46,0x0A,0x0C,0xCE,0x39,0x2A,0x93,0x91,
   0x1F,0xFD,0x32,0x62,0x5F,0x54,0x51,0x6B,0x8C,0x76,0xBA,0xD2,0xF5,0x25,0x27,0x18,
   0x4D,0xE0,0x69,0x77,0x82,0xA3,0x36,0xDA,0x00,0x78,0x21,0xC8,0xD3,0xC6,0xAF,0x5A,
   0x81,0xC2,0x58,0x98,0x10,0x5C,0xE0,0xFE,0xD7,0x4B,0xBB,0x27,0xE2,0x5C,0xBB,0x71,
   0xD3,0x36,0x19,0x03,0x35,0x96,0x50,0xC9,0x58,0x87,0x2B,0xBD,0xDB,0x1E,0x35,0xDE},

  {0x1B,0xC7,0xC1,0x66,0x90,0x34,0xD0,0x0B,0x4F,0x77,0x4E,0x1C,0xF9,0x8F,0x71,0xDC,
   0x58,0x7D,0x2F,0x2D,0x0F,0x1F,0x07,0x23,0x39,0x47,0xF0,0xE5,0xD1,0xB9,0x99,0x2C,
   0xFA,0x1D,0xB5,0x73,0xF5,0x5F,0x61,0xF6,0x80,0x04,0x54,0xAB,0xEA,0xD5,0x5E,0xDC,
   0xDE,0xBD,0xAC,0x8F,0xC6,0xAB,0x23,0xE1,0x13,0xF7,0x74,0xA3,0x2B,0x66,0xC4,0x82,
   0x9F,0x68,0x63,0xD3,0xCB,0x94,0xFD,0xA2,0x75,0xA4,0x5A,0x4C,0x1F,0x77,0x52,0xD7,
   0x16,0x26,0xDC,0x14,0x10,0x9C,0x51,0x59,0x0C,0xAD,0x14,0x3E,0x10,0x48,0x5C,0x19,
   0x21,0xDE,0x94,0xBC,0x33,0x4C,0xF9,0xEF,0x02,0x98,0x2C,0xC5,0x74,0x2F,0x30,0x5F,
   0x61,0x66,0x82,0x34,0x96,0x13,0x18,0x69,0x19,0x94,0x10,0x96,0xB4,0xBF,0x5E,0x6E},

  {0x0D,0xC7,0x7A,0xD8,0xAE,0x24,0x2F,0x7B,0xBB,0xBC,0x59,0x36,0xE1,0x52,0x66,0x4D,
   0xFE,0x9E,0x28,0x43,0xD9,0x08,0xCB,0x5A,0xC5,0x48,0xE1,0x8B,0xBC,0x10,0x6B,0x45,
   0xF9,0x62,0x50,0xD6,0x8F,0xDC,0x8B,0x42,0x94,0xEA,0x17,0xED,0x97,0x38,0x62,0x7F,
   0x06,0x2A,0x13,0xC6,0xAA,0x28,0x72,0xEB,0x39,0x09,0xC8,0x95,0xA2,0x40,0xEF,0x0B,
   0x5F,0x22,0xCB,0x0B,0x01,0xF4,0xAB,0xC6,0x89,0x4D,0x2A,0x03,0x6A,0x5E,0x68,0x30,
   0xBB,0xC5,0x86,0xA5,0xD9,0x83,0xE2,0x98,0x8A,0xC7,0x8E,0xC5,0x71,0x8F,0x25,0xB8,
   0xF1,0xD0,0xF2,0xAE,0x22,0xC4,0xF5,0x67,0xE8,0x71,0xE7,0x34,0x3F,0x16,0xF5,0xC8,
   0x38,0x4E,0xCF,0xAF,0x47,0xD1,0xBC,0x39,0x39,0x1D,0x8D,0x03,0x03,0x6F,0xEB,0xB6},

  {0x06,0xC7,0x27,0x07,0xB1,0x2C,0x50,0xC3,0xC1,0xD9,0xD2,0xA3,0xED,0x3C,0xED,0x85,
   0x2D,0xEF,0xAB,0xF4,0xB2,0x03,0x2D,0x66,0x3B,0x4F,0x69,0x3C,0x8A,0xC4,0x92,0x71,
   0x78,0xDD,0xA2,0x04,0x32,0x9D,0x7E,0x18,0x9E,0x9D,0x36,0x4E,0xA9,0xCE,0xFC,0x2E,
   0xEA,0x61,0xCC,0x62,0x1C,0x69,0xDA,0x6E,0x2C,0x76,0x96,0x8E,0xE6,0xD3,0xFA,0xCF,
   0xBF,0x07,0x9F,0x67,0x64,0xC4,0x80,0xF4,0xF7,0x39,0x92,0x24,0xD0,0xCA,0xF5,0x43,
   0x6D,0x34,0x2B,0xFD,0x3D,0x0C,0x3B,0x78,0x49,0xF2,0xC3,0xB8,0xC1,0x6C,0x99,0x68,
   0x19,0xD7,0xC1,0xA7,0x2A,0x00,0xF3,0x23,0x9D,0x05,0x02,0xCC,0x9A,0x8A,0x17,0x03,
   0x94,0xDA,0xE9,0x62,0xAF,0x30,0xEE,0x11,0x29,0x59,0x43,0xC9,0xD8,0x87,0xB1,0x5A},

  {0x69,0x3B,0x93,0xA8,0xE1,0x51,0x06,0x41,0x5C,0x11,0xBA,0x6C,0x50,0x9B,0x79,0xFE,
   0x3F,0x18,0x42,0xD9,0x22,0x69,0x0C,0x23,0x64,0x98,0xEF,0xED,0xAE,0x01,0xE3,0x49,
   0x71,0x88,0xB9,0x1D,0x7E,0x97,0xB2,0x87,0x03,0xB3,0x3F,0xA0,0x0C,0xE9,0xA0,0xDB,
   0x56,0x0A,0x9A,0x5B,0x3F,0x9F,0xA2,0x5E,0xE1,0x25,0xC9,0xE2,0xDE,0x3C,0xCE,0xF5,
   0xBE,0xCB,0x5D,0x25,0x55,0xC5,0x11,0xE9,0xAA,0x00,0xEC,0x0D,0xB7,0xF2,0x47,0xA8,
   0x48,0x3D,0x98,0x87,0x05,0x58,0xB1,0x7C,0x58,0xC1,0x71,0x0E,0x3B,0x7A,0x22,0x6D,
   0x99,0xF9,0x2E,0x28,0xAE,0x3F,0x2F,0x1E,0xAF,0xC0,0xC8,0x4A,0xAD,0xCF,0xF3,0x6A,
   0x2C,0x1B,0x32,0xD7,0x2A,0xC3,0xF0,0x6E,0x2A,0x9D,0x18,0x52,0xB2,0xFB,0x2B,0x1A},

  {0x34,0xB9,0x53,0xBF,0x96,0x96,0xC4,0x5E,0xB2,0x0F,0x23,0x0E,0xB5,0xD8,0x62,0x5C,
   0xCD,0x2C,0x9E,0xB9,0xCF,0xB3,0xCE,0xDA,0xEB,0xA7,0x6E,0x0F,0x83,0xCC,0x56,0x77,
   0x3C,0xA8,0xD6,0xE1,0xCA,0x38,0xE2,0xFA,0x55,0x31,0xA2,0x68,0x64,0x26,0x1D,0x7C,
   0xC2,0x71,0x88,0xAC,0xD6,0xB2,0x32,0x34,0xC0,0x60,0x96,0x35,0x58,0xED,0xEA,0x30,
   0xCF,0xF3,0x54,0x70,0x4E,0xDC,0x5D,0xE3,0x66,0x9F,0x71,0x23,0xBE,0x1C,0xE2,0x8F,
   0x14,0xC8,0x24,0xEC,0x53,0x61,0x92,0x8A,0x20,0xF1,0xBC,0x5D,0x64,0x16,0x1A,0x82,
   0xAD,0xC3,0x2F,0xE4,0x6C,0x7D,0x1E,0x1F,0x3E,0xDD,0x95,0x73,0xD3,0xE6,0x94,0x52,
   0x9E,0xF0,0x17,0xDE,0x99,0xB9,0xC8,0x3A,0xA0,0x99,0x09,0x61,0x00,0x4D,0xD1,0x0C},

  {0x69,0x80,0x2D,0x96,0xF1,0xAE,0x76,0xB5,0x97,0x06,0x90,0x74,0x8D,0x8C,0xE8,0x58,
   0xDC,0x1F,0x2C,0x0F,0x35,0xA5,0x75,0xDF,0x6B,0x89,0x81,0x80,0x07,0xF3,0x8A,0x4A,
   0x0E,0x6D,0x1C,0x67,0xFD,0x7D,0x06,0x93,0xED,0xF0,0x79,0xDD,0xE1,0xD5,0x03,0x03,
   0xC1,0xB5,0xD3,0x37,0xBC,0xCE,0xA8,0x74,0x1F,0x99,0xFF,0x6B,0xF8,0x17,0x47,0x35,
   0xF4,0x63,0x85,0xEF,0x35,0x93,0x75,0x15,0x43,0x70,0xA3,0x78,0x9E,0xC8,0xA0,0x05,
   0xAB,0x67,0x29,0x4E,0x1A,0xEB,0x70,0xFA,0x32,0x5B,0x8A,0x6F,0xFC,0x03,0x83,0xBD,
   0x97,0x9F,0x3C,0x39,0x26,0x33,0xA7,0xF4,0x46,0x0B,0x39,0x01,0x94,0x0A,0x64,0x33,
   0x04,0x56,0xA9,0x06,0xE8,0x67,0xA0,0x4E,0xA3,0x00,0x8D,0xE5,0x62,0x4E,0xF3,0x36},

  {0x34,0xE4,0x8C,0xA0,0x9E,0xE9,0x7C,0x24,0xD7,0x84,0xB6,0x02,0xDB,0x53,0xAA,0x8F,
   0xBC,0xAF,0x29,0xD2,0xC4,0x55,0xF2,0x24,0xEC,0x2F,0xD9,0x39,0x57,0x35,0x62,0xF6,
   0x83,0x5A,0x04,0x5C,0x8B,0xCD,0xB8,0xF0,0x22,0x10,0x01,0x56,0x92,0xB8,0x4C,0x90,
   0x89,0xAE,0x2C,0x1A,0x97,0x1A,0xB7,0x21,0xBF,0x3E,0x8D,0x71,0xCB,0xF8,0x2E,0xD0,
   0xEA,0xA7,0x38,0x15,0x7E,0xF7,0x6F,0x9D,0x12,0x27,0x56,0x99,0x2A,0x81,0x91,0x59,
   0xE5,0x65,0x7C,0x08,0xDC,0xB8,0x72,0x49,0x15,0xBC,0xC1,0xED,0x87,0xAA,0xCA,0x6A,
   0xAA,0xF0,0x26,0xEC,0xA8,0x7B,0x5A,0x6A,0x4A,0x38,0x6D,0xD6,0x4F,0x04,0x5F,0xFE,
   0x0A,0xD6,0xDA,0x36,0x78,0xEB,0xE0,0x2A,0xE4,0x57,0xC3,0xBA,0xE8,0x17,0x3D,0x1A},

  {0x1A,0x56,0xDC,0x3B,0xA9,0x4A,0xF9,0x6C,0x77,0xC5,0xA5,0x39,0xF0,0x3C,0x0B,0xE4,
   0x0C,0xF7,0x2B,0x3C,0x3C,0xAD,0xB1,0xD9,0x2F,0xFC,0xF5,0x65,0xFF,0x56,0x16,0xA8,
   0xC5,0xC1,0x88,0x41,0x30,0x95,0xE7,0xC1,0xC5,0xE0,0x3D,0x13,0x2B,0x0E,0xEB,0x59,
   0x2D,0xA3,0xD3,0x8C,0x02,0xF0,0xB8,0x8B,0x6F,0x6D,0x34,0x7C,0xD2,0x0F,0x9A,0x22,
   0x65,0xC5,0x66,0xE8,0x5B,0x45,0x62,0xD9,0x3A,0x8C,0xAC,0x69,0xF0,0xA5,0x09,0xF7,
   0xC2,0x64,0x56,0xAB,0xBF,0x91,0xF3,0x10,0x86,0x4F,0x64,0x2C,0xBA,0x7E,0x6E,0x81,
   0x34,0x47,0xAB,0x86,0x6F,0x5F,0x24,0xA5,0x4C,0x21,0xC7,0xBD,0xA2,0x83,0x42,0x18,
   0x8D,0x96,0xE3,0xAE,0x30,0xAD,0xC0,0x18,0xC7,0xFC,0x64,0x95,0x2D,0x3B,0xDA,0x0C},

  {0x2E,0xD7,0x63,0xC9,0x7B,0xF7,0xB8,0x99,0x2D,0xCF,0xD2,0x18,0x53,0x02,0xFD,0x7A,
   0x5D,0x1B,0x60,0xE3,0x98,0x1B,0xF0,0x71,0xAA,0xE9,0x8A,0x3B,0xE4,0x12,0x85,0xBD,
   0x01,0x21,0xAD,0x15,0x5C,0xF0,0xD7,0xEA,0xF7,0x9E,0x49,0xAB,0x0C,0xB4,0x7F,0xC4,
   0x5E,0x75,0x22,0x21,0x5A,0x14,0x37,0x52,0x8F,0xAE,0x01,0x31,0xAF,0xBF,0x79,0x9E,
   0xB5,0x2D,0x61,0xDB,0x52,0x4D,0xA6,0xDF,0x7E,0xF9,0xD8,0x8C,0x08,0x00,0x95,0x15,
   0x09,0xC8,0xC2,0x8D,0x72,0x79,0x12,0x42,0xAE,0x5F,0x20,0xE2,0x70,0xA5,0x86,0xDB,
   0xD8,0x51,0xAB,0x3D,0xD1,0x61,0x34,0x5A,0x3C,0xD5,0x0C,0xAA,0x88,0x9B,0x9F,0x0F,
   0x1C,0x30,0x00,0xA9,0x5E,0x7B,0xD7,0xD4,0xBE,0x00,0x6B,0x84,0x43,0x56,0x73,0xFE},

  {0x17,0x4F,0x2B,0x8F,0x5B,0xC5,0x9B,0x32,0x8A,0xE0,0x17,0x34,0xB4,0x14,0xA0,0x1E,
   0xFC,0x2D,0x0F,0xA4,0x92,0x8A,0xB0,0xF3,0x8C,0x9F,0xDC,0xE4,0xA6,0xC5,0xE5,0x0D,
   0x04,0xFC,0x5C,0xE5,0xDB,0x0B,0x50,0x4C,0xAF,0x27,0x19,0x6D,0xE4,0x08,0xF2,0xF3,
   0x46,0x4E,0x54,0x91,0xE4,0x77,0xF8,0xB2,0xF7,0x25,0x72,0x5C,0xE0,0x2C,0x31,0x85,
   0x4A,0x00,0x4A,0x0F,0x4D,0x18,0x06,0x78,0x0C,0xE3,0xEB,0x63,0x61,0xE5,0x8B,0xD1,
   0xB4,0x32,0x89,0xE9,0x68,0xF1,0x43,0x15,0x5B,0xBE,0x94,0xAB,0x41,0xF9,0xC8,0xD9,
   0x8D,0x17,0x6D,0x6E,0xD3,0xD2,0x13,0xBD,0x77,0x57,0x77,0x03,0xC1,0x4C,0xA2,0x60,
   0x06,0xE5,0x8E,0xE1,0xA3,0xE5,0xDB,0xE7,0xEA,0xD7,0xB0,0x8A,0x78,0x9B,0x7D,0x7E},

  {0x05,0xE5,0x1D,0xBD,0xC3,0xD0,0x02,0x0D,0xB0,0xBC,0x04,0xE9,0xFE,0x5A,0x19,0xF5,
   0x84,0xFB,0xA3,0xD6,0xD5,0x66,0x40,0x92,0x16,0x39,0x62,0x3C,0xD7,0x1B,0x8D,0x79,
   0x07,0x65,0xD8,0x61,0xB9,0x08,0x72,0x76,0x15,0x55,0xE5,0x3F,0x2A,0x79,0x97,0x25,
   0x0C,0x5D,0x32,0x65,0x94,0xDE,0xEC,0xBA,0xD5,0x42,0x17,0x31,0x14,0x01,0x47,0x8E,
   0x4A,0x5D,0x95,0x10,0x45,0x67,0xBE,0x02,0x69,0x68,0x7E,0x6F,0x0F,0x6E,0x43,0x02,
   0xC5,0xB1,0x3E,0x82,0x63,0x17,0x7F,0xEB,0x5C,0x36,0x23,0x9D,0x95,0x00,0xFC,0x58,
   0x32,0xE5,0xBF,0xD3,0x92,0x27,0x49,0xB7,0x00,0x76,0xD4,0x3D,0x37,0xD2,0xF3,0x8C,
   0x4D,0x3A,0x2A,0x57,0xE2,0x4D,0x5E,0xF2,0x95,0x89,0xAB,0xCE,0xEB,0x8E,0xB9,0x9E},

  {0x02,0xD6,0x14,0xB5,0x07,0xD6,0x46,0x78,0xC4,0x59,0xFC,0x4C,0x62,0xB8,0xD2,0x59,
   0x10,0xDD,0x6E,0x3E,0x34,0x34,0x68,0x82,0x52,0xF7,0xA8,0xE7,0x3F,0x41,0x61,0x6F,
   0x07,0xDE,0x66,0x5F,0xA9,0xF7,0x02,0x82,0xDE,0x42,0xCF,0x27,0xF7,0x6E,0x06,0x83,
   0xEF,0x5A,0x5C,0xB3,0x83,0x12,0x95,0x46,0xDA,0x53,0x79,0x5C,0xBD,0xF3,0x2E,0x8D,
   0x35,0xB8,0x30,0x6A,0xC6,0x8D,0x0A,0x16,0x87,0x2B,0x38,0x12,0xE2,0x52,0xE0,0xDA,
   0x52,0x0E,0x77,0xEE,0xE0,0x46,0x75,0xC1,0xA2,0x8A,0x15,0x14,0xB3,0x2B,0x75,0x98,
   0x78,0x4D,0x67,0x19,0xF2,0x71,0x2D,0x4B,0xE9,0x06,0x9B,0x48,0x1E,0xE8,0x14,0x21,
   0xAE,0x60,0x9B,0x9E,0xFD,0xFE,0x9F,0x74,0xFF,0x13,0x50,0xAF,0x2C,0xF7,0x18,0x4E},

  {0x01,0x4F,0x90,0x31,0x65,0xD5,0x64,0x42,0x7E,0x2B,0x00,0x1E,0xAC,0xC9,0xB7,0x8F,
   0x5A,0xCE,0x08,0xCA,0x44,0x9D,0x7C,0x8A,0x70,0x90,0xCD,0x8A,0xCB,0x6C,0x17,0x64,
   0x07,0x83,0xB9,0x40,0xA1,0x88,0xBA,0xF8,0xBB,0xC9,0x5A,0x2B,0x99,0xE5,0xCE,0x50,
   0x9E,0xD9,0xEB,0xD8,0x88,0xF4,0xA9,0xB8,0xDD,0xDB,0xCE,0x6A,0x69,0x0A,0x1A,0x0C,
   0x8A,0x4A,0xE2,0xD7,0x87,0x78,0x50,0x1C,0xF0,0x0A,0x9B,0x2C,0x14,0xCC,0xB1,0x36,
   0x19,0xD1,0xD3,0x58,0xA1,0xEE,0xF0,0xD4,0xDD,0xD4,0x0E,0x50,0x20,0x3E,0xB1,0x78,
   0x5D,0x19,0x0B,0x7C,0xC2,0x5A,0x1F,0x35,0x9D,0xBE,0xBC,0xF2,0x8A,0x75,0x67,0xF7,
   0x5F,0xCD,0xC3,0x7A,0x72,0x27,0x7F,0xB7,0xCA,0x5E,0x2D,0x1F,0xCF,0x4B,0xC8,0xA6},

  {0x00,0x83,0x52,0x73,0x54,0xD4,0xF5,0x5F,0x23,0x12,0x7E,0x37,0xCB,0xF1,0x05,0x64,
   0x7F,0xC7,0xBB,0xB0,0x7C,0xC9,0xF6,0x8E,0x61,0xA3,0x7F,0x3C,0x31,0x7A,0xAC,0x61,
   0x87,0xAD,0x56,0xCF,0x25,0xB7,0x66,0xC5,0x89,0x0C,0x90,0xAD,0xAE,0xA0,0x2A,0x39,
   0x26,0x18,0x30,0x6D,0x0D,0x07,0xB7,0xC7,0xDE,0x1F,0x95,0xF1,0x03,0x76,0x80,0x4C,
   0x55,0xB3,0x8B,0x89,0x27,0x82,0xFD,0x19,0xCB,0x9A,0x4A,0xB3,0x6F,0x83,0x99,0xC0,
   0x3C,0x3E,0x01,0x03,0x81,0x3A,0xB2,0x5E,0x62,0x7B,0x03,0xF2,0x69,0xB4,0x53,0x08,
   0x4F,0xB3,0x3D,0x4E,0x5A,0x4F,0x86,0x0A,0xA7,0xE2,0xAF,0x2F,0xC0,0x3B,0xDE,0x1C,
   0x27,0x1B,0x6F,0x08,0x35,0xCB,0x8F,0xD6,0x50,0xF8,0x93,0xC7,0xBE,0x95,0xA0,0xD2},

  {0x00,0x65,0x33,0x52,0x4C,0x54,0x3D,0xD1,0x8D,0x8E,0xC1,0x23,0x78,0x6D,0x5C,0x11,
   0xED,0x43,0x62,0x0D,0x60,0xE3,0xB3,0x8C,0x69,0x3A,0xA6,0x67,0x4C,0x71,0xF1,0xE3,
   0x47,0xBA,0x21,0x08,0xE7,0xA8,0x88,0xDB,0x10,0x6E,0x75,0xEE,0xB5,0x02,0xD8,0x0D,
   0xFA,0x78,0xDD,0xB7,0xCF,0xFE,0x38,0xF8,0x5F,0xFD,0xB8,0x3C,0xB6,0x48,0xCD,0x6C,
   0x3A,0x4F,0x3F,0x26,0x77,0xFF,0xAB,0x9B,0x56,0x52,0x22,0x7C,0xD2,0x24,0x0D,0xBB,
   0x2E,0xC9,0xE8,0x2E,0x11,0x50,0x93,0x1B,0x3D,0xAC,0x85,0x23,0x4D,0x71,0x22,0x30,
   0x46,0xE6,0x26,0x57,0x16,0x45,0x4A,0x95,0x3A,0xCC,0xA6,0xC1,0x65,0x1C,0x82,0xE9,
   0x9B,0x70,0x39,0x31,0x16,0x3D,0xF7,0xE6,0x9D,0xAB,0xCC,0xAB,0x86,0x7A,0x94,0xE8},

  {0x64,0x7F,0xBB,0x03,0x1B,0xA1,0xDC,0x65,0xC4,0xBF,0x9D,0x6D,0x7A,0x38,0x1A,0xD7,
   0xBE,0x41,0xD1,0x2A,0x87,0xA8,0x31,0xF7,0xC0,0x73,0x71,0x5A,0x23,0x6B,0x24,0x6D,
   0x0F,0x47,0xBF,0x04,0xD4,0xFC,0x5F,0x00,0x1E,0x55,0xB8,0xE0,0x35,0x71,0xE8,0x9D,
   0x76,0x38,0xB9,0xF0,0x98,0xB0,0x67,0x72,0x06,0x33,0x94,0x86,0x7F,0x0A,0xA1,0xB2,
   0xB4,0x5A,0x1D,0xA7,0x73,0xB3,0x47,0x36,0xE8,0xD7,0x8C,0xBD,0xB2,0x2F,0xB6,0x58,
   0xCF,0xC6,0x1F,0x21,0x5D,0xE1,0xE1,0xBA,0xB0,0x7D,0xFC,0x39,0x23,0x41,0x54,0xDD,
   0x27,0x9A,0xE1,0xC8,0xD6,0xB4,0x5C,0x73,0xE0,0x53,0x80,0x51,0x52,0xE2,0xD8,0xBE,
   0x33,0x4E,0x92,0x70,0x58,0xD9,0xC3,0x81,0x43,0x78,0x06,0x96,0x0F,0x01,0x60,0x7E},

  {0x32,0x1B,0x47,0xEA,0x6B,0xEE,0xA9,0x4C,0xFE,0x58,0x30,0x8E,0x20,0x89,0xD3,0xC8,
   0x0D,0x80,0x57,0x40,0x1D,0x53,0x50,0x30,0xB9,0xD2,0xA1,0x54,0x45,0x79,0x35,0xE5,
   0x03,0xCF,0x55,0xED,0x1F,0x0D,0x14,0x39,0xDB,0xC2,0xE1,0xC8,0x78,0xEA,0x39,0x5F,
   0xD2,0x68,0x99,0x79,0x05,0x25,0xD0,0xA2,0xB3,0xEB,0xB8,0x87,0x08,0x76,0xDD,0x93,
   0x4A,0xBB,0xF4,0x31,0x5D,0xE7,0x76,0x8C,0xC7,0xF4,0xC1,0x7B,0xBC,0xF2,0x1A,0x77,
   0x57,0x35,0xE7,0x3F,0x7F,0x3D,0x3A,0xE9,0x54,0xAF,0xFA,0xC6,0xE8,0x0B,0xA1,0xDA,
   0xF2,0xF2,0xC8,0x14,0x50,0x38,0xA7,0xA9,0x99,0x14,0x31,0x7E,0x2C,0x70,0x01,0xB8,
   0x91,0x5A,0xC7,0x8D,0x20,0xB4,0xD1,0xCD,0x14,0x6B,0x86,0x03,0x5E,0xB0,0xF4,0xBE},

  {0x19,0x29,0x39,0x9E,0xD3,0xC9,0x13,0xD8,0x63,0x2B,0xE6,0x7F,0x8D,0xD1,0x37,0x47,
   0xD4,0x60,0x94,0x75,0x50,0x2E,0xE0,0xD3,0x05,0x02,0x49,0x53,0x76,0x70,0x3D,0x21,
   0x05,0x8B,0x20,0x99,0xFA,0xF5,0xB1,0xA5,0x39,0x09,0x4D,0x5C,0x5E,0x27,0xD1,0xBE,
   0x80,0x40,0x89,0x3D,0xCB,0xEF,0x0B,0x4A,0xE9,0x07,0xAE,0x87,0xB3,0xC8,0xE3,0x83,
   0xB5,0xCB,0x00,0xFA,0x4A,0xCD,0x6E,0x51,0xD0,0x65,0x67,0x98,0xBB,0x9C,0xCC,0x60,
   0x9B,0x4C,0x1B,0x30,0x6E,0x53,0x57,0x40,0xA6,0xC6,0xF9,0xB9,0x0D,0xAE,0xDB,0x59,
   0x18,0x46,0xDC,0xFA,0x13,0x7E,0xDA,0x44,0xA5,0xB7,0xE9,0xE9,0x93,0x39,0x6D,0x3B,
   0xC0,0x50,0xED,0x73,0x9C,0x82,0x58,0xEB,0x3F,0xE2,0x46,0x49,0xF6,0x68,0x3E,0xDE},

  {0x0C,0xB0,0x06,0xA4,0x8F,0xDA,0xCE,0x92,0x2D,0x92,0x0D,0x07,0x5B,0x7D,0x45,0x00,
   0x38,0x90,0xF5,0xEF,0xF6,0x90,0x38,0xA2,0xDB,0x6A,0x3D,0x50,0xEF,0xF4,0xB9,0x43,
   0x06,0xA9,0x1A,0x23,0x88,0x09,0xE3,0x6B,0x48,0x6C,0x9B,0x16,0x4D,0x41,0x25,0xCE,
   0x29,0x54,0x81,0x1F,0xAC,0x8A,0x66,0xBE,0xC4,0x71,0xA5,0x87,0xEE,0x17,0xFC,0x8B,
   0xCA,0x73,0x7A,0x9F,0xC1,0x58,0x62,0x3F,0x5B,0xAD,0xB4,0xE9,0x38,0x2B,0xA7,0x6B,
   0x7D,0x70,0xE5,0x37,0xE6,0xE4,0x61,0x94,0x5F,0xF2,0x78,0x06,0xFF,0x7C,0x66,0x18,
   0xED,0x1C,0xD6,0x8D,0x32,0xDD,0xE4,0xB2,0x3B,0xE6,0x05,0xA2,0x4C,0x9D,0xDB,0x7A,
   0x68,0xD5,0xF8,0x0C,0xC2,0x99,0x1C,0x78,0x2A,0x26,0xA6,0x6C,0xA2,0x04,0x5B,0xEE},

  {0x06,0x7C,0x99,0x39,0xA1,0xD3,0x20,0x37,0x0A,0xCE,0xF8,0xBB,0x30,0x2B,0x7C,0x23,
   0xCE,0xE8,0xC5,0x22,0xA5,0xCF,0x54,0x9A,0x34,0x5E,0x07,0x51,0x23,0x36,0xFB,0x72,
   0x07,0x38,0x07,0x7E,0xB1,0x77,0xCA,0x0C,0x70,0xDE,0x70,0x33,0x44,0xF2,0x5F,0xF6,
   0x7D,0xDE,0x85,0x0E,0x9F,0x38,0xD0,0x44,0xD2,0xCA,0xA0,0x07,0xC0,0xF8,0x73,0x0F,
   0xF5,0xAF,0x47,0xAD,0x04,0x92,0xE4,0x08,0x1E,0x49,0xDD,0x51,0xF9,0xF0,0x12,0xEE,
   0x8E,0x6E,0x9A,0x34,0x22,0xBF,0xFA,0xFE,0x23,0x68,0x38,0xD9,0x06,0x15,0x38,0xB8,
   0x17,0xB1,0xD3,0xB6,0xA2,0x0C,0x7B,0xC9,0x74,0xCE,0xF3,0x87,0xA3,0x4F,0x80,0x5A,
   0xBC,0x97,0x72,0xB3,0x6D,0x94,0xBE,0x31,0xA0,0xC4,0xD6,0x7E,0x08,0x32,0x69,0x76},

  {0x03,0x1A,0xD6,0xF7,0x36,0xD7,0xD7,0x65,0x99,0x60,0x82,0x65,0x05,0x80,0x60,0xB2,
   0x35,0xD4,0xDD,0x44,0x0C,0x60,0xE2,0x86,0x43,0xC4,0x1A,0x51,0xC5,0x57,0xDA,0x6A,
   0x87,0xF0,0x89,0xD0,0x2D,0xC8,0xDE,0xBF,0xEC,0x87,0x05,0xA1,0xC0,0x2B,0xE2,0xEA,
   0x57,0x9B,0x87,0x06,0x06,0xE1,0x8B,0x39,0xD9,0x97,0x22,0xC7,0xD7,0x8F,0xB4,0xCD,
   0xEA,0x41,0x59,0x34,0x66,0x77,0xA7,0x13,0xBC,0xBB,0xE9,0x8D,0x99,0x1D,0xC8,0x2C,
   0x77,0xE1,0xA5,0xB5,0xC0,0x92,0x37,0x4B,0x1D,0x25,0x18,0xB6,0xFA,0xA1,0x97,0xE8,
   0x6A,0xE7,0x51,0x2B,0x6A,0x64,0xB4,0x74,0xD3,0x5A,0x88,0x95,0x54,0xA6,0xAD,0xCA,
   0xD6,0xB6,0x37,0xEC,0xBA,0x12,0x6F,0x15,0x65,0xB5,0xEE,0x77,0x5D,0x29,0x70,0x3A},

  {0x73,0x08,0x7C,0xC1,0x04,0x4F,0xC2,0xFC,0xA6,0x5A,0xDE,0x76,0xD8,0xCA,0x2E,0x0B,
   0xDE,0xAC,0x0B,0xE8,0x7E,0x27,0x0E,0x76,0x22,0x5E,0xBC,0xEF,0x1D,0x26,0x04,0x02,
   0xF3,0xF3,0x10,0x54,0xA9,0xAA,0xDD,0x9D,0xD6,0x3D,0x77,0x5D,0x92,0xE5,0x93,0x8F,
   0x81,0xD1,0x94,0x60,0xF2,0x91,0x22,0x2D,0xD1,0xB5,0x45,0xA2,0xBA,0x7B,0x99,0xBB,
   0xE1,0x41,0x04,0xEB,0x79,0x7F,0xD8,0xAB,0xC6,0xDE,0x62,0x18,0x95,0x73,0x43,0xE4,
   0xA4,0x90,0x26,0x02,0xAB,0x99,0xD1,0x77,0xE3,0x22,0x90,0x01,0xCC,0x75,0x6E,0xDC,
   0xEB,0x58,0xA3,0xF9,0xD7,0x25,0x41,0x2E,0xD9,0x2D,0xA9,0x36,0x6A,0x50,0x33,0x9B,
   0x3A,0xFD,0xE8,0x48,0x0C,0x53,0xC7,0x90,0x70,0xCA,0xB0,0x6C,0x19,0xBA,0x65,0xFE},

  {0x0E,0x5E,0xFE,0x5C,0xCC,0x27,0x0D,0x41,0x01,0xCE,0xA5,0x64,0x2F,0x23,0x03,0x9B,
   0xB4,0x8D,0x4E,0xB7,0xA9,0xA1,0xDF,0x52,0xE7,0x2F,0x84,0xE6,0x48,0x3D,0x1D,0x4E,
   0xF9,0x3F,0x8F,0xC9,0x87,0xA3,0x33,0x38,0xF1,0x61,0x82,0xE1,0xF9,0xB3,0xAA,0xAC,
   0x77,0xA9,0xA4,0xAD,0xA1,0xCE,0x4E,0x15,0x3E,0x81,0x7F,0xA3,0x76,0xB9,0xDB,0x8A,
   0xE0,0xD0,0x19,0xB6,0x40,0x01,0xF1,0xCC,0xFE,0x6C,0x89,0x3D,0x9C,0xC0,0x39,0xDC,
   0xF0,0x1A,0x22,0x13,0x98,0x2B,0x67,0x8D,0xF5,0x99,0x95,0x81,0xE2,0x9A,0xE1,0x58,
   0xD4,0x84,0x9E,0xCB,0x12,0xEF,0xC7,0x19,0x9C,0xC9,0xC0,0x8E,0xAB,0x8B,0x86,0x1E,
   0xC9,0xE3,0x97,0x4B,0xC8,0x08,0x5C,0xFA,0x0C,0x50,0xF0,0xB3,0xE0,0xD3,0x3B,0x5E},

  {0x07,0x0B,0xE5,0x45,0x80,0x2D,0xC1,0xDE,0x9C,0xE0,0xAC,0x8A,0x8A,0x04,0x5F,0x6E,
   0x08,0xE6,0x18,0x8E,0x8A,0x57,0xA7,0x62,0x2A,0x7C,0xDB,0x8A,0x70,0xD2,0x29,0x74,
   0xF8,0xF3,0x4D,0x8B,0xB6,0xA2,0xA2,0x25,0xAC,0x58,0xFC,0xC8,0x9E,0x8B,0x18,0x47,
   0x52,0xA0,0x17,0xD7,0x99,0x9A,0xC4,0x11,0x2F,0xB2,0xCD,0x15,0x8C,0xAF,0x60,0x8F,
   0x60,0xFE,0xF6,0x39,0xC4,0x3E,0x2D,0xF1,0xCC,0xA9,0x43,0xBB,0xAB,0x85,0xDD,0xB5,
   0x48,0xDB,0xF9,0xA6,0x1D,0xD8,0x79,0xF2,0xF6,0x5D,0xCE,0x1A,0x88,0xE6,0x7B,0x18,
   0x0B,0x7D,0xF7,0x95,0xB2,0x15,0x6A,0x1C,0xA7,0x59,0x11,0x11,0xD0,0xC4,0xAE,0xE8,
   0xEC,0x0C,0x45,0x10,0xE8,0xDC,0x1E,0x70,0xB3,0xFF,0xFD,0x11,0xA9,0x59,0xD9,0x2E},

  {0x03,0xA1,0x68,0xC9,0x26,0x28,0xA7,0x91,0x52,0x77,0xA8,0x7D,0xD8,0x97,0xF1,0x14,
   0xD6,0xD3,0xB3,0x92,0x1B,0xAC,0x9B,0x7A,0x4C,0xD5,0x74,0x3C,0x6C,0xA5,0xB3,0x69,
   0xF8,0x15,0x2C,0xAA,0xAE,0x22,0x6A,0xAB,0x02,0xC4,0x43,0xDC,0x2D,0x17,0x41,0x32,
   0xC0,0x24,0xCE,0x6A,0x85,0xB0,0x81,0x13,0x27,0x2B,0x14,0x4E,0xF1,0xA4,0x3D,0x0D,
   0xA0,0xE9,0x81,0xFE,0x06,0x21,0xC3,0xEF,0x55,0xCB,0xA6,0xF8,0xB0,0x27,0x2F,0x81,
   0x94,0xBB,0x14,0x7C,0xDF,0x21,0xF6,0xCD,0x77,0xBF,0xE3,0xD7,0x3D,0xD8,0x36,0x38,
   0x64,0x81,0x43,0x3A,0xE2,0x68,0x3C,0x9E,0x3A,0x91,0x79,0xDE,0x6D,0x63,0x3A,0x93,
   0xFE,0xFB,0xAC,0x3D,0x78,0xB6,0x3F,0x35,0xEC,0x28,0x7B,0xC0,0x8D,0x9C,0xA8,0x16},

  {0x01,0xF4,0x2E,0x0F,0x75,0x2A,0x14,0xB6,0xB5,0x3C,0x2A,0x06,0x71,0xDE,0x26,0x29,
   0xB9,0xC9,0x66,0x1C,0x53,0x51,0x05,0x76,0x7F,0x81,0xA3,0xE7,0x62,0x9E,0x7E,0x67,
   0x78,0x66,0x1C,0x3A,0x22,0x62,0x0E,0xEC,0x55,0x8A,0x1C,0x56,0x74,0xD9,0x6D,0x88,
   0x09,0x66,0xA2,0xB4,0x0B,0xA5,0xA3,0x92,0x23,0x67,0xF8,0xE3,0x4F,0x21,0x93,0xCC,
   0xC0,0xE2,0x3A,0x1D,0xE7,0x2E,0x34,0xE0,0x19,0x7A,0xD4,0x59,0x3D,0xF6,0x56,0x9B,
   0xFA,0x8B,0x62,0x91,0xBE,0x5D,0x31,0x52,0xB7,0x4E,0xF5,0x31,0xE7,0x47,0x10,0xA8,
   0x53,0x7F,0x19,0x6D,0x4A,0x56,0x97,0xDF,0x74,0x75,0x4D,0xB9,0xB3,0xB0,0xF0,0xAE,
   0x77,0x80,0x58,0xAB,0xB0,0x83,0x2F,0x97,0x43,0xC3,0xB8,0xA8,0x1F,0xFE,0x10,0x8A},

  {0x00,0xDE,0x8D,0x6C,0x5C,0xAB,0x4D,0x25,0x46,0x99,0xEB,0x3B,0xA5,0x7A,0xCD,0xB7,
   0x0E,0x44,0x0C,0xDB,0x77,0x2F,0xCA,0x70,0x66,0x2B,0xC8,0x0A,0xE5,0x83,0x98,0xE0,
   0x38,0x5F,0x84,0x72,0x64,0x42,0x3C,0xCF,0xFE,0x2D,0x33,0x93,0x58,0x3E,0x7B,0xD5,
   0x6D,0xC7,0x94,0xDB,0x4C,0xAF,0x32,0xD2,0xA1,0x41,0x8E,0xB5,0x90,0x63,0x44,0xAC,
   0x70,0xE7,0xE7,0xEC,0x17,0xA9,0xCF,0x67,0xBF,0x22,0x6D,0x09,0xFB,0x1E,0xEA,0x16,
   0xCD,0x93,0x59,0xE7,0x0E,0xE3,0x52,0x9D,0x57,0x36,0x7E,0x42,0x8A,0x08,0x83,0xE0,
   0x48,0x80,0x34,0x46,0x9E,0x49,0xC2,0x7F,0xD3,0x07,0x57,0x8A,0x5C,0xD9,0x15,0xB0,
   0xB3,0x3D,0xA2,0xE0,0xD4,0x99,0xA7,0xC6,0x14,0x36,0x59,0x1C,0x56,0xCF,0x4C,0xC4},

  {0x6A,0x19,0xA9,0x12,0x93,0xAD,0x54,0x8F,0x2D,0x74,0x6C,0x26,0x43,0xFD,0x8D,0x8E,
   0x96,0x0C,0x4A,0xFB,0x45,0x0C,0x61,0xD7,0x49,0xEE,0xE4,0xED,0xF3,0xDE,0xFC,0x41,
   0x0E,0x30,0xC3,0x78,0xF5,0x02,0xBE,0xE9,0x88,0x7B,0xEC,0xD1,0x8F,0x5E,0xCB,0xD0,
   0xB0,0x36,0x64,0x5C,0xB7,0x28,0x94,0x8A,0x18,0x11,0x48,0x5D,0x2C,0xEE,0x73,0xB4,
   0x4B,0x91,0x57,0x52,0x74,0x66,0x2F,0x1F,0x34,0x51,0x00,0x46,0x68,0x56,0xF1,0xE9,
   0xE0,0xB8,0x8D,0xF8,0x5B,0x43,0xF5,0xEF,0x4D,0x05,0x91,0x2B,0x6F,0x16,0x47,0x5D,
   0xB2,0xCB,0x50,0x5C,0x16,0x18,0x95,0x8A,0x32,0xB3,0x1E,0xBB,0x00,0x97,0x17,0xE5,
   0xF5,0xFB,0xF1,0xE2,0x67,0xBE,0x40,0x8D,0x96,0x4D,0xF0,0x55,0x81,0xF2,0x23,0xDE},

  {0x35,0x28,0x4E,0xE2,0xAF,0xE8,0xED,0x39,0x8A,0xBD,0xC8,0x2B,0xBC,0x6B,0x18,0x64,
   0x99,0xA6,0x9A,0xA8,0xFC,0x01,0x78,0x20,0xFD,0x1C,0x6B,0x8F,0xAD,0x23,0xD9,0xF3,
   0x03,0x74,0xEB,0xD3,0x0F,0xF2,0x64,0xCD,0x10,0xD5,0xCB,0xD0,0xA5,0xFD,0xA8,0xF9,
   0x31,0x6F,0xF7,0xAF,0x12,0xE9,0xA9,0x5E,0xBC,0xFA,0xD6,0xEA,0xA1,0x84,0xB4,0x90,
   0x35,0x5E,0x51,0x4B,0xDE,0x0D,0xC2,0x98,0x29,0xB7,0x87,0x06,0x51,0xCE,0xB9,0xAF,
   0xC0,0x8A,0xAE,0x53,0xFC,0x6C,0x30,0xC3,0xAA,0x13,0xCC,0x4F,0xCE,0x20,0x28,0x1A,
   0xB8,0x5A,0x10,0xDE,0x30,0x6E,0xC3,0x55,0x70,0x64,0x7E,0x0B,0x05,0x4A,0xE6,0x15,
   0x72,0x00,0x76,0x44,0x3F,0x07,0x10,0x4B,0x7E,0xF1,0x7D,0x62,0x99,0xC9,0x55,0x6E},

  {0x1A,0xB0,0xBD,0x1A,0xB1,0xCA,0x31,0xE2,0xD9,0x59,0x1A,0x2D,0x43,0xA0,0x52,0x91,
   0x9E,0x73,0xF2,0x81,0x20,0x87,0xF4,0xDB,0x27,0x65,0x2C,0x3E,0x82,0x5D,0x4B,0x2A,
   0x05,0xD6,0xFF,0x86,0xF2,0x8A,0x09,0xDF,0x5C,0x82,0xD8,0x50,0x30,0xAC,0x19,0x6D,
   0xF1,0xC3,0x3E,0x56,0xC0,0x09,0x37,0xB4,0xEE,0x8F,0x19,0xB1,0x67,0x31,0xD7,0x02,
   0x0A,0x39,0xD2,0x47,0x0B,0x38,0x34,0x5B,0xA7,0x44,0xC4,0xA6,0x4D,0x02,0x9D,0x8C,
   0xD0,0x93,0xBF,0x86,0x2F,0xFB,0xD2,0x55,0xD9,0x98,0xE2,0xFD,0x9E,0xBB,0x1F,0xB9,
   0x3D,0x12,0xB0,0x9F,0x23,0x55,0xE8,0x3A,0xD1,0x0F,0xCE,0x53,0x07,0xA4,0x1E,0xED,
   0x31,0xFD,0xB5,0x97,0x13,0x5B,0xB8,0x28,0x0A,0xAF,0x3B,0xF9,0x15,0xD4,0xEE,0x36},

  {0x0D,0x7C,0xC4,0xE6,0xBE,0xDB,0x5F,0x8F,0x70,0xAB,0x73,0x2E,0x3C,0x45,0xF7,0xEB,
   0x1D,0x99,0x46,0x95,0xCE,0xC4,0xB2,0xA6,0xCA,0x59,0x8F,0xE6,0x15,0xE2,0x02,0x46,
   0x86,0x87,0xF5,0xAC,0x0C,0x36,0x3F,0x56,0x7A,0xA9,0x51,0x90,0x7A,0x04,0xC1,0xA7,
   0x91,0x95,0x5A,0xAA,0x29,0x79,0x78,0xC1,0xC7,0xB5,0xFE,0x1C,0x84,0x6B,0x66,0xCB,
   0x15,0x8A,0x13,0xC1,0x61,0xA2,0xCF,0x3A,0x60,0x3D,0x65,0x76,0x43,0x64,0x8F,0x9D,
   0x58,0x9F,0x37,0x6C,0xC6,0x30,0x23,0x1E,0xE0,0x5D,0x75,0xA4,0xB6,0xF6,0x84,0x68,
   0xFF,0xB6,0xE0,0xBF,0xAA,0xC8,0x7D,0x8D,0x01,0xBA,0x16,0x7F,0x06,0xD3,0x62,0x91,
   0x10,0x03,0x54,0x7E,0x85,0x75,0xEC,0x19,0xB0,0x80,0x18,0xB4,0xD3,0xDA,0x33,0x9A}
};

static const u8 sbas_ca_codes[NUM_SATS_SBAS][128] = {
// 120
  {0x6E,0x61,0xF0,0xB5,0xD5,0x02,0x6D,0x98,0x6E,0x06,0xA4,0xC1,0x87,0x74,0xC2,0xCF,
   0x0A,0x3C,0x55,0x7C,0x31,0x80,0xF3,0x67,0x25,0x59,0x00,0xED,0x21,0x0A,0xD7,0xA1,
   0xA4,0x6F,0x9B,0xF5,0x10,0xCE,0xAE,0x7F,0x92,0x9A,0xD7,0x93,0x8A,0xEA,0x50,0x35,
   0xC7,0x98,0xCC,0x56,0xA9,0x63,0x22,0x44,0xBB,0xA0,0xB5,0xF7,0x95,0xD7,0xDF,0x49,
   0x07,0x49,0x4F,0xE6,0x09,0xE2,0x79,0xA9,0x1C,0x93,0x90,0xA3,0x42,0x27,0x46,0xEB,
   0x81,0x4B,0x41,0x53,0x73,0x67,0x04,0x2B,0x7F,0xF4,0xEE,0xA7,0xA0,0x79,0x34,0xE2,
   0x29,0x73,0xF8,0xED,0xB6,0x6D,0x33,0xBB,0xE1,0xF6,0x28,0x06,0x93,0xB6,0x66,0x0F,
   0x28,0x85,0x0A,0x51,0x74,0xEC,0x01,0xF7,0xC7,0x73,0x6F,0xA1,0xC5,0xEE,0x3D,0x2E},
// 121
  {0x57,0xA4,0x67,0xEC,0xFD,0x4F,0xF6,0x49,0xAC,0xA2,0xF4,0xE1,0x0E,0xE9,0x57,0x78,
   0xDA,0x89,0x8D,0x50,0x30,0xF7,0x8F,0x1B,0x04,0xA7,0xE1,0x04,0xB7,0x24,0x83,0x90,
   0x1E,0x40,0xB8,0xB8,0xD6,0xC0,0x73,0xD3,0xBD,0x02,0x24,0x6B,0x94,0xC8,0x8C,0xEE,
   0x94,0x7C,0x1E,0x50,0x56,0xA8,0x68,0xB1,0xBC,0x75,0xDD,0xDE,0x49,0x51,0x53,0xB2,
   0xDD,0x30,0x08,0x57,0x26,0x9C,0xEB,0x70,0x1A,0x31,0x84,0xF6,0xBF,0x5F,0x2B,0xF9,
   0xCD,0x1C,0x52,0xD3,0xE6,0x36,0xB5,0xBF,0xBF,0x58,0x27,0x5E,0x72,0x99,0xAA,0x08,
   0x7B,0x06,0x37,0xB6,0x70,0xD8,0x50,0x29,0xDE,0x91,0xAB,0x0A,0x46,0x83,0x90,0xCC,
   0xA6,0x76,0x49,0xF1,0x68,0x20,0x25,0xD4,0xD7,0x6A,0x7E,0x74,0x16,0x79,0xDF,0xB8},
// 122
  {0xD2,0x0A,0xA6,0x8C,0xF6,0xF4,0x15,0xA8,0xA3,0x55,0xFB,0xAD,0xBF,0x9B,0xF6,0xF5,
   0x0B,0xF0,0x97,0x3E,0x00,0x81,0x62,0x7A,0x78,0x60,0x7E,0xC4,0x46,0x32,0x65,0x4A,
   0x81,0x66,0x28,0x8F,0x28,0x9A,0x24,0x7B,0x83,0xA9,0x65,0x25,0x70,0xFC,0xEB,0x30,
   0x47,0xB6,0x23,0xD9,0x2D,0x5C,0xFE,0x79,0x89,0x65,0x7F,0x71,0xA2,0x92,0x3B,0x20,
   0xBF,0x88,0x94,0x53,0x8C,0x11,0x67,0xD6,0x1F,0x57,0xCB,0x38,0x28,0x5B,0xDC,0xAB,
   0x5E,0xB2,0x28,0x0D,0xD3,0x9D,0xA9,0x2E,0x44,0x64,0x3F,0x38,0xDB,0x36,0x1C,0x14,
   0x0C,0x9C,0x2A,0xB6,0x96,0xB9,0x71,0x31,0x5E,0x59,0x25,0xA4,0xDA,0x3C,0x84,0x7F,
   0x3A,0x2F,0x3C,0x63,0xEC,0xF9,0x98,0xC8,0xFD,0x2F,0x7C,0x7C,0x8F,0xA0,0x84,0xC4},
// 123
  {0xD9,0x57,0x24,0x4C,0xE1,0x83,0xD2,0x6A,0xBC,0xBB,0xE5,0x34,0xDD,0x7E,0xB5,0xEE,
   0xA9,0x02,0xA3,0xE2,0x60,0x6C,0xB8,0xB8,0x81,0xEF,0x41,0x45,0xA4,0x1F,0xA8,0xFF,
   0xBF,0x2B,0x08,0xE0,0xD4,0x2E,0x8B,0x2B,0xFE,0xFF,0xE7,0xB8,0xB8,0x94,0x24,0x8D,
   0xE0,0x22,0x58,0xCB,0xDA,0xB5,0xD3,0xE9,0xE3,0x44,0x3A,0x2E,0x75,0x14,0xEA,0x04,
   0x7A,0xF9,0xAC,0x5A,0xD9,0x0A,0x7E,0x9A,0x15,0x9B,0x54,0xA5,0x06,0x52,0x32,0x0E,
   0x79,0xEE,0xDD,0xB1,0xB8,0xCB,0x90,0x0D,0xB2,0x1C,0x0F,0xF5,0x88,0x69,0x70,0x2C,
   0xE3,0xA8,0x10,0xB7,0x5A,0x7B,0x33,0x00,0x5F,0xC8,0x38,0xF9,0xE3,0x42,0xAD,0x18,
   0x02,0x9D,0xD7,0x46,0xE5,0x4A,0xE2,0xF0,0xA9,0xA5,0x78,0x6D,0xBC,0x12,0x32,0x3E},
// 124
  {0x1C,0x26,0x1C,0x45,0xB4,0x98,0xCB,0x26,0xB6,0x77,0x7A,0xA9,0xF3,0x77,0x5B,0x4B,
   0x8E,0x5E,0x56,0x5E,0x0B,0x3A,0x81,0x9B,0x77,0x97,0x71,0x88,0xF7,0x40,0xC4,0xC7,
   0x50,0x1F,0x32,0xE1,0x18,0xEC,0xC9,0x1A,0xFF,0x6E,0xFA,0xE5,0x81,0xEA,0x0D,0xEA,
   0xD8,0x90,0xB3,0xEE,0xD3,0x06,0xA9,0xD1,0xB7,0xCE,0x3E,0x3F,0x46,0xA6,0x5C,0xFE,
   0x6C,0x42,0xA9,0xDA,0xF7,0xE5,0xF1,0x1E,0x2A,0x47,0x69,0x97,0xC3,0x98,0xB4,0x39,
   0x3C,0x0A,0xB4,0x09,0x79,0x10,0x25,0x88,0x41,0x02,0x70,0xF6,0x4C,0x32,0xEB,0x46,
   0x9F,0x32,0x50,0x68,0xA3,0x12,0x6D,0xA0,0xA5,0x65,0x3D,0xC2,0x73,0x93,0x32,0x63,
   0x4D,0xB5,0x21,0x63,0x0A,0x98,0xB9,0xD0,0x7D,0xE7,0xF2,0xD2,0x13,0x1F,0x90,0x76},
// 125
  {0x70,0x43,0x2C,0x6E,0x86,0xE6,0xBF,0x9E,0x91,0xCD,0xEE,0x34,0x80,0xA1,0xAA,0xE6,
   0xD6,0xBA,0xD9,0x87,0xEB,0xF9,0xB4,0xD6,0x0A,0x27,0x92,0x34,0x27,0xA8,0xC3,0xF4,
   0x59,0x17,0xF6,0xE7,0x5C,0x99,0xBD,0xFF,0x07,0xCB,0x66,0x07,0x4A,0x46,0x99,0xCC,
   0x15,0x78,0x52,0xBC,0xFB,0x16,0xED,0x1F,0x7D,0x15,0xD6,0x65,0xAA,0xB0,0x5C,0x45,
   0xD2,0x7C,0xB9,0x25,0x87,0x11,0x3A,0x09,0x00,0x5F,0xB4,0x80,0x52,0x3E,0x57,0x3E,
   0x52,0xDC,0xA3,0xC5,0x00,0xEC,0x2A,0x99,0x2F,0x6F,0xD9,0x04,0x25,0x31,0x94,0xA3,
   0x3A,0x48,0xD3,0x82,0x17,0x06,0x83,0xE3,0xE3,0x18,0xD0,0xFE,0xD0,0x4B,0xA5,0xDC,
   0x04,0xD9,0xA2,0x32,0x00,0xB2,0x47,0x6C,0x4B,0x6E,0xD4,0xF9,0x9A,0xDF,0xDA,0xDE},
// 126
  {0x02,0xE2,0xA1,0xBF,0xFF,0xFC,0xD1,0xAE,0xE7,0x20,0x8F,0x48,0x47,0x3E,0x6A,0x17,
   0xC0,0x5C,0x03,0x18,0xCD,0x69,0x83,0x28,0x50,0x70,0x3A,0x0A,0x8C,0xE9,0x8D,0x10,
   0x6D,0x70,0x28,0x34,0x96,0xA4,0x34,0x84,0xF3,0x5D,0xAE,0x32,0x5A,0xE4,0x36,0x27,
   0xD6,0x10,0xC0,0xDE,0x43,0x8A,0xE9,0xB5,0xF0,0x99,0x70,0x60,0xCC,0xFF,0x92,0xD2,
   0xD6,0x8B,0xEB,0xB6,0x29,0x6B,0xE4,0x3C,0xAB,0x43,0x25,0x7B,0x6E,0x26,0x31,0x97,
   0xFD,0x6A,0xBF,0xB2,0x9A,0xF1,0x2A,0x7F,0x4E,0x4E,0xC1,0x84,0xED,0xBF,0x3A,0x3F,
   0x85,0x5C,0x60,0x1E,0x4E,0x73,0x11,0x67,0x3A,0xA5,0xCC,0xD4,0x95,0x49,0xAD,0x45,
   0xDD,0x82,0xDF,0x39,0x5D,0x30,0x87,0x7B,0x3C,0xA9,0x16,0xE6,0x74,0xC1,0x43,0xBC},
// 127
  {0x8C,0x11,0xE2,0x1F,0xE3,0x30,0xF5,0x8D,0xF7,0x39,0x9E,0x9D,0x94,0xA9,0x88,0x81,
   0xB3,0xD7,0x2D,0xAA,0x9D,0xF2,0xB4,0x8B,0xD5,0x38,0x9A,0x4B,0x9F,0xD2,0xA6,0x7F,
   0xCC,0x1B,0x98,0x6C,0x94,0x4A,0xCC,0x7C,0xB0,0xA0,0x6D,0xE1,0x76,0xB8,0x9E,0x44,
   0xA2,0x4E,0x86,0x45,0xCF,0x97,0x52,0xED,0xAF,0xA8,0x97,0x90,0xF0,0xBA,0x2B,0x64,
   0x71,0x42,0x4F,0xBB,0xD6,0xFD,0x71,0xD6,0xA4,0xE9,0xF5,0x28,0xD7,0x2B,0x28,0x60,
   0x49,0x98,0x30,0xD0,0xC4,0x0C,0x0F,0xCD,0x43,0x0A,0xE9,0x2F,0x17,0x4F,0xE0,0x1B,
   0x1D,0xF2,0x47,0x1F,0x64,0xD0,0x72,0x4E,0xBB,0xFC,0x5F,0x27,0x30,0x88,0x90,0x91,
   0x79,0x69,0x41,0x8E,0xD0,0x5A,0x40,0x5F,0x42,0x66,0x10,0xFF,0xDE,0xAA,0xAE,0x3A},
// 128
  {0x29,0x6B,0x61,0xF5,0x57,0x24,0x1B,0xCE,0xB1,0x44,0x73,0xA1,0x37,0x71,0x1F,0x3E,
   0xFA,0xBB,0xAE,0xFB,0x97,0xD8,0x4A,0x37,0xE3,0xB1,0xBC,0x60,0x16,0x12,0xEC,0xD7,
   0x14,0xD1,0xF8,0x3A,0xF0,0xB6,0x25,0x0C,0xFF,0xD5,0x44,0xDB,0x91,0x15,0x7D,0x1E,
   0x13,0x87,0x99,0xF6,0x0E,0x11,0x38,0x77,0x54,0xC9,0x50,0xE9,0x51,0x6A,0x25,0x02,
   0x63,0x53,0xC7,0xB7,0x5E,0x17,0x98,0x1D,0x55,0xA2,0xCC,0xED,0x40,0x72,0x00,0x2D,
   0xD2,0x49,0xF2,0x74,0x94,0x2C,0x86,0x50,0xD6,0xBD,0x39,0x9A,0xCF,0x63,0xE1,0x6C,
   0x61,0x8E,0x66,0xE1,0x85,0x39,0xE4,0x60,0xEF,0xCD,0xE5,0x08,0x13,0xC5,0x56,0x9F,
   0xA4,0xC5,0x6E,0x16,0x23,0xA2,0x5E,0x7D,0x9E,0xBD,0x43,0x1B,0x0C,0xAC,0x51,0xF0},
// 129
  {0x55,0xF1,0x21,0x2A,0xAE,0x4D,0x45,0x6E,0x4B,0xE9,0x76,0x9A,0xA7,0xA0,0x80,0x45,
   0xB5,0x93,0x58,0xDE,0x78,0x0A,0x11,0x17,0x37,0xF3,0x36,0xDF,0xB9,0x1F,0x4E,0x9E,
   0x9E,0x33,0x88,0x28,0x5A,0x80,0x17,0x94,0xEA,0x4C,0x7B,0xE1,0xCD,0x06,0xA0,0x54,
   0x5D,0x3E,0x72,0x8E,0xD8,0xBD,0x4A,0x30,0xB8,0x39,0x31,0x73,0xF7,0xD4,0xFD,0x73,
   0xBD,0x3B,0xB3,0xB4,0xC7,0x93,0x1C,0x7F,0x56,0x80,0xF6,0x57,0x32,0x8E,0x52,0xE3,
   0xA3,0x2C,0x24,0x3E,0x87,0x4A,0x72,0x20,0x7F,0xA9,0x31,0xB8,0xA8,0x06,0x8C,0x98,
   0x4C,0xF8,0x6D,0xE1,0xD8,0xE6,0xFB,0x68,0x90,0x75,0x9F,0x6D,0x98,0x50,0x5A,0xF1,
   0x2F,0x0D,0xBD,0x67,0xA0,0x15,0x35,0x76,0x78,0x81,0xBD,0x1C,0x84,0x1B,0x67,0x24},
// 130
  {0xC7,0x93,0x99,0xB6,0xAA,0xE7,0xC8,0xE2,0xED,0xEC,0x10,0xD5,0x69,0x37,0x84,0xB2,
   0xE7,0x00,0xF6,0xA4,0xA6,0x3F,0xBA,0x0B,0xA6,0x08,0x0A,0xC7,0xDF,0xB6,0xE1,0x28,
   0x82,0x44,0x12,0x35,0x5A,0x66,0x76,0xB5,0xF2,0xCC,0xB3,0x6F,0x63,0x9A,0x1F,0x40,
   0xEE,0xA2,0x2B,0xFB,0x4A,0x39,0x93,0x8D,0xA4,0x13,0x74,0x71,0xFF,0x4D,0x24,0x28,
   0xC0,0x30,0xEE,0x36,0x07,0x84,0x6B,0xB8,0x94,0x9F,0x18,0x49,0xAB,0xEC,0xB7,0xA0,
   0xB8,0x8E,0xD6,0x0A,0x5B,0x2A,0x9F,0xFA,0xBD,0x50,0xBE,0x87,0x29,0xE4,0xA1,0x55,
   0xF9,0xC6,0x20,0xC1,0xB7,0x1A,0x4F,0xC7,0xC0,0x08,0xC9,0xEF,0x05,0x98,0x32,0x3E,
   0x92,0xAA,0x29,0x1C,0xB2,0xE2,0xDC,0x5B,0xE8,0xEB,0x9C,0x59,0xDB,0xCC,0xE1,0xF4},
// 131
  {0xA5,0x90,0xBB,0x8C,0x10,0x95,0x34,0xB0,0x23,0x9D,0x75,0x03,0x23,0x24,0xE2,0x46,
   0x97,0xA9,0xE2,0xAC,0x84,0x58,0xDF,0x66,0x52,0x25,0x7C,0xCC,0xDF,0xEB,0x3E,0x37,
   0x8A,0x3B,0xAA,0x4F,0x3F,0xED,0xE3,0xB9,0x9C,0x47,0x7B,0xBC,0x12,0x19,0xA8,0x2B,
   0xE5,0x44,0x17,0x05,0x4D,0xB1,0x24,0xBB,0x70,0xEA,0x40,0xF0,0x40,0xBF,0xF6,0x95,
   0x81,0xC5,0xB4,0x3C,0x70,0xA5,0xC8,0x86,0x62,0x01,0x49,0xA5,0xE0,0x33,0x13,0x16,
   0xF9,0x26,0x53,0x1F,0x24,0x74,0x84,0xBE,0x2E,0x45,0x7A,0x67,0x0C,0xB0,0xCD,0x30,
   0xC9,0xED,0x12,0xBF,0xC3,0xA2,0x68,0x7D,0x54,0x95,0xBA,0x39,0xF4,0x35,0x6A,0xDA,
   0x1D,0x73,0xC9,0xDF,0x87,0xAF,0xA1,0xEB,0x0B,0x57,0x4C,0xB1,0xDC,0xFF,0xE8,0xFC},
// 132
  {0xAB,0xF6,0xA9,0x9D,0x98,0x99,0xBC,0x5A,0xCA,0x56,0x84,0x48,0x1A,0xE1,0x75,0x1F,
   0xBF,0xE4,0x79,0x7D,0x46,0xFC,0x8F,0x46,0xDB,0xB8,0xE9,0x7B,0x0F,0x5E,0xE6,0x1B,
   0x8B,0x4C,0xD6,0x33,0x1E,0x13,0x02,0x50,0x0A,0x69,0x2F,0x8D,0xA8,0x36,0x8B,0x66,
   0x23,0x4A,0xCA,0xA9,0x62,0x29,0xD7,0x43,0x6E,0xC8,0x9C,0x2B,0x13,0x5B,0x24,0x93,
   0x7E,0x0E,0xFE,0xC9,0x77,0x70,0xA0,0xAF,0xBE,0x87,0xC5,0x5E,0x3A,0x4A,0x54,0xA7,
   0xD6,0x58,0xC1,0xC6,0x22,0xD6,0x90,0xEB,0xD3,0x3D,0x17,0x75,0x40,0xE7,0xDE,0xB0,
   0x5C,0xBC,0xA3,0x2B,0x03,0x0E,0xA1,0x84,0x86,0x75,0x24,0xD3,0xA6,0x40,0xA5,0x81,
   0xDB,0xC6,0xAA,0x4D,0xB8,0xC8,0x22,0xE7,0xDE,0x62,0xBA,0x72,0x52,0x0C,0xAB,0x5C},
// 133
  {0x09,0xBF,0x23,0x7F,0xE8,0x8B,0x16,0x6C,0xF8,0xCE,0x91,0xD1,0x25,0xDB,0x29,0x0C,
   0x62,0xAE,0x37,0xC4,0xAD,0x84,0x59,0xEA,0xA9,0xFF,0x05,0x8B,0x6E,0xC4,0x40,0xA5,
   0x53,0x3D,0x08,0x5B,0x6A,0x10,0x9B,0xD4,0x8E,0x0B,0x2C,0xAF,0x92,0x8C,0xF9,0x9A,
   0x71,0x84,0xBB,0xCC,0xB4,0x63,0xC4,0x25,0x9A,0xB8,0x35,0x3F,0x1B,0x79,0x43,0xF6,
   0x13,0xFA,0xD3,0xBF,0x7C,0x70,0xFD,0x70,0xA1,0x8F,0xBA,0xE6,0x40,0x2F,0xDF,0x32,
   0xDA,0x36,0x4A,0x0E,0xF1,0xA7,0x13,0x5C,0xB8,0x36,0xF1,0x49,0xBE,0xE0,0x56,0x07,
   0x6A,0x68,0x5A,0x1F,0x82,0xB1,0x53,0x56,0x3B,0x34,0xD1,0x89,0xAC,0x37,0x84,0x22,
   0xE5,0x30,0x34,0x1C,0x54,0x83,0xFD,0x43,0x68,0x23,0x12,0xF7,0x47,0x73,0xF5,0x46},
// 134
  {0x8E,0x44,0xA4,0xD9,0xB0,0x32,0x46,0xAA,0x10,0x72,0x1C,0xE6,0x3D,0xE0,0x5F,0xBC,
   0xDC,0xCD,0xF8,0x24,0xD5,0x0F,0x2A,0x87,0xE6,0x6C,0x4D,0x90,0x91,0xE9,0x6B,0x71,
   0x4C,0x68,0xA8,0xFC,0x18,0x0A,0xA8,0x3B,0xE7,0xEE,0x32,0x6B,0x2F,0x76,0xB2,0xFE,
   0x6B,0x0C,0xEA,0x9B,0x41,0x82,0x70,0x6C,0xAB,0xE4,0x7B,0x3D,0x4E,0x3F,0x85,0xA5,
   0x11,0x49,0xF4,0x58,0x37,0xF2,0x86,0xD9,0xE8,0x58,0x87,0x89,0x5A,0xFA,0x51,0x7A,
   0x27,0xA8,0x46,0x3D,0xA5,0x70,0xC8,0x52,0x83,0xFB,0xFF,0xC9,0xCD,0xD0,0xC6,0x8B,
   0x2A,0x0C,0x1D,0x48,0xCC,0xEE,0xD9,0x0F,0xF5,0x18,0x6B,0x40,0xEE,0x5B,0x5A,0xAC,
   0xF0,0x12,0xB5,0x18,0x18,0x6F,0x50,0xFD,0xED,0x8D,0xD3,0x97,0x4C,0xC8,0x16,0xA6},
// 135
  {0x5C,0x42,0x5B,0x12,0xFA,0xC7,0x41,0x7F,0x78,0x5B,0xC0,0x60,0xB1,0x1B,0x85,0xC5,
   0x9B,0x7C,0xD7,0x5A,0x47,0xD6,0x2C,0x25,0xF2,0x39,0xB0,0xE8,0xFC,0xFB,0x27,0x26,
   0x5F,0xE8,0x3D,0xAD,0xA9,0x9E,0x68,0x97,0x2E,0x17,0xE0,0x8B,0xB1,0x9C,0xE0,0x8B,
   0xA4,0x57,0x2C,0x2E,0x22,0x10,0x4F,0x0B,0x28,0xE8,0xAE,0x08,0xB8,0xFC,0x0B,0x56,
   0x52,0xE9,0xE8,0x94,0x13,0xD1,0x96,0xC0,0xF9,0x8D,0x54,0x1E,0xB8,0x6C,0x22,0xF1,
   0x09,0x1A,0x16,0xA6,0x92,0xD3,0x4D,0x1A,0x23,0xBA,0xEC,0xF2,0xE6,0xBF,0x67,0xE0,
   0x9A,0x54,0x1F,0xA6,0x34,0x16,0x9A,0xF2,0x36,0xCB,0x47,0x1C,0x46,0x38,0x2E,0xF2,
   0xB6,0x89,0x39,0x05,0xA3,0x37,0x0F,0xCC,0x0A,0x7D,0xEF,0xD2,0xF5,0x7E,0xB1,0x6E},
// 136
  {0x87,0xF7,0xDE,0xE1,0xE4,0xB8,0x42,0xBB,0x23,0xC0,0xAA,0x1C,0x2B,0x5B,0x5A,0x3C,
   0xF2,0x22,0x77,0xA0,0xEA,0xD3,0x17,0xB5,0x23,0xA6,0xCB,0xA7,0xD4,0x0D,0x02,0xC9,
   0x8D,0xB3,0x1D,0x79,0xEB,0x14,0xD7,0x38,0x23,0xB5,0xA9,0x01,0x53,0xEC,0xF2,0x21,
   0x92,0x65,0xB4,0x3B,0xBB,0x2F,0x75,0x57,0x3B,0x35,0xE4,0x46,0x01,0x17,0x73,0x80,
   0xFE,0x9B,0xAF,0x78,0xE3,0xB0,0x0C,0x66,0x47,0x55,0x25,0xC0,0xD0,0x18,0x21,0x68,
   0x8D,0x9E,0x74,0xA5,0xB0,0xE9,0xF7,0x68,0xDF,0xE8,0x22,0x83,0x83,0x69,0x2D,0xF3,
   0xFC,0xA0,0x6F,0x0F,0x20,0x1E,0xB8,0x95,0x53,0xA6,0xB3,0x31,0x30,0x33,0x2E,0xAF,
   0x69,0x96,0x31,0x7A,0x1B,0x4D,0x6A,0x47,0x9F,0x71,0x81,0x59,0x3D,0xAD,0xC0,0xEC},
// 137
  {0x7E,0x25,0x3E,0x7F,0x0E,0xEA,0x37,0x74,0x78,0x06,0x1F,0x7F,0xB9,0x64,0x3D,0xBF,
   0xFE,0xF7,0x42,0x56,0x29,0x5D,0xE4,0xF6,0x83,0xBA,0x07,0x83,0xF7,0x1D,0x1B,0xD8,
   0x58,0x60,0x8A,0x9B,0x7D,0x67,0x5C,0x16,0x91,0xE5,0x32,0x36,0xF0,0x69,0xBA,0x81,
   0xD3,0x76,0x8F,0x10,0xD4,0x8E,0x1E,0xE7,0x63,0x37,0x0A,0xBE,0xF9,0x54,0x8E,0x43,
   0x2D,0xB7,0xF3,0xD0,0x80,0xC4,0x52,0x20,0xDC,0xD9,0x38,0x7B,0x88,0x47,0x10,0x8F,
   0x7D,0xA2,0x31,0x1C,0x06,0x4E,0x3E,0xCC,0xD2,0x17,0xB4,0x16,0x69,0x66,0x87,0x23,
   0xAF,0x19,0x62,0x16,0xD7,0xAA,0x4A,0x1A,0x31,0xF8,0x4E,0x14,0x82,0x3E,0x6A,0x87,
   0xC2,0x6C,0xC1,0xA0,0x3F,0xD5,0xC4,0x60,0x9E,0x5B,0x22,0x3A,0x14,0x2C,0x99,0x7E},
// 138
  {0xB5,0xD4,0x75,0x46,0xCB,0x7D,0x6E,0x5C,0x35,0x9D,0xCE,0xBD,0x1D,0x34,0x1D,0x36,
   0x63,0x62,0xF5,0x86,0x9C,0x85,0xC8,0xF7,0xF4,0xC6,0x7B,0xA2,0x09,0xFC,0xF2,0x4E,
   0x76,0x34,0xBB,0x21,0x52,0x44,0x11,0xD0,0x9F,0x38,0x9E,0x19,0x68,0x9A,0x42,0x9F,
   0xF1,0xAA,0x54,0x43,0x30,0x5C,0x18,0x18,0xA8,0x7D,0xFF,0xB9,0x2C,0x3C,0xA7,0x9F,
   0xAB,0x3B,0x08,0x0A,0xF9,0x83,0xE3,0x0F,0xA2,0x4B,0xE1,0x7D,0x2A,0x53,0x45,0x72,
   0x05,0xCF,0x23,0x50,0x51,0x5D,0xBE,0x59,0x83,0xA6,0x20,0xD6,0xC5,0xAF,0x7E,0xF1,
   0x4F,0x87,0x88,0x44,0xA2,0x65,0x11,0xDC,0x84,0x9B,0xDC,0x2B,0xE5,0xBD,0x66,0x52,
   0xF7,0x9A,0x02,0x2E,0xCC,0x96,0x64,0x7C,0x52,0x7F,0x01,0x2A,0x0D,0x3D,0x4C,0xAC}
};
