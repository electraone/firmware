#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

#ifdef __cplusplus

int32_t random(void);
uint32_t random(uint32_t howbig);
int32_t random(int32_t howsmall, int32_t howbig);
void randomSeed(uint32_t newseed);
void srandom(unsigned int newseed);

#endif // __cplusplus

#endif // RANDOM_H
