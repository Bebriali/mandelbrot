__m128 _mm_add_ps (__m128 a, __m128 b)
    Synopsis
            __m128 _mm_add_ps (__m128 a, __m128 b)
            #include <xmmintrin.h>
            Instruction: addps xmm, xmm
            CPUID Flags: SSE
    Description
            Add packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.
__m128 _mm_sub_ps (__m128 a, __m128 b)
    Synopsis
        __m128 _mm_sub_ps (__m128 a, __m128 b)
        #include <xmmintrin.h>
        Instruction: subps xmm, xmm
        CPUID Flags: SSE
    Description
        Subtract packed single-precision (32-bit) floating-point elements in b from packed single-precision (32-bit) floating-point elements in a, and store the results in dst.
__m128 _mm_div_ps (__m128 a, __m128 b)
    Synopsis
        __m128 _mm_div_ps (__m128 a, __m128 b)
        #include <xmmintrin.h>
        Instruction: divps xmm, xmm
        CPUID Flags: SSE
    Description
        Divide  packed single-precision (32-bit) floating-point elements in a by packed elements in b, and store the results in dst.
__m128 _mm_mul_ps (__m128 a, __m128 b)
    Synopsis
        __m128 _mm_mul_ps (__m128 a, __m128 b)
        #include <xmmintrin.h>
        Instruction: mulps xmm, xmm
        CPUID Flags: SSE
    Description
        Multiply packed single-precision (32-bit) floating-point elements in a and b, and store the results in dst.

__m128 _mm_cmp_ps (__m128 a, __m128 b, const int imm8)
    Synopsis
        __m128 _mm_cmp_ps (__m128 a, __m128 b, const int imm8)
        #include <immintrin.h>
        Instruction: vcmpps xmm, xmm, xmm, imm8
        CPUID Flags: AVX
    Description
        Compare packed single-precision (32-bit) floating-point elements in a and b based on the comparison operand specified by imm8, and store the results in dst.
        Operation
        CASE (imm8[4:0]) OF
        0: OP := _CMP_EQ_OQ
        1: OP := _CMP_LT_OS
        2: OP := _CMP_LE_OS
        3: OP := _CMP_UNORD_Q
        4: OP := _CMP_NEQ_UQ
        5: OP := _CMP_NLT_US
        6: OP := _CMP_NLE_US
        7: OP := _CMP_ORD_Q
        8: OP := _CMP_EQ_UQ
        9: OP := _CMP_NGE_US
        10: OP := _CMP_NGT_US
        11: OP := _CMP_FALSE_OQ
        12: OP := _CMP_NEQ_OQ
        13: OP := _CMP_GE_OS
        14: OP := _CMP_GT_OS
        15: OP := _CMP_TRUE_UQ
        16: OP := _CMP_EQ_OS
        17: OP := _CMP_LT_OQ
        18: OP := _CMP_LE_OQ
        19: OP := _CMP_UNORD_S
        20: OP := _CMP_NEQ_US
        21: OP := _CMP_NLT_UQ
        22: OP := _CMP_NLE_UQ
        23: OP := _CMP_ORD_S
        24: OP := _CMP_EQ_US
        25: OP := _CMP_NGE_UQ
        26: OP := _CMP_NGT_UQ
        27: OP := _CMP_FALSE_OS
        28: OP := _CMP_NEQ_OS
        29: OP := _CMP_GE_OQ
        30: OP := _CMP_GT_OQ
        31: OP := _CMP_TRUE_US
        ESAC
        FOR j := 0 to 3
        	i := j*32
        	dst[i+31:i] := ( a[i+31:i] OP b[i+31:i] ) ? 0xFFFFFFFF : 0
        ENDFOR
        // dst[MAX:128] := 0

__m128 _mm_set_ps (float e3, float e2, float e1, float e0)
Synopsis
__m128 _mm_set_ps (float e3, float e2, float e1, float e0)
#include <xmmintrin.h>
Instruction: Sequence
CPUID Flags: SSE
Description
Set packed single-precision (32-bit) floating-point elements in dst with the supplied values.
Operation
dst[31:0] := e0
dst[63:32] := e1
dst[95:64] := e2
dst[127:96] := e3
