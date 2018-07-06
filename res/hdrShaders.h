#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer cb
// {
//
//   float brightnessScale;             // Offset:    0 Size:     4
//   bool wcg;                          // Offset:    4 Size:     4
//   bool enabled;                      // Offset:    8 Size:     4
//   float pad3;                        // Offset:   12 Size:     4 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// linearTex                         texture  float4          2d             t0      1 
// originalTex                       texture  float4          2d             t1      1 
// outHdrTex                             UAV  float4          2d             u0      1 
// cb                                cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[1], immediateIndexed
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_uav_typed_texture2d (float,float,float,float) u0
dcl_input vThreadID.xy
dcl_temps 4
dcl_thread_group 8, 8, 1
if_z cb0[0].z
  mov r0.xy, vThreadID.xyxx
  mov r0.zw, l(0,0,0,0)
  ld_indexable(texture2d)(float,float,float,float) r0.xyzw, r0.xyzw, t1.xyzw
  lt r1.xyz, r0.xyzx, l(0.040450, 0.040450, 0.040450, 0.000000)
  mul r2.xyz, r0.xyzx, l(0.077399, 0.077399, 0.077399, 0.000000)
  add r3.xyz, r0.xyzx, l(0.055000, 0.055000, 0.055000, 0.000000)
  mul r3.xyz, r3.xyzx, l(0.947867, 0.947867, 0.947867, 0.000000)
  log r3.xyz, r3.xyzx
  mul r3.xyz, r3.xyzx, l(2.400000, 2.400000, 2.400000, 0.000000)
  exp r3.xyz, r3.xyzx
  movc r1.xyz, r1.xyzx, r2.xyzx, r3.xyzx
  dp3 r2.x, l(0.627402, 0.329292, 0.043306, 0.000000), r1.xyzx
  dp3 r2.y, l(0.069095, 0.919544, 0.011360, 0.000000), r1.xyzx
  dp3 r2.z, l(0.016394, 0.088028, 0.895578, 0.000000), r1.xyzx
  mul r1.xyz, r2.xyzx, l(0.018000, 0.018000, 0.018000, 0.000000)
  log r1.xyz, r1.xyzx
  mul r1.xyz, r1.xyzx, l(0.159302, 0.159302, 0.159302, 0.000000)
  exp r1.xyz, r1.xyzx
  mad r2.xyz, r1.xyzx, l(18.851563, 18.851563, 18.851563, 0.000000), l(0.835938, 0.835938, 0.835938, 0.000000)
  mad r1.xyz, r1.xyzx, l(18.687500, 18.687500, 18.687500, 0.000000), l(1.000000, 1.000000, 1.000000, 0.000000)
  div r1.xyz, r2.xyzx, r1.xyzx
  log r1.xyz, r1.xyzx
  mul r1.xyz, r1.xyzx, l(78.843750, 78.843750, 78.843750, 0.000000)
  exp r0.xyz, r1.xyzx
else 
  mov r1.xy, vThreadID.xyxx
  mov r1.zw, l(0,0,0,0)
  ld_indexable(texture2d)(float,float,float,float) r1.xyzw, r1.xyzw, t0.xyzw
  mul r0.xyzw, r1.xyzw, cb0[0].xxxx
  dp3 r1.x, l(0.627402, 0.329292, 0.043306, 0.000000), r0.xyzx
  dp3 r1.y, l(0.069095, 0.919544, 0.011360, 0.000000), r0.xyzx
  dp3 r1.z, l(0.016394, 0.088028, 0.895578, 0.000000), r0.xyzx
  movc r1.xyz, cb0[0].yyyy, r0.xyzx, r1.xyzx
  log r1.xyz, r1.xyzx
  mul r1.xyz, r1.xyzx, l(0.159302, 0.159302, 0.159302, 0.000000)
  exp r1.xyz, r1.xyzx
  mad r2.xyz, r1.xyzx, l(18.851563, 18.851563, 18.851563, 0.000000), l(0.835938, 0.835938, 0.835938, 0.000000)
  mad r1.xyz, r1.xyzx, l(18.687500, 18.687500, 18.687500, 0.000000), l(1.000000, 1.000000, 1.000000, 0.000000)
  div r1.xyz, r2.xyzx, r1.xyzx
  log r1.xyz, r1.xyzx
  mul r1.xyz, r1.xyzx, l(78.843750, 78.843750, 78.843750, 0.000000)
  exp r0.xyz, r1.xyzx
