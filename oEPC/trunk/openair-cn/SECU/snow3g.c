/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
   included in this distribution in the file called "COPYING". If not,
   see <http://www.gnu.org/licenses/>.

  Contact Information
  OpenAirInterface Admin: openair_admin@eurecom.fr
  OpenAirInterface Tech : openair_tech@eurecom.fr
  OpenAirInterface Dev  : openair4g-devel@eurecom.fr

  Address      : Eurecom, Compus SophiaTech 450, route des chappes, 06451 Biot, France.

 *******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "rijndael.h"
#include "snow3g.h"

static uint8_t  _MULx(uint8_t V, uint8_t c);
static uint8_t  _MULxPOW(uint8_t V, uint8_t i, uint8_t c);
static uint32_t _MULalpha(uint8_t c);
static uint32_t _DIValpha(uint8_t c);
static uint32_t _S1(uint32_t w);
static uint32_t _S2(uint32_t w);
static void     _snow3g_clock_LFSR_initialization_mode(uint32_t F, snow_3g_context_t *s3g_ctx_pP);
static void     _snow3g_clock_LFSR_key_stream_mode(snow_3g_context_t *snow_3g_context_pP);
static uint32_t _snow3g_clock_fsm(snow_3g_context_t *snow_3g_context_pP);
void snow3g_initialize(uint32_t k[4], uint32_t IV[4], snow_3g_context_t *snow_3g_context_pP);
void snow3g_generate_key_stream(uint32_t n, uint32_t *ks, snow_3g_context_t *snow_3g_context_pP);


/* _MULx.
* Input V: an 8-bit input.
* Input c: an 8-bit input.
* Output : an 8-bit input.
* MULx maps 16 bits to 8 bits
*/

static uint8_t _MULx(uint8_t V, uint8_t c)
{
	//If the leftmost (i.e. the most significant) bit of V equals 1
	if ( V & 0x80 )
		return ( (V << 1) ^ c);
	else
		return ( V << 1);
}

/* _MULxPOW.
* Input V: an 8-bit input.
* Input i: a positive integer.
* Input c: an 8-bit input.
* Output : an 8-bit output.
* MULxPOW maps 16 bits and a positive integer i to 8 bit.
*/

static uint8_t _MULxPOW(uint8_t V, uint8_t i, uint8_t c)
{
	if ( i == 0)
		return V;
	else
		return _MULx( _MULxPOW( V, i-1, c ), c);
}

/* The function _MULalpha.
* Input c: 8-bit input.
* Output : 32-bit output.
* maps 8 bits to 32 bits.
*/

static uint32_t _MULalpha(uint8_t c)
{
	return 	( ( ((uint32_t)_MULxPOW(c,23, 0xa9)) << 24 ) |
		( ((uint32_t)_MULxPOW(c, 245, 0xa9)) << 16 ) |
		( ((uint32_t)_MULxPOW(c, 48, 0xa9)) << 8 ) |
		( ((uint32_t)_MULxPOW(c, 239, 0xa9))	)	) ;
}


/* The function DIV alpha.
* Input c: 8-bit input.
* Output : 32-bit output.
* maps 8 bits to 32 bit.
*/

static uint32_t _DIValpha(uint8_t c)
{
	return 	( ( ((uint32_t)_MULxPOW(c, 16, 0xa9)) << 24 ) |
		( ((uint32_t)_MULxPOW(c, 39, 0xa9)) << 16 ) |
		( ((uint32_t)_MULxPOW(c, 6, 0xa9)) << 8 ) |
		( ((uint32_t)_MULxPOW(c, 64, 0xa9))	)	) ;
}

/* The 32x32-bit S-Box S1
* Input: a 32-bit input.
* Output: a 32-bit output of S1 box.
* The S-Box S1 maps a 32-bit input to a 32-bit output.
* w = w0 || w1 || w2 || w3 the 32-bit input with w0 the most and w3 the least significant byte.
* S1(w)= r0 || r1 || r2 || r3 with r0 the most and r3 the least significant byte.
*/

