// SPDX-FileCopyrightText: Copyright (C) ARDUINO SRL (http://www.arduino.cc)
//
// SPDX-License-Identifier: MPL-2.0

// Example sketch using Arduino_LED_Matrix and RouterBridge. This sketch
// exposes four providers:
//  - "draw" which accepts a std::vector<uint8_t> (by-value) and calls matrix.draw()
//  - "load_frame" which loads frame data into animation buffer
//  - "play_animation" which starts playback of loaded animation frames
//  - "stop_animation" which halts any running animation
#include <Arduino_RouterBridge.h>
#include <Arduino_LED_Matrix.h>
#include <vector>

// Scrolling: "il pulcino purtroppo non lo posso fare per via di questo mal di schiena che mi perseguita"
// 257 frames, step=2px, 65ms/frame
// Animation: Animation
const uint32_t animation[][5] = {
    {0x00000000, 0x00000000, 0x00000000, 0x00000000, 65},  // Frame 1
    {0x00000000, 0x10000000, 0x00001000, 0x00000000, 65},  // Frame 2
    {0x00200000, 0x70020020, 0x02007000, 0x00000000, 65},  // Frame 3
    {0x00800001, 0xc0080080, 0x0801c000, 0x00000000, 65},  // Frame 4
    {0x02100007, 0x00200200, 0x20071000, 0x00000000, 65},  // Frame 5
    {0x0860021c, 0x20820820, 0x821c7000, 0x00000000, 65},  // Frame 6
    {0x21800870, 0x82082082, 0x0871c000, 0x00000000, 65},  // Frame 7
    {0x860020c2, 0x08208208, 0x20c70000, 0x00000000, 65},  // Frame 8
    {0x18008008, 0x00800800, 0x801c0000, 0x00000000, 65},  // Frame 9
    {0x60020120, 0x22022032, 0x02702000, 0x00000000, 65},  // Frame 10
    {0x80080780, 0x880880f8, 0x08c08000, 0x00000000, 65},  // Frame 11
    {0x00001e02, 0x202203c0, 0x20020000, 0x00000000, 65},  // Frame 12
    {0x00007808, 0xa08a0f20, 0x82081000, 0x00000000, 65},  // Frame 13
    {0x0001e022, 0x82283c82, 0x08207000, 0x00000000, 65},  // Frame 14
    {0x0007808a, 0x28a2f228, 0x2281e000, 0x00000000, 65},  // Frame 15
    {0x001e0028, 0x8288c880, 0x88079000, 0x00000000, 65},  // Frame 16
    {0x006802a2, 0x2a222222, 0x221e7000, 0x00000000, 65},  // Frame 17
    {0x01800888, 0x88888888, 0x8879c000, 0x00000000, 65},  // Frame 18
    {0x06002022, 0x12222222, 0x22e71000, 0x00000000, 65},  // Frame 19
    {0x18008088, 0x78888888, 0x889c7000, 0x00000000, 65},  // Frame 20
    {0x60020021, 0xc2202202, 0x2071c000, 0x00000000, 65},  // Frame 21
    {0x80080087, 0x18808808, 0x80c71000, 0x00000000, 65},  // Frame 22
    {0x0020001c, 0x72022022, 0x021c7000, 0x00000000, 65},  // Frame 23
    {0x00800071, 0xc8088088, 0x0871c000, 0x00000000, 65},  // Frame 24
    {0x020000c7, 0x20230220, 0x22c72000, 0x00000000, 65},  // Frame 25
    {0x0800001c, 0xb08c0880, 0x881c8000, 0x00000000, 65},  // Frame 26
    {0x20000072, 0xc2322222, 0x22722000, 0x00000000, 65},  // Frame 27
    {0x800000cb, 0x18ca88a8, 0x8ac89000, 0x00000000, 65},  // Frame 28
    {0x0000002c, 0x73282282, 0x28227000, 0x00000000, 65},  // Frame 29
    {0x000000b1, 0xcca28a28, 0xa289c000, 0x00000000, 65},  // Frame 30
    {0x000000c7, 0x02882882, 0x88270000, 0x00000000, 65},  // Frame 31
    {0x0000001c, 0x0a20a20a, 0x209c0000, 0x00000000, 65},  // Frame 32
    {0x00000170, 0x28828838, 0x82702000, 0x00000000, 65},  // Frame 33
    {0x000007c0, 0x820820f2, 0x08c08000, 0x00000000, 65},  // Frame 34
    {0x00001e02, 0x282283c8, 0x20020000, 0x00000000, 65},  // Frame 35
    {0x00007808, 0xa08a0f20, 0x82081000, 0x00000000, 65},  // Frame 36
    {0x0001e022, 0x82283c82, 0x08207000, 0x00000000, 65},  // Frame 37
    {0x0007808a, 0x28a2f228, 0x2281e000, 0x00000000, 65},  // Frame 38
    {0x000e0028, 0xa28bc8a0, 0x8a07a000, 0x00000000, 65},  // Frame 39
    {0x000800a2, 0xba2c2282, 0x281e8000, 0x00000000, 65},  // Frame 40
    {0x0000008a, 0xc8b28a08, 0xa07a0000, 0x00000000, 65},  // Frame 41
    {0x0000002b, 0x32c82802, 0x80e80000, 0x00000000, 65},  // Frame 42
    {0x000002ac, 0xfb22a02a, 0x02a01000, 0x00000000, 65},  // Frame 43
    {0x000008b3, 0xec888088, 0x08806000, 0x00000000, 65},  // Frame 44
    {0x000020cf, 0xa2230220, 0x2201a000, 0x00000000, 65},  // Frame 45
    {0x0000803e, 0xb88c0880, 0x88068000, 0x00000000, 65},  // Frame 46
    {0x000200fa, 0xc2322202, 0x201a0000, 0x00000000, 65},  // Frame 47
    {0x000800eb, 0x18ca8828, 0x82681000, 0x00000000, 65},  // Frame 48
    {0x000000ac, 0x73282082, 0x08a07000, 0x00000000, 65},  // Frame 49
    {0x000000b1, 0xcca28228, 0x2281c000, 0x00000000, 65},  // Frame 50
    {0x000001c7, 0x228a08b0, 0x8a072000, 0x00000000, 65},  // Frame 51
    {0x0000071c, 0x8a2822f2, 0x281c8000, 0x00000000, 65},  // Frame 52
    {0x00001e72, 0x28a28bc8, 0xa0720000, 0x00000000, 65},  // Frame 53
    {0x000079c8, 0xa28a2f32, 0x82c82000, 0x00000000, 65},  // Frame 54
    {0x0001e722, 0x8a28bcfa, 0x08208000, 0x00000000, 65},  // Frame 55
    {0x00079e8a, 0x28a2f3c8, 0x20820000, 0x00000000, 65},  // Frame 56
    {0x000e7828, 0x928acf20, 0x82081000, 0x00000000, 65},  // Frame 57
    {0x0009e0a2, 0x7a283c82, 0x08207000, 0x00000000, 65},  // Frame 58
    {0x00078089, 0xc8a2f228, 0x2281c000, 0x00000000, 65},  // Frame 59
    {0x000e0027, 0x0288c880, 0x88070000, 0x00000000, 65},  // Frame 60
    {0x0008009c, 0x0a202202, 0x201c0000, 0x00000000, 65},  // Frame 61
    {0x00000070, 0x28838828, 0x82702000, 0x00000000, 65},  // Frame 62
    {0x000000c0, 0xb20c2082, 0x08c08000, 0x00000000, 65},  // Frame 63
    {0x00000002, 0xc8328228, 0x22022000, 0x00000000, 65},  // Frame 64
    {0x0000000b, 0x10ca08a0, 0x8a089000, 0x00000000, 65},  // Frame 65
    {0x0000002c, 0x73282282, 0x28227000, 0x00000000, 65},  // Frame 66
    {0x000000b1, 0xcca28a28, 0xa289c000, 0x00000000, 65},  // Frame 67
    {0x000000c7, 0x228b28a2, 0x8a272000, 0x00000000, 65},  // Frame 68
    {0x0000001c, 0xba2ca28a, 0x289c8000, 0x00000000, 65},  // Frame 69
    {0x00000072, 0xc8b28a28, 0xa2722000, 0x00000000, 65},  // Frame 70
    {0x000000cb, 0x02c82882, 0x88c88000, 0x00000000, 65},  // Frame 71
    {0x0000002c, 0x0b20a20a, 0x20220000, 0x00000000, 65},  // Frame 72
    {0x001000b0, 0x0c808808, 0x80881000, 0x00000000, 65},  // Frame 73
    {0x006002c0, 0x22022022, 0x02207000, 0x00000000, 65},  // Frame 74
    {0x01800800, 0x88088088, 0x0881c000, 0x00000000, 65},  // Frame 75
    {0x06002002, 0x10220220, 0x22071000, 0x00000000, 65},  // Frame 76
    {0x18008008, 0x70880880, 0x881c7000, 0x00000000, 65},  // Frame 77
    {0x60020021, 0xc2222222, 0x2271c000, 0x00000000, 65},  // Frame 78
    {0x80080087, 0x08888888, 0x88c70000, 0x00000000, 65},  // Frame 79
    {0x0000001c, 0x02202202, 0x201c0000, 0x00000000, 65},  // Frame 80
    {0x00000170, 0x28828838, 0x82702000, 0x00000000, 65},  // Frame 81
    {0x000007c0, 0x820820f2, 0x08c08000, 0x00000000, 65},  // Frame 82
    {0x00001e02, 0x282283c8, 0x20020000, 0x00000000, 65},  // Frame 83
    {0x00007808, 0x908a0f20, 0x82081000, 0x00000000, 65},  // Frame 84
    {0x0001e022, 0x72283c82, 0x08207000, 0x00000000, 65},  // Frame 85
    {0x00078089, 0xc8a2f228, 0x2281c000, 0x00000000, 65},  // Frame 86
    {0x000e0027, 0x128ac890, 0x88073000, 0x00000000, 65},  // Frame 87
    {0x0008009c, 0x7a282272, 0x201cf000, 0x00000000, 65},  // Frame 88
    {0x00000071, 0xe8a089c8, 0x8273c000, 0x00000000, 65},  // Frame 89
    {0x000000c7, 0x92822712, 0x08cf3000, 0x00000000, 65},  // Frame 90
    {0x0000001e, 0x7a089c78, 0x203cf000, 0x00000000, 65},  // Frame 91
    {0x00000079, 0xe82071c0, 0x82f3c000, 0x00000000, 65},  // Frame 92
    {0x000000e7, 0x9082c722, 0x0acf1000, 0x00000000, 65},  // Frame 93
    {0x0000009e, 0x72081c88, 0x283c7000, 0x00000000, 65},  // Frame 94
    {0x00000079, 0xc8227220, 0xa2f1c000, 0x00000000, 65},  // Frame 95
    {0x000000e7, 0x0088c882, 0x88c70000, 0x00000000, 65},  // Frame 96
    {0x0000009c, 0x0220220a, 0x201c0000, 0x00000000, 65},  // Frame 97
    {0x00000070, 0x18838818, 0x81701000, 0x00000000, 65},  // Frame 98
    {0x000003c0, 0x420f2042, 0x04c04000, 0x00000000, 65},  // Frame 99
    {0x00000c01, 0x083c8108, 0x10010000, 0x00000000, 65},  // Frame 100
    {0x00003004, 0x10f00410, 0x42041000, 0x00000000, 65},  // Frame 101
    {0x0000c010, 0x73c01071, 0x08107000, 0x00000000, 65},  // Frame 102
    {0x00030041, 0xcf0241e4, 0x2241e000, 0x00000000, 65},  // Frame 103
    {0x000c0007, 0x2c0b07a0, 0x8a07a000, 0x00000000, 65},  // Frame 104
    {0x0000001c, 0xb02c1e82, 0x281e8000, 0x00000000, 65},  // Frame 105
    {0x00000072, 0xc0b27a08, 0xa07a0000, 0x00000000, 65},  // Frame 106
    {0x000000cb, 0x12cae832, 0x82e81000, 0x00000000, 65},  // Frame 107
    {0x0000002c, 0x7b28a0fa, 0x08a07000, 0x00000000, 65},  // Frame 108
    {0x000000b1, 0xcca283e8, 0x2081c000, 0x00000000, 65},  // Frame 109
    {0x000000c7, 0x02880f80, 0x80070000, 0x00000000, 65},  // Frame 110
    {0x0000001c, 0x0a203e02, 0x001c0000, 0x00000000, 65},  // Frame 111
    {0x00000170, 0x2882f838, 0x02702000, 0x00000000, 65},  // Frame 112
    {0x000007c0, 0x8208e0f0, 0x08c08000, 0x00000000, 65},  // Frame 113
    {0x00001e02, 0x282283c0, 0x20020000, 0x00000000, 65},  // Frame 114
    {0x00007808, 0x908a0f30, 0x82081000, 0x00000000, 65},  // Frame 115
    {0x0001e022, 0x72283cf2, 0x08207000, 0x00000000, 65},  // Frame 116
    {0x00078089, 0xc8a2f3e8, 0x2081c000, 0x00000000, 65},  // Frame 117
    {0x000e0027, 0x228bcfa0, 0x82072000, 0x00000000, 65},  // Frame 118
    {0x0008009c, 0xba2c3e82, 0x081c8000, 0x00000000, 65},  // Frame 119
    {0x00000072, 0xc8b2fa08, 0x20720000, 0x00000000, 65},  // Frame 120
    {0x000000cb, 0x02c8e800, 0x80c80000, 0x00000000, 65},  // Frame 121
    {0x0000002c, 0x0b20a002, 0x00200000, 0x00000000, 65},  // Frame 122
    {0x000000b0, 0x2c828018, 0x01800000, 0x00000000, 65},  // Frame 123
    {0x000000c0, 0x82080050, 0x05002000, 0x00000000, 65},  // Frame 124
    {0x00000002, 0x28220140, 0x14008000, 0x00000000, 65},  // Frame 125
    {0x00000008, 0x90880500, 0x50021000, 0x00000000, 65},  // Frame 126
    {0x00200022, 0x72221421, 0x42087000, 0x00000000, 65},  // Frame 127
    {0x00800089, 0xc8885085, 0x0821c000, 0x00000000, 65},  // Frame 128
    {0x02000027, 0x12204214, 0x22871000, 0x00000000, 65},  // Frame 129
    {0x0800009c, 0x78800870, 0x881c7000, 0x00000000, 65},  // Frame 130
    {0x20000071, 0xc20221e2, 0x2271e000, 0x00000000, 65},  // Frame 131
    {0x800000c7, 0x08088788, 0x88c78000, 0x00000000, 65},  // Frame 132
    {0x0000001c, 0x00201e02, 0x201e0000, 0x00000000, 65},  // Frame 133
    {0x00000070, 0x10827828, 0x82781000, 0x00000000, 65},  // Frame 134
    {0x000000c0, 0x7208e082, 0x08e07000, 0x00000000, 65},  // Frame 135
    {0x00200201, 0xe8228228, 0x2281e000, 0x00000000, 65},  // Frame 136
    {0x00800807, 0x90880880, 0x88079000, 0x00000000, 65},  // Frame 137
    {0x0220201e, 0x72222222, 0x221e7000, 0x00000000, 65},  // Frame 138
    {0x08808079, 0xc8888888, 0x8879c000, 0x00000000, 65},  // Frame 139
    {0x220200e7, 0x02202202, 0x20e70000, 0x00000000, 65},  // Frame 140
    {0x8808009c, 0x08808808, 0x809c0000, 0x00000000, 65},  // Frame 141
    {0x20000170, 0x22022012, 0x00700000, 0x00000000, 65},  // Frame 142
    {0x800007c0, 0x88088078, 0x00c00000, 0x00000000, 65},  // Frame 143
    {0x00001e02, 0x202201e0, 0x02002000, 0x00000000, 65},  // Frame 144
    {0x00007808, 0xa08a07a0, 0x0a009000, 0x00000000, 65},  // Frame 145
    {0x0001e022, 0x82281e80, 0x28027000, 0x00000000, 65},  // Frame 146
    {0x0007808a, 0x28a27a20, 0xa209e000, 0x00000000, 65},  // Frame 147
    {0x000e0028, 0x928ae8b2, 0x8a279000, 0x00000000, 65},  // Frame 148
    {0x000800a2, 0x7a28a2fa, 0x289e7000, 0x00000000, 65},  // Frame 149
    {0x00000089, 0xc8a28be8, 0xa079c000, 0x00000000, 65},  // Frame 150
    {0x00000027, 0x128a2f92, 0x80e73000, 0x00000000, 65},  // Frame 151
    {0x0000009c, 0x7a28be7a, 0x009cf000, 0x00000000, 65},  // Frame 152
    {0x00000071, 0xe8a0f9c8, 0x0273c000, 0x00000000, 65},  // Frame 153
    {0x000000c7, 0xb280e700, 0x08cf0000, 0x00000000, 65},  // Frame 154
    {0x0000021e, 0xfa029c20, 0x223c1000, 0x00000000, 65},  // Frame 155
    {0x0000087b, 0xe8087080, 0x88f06000, 0x00000000, 65},  // Frame 156
    {0x000020ef, 0x9022c222, 0x22c19000, 0x00000000, 65},  // Frame 157
    {0x000080be, 0x70880888, 0x88067000, 0x00000000, 65},  // Frame 158
    {0x000200f9, 0xc2222222, 0x2219c000, 0x00000000, 65},  // Frame 159
    {0x000800e7, 0x08888888, 0x88670000, 0x00000000, 65},  // Frame 160
    {0x0000009c, 0x02202202, 0x209c0000, 0x00000000, 65},  // Frame 161
    {0x00000070, 0x28838828, 0x82702000, 0x00000000, 65},  // Frame 162
    {0x000000c0, 0xb20d20a2, 0x0ac0a000, 0x00000000, 65},  // Frame 163
    {0x00000002, 0xc83682a8, 0x2a02a000, 0x00000000, 65},  // Frame 164
    {0x0000000b, 0x10d80a90, 0xaa0a9000, 0x00000000, 65},  // Frame 165
    {0x0000002c, 0x73602a72, 0xa82a7000, 0x00000000, 65},  // Frame 166
    {0x000000b1, 0xcd82a9ea, 0xa2a9e000, 0x00000000, 65},  // Frame 167
    {0x001000c7, 0x0608a78a, 0x88a79000, 0x00000000, 65},  // Frame 168
    {0x0060021c, 0x28229e2a, 0x229e7000, 0x00000000, 65},  // Frame 169
    {0x01800870, 0x80887888, 0x8879c000, 0x00000000, 65},  // Frame 170
    {0x060020c2, 0x0220e202, 0x20e70000, 0x00000000, 65},  // Frame 171
    {0x18008008, 0x08808808, 0x809c0000, 0x00000000, 65},  // Frame 172
    {0x60020020, 0x12022022, 0x02701000, 0x00000000, 65},  // Frame 173
    {0x80080080, 0x78088088, 0x08c07000, 0x00000000, 65},  // Frame 174
    {0x00200201, 0xe0220220, 0x2201e000, 0x00000000, 65},  // Frame 175
    {0x00800807, 0x90880880, 0x88079000, 0x00000000, 65},  // Frame 176
    {0x0220201e, 0x72222222, 0x221e7000, 0x00000000, 65},  // Frame 177
    {0x08808079, 0xc8888888, 0x8879c000, 0x00000000, 65},  // Frame 178
    {0x220200e7, 0x02202202, 0x20e70000, 0x00000000, 65},  // Frame 179
    {0x8808009c, 0x08808808, 0x809c0000, 0x00000000, 65},  // Frame 180
    {0x20000070, 0x12022012, 0x00703000, 0x00000000, 65},  // Frame 181
    {0x800000c0, 0x78088078, 0x00c0f000, 0x00000000, 65},  // Frame 182
    {0x00000001, 0xe02001c0, 0x0203c000, 0x00000000, 65},  // Frame 183
    {0x00000007, 0x90820720, 0x0a0f1000, 0x00000000, 65},  // Frame 184
    {0x0000001e, 0x72081c80, 0x283c7000, 0x00000000, 65},  // Frame 185
    {0x00000079, 0xc8207200, 0xa0f1c000, 0x00000000, 65},  // Frame 186
    {0x002002e7, 0x2083c822, 0x82c72000, 0x00000000, 65},  // Frame 187
    {0x0080089c, 0xb20c208a, 0x081c8000, 0x00000000, 65},  // Frame 188
    {0x02002072, 0xc8328228, 0x22722000, 0x00000000, 65},  // Frame 189
    {0x080080cb, 0x10c80880, 0x88c89000, 0x00000000, 65},  // Frame 190
    {0x2022002c, 0x73222222, 0x22227000, 0x00000000, 65},  // Frame 191
    {0x808800b1, 0xcc888888, 0x8889c000, 0x00000000, 65},  // Frame 192
    {0x020000c7, 0x12222232, 0x22271000, 0x00000000, 65},  // Frame 193
    {0x0800001c, 0x788888f8, 0x889c7000, 0x00000000, 65},  // Frame 194
    {0x20000071, 0xc22223e2, 0x2071c000, 0x00000000, 65},  // Frame 195
    {0x800000c7, 0x288b8fa8, 0x82c72000, 0x00000000, 65},  // Frame 196
    {0x0000001c, 0xb22c3e82, 0x081c8000, 0x00000000, 65},  // Frame 197
    {0x00000072, 0xc8b2fa28, 0x22722000, 0x00000000, 65},  // Frame 198
    {0x000000cb, 0x12c8e890, 0x8ac89000, 0x00000000, 65},  // Frame 199
    {0x0000002c, 0x7b20a272, 0x28227000, 0x00000000, 65},  // Frame 200
    {0x000000b1, 0xcc8289e8, 0xa289e000, 0x00000000, 65},  // Frame 201
    {0x000000c7, 0x02082782, 0x88278000, 0x00000000, 65},  // Frame 202
    {0x0000001c, 0x08209e0a, 0x209e0000, 0x00000000, 65},  // Frame 203
    {0x00000070, 0x10827828, 0x82781000, 0x00000000, 65},  // Frame 204
    {0x000000c0, 0x7208e082, 0x08e07000, 0x00000000, 65},  // Frame 205
    {0x00000001, 0xc8208208, 0x2081c000, 0x00000000, 65},  // Frame 206
    {0x00200207, 0x20830820, 0x82072000, 0x00000000, 65},  // Frame 207
    {0x0080081c, 0xb20c2082, 0x081c8000, 0x00000000, 65},  // Frame 208
    {0x02002072, 0xc8328228, 0x22722000, 0x00000000, 65},  // Frame 209
    {0x080080cb, 0x10ca08b0, 0x8ac89000, 0x00000000, 65},  // Frame 210
    {0x2002002c, 0x732822f2, 0x28227000, 0x00000000, 65},  // Frame 211
    {0x800800b1, 0xcca28be8, 0xa089c000, 0x00000000, 65},  // Frame 212
    {0x000000c7, 0x02882f82, 0x80270000, 0x00000000, 65},  // Frame 213
    {0x0000001c, 0x0a20be0a, 0x009c0000, 0x00000000, 65},  // Frame 214
    {0x00000070, 0x2883f828, 0x02702000, 0x00000000, 65},  // Frame 215
    {0x000000c0, 0xb20de0a0, 0x0ac0a000, 0x00000000, 65},  // Frame 216
    {0x00000002, 0xc83682a0, 0x2a02a000, 0x00000000, 65},  // Frame 217
    {0x0000000b, 0x10d80a80, 0xa80a9000, 0x00000000, 65},  // Frame 218
    {0x0020002c, 0x73622a22, 0xa22a7000, 0x00000000, 65},  // Frame 219
    {0x008000b1, 0xcd88a88a, 0x88a9c000, 0x00000000, 65},  // Frame 220
    {0x020000c7, 0x0620a20a, 0x20a70000, 0x00000000, 65},  // Frame 221
    {0x0800001c, 0x08808808, 0x809c0000, 0x00000000, 65},  // Frame 222
    {0x20000170, 0x22022032, 0x02702000, 0x00000000, 65},  // Frame 223
    {0x800007c0, 0x880880f8, 0x08c08000, 0x00000000, 65},  // Frame 224
    {0x00001e02, 0x202203c0, 0x20020000, 0x00000000, 65},  // Frame 225
    {0x00007808, 0x908a0f30, 0x82081000, 0x00000000, 65},  // Frame 226
    {0x0001e022, 0x72283cf2, 0x08207000, 0x00000000, 65},  // Frame 227
    {0x00078089, 0xc8a2f3e8, 0x2081c000, 0x00000000, 65},  // Frame 228
    {0x000e0027, 0x228bcfa0, 0x82072000, 0x00000000, 65},  // Frame 229
    {0x0008009c, 0xba2c3e82, 0x081c8000, 0x00000000, 65},  // Frame 230
    {0x00000072, 0xc8b2fa08, 0x20720000, 0x00000000, 65},  // Frame 231
    {0x000000cb, 0x12cae810, 0x80c83000, 0x00000000, 65},  // Frame 232
    {0x0000002c, 0x7b28a072, 0x0020f000, 0x00000000, 65},  // Frame 233
    {0x000000b1, 0xeca081c8, 0x0283c000, 0x00000000, 65},  // Frame 234
    {0x000000c7, 0x92820730, 0x0a0f1000, 0x00000000, 65},  // Frame 235
    {0x0000001e, 0x7a081cf0, 0x283c7000, 0x00000000, 65},  // Frame 236
    {0x00000079, 0xc82273e0, 0xa0f1c000, 0x00000000, 65},  // Frame 237
    {0x000001e7, 0x208acf92, 0x80c71000, 0x00000000, 65},  // Frame 238
    {0x0000079c, 0x82283e7a, 0x001c7000, 0x00000000, 65},  // Frame 239
    {0x00001e72, 0x28a2f9e8, 0x0271c000, 0x00000000, 65},  // Frame 240
    {0x000078c8, 0xa28ae7a0, 0x0ac71000, 0x00000000, 65},  // Frame 241
    {0x0001e022, 0x8a289e80, 0x281c7000, 0x00000000, 65},  // Frame 242
    {0x0007808a, 0x28a27a20, 0xa271e000, 0x00000000, 65},  // Frame 243
    {0x000e0028, 0x9288e882, 0x88c79000, 0x00000000, 65},  // Frame 244
    {0x002800a2, 0x7a22a22a, 0x221e7000, 0x00000000, 65},  // Frame 245
    {0x00800089, 0xc8888888, 0x8879c000, 0x00000000, 65},  // Frame 246
    {0x02000027, 0x32202202, 0x20e70000, 0x00000000, 65},  // Frame 247
    {0x0800029c, 0xf8828828, 0x829c1000, 0x00000000, 65},  // Frame 248
    {0x20000873, 0xe2082082, 0x08706000, 0x00000000, 65},  // Frame 249
    {0x800020cf, 0x98208218, 0x22c19000, 0x00000000, 65},  // Frame 250
    {0x0000803e, 0x70800870, 0x88067000, 0x00000000, 65},  // Frame 251
    {0x000200f9, 0xc20221e2, 0x2219e000, 0x00000000, 65},  // Frame 252
    {0x000800e7, 0x08088788, 0x88678000, 0x00000000, 65},  // Frame 253
    {0x0000009c, 0x00201e02, 0x209e0000, 0x00000000, 65},  // Frame 254
    {0x00000070, 0x00807808, 0x80780000, 0x00000000, 65},  // Frame 255
    {0x000000c0, 0x0200e002, 0x00e00000, 0x00000000, 65},  // Frame 256
    {0x00000000, 0x08008008, 0x00800000, 0x00000000, 65},  // Frame 257
};