endif 
store_uav_typed u0.xyzw, vThreadID.xyyy, r0.xyzw
ret 
// Approximately 46 instruction slots used
#endif

const BYTE copyShader[] =
{
     68,  88,  66,  67,  28, 119, 
    152, 226,  18,  79, 185,  13, 
    195, 127, 246, 231, 153, 241, 
    167, 147,   1,   0,   0,   0, 
     96,   9,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    120,   2,   0,   0, 136,   2, 
      0,   0, 152,   2,   0,   0, 
    196,   8,   0,   0,  82,  68, 
     69,  70,  60,   2,   0,   0, 
      1,   0,   0,   0, 224,   0, 
      0,   0,   4,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
     17,   2,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    188,   0,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0, 198,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 210,   0, 
      0,   0,   4,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    220,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 108, 105, 110, 101, 
     97, 114,  84, 101, 120,   0, 
    111, 114, 105, 103, 105, 110, 
     97, 108,  84, 101, 120,   0, 
    111, 117, 116,  72, 100, 114, 
     84, 101, 120,   0,  99,  98, 
      0, 171, 220,   0,   0,   0, 
      4,   0,   0,   0, 248,   0, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 152,   1,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    176,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    212,   1,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 224,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   4,   2, 
      0,   0,   8,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 224,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  12,   2,   0,   0, 
     12,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    176,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     98, 114, 105, 103, 104, 116, 
    110, 101, 115, 115,  83,  99, 
     97, 108, 101,   0, 102, 108, 
    111,  97, 116,   0, 171, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 168,   1,   0,   0, 
    119,  99, 103,   0,  98, 111, 
    111, 108,   0, 171, 171, 171, 
      0,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 216,   1,   0,   0, 
    101, 110,  97,  98, 108, 101, 
    100,   0, 112,  97, 100,  51, 
      0,  77, 105,  99, 114, 111, 
    115, 111, 102, 116,  32,  40, 
     82,  41,  32,  72,  76,  83, 
     76,  32,  83, 104,  97, 100, 
    101, 114,  32,  67, 111, 109, 
    112, 105, 108, 101, 114,  32, 
     49,  48,  46,  49,   0, 171, 
    171, 171,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88,  36,   6,   0,   0, 
     80,   0,   5,   0, 137,   1, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   1,   0, 
      0,   0,  85,  85,   0,   0, 
    156,  24,   0,   4,   0, 224, 
     17,   0,   0,   0,   0,   0, 
     85,  85,   0,   0,  95,   0, 
      0,   2,  50,   0,   2,   0, 
    104,   0,   0,   2,   4,   0, 
      0,   0, 155,   0,   0,   4, 
      8,   0,   0,   0,   8,   0, 
      0,   0,   1,   0,   0,   0, 
     31,   0,   0,   4,  42, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   4,  50,   0,  16,   0, 
      0,   0,   0,   0,  70,   0, 
      2,   0,  54,   0,   0,   8, 
    194,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  45,   0, 
      0, 137, 194,   0,   0, 128, 
     67,  85,  21,   0, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 126,  16,   0, 
      1,   0,   0,   0,  49,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0, 230, 174, 
     37,  61, 230, 174,  37,  61, 
    230, 174,  37,  61,   0,   0, 
      0,   0,  56,   0,   0,  10, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 145, 131, 158,  61, 
    145, 131, 158,  61, 145, 131, 
    158,  61,   0,   0,   0,   0, 
      0,   0,   0,  10, 114,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
    174,  71,  97,  61, 174,  71, 
     97,  61, 174,  71,  97,  61, 
      0,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0, 111, 167, 
    114,  63, 111, 167, 114,  63, 
    111, 167, 114,  63,   0,   0, 
      0,   0,  47,   0,   0,   5, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      3,   0,   0,   0,  70,   2, 
     16,   0,   3,   0,   0,   0, 
      2,  64,   0,   0, 154, 153, 
     25,  64, 154, 153,  25,  64, 
    154, 153,  25,  64,   0,   0, 
      0,   0,  25,   0,   0,   5, 
    114,   0,  16,   0,   3,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  55,   0, 
      0,   9, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      3,   0,   0,   0,  16,   0, 
      0,  10,  18,   0,  16,   0, 
      2,   0,   0,   0,   2,  64, 
      0,   0, 107, 157,  32,  63, 
    246, 152, 168,  62, 162,  97, 
     49,  61,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  16,   0,   0,  10, 
     34,   0,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
    174, 129, 141,  61,  60, 103, 
    107,  63,  75,  31,  58,  60, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,  10,  66,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 182,  76, 
    134,  60,   6,  72, 180,  61, 
    154,  68, 101,  63,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 188, 116, 
    147,  60, 188, 116, 147,  60, 
    188, 116, 147,  60,   0,   0, 
      0,   0,  47,   0,   0,   5, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,  32, 
     35,  62,   0,  32,  35,  62, 
      0,  32,  35,  62,   0,   0, 
      0,   0,  25,   0,   0,   5, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  15, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0, 208, 
    150,  65,   0, 208, 150,  65, 
      0, 208, 150,  65,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,  86,  63,   0,   0, 
     86,  63,   0,   0,  86,  63, 
      0,   0,   0,   0,  50,   0, 
      0,  15, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0, 128, 
    149,  65,   0, 128, 149,  65, 
      0, 128, 149,  65,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,  14,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  47,   0,   0,   5, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0, 176, 
    157,  66,   0, 176, 157,  66, 
      0, 176, 157,  66,   0,   0, 
      0,   0,  25,   0,   0,   5, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  18,   0, 
      0,   1,  54,   0,   0,   4, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,   0,   2,   0, 
     54,   0,   0,   8, 194,   0, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  45,   0,   0, 137, 
    194,   0,   0, 128,  67,  85, 
     21,   0, 242,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 126,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,  10,  18,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 107, 157,  32,  63, 
    246, 152, 168,  62, 162,  97, 
     49,  61,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  16,   0,   0,  10, 
     34,   0,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
    174, 129, 141,  61,  60, 103, 
    107,  63,  75,  31,  58,  60, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     16,   0,   0,  10,  66,   0, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0, 182,  76, 
    134,  60,   6,  72, 180,  61, 
    154,  68, 101,  63,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  55,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  86, 133, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  47,   0,   0,   5, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,  32, 
     35,  62,   0,  32,  35,  62, 
      0,  32,  35,  62,   0,   0, 
      0,   0,  25,   0,   0,   5, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  15, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0, 208, 
    150,  65,   0, 208, 150,  65, 
      0, 208, 150,  65,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,  86,  63,   0,   0, 
     86,  63,   0,   0,  86,  63, 
      0,   0,   0,   0,  50,   0, 
      0,  15, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0, 128, 
    149,  65,   0, 128, 149,  65, 
      0, 128, 149,  65,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,  14,   0, 
      0,   7, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  47,   0,   0,   5, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0, 176, 
    157,  66,   0, 176, 157,  66, 
      0, 176, 157,  66,   0,   0, 
      0,   0,  25,   0,   0,   5, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  21,   0, 
      0,   1, 164,   0,   0,   6, 
    242, 224,  17,   0,   0,   0, 
      0,   0,  70,   5,   2,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  46,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     33,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0
};