static uint32_t _S1(uint32_t w)
{
	uint8_t r0=0, r1=0, r2=0, r3=0;
	uint8_t srw0 = SR[ (uint8_t)((w >> 24)	& 0xff) ];
	uint8_t srw1 = SR[ (uint8_t)((w >> 16)	& 0xff) ];
	uint8_t srw2 = SR[ (uint8_t)((w >> 8)	& 0xff) ];
	uint8_t srw3 = SR[ (uint8_t)((w)		& 0xff) ];

	r0 = 	( ( _MULx( srw0 , 0x1b) ) ^
		( srw1 ) ^
		( srw2 ) ^
		( (_MULx( srw3, 0x1b)) ^ srw3 )
		);

	r1 = 	( ( ( _MULx( srw0 , 0x1b) ) ^ srw0 ) ^
		( _MULx(srw1, 0x1b) ) ^
		( srw2 ) ^
		( srw3 )
		);

	r2 = 	( ( srw0 ) ^
		( ( _MULx( srw1 , 0x1b) ) ^ srw1 ) ^
		( _MULx(srw2, 0x1b) ) ^
		( srw3 )
		);

	r3 = 	( ( srw0 ) ^
		( srw1 ) ^
		( ( _MULx( srw2 , 0x1b) ) ^ srw2 ) ^
		( _MULx( srw3, 0x1b) )
		);

	return ( ( ((uint32_t)r0) << 24 ) | ( ((uint32_t)r1) << 16 ) | ( ((uint32_t)r2) << 8 ) | ( ((uint32_t)r3) ) );
}

/* The 32x32-bit S-Box S2
* Input: a 32-bit input.
* Output: a 32-bit output of S2 box.
* The S-Box S2 maps a 32-bit input to a 32-bit output.
* Let w = w0 || w1 || w2 || w3 the 32-bit input with w0 the most and w3 the least significant byte.
* Let S2(w)= r0 || r1 || r2 || r3 with r0 the most and r3 the least significant byte.
*/

static uint32_t _S2(uint32_t w)
{
	uint8_t r0=0, r1=0, r2=0, r3=0;
	uint8_t sqw0 = SQ[ (uint8_t)((w >> 24) 	& 0xff) ];
	uint8_t sqw1 = SQ[ (uint8_t)((w >> 16) 	& 0xff) ];
	uint8_t sqw2 = SQ[ (uint8_t)((w >> 8) 	& 0xff) ];
	uint8_t sqw3 = SQ[ (uint8_t)((w)		& 0xff) ];


	r0 =	( ( _MULx( sqw0 , 0x69) ) ^
		( sqw1 ) ^
		( sqw2 ) ^
		( (_MULx( sqw3, 0x69)) ^ sqw3 )
		);

	r1 = 	( ( ( _MULx( sqw0 , 0x69) ) ^ sqw0 ) ^
		( _MULx(sqw1, 0x69) ) ^
		( sqw2 ) ^
		( sqw3 )
		);

	r2 = 	( ( sqw0 ) ^
		( ( _MULx( sqw1 , 0x69) ) ^ sqw1 ) ^
		( _MULx(sqw2, 0x69) ) ^
		( sqw3 )
		);

	r3 = 	( ( sqw0 ) ^
		( sqw1 ) ^
		( ( _MULx( sqw2 , 0x69) ) ^ sqw2 ) ^
		( _MULx( sqw3, 0x69) )
		);

	return ( ( ((uint32_t)r0) << 24 ) | ( ((uint32_t)r1) << 16 ) | ( ((uint32_t)r2) << 8 ) | ( ((uint32_t)r3) ) );
}

/* Clocking LFSR in initialization mode.
* LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
* Input F: a 32-bit word comes from output of FSM.
* See section 3.4.4.
*/

static void _snow3g_clock_LFSR_initialization_mode(uint32_t F, snow_3g_context_t *s3g_ctx_pP)
{
	uint32_t v = ( ( (s3g_ctx_pP->LFSR_S0 << 8) & 0xffffff00 ) 		^
		( _MULalpha( (uint8_t)((s3g_ctx_pP->LFSR_S0>>24) & 0xff) ) )	^
		( s3g_ctx_pP->LFSR_S2 )					^
		( (s3g_ctx_pP->LFSR_S11 >> 8) & 0x00ffffff )		^
		( _DIValpha( (uint8_t)( ( s3g_ctx_pP->LFSR_S11) & 0xff ) ) )	^
		( F )
	);

	s3g_ctx_pP->LFSR_S0  = s3g_ctx_pP->LFSR_S1;
	s3g_ctx_pP->LFSR_S1  = s3g_ctx_pP->LFSR_S2;
	s3g_ctx_pP->LFSR_S2  = s3g_ctx_pP->LFSR_S3;
	s3g_ctx_pP->LFSR_S3  = s3g_ctx_pP->LFSR_S4;
	s3g_ctx_pP->LFSR_S4  = s3g_ctx_pP->LFSR_S5;
	s3g_ctx_pP->LFSR_S5  = s3g_ctx_pP->LFSR_S6;
	s3g_ctx_pP->LFSR_S6  = s3g_ctx_pP->LFSR_S7;
	s3g_ctx_pP->LFSR_S7  = s3g_ctx_pP->LFSR_S8;
	s3g_ctx_pP->LFSR_S8  = s3g_ctx_pP->LFSR_S9;
	s3g_ctx_pP->LFSR_S9  = s3g_ctx_pP->LFSR_S10;
	s3g_ctx_pP->LFSR_S10 = s3g_ctx_pP->LFSR_S11;
	s3g_ctx_pP->LFSR_S11 = s3g_ctx_pP->LFSR_S12;
	s3g_ctx_pP->LFSR_S12 = s3g_ctx_pP->LFSR_S13;
	s3g_ctx_pP->LFSR_S13 = s3g_ctx_pP->LFSR_S14;
	s3g_ctx_pP->LFSR_S14 = s3g_ctx_pP->LFSR_S15;
	s3g_ctx_pP->LFSR_S15 = v;
}

/* Clocking LFSR in keystream mode.
* LFSR Registers S0 to S15 are updated as the LFSR receives a single clock.
* See section 3.4.5.
*/
static void _snow3g_clock_LFSR_key_stream_mode(snow_3g_context_t *snow_3g_context_pP)
{

	uint32_t v = ( ( (snow_3g_context_pP->LFSR_S0 << 8) & 0xffffff00 )		^
		( _MULalpha( (uint8_t)((snow_3g_context_pP->LFSR_S0>>24) & 0xff) ) )	^
		( snow_3g_context_pP->LFSR_S2 )					^
		( (snow_3g_context_pP->LFSR_S11 >> 8) & 0x00ffffff )		^
		( _DIValpha( (uint8_t)( ( snow_3g_context_pP->LFSR_S11) & 0xff ) ) )
	);

	snow_3g_context_pP->LFSR_S0  = snow_3g_context_pP->LFSR_S1;
	snow_3g_context_pP->LFSR_S1  = snow_3g_context_pP->LFSR_S2;
	snow_3g_context_pP->LFSR_S2  = snow_3g_context_pP->LFSR_S3;
	snow_3g_context_pP->LFSR_S3  = snow_3g_context_pP->LFSR_S4;
	snow_3g_context_pP->LFSR_S4  = snow_3g_context_pP->LFSR_S5;
	snow_3g_context_pP->LFSR_S5  = snow_3g_context_pP->LFSR_S6;
	snow_3g_context_pP->LFSR_S6  = snow_3g_context_pP->LFSR_S7;
	snow_3g_context_pP->LFSR_S7  = snow_3g_context_pP->LFSR_S8;
	snow_3g_context_pP->LFSR_S8  = snow_3g_context_pP->LFSR_S9;
	snow_3g_context_pP->LFSR_S9  = snow_3g_context_pP->LFSR_S10;
	snow_3g_context_pP->LFSR_S10 = snow_3g_context_pP->LFSR_S11;
	snow_3g_context_pP->LFSR_S11 = snow_3g_context_pP->LFSR_S12;
	snow_3g_context_pP->LFSR_S12 = snow_3g_context_pP->LFSR_S13;
	snow_3g_context_pP->LFSR_S13 = snow_3g_context_pP->LFSR_S14;
	snow_3g_context_pP->LFSR_S14 = snow_3g_context_pP->LFSR_S15;
	snow_3g_context_pP->LFSR_S15 = v;
}

/* Clocking FSM.
* Produces a 32-bit word F.
* Updates FSM registers R1, R2, R3.
* See Section 3.4.6.
*/

