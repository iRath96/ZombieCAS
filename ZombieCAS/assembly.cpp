//
//  assembly.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 03.11.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>

#include <iostream>

#include "Tokenizer.h"
#include "Parser.h"
#include "Token.h"

#include "Constant.h"
#include "Variable.h"
#include "Invocation.h"

#include "OpAdd.h"
#include "OpMultiply.h"

#include "Function.h"

#include "Node.h"

double fastCall(const double data[]) {
  return 3.14159265358979323846264;
  //return data[0] + 3.14159f;
  //return data[0] * data[1] + data[2];
  //return (data[2] * data[3] + data[0]) / ((data[1] + data[2]) / ((data[0] + data[1]) / (data[0] + data[1])));
}

unsigned char assembly[256];
size_t assemblyIndex = 0;
size_t assemblyLastOutput = 0;

#define BYTE(b) assembly[assemblyIndex++] = b
#define BYTES(b) for(int i = 0; i < sizeof(b)-1; ++i) BYTE(b[i])
#define OUTPUT(str) assembly_output(str)

void assembly_output(const char *str) {
  printf("0x%04zx:\t", assemblyLastOutput);
  int spacesRequired = 16 - (int)(assemblyIndex - assemblyLastOutput) * 3;
  for(; assemblyLastOutput < assemblyIndex; ++assemblyLastOutput)
    printf("%02x ", assembly[assemblyLastOutput]);
  for(; spacesRequired > 0; --spacesRequired) printf(" ");
  printf("%s\n", str);
}

void assembly_header() {
  BYTE(0x55);
  OUTPUT("push  rbp");
  
  BYTES("\x48\x89\xe5");
  OUTPUT("mov   rbp, rsp");
  
  BYTES("\x48\x89\x7d\xf8");
  OUTPUT("mov   qword ptr [rbp - 0x8], rdi");
}

void assembly_footer() {
  BYTE(0x5d);
  OUTPUT("pop   rbp");
  
  BYTE(0xc3);
  OUTPUT("ret");
}

#define XMM0     (assd_data_t){ kXMM, 0, 0x00 }
#define XMM1     (assd_data_t){ kXMM, 1, 0x00 }
#define XMM2     (assd_data_t){ kXMM, 2, 0x00 }
#define XMM3     (assd_data_t){ kXMM, 3, 0x00 }

#define RDI(off) (assd_data_t){ kREG, 7, off  }

enum assd_instr_t {
  kMOVSD = 0x10,
  kADDSD = 0x58,
  kMULSD = 0x59,
  kSUBSD = 0x5c,
  kDIVSD = 0x5e
};

enum assd_reg_t {
  kXMM,
  kREG
};

struct assd_data_t {
  assd_reg_t reg;
  uint8_t index;
  int8_t offset;
};

void assd_describe_data(assd_data_t data, char *output) {
  if(data.reg == kXMM) sprintf(output, "xmm%i", data.index);
  else {
    if(data.offset == 0)
      sprintf(output, "qword ptr [rdi       ]");
    else
      sprintf(output, "qword ptr [rdi + 0x%02x]", data.offset);
  }
}

void assembly_sd(assd_instr_t instr, assd_data_t dest, assd_data_t src) {
  // clang would add this (pointless) instruction:
  
  //BYTES("\x48\x8b\x7d\xf8");
  //OUTPUT("mov   rdi , qword ptr [rbp - 0x08]");
  
  // Actual instruction synthesis.
  
  BYTES("\xf2\x0f");
  BYTE(instr);
  
  uint8_t byte3 = 0;
  if(src.reg == kXMM) byte3 = 0xc0;
  else if(src.offset) byte3 = 0x40;
  
  byte3 |= dest.index << 3;
  byte3 |= src.index;
  
  BYTE(byte3);
  if(src.offset) BYTE(src.offset);
  
  const char *op;
  switch(instr) {
    case kMOVSD: op = "movsd"; break;
    case kADDSD: op = "addsd"; break;
    case kMULSD: op = "mulsd"; break;
    case kSUBSD: op = "subsd"; break;
    case kDIVSD: op = "divsd"; break;
  }
  
  char output[128];
  char regA[32], regB[32];
  assd_describe_data(dest, regA);
  assd_describe_data(src, regB);
  sprintf(output, "%s %s, %s", op, regA, regB);
  
  OUTPUT(output);
}

#undef BYTE
#undef OUTPUT

#define TIME(name, iterations, block) {\
unsigned long a, b;\
struct timeval tv;\
gettimeofday(&tv, NULL); a = tv.tv_sec * 1000000 + tv.tv_usec;\
for(size_t i = 0; i < iterations; ++i) block\
gettimeofday(&tv, NULL); b = tv.tv_sec * 1000000 + tv.tv_usec - a;\
if(iterations) printf("%s finished in %ldµs", name, b);\
}

void assembly_main() {
  TIME("fix crash", 0, {});
  
  assembly_header();
  assembly_sd(kMOVSD, XMM0, RDI(0 * 0x8)); // xmm0  = $0
  assembly_sd(kMULSD, XMM0, RDI(1 * 0x8)); // xmm0 *= $1
  assembly_sd(kADDSD, XMM0, RDI(2 * 0x8)); // xmm0 += $2
  assembly_footer();
  
  //printf("real: %lf\n", fastCall((double[]){ 1.0, 2.0, 3.0 }));
  //return 0;
  
  // No mprotect needed.
  if(mprotect(assembly - (((uint64_t)assembly) & 0xfff), 4096, PROT_READ | PROT_EXEC)) {
    printf("mprotect (1) failed.\n");
    return -1;
  }
  
  if(mprotect((uint8_t *)(&fastCall) - (((uint64_t)&fastCall) & 0xfff), 4096, PROT_READ | PROT_WRITE | PROT_EXEC)) {
    printf("mprotect (2) failed.\n");
    return -1;
  }
  
  double lf = 0.0;
#define ITERATIONS 0x10000000
  
  TIME("real", ITERATIONS, {
    lf = ((double (*)(const double data[]))fastCall)((double[]){ 1.0, 2.0, 3.0 });
  });
  
  printf(" | result: %lf\n", lf);
  
  TIME("synd", ITERATIONS, {
    lf = ((double (*)(const double data[]))assembly)((double[]){ 1.0, 2.0, 3.0 });
  });
  
  printf(" | result: %lf\n", lf);
}