/*
MIT License

Copyright(c) 2018-2019 megai2

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#pragma once
#include "stdafx.h"

#define d912pxy_hlsl_generator_op_handler_group_size 97
#define d912pxy_hlsl_generator_op_handler_2_x 0
#define d912pxy_hlsl_generator_op_handler_3_x 1
#define d912pxy_hlsl_generator_op_handler_1_x 2
#define d912pxy_hlsl_generator_op_handler_cnt 3

#define d912pxy_hlsl_generator_max_code_lines 4096*10
#define d912pxy_hlsl_generator_head_priority_group_size 16
#define d912pxy_hlsl_generator_heado_offset 512
#define d912pxy_hlsl_generator_proc_predef_offset 1024
#define d912pxy_hlsl_generator_proc_offset 1324
#define d912pxy_hlsl_generator_max_line_length 1024

#define HLSL_HIO_PRIOG_POS 1
#define HLSL_HIO_PRIOG_TEXC 2
#define HLSL_HIO_PRIOG_NC 30
#define HLSL_HIO_PRIOG_END 31
#define HLSL_HIO_PRIORITY(a,b) d912pxy_hlsl_generator_head_priority_group_size * a + b

static const UINT HLSL_HIO_PRIOG_FROM_D3DDECLUSAGE[] = {
	1,//D3DDECLUSAGE_POSITION = 0,
	2,//D3DDECLUSAGE_BLENDWEIGHT,   // 1
	3,//D3DDECLUSAGE_BLENDINDICES,  // 2
	4,//D3DDECLUSAGE_NORMAL,        // 3
	5,//D3DDECLUSAGE_PSIZE,         // 4
	14,//D3DDECLUSAGE_TEXCOORD,      // 5
	6,//D3DDECLUSAGE_TANGENT,       // 6
	7,//D3DDECLUSAGE_BINORMAL,      // 7
	8,//D3DDECLUSAGE_TESSFACTOR,    // 8
	9,//D3DDECLUSAGE_POSITIONT,     // 9
	10,//D3DDECLUSAGE_COLOR,         // 10
	15,//D3DDECLUSAGE_FOG,           // 11
	12,//D3DDECLUSAGE_DEPTH,         // 12
	13//D3DDECLUSAGE_SAMPLE// 13
};

#define HLSL_GEN_WRITE_PROC_PD(fmt, ...) WriteProcLinePredef(fmt, __VA_ARGS__)
#define HLSL_GEN_WRITE_PROC(fmt, ...) WriteProcLine(fmt, __VA_ARGS__)
#define HLSL_GEN_WRITE_HEADI(prio, fmt, ...) WriteHeadILine(prio, fmt, __VA_ARGS__)
#define HLSL_GEN_WRITE_HEADO(prio, fmt, ...) WriteHeadOLine(prio, fmt, __VA_ARGS__)
#define HLSL_GEN_VTEXTURE_OFFSET 17

typedef struct d912pxy_hlsl_generator_regtext {
	char t[512];
} d912pxy_hlsl_generator_regtext;

class d912pxy_hlsl_generator : public d912pxy_noncom
{
public:
	d912pxy_hlsl_generator(DWORD* src, UINT len, wchar_t* ofn, d912pxy_shader_uid uid);
	~d912pxy_hlsl_generator();
	
	void Process();

	UINT GetRegType(DWORD dst);
	UINT GetRegNumber(DWORD op);
	UINT GetWriteMask(DWORD op);
	D3DSHADER_PARAM_SRCMOD_TYPE GetSrcMod(DWORD op);

	d912pxy_hlsl_generator_regtext FormatDstRegister(DWORD* reg);
	d912pxy_hlsl_generator_regtext FormatSrcRegister(DWORD* reg, UINT8 wm, UINT id, UINT haveDst, UINT allowFmtConvert);
	d912pxy_hlsl_generator_regtext FormatDstModifier(d912pxy_hlsl_generator_regtext statement, DWORD dstOp, UINT8 dstLen);
	d912pxy_hlsl_generator_regtext FormatDstModifierForSrc(d912pxy_hlsl_generator_regtext statement, DWORD dstOp, UINT8 dstLen);
	d912pxy_hlsl_generator_regtext FormatRightSide1(DWORD dstOp, const char* pre, const char* post, d912pxy_hlsl_generator_regtext op1, UINT8 dstLen);
	d912pxy_hlsl_generator_regtext FormatRightSide2(DWORD dstOp, const char* pre, const char* post, const char* mid, d912pxy_hlsl_generator_regtext op1, d912pxy_hlsl_generator_regtext op2, UINT8 dstLen);
	d912pxy_hlsl_generator_regtext FormatRightSide3(DWORD dstOp, const char* pre, const char* post, const char* mid[2], d912pxy_hlsl_generator_regtext op1, d912pxy_hlsl_generator_regtext op2, d912pxy_hlsl_generator_regtext op3, UINT8 dstLen);

	UINT64 FormatCmpString(DWORD op);
	UINT GetDstModifier(DWORD op);

	void WriteProcLinePredef(const char* fmt, ...);
	void WriteProcLine(const char* fmt, ...);
	void WriteHeadILine(UINT prio, const char* fmt, ...);
	void WriteHeadOLine(UINT prio, const char* fmt, ...);

	UINT GetDstLenByWriteMask(DWORD op);
	UINT GetDstLenByWriteMask2(DWORD wmask);

	const char* GetRegTypeStr(DWORD op, UINT8 proc);
	const char* GetUsageString(UINT usage, UINT type);
	UINT64 GetWriteMaskStr(DWORD op);
	UINT64 GetSwizzleStr(DWORD op, DWORD opDst);

	void CheckRegDefinition(DWORD op, UINT isDst);
	void DefineIOReg(DWORD op);

	void LoadBugDefs();

	UINT GetMaxShaderPassedVars();

private:
	void ProcSIO_DEF(DWORD* op);
	void ProcSIO_DCL(DWORD* op);	
	void ProcSIO_DP2(DWORD* op);
	void ProcSIO_DP3(DWORD* op);
	void ProcSIO_DP4(DWORD* op);
	void ProcSIO_TEXLD(DWORD* op);
	void ProcSIO_TEXLDL(DWORD* op);
	void ProcSIO_MUL(DWORD* op);
	void ProcSIO_MAD(DWORD* op);
	void ProcSIO_MOV(DWORD* op);
	void ProcSIO_REP(DWORD* op);
	void ProcSIO_ENDREP(DWORD* op);
	void ProcSIO_MAX(DWORD* op);
	void ProcSIO_MIN(DWORD* op);
	void ProcSIO_RCP(DWORD* op);
	void ProcSIO_CMP(DWORD* op);
	void ProcSIO_DP2ADD(DWORD* op);
	void ProcSIO_FRC(DWORD* op);
	void ProcSIO_POW(DWORD* op);
	void ProcSIO_RSQ(DWORD* op);
	void ProcSIO_NRM(DWORD* op);
	void ProcSIO_LOG(DWORD* op);
	void ProcSIO_EXP(DWORD* op);
	void ProcSIO_EXPP(DWORD* op);	
	void ProcSIO_TEXKILL(DWORD* op);
	void ProcSIO_IF(DWORD* op);
	void ProcSIO_ELSE(DWORD* op);
	void ProcSIO_ENDIF(DWORD* op);
	void ProcSIO_BREAK(DWORD* op);
	void ProcSIO_LRP(DWORD* op);
	void ProcSIO_SLT(DWORD* op);
	void ProcSIO_ABS(DWORD* op);
	void ProcSIO_SGE(DWORD* op);
	void ProcSIO_SGN(DWORD* op);
	void ProcSIO_SINCOS(DWORD* op);

	void ProcSIO_DOTX(DWORD* op, UINT sz);
	void ProcSIO_3OP(DWORD * op, const char * pre, const char * mid[2], const char * post);
	void ProcSIO_2OP(DWORD * op, const char * pre, const char * mid, const char * post);
	void ProcSIO_1OP(DWORD* op, const char* pre, const char* post);

	void ProcSIO_ADD(DWORD* op);
	void ProcSIO_UNK(DWORD* op);
	
	d912pxy_shader_uid mUID;
	UINT8 minVer;
	UINT8 majVer;
	UINT8 isPS;
	UINT8 PSpositionUsed;

	FILE * of;
	DWORD * oCode;
	UINT oLen;

	UINT procIdent;
	char* lines[d912pxy_hlsl_generator_max_code_lines];
	UINT headerOffsetO;
	UINT headerOffsetI;
	UINT procOffsetPredef;
	UINT procOffset;

	UINT8 forcePCFsampler[32];
	UINT genAlphatest;
	UINT genSRGBRead;
	UINT genSRGBWrite;
	UINT genVSClipplane0;

	UINT maxShaderPassedVars;

	UINT64 regDefined[(D3DSPR_PREDICATE + 1) * 32];

	void (d912pxy_hlsl_generator::*SIOhandlers[d912pxy_hlsl_generator_op_handler_group_size*d912pxy_hlsl_generator_op_handler_cnt])(DWORD* op);
	

};

