//----------------------------------------------------------------------------------
// # Project:	Zodea eMU Suite
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S
// # Hint:		[Aug 19 2011] This Script For Msg Digest V5 Class of the Application.
// # More Info: This Script has been Modified By WolF in [Nov 27 2011]
//----------------------------------------------------------------------------------
#ifndef MSG_DIGEST_V5_H
#define MSG_DIGEST_V5_H

#include "StdAfx.H"


class MsgDigestV5
{
public:
	MsgDigestV5();
	// -----
	MsgDigestV5(unsigned char *string);
	MsgDigestV5(istream& stream);
	MsgDigestV5(FILE *file);
	MsgDigestV5(ifstream& stream);
	// -----
	bool MD5_EncodeKeyVal
	(
		char *	lpszInputStr,
		char *	lpszOutputKeyVal,
		int		iKeyIndex
	);
	// -----
	bool MD5_EncodeString
	(
		char *	lpszInputStr,
		char *	lpszOutputStr,
		int		iKeyIndex
	);
	// -----
	bool MD5_CheckValue	
	(
		char *	lpszInputStr,
		char *	szKeyVal,
		int		iKeyIndex
	);
	// -----
	void		update(unsigned char *input, unsigned int input_length);
	void		update(istream& stream);
	void		update(FILE *file);
	void		update(ifstream& stream);
	void		finalize();
	// -----
	BYTE	*	raw_digest();
	char	*	hex_digest();
	// -----
	friend ostream&	operator<< (ostream&, MsgDigestV5 context);
	// -----
private:
	void		setmagicnum(int keyindex);
	BYTE		m_cRaw_digest[16];  
	char		m_cHex_digest[33];
	typedef unsigned int uint4;
	typedef unsigned short int uint2;
	typedef unsigned char uint1;
	// -----
	uint4		state[4];
	uint4		count[2];
	uint1		buffer[64];
	uint1		digest[16];
	uint1		finalized;
	// -----
	void		init();
	void		transform(uint1 *buffer);
	// -----
	static void encode(uint1 *dest, uint4 *src, uint4 length);
	static void decode(uint4 *dest, uint1 *src, uint4 length);
	static void memcpy(uint1 *dest, uint1 *src, uint4 length);
	static void memset(uint1 *start, uint1 val, uint4 length);
	// -----
    static inline uint4  rotate_left(uint4 x, uint4 n);
    static inline uint4  F(uint4 x, uint4 y, uint4 z);
    static inline uint4  G(uint4 x, uint4 y, uint4 z);
    static inline uint4  H(uint4 x, uint4 y, uint4 z);
    static inline uint4  I(uint4 x, uint4 y, uint4 z);
    static inline void   FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
    static inline void   GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void   HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void   II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
enum T_STATES
{
	S11		=	7,
	S12		=	12,
	S13		=	17,
	S14		=	22,
	S21		=	5,
	S22		=	9,
	S23		=	14,
	S24		=	20,
	S31		=	4,
	S32		=	11,
	S33		=	16,
	S34		=	23,
	S41		=	6,
	S42		=	10,
	S43		=	15,
	S44		=	21
};
// -----------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* MSG_DIGEST_V5_H */
// -----------------------------------------------------------------------------------------------------------------------------------------------------