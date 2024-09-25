/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include <stdint.h>
#include <stdio.h>

#define ADDITIVE_OP_COST 3
#define MULTIPLICATIVE_OP_COST 7
#define BITWISE_OP_COST 1

// A flag indicating if instrumentation is enabled or not
// This flag can only have two values: 0 or 1.
// Disabled by default when the program starts
int64_t isDynamicInstructionCountingEnabled;

// Counters for instructions from a representative domain
// Only modified when instrumentation is enabled with the
// isInstrumentationEnabledFlag=1
int64_t additiveInstructionCounter;
int64_t multiplicativeInstructionCounter;
int64_t bitwiseInstructionCounter;

void enableDynamicInstructionCounting() {
  isDynamicInstructionCountingEnabled = 1;
}
void disableDynamicInstructionCounting() {
  isDynamicInstructionCountingEnabled = 0;
}
void resetInstructionCountingStatistics() {
  additiveInstructionCounter = 0;
  multiplicativeInstructionCounter = 0;
  bitwiseInstructionCounter = 0;
}

void printInstructionCountingStatistics(int totalNumberOfPlanets) {
  printf("Number of additive instructions: %ld (%f per planet)\n",
         additiveInstructionCounter,
         (float)additiveInstructionCounter / totalNumberOfPlanets);
  printf("Number of multiplicative instructions: %ld (%f per planet)\n",
         multiplicativeInstructionCounter,
         (float)multiplicativeInstructionCounter / totalNumberOfPlanets);
  printf("Number of bitwise instructions: %ld (%f per planet)\n",
         bitwiseInstructionCounter,
         (float)bitwiseInstructionCounter / totalNumberOfPlanets);
  printf("Metric of computational cost: %ld (%f per planet)\n",
         additiveInstructionCounter * ADDITIVE_OP_COST + multiplicativeInstructionCounter * MULTIPLICATIVE_OP_COST +
             bitwiseInstructionCounter * BITWISE_OP_COST,
         (float)(additiveInstructionCounter * ADDITIVE_OP_COST +
                 multiplicativeInstructionCounter * MULTIPLICATIVE_OP_COST +
                 bitwiseInstructionCounter * BITWISE_OP_COST) /
             totalNumberOfPlanets);
}