static uint32_t _snow3g_clock_fsm(snow_3g_context_t *snow_3g_context_pP)
{
	uint32_t F = ( ( snow_3g_context_pP->LFSR_S15 + snow_3g_context_pP->FSM_R1 ) & 0xffffffff ) ^ snow_3g_context_pP->FSM_R2 ;
	uint32_t r = ( snow_3g_context_pP->FSM_R2 + ( snow_3g_context_pP->FSM_R3 ^ snow_3g_context_pP->LFSR_S5 ) ) & 0xffffffff ;

	snow_3g_context_pP->FSM_R3 = _S2(snow_3g_context_pP->FSM_R2);
	snow_3g_context_pP->FSM_R2 = _S1(snow_3g_context_pP->FSM_R1);
	snow_3g_context_pP->FSM_R1 = r;

	return F;
}

/*	Initialization.
 *	Input k[4]: Four 32-bit words making up 128-bit key.
 *	Input IV[4]: Four 32-bit words making 128-bit initialization variable.
 *	Output: All the LFSRs and FSM are initialized for key generation.
 *	See Section 4.1.
 */

void snow3g_initialize(uint32_t k[4], uint32_t IV[4], snow_3g_context_t *snow_3g_context_pP)
{

	uint8_t  i = 0;
	uint32_t F = 0x0;

	snow_3g_context_pP->LFSR_S15 = k[3] ^ IV[0];
	snow_3g_context_pP->LFSR_S14 = k[2];
	snow_3g_context_pP->LFSR_S13 = k[1];
	snow_3g_context_pP->LFSR_S12 = k[0] ^ IV[1];

	snow_3g_context_pP->LFSR_S11 = k[3]	^ 0xffffffff;
	snow_3g_context_pP->LFSR_S10 = k[2]	^ 0xffffffff ^ IV[2];
	snow_3g_context_pP->LFSR_S9	 = k[1]	^ 0xffffffff ^ IV[3];
	snow_3g_context_pP->LFSR_S8	 = k[0]	^ 0xffffffff;

	snow_3g_context_pP->LFSR_S7	 = k[3];
	snow_3g_context_pP->LFSR_S6	 = k[2];
	snow_3g_context_pP->LFSR_S5	 = k[1];
	snow_3g_context_pP->LFSR_S4	 = k[0];

	snow_3g_context_pP->LFSR_S3	 = k[3] ^ 0xffffffff;
	snow_3g_context_pP->LFSR_S2  = k[2] ^ 0xffffffff;
	snow_3g_context_pP->LFSR_S1  = k[1] ^ 0xffffffff;
	snow_3g_context_pP->LFSR_S0  = k[0] ^ 0xffffffff;

	snow_3g_context_pP->FSM_R1	 = 0x0;
	snow_3g_context_pP->FSM_R2	 = 0x0;
	snow_3g_context_pP->FSM_R3	 = 0x0;

	for(i=0;i<32;i++)
	{
		F = _snow3g_clock_fsm(snow_3g_context_pP);
		_snow3g_clock_LFSR_initialization_mode(F, snow_3g_context_pP);
	}

}

/* 	Generation of Keystream.
*	input n: number of 32-bit words of keystream.
* 	input z: space for the generated keystream, assumes
* 	memory is allocated already.
* 	output: generated keystream which is filled in z
* 	See section 4.2.
*/

void snow3g_generate_key_stream(uint32_t n, uint32_t *ks, snow_3g_context_t *snow_3g_context_pP)
{
	uint32_t t = 0;
	uint32_t F = 0x0;

	_snow3g_clock_fsm(snow_3g_context_pP); /* Clock FSM once. Discard the output. */
	_snow3g_clock_LFSR_key_stream_mode(snow_3g_context_pP); /* Clock LFSR in keystream mode once. */

	for ( t=0; t<n; t++)
	{
		F = _snow3g_clock_fsm(snow_3g_context_pP);	/* STEP 1 */
		ks[t] = F ^ snow_3g_context_pP->LFSR_S0; /* STEP 2 */

		/* Note that ks[t] corresponds to z_{t+1} in section 4.2*/

		_snow3g_clock_LFSR_key_stream_mode(snow_3g_context_pP); /* STEP 3 */
	}
}