Arduino_LED_Matrix matrix;

// Animation playback state (cooperative, interruptible by `stop_animation`)
static const int MAX_FRAMES = 300;
static uint32_t animation_buf[MAX_FRAMES][5]; // 4 words + duration
static int animation_frame_count = 0;
static volatile bool animation_running = false;
static volatile int animation_current_frame = 0;
static unsigned long animation_next_time = 0;

void setup() {
  matrix.begin();
  Serial.begin(115200);
  // configure grayscale bits to 3 so the display accepts 0..7 brightness
  // The backend will send quantized values in 0..(2^3-1) == 0..7.
  matrix.setGrayscaleBits(3);
  matrix.clear();

  Bridge.begin();
  Bridge.provide("draw", draw);
  Bridge.provide("load_frame", load_frame);
  Bridge.provide("play_animation", play_animation);
  Bridge.provide("stop_animation", stop_animation);
   size_t n = sizeof(animation) / sizeof(animation[0]);
    animation_frame_count = min((int)n, MAX_FRAMES);
    memcpy(animation_buf, animation, animation_frame_count * sizeof(animation[0]));
    play_animation();
}

void loop() {
  // Keep loop fast and let animation_tick handle playback timing
  animation_tick();
}

void draw(std::vector<uint8_t> frame) {
  if (frame.empty()) {
    Serial.println("[sketch] draw called with empty frame");
    return;
  }
  Serial.print("[sketch] draw called, frame.size=");
  Serial.println((int)frame.size());
  matrix.draw(frame.data());
}

void load_frame(std::array<uint32_t,5> animation_bytes){
  Serial.print("[sketch] load_frame ");
  if (animation_bytes.empty()) {
    Serial.println("[sketch] load_frame called with empty data");
    return;
  }

  // Limit frames to MAX_FRAMES to avoid buffer overflow
  if (animation_frame_count >= MAX_FRAMES) {
    Serial.print("[sketch] Too many frames, truncating to ");
    Serial.println(MAX_FRAMES);
    animation_frame_count = MAX_FRAMES;
    return;
  }
  
  animation_buf[animation_frame_count][0] = animation_bytes[0];
  animation_buf[animation_frame_count][1] = animation_bytes[1];
  animation_buf[animation_frame_count][2] = animation_bytes[2];
  animation_buf[animation_frame_count][3] = animation_bytes[3];
  animation_buf[animation_frame_count][4] = animation_bytes[4];

  Serial.print(" time=");
  Serial.println(animation_bytes[4]);

  animation_frame_count++;
}

void play_animation() {
  animation_current_frame = 0;
  animation_running = true;
  animation_next_time = millis();
  Serial.print("[sketch] Animation queued, frames=");
  Serial.println(animation_frame_count);
}

// Provider to stop any running animation
void stop_animation() {
  if (!animation_running) {
    Serial.println("[sketch] stop_animation called but no animation running");
    return;
  }
  animation_running = false;
  animation_frame_count = 0;
  Serial.println("[sketch] stop_animation: animation halted");
}

// Cooperative animation tick executed from loop()
void animation_tick() {
  if (!animation_running || animation_frame_count == 0) return;

  unsigned long now = millis();
  if (now < animation_next_time) return;

  Serial.print("animation tick, frame num:");
  Serial.println(animation_current_frame);
  
  // Prepare frame words (reverse bits as the library expects)
  uint32_t frame[4];
  frame[0] = reverse(animation_buf[animation_current_frame][0]);
  frame[1] = reverse(animation_buf[animation_current_frame][1]);
  frame[2] = reverse(animation_buf[animation_current_frame][2]);
  frame[3] = reverse(animation_buf[animation_current_frame][3]);

  // Display frame
  matrixWrite(frame);

  // Schedule next frame
  uint32_t interval = animation_buf[animation_current_frame][4];
  if (interval == 0) interval = 1;
  animation_next_time = now + interval;

  animation_current_frame++;
  if (animation_current_frame >= animation_frame_count) {
      animation_running = false;
      animation_frame_count = 0;
      animation_current_frame = 0;
      Serial.println("[sketch] Animation finished");
  }
}
