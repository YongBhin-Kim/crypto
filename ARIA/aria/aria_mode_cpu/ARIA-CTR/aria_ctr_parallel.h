#ifndef ARIA_CTR_PARALLEL
#define ARIA_CTR_PARALLEL

#include "../../ARIA-C/aria.h"
#include "aria_ctr.h"
#include "../Util/util.h"

void counter_inc(unsigned char counter[16]);
void aria_ctr(unsigned char PT[], int pt_len, unsigned char mk[16], unsigned char CTR[16], unsigned char CT[], int round);
void aria_ctr_parallel(unsigned char pt[], int pt_len, unsigned char mk[16], unsigned char ctr[16], unsigned char ct[], int round);
void *enc_parallel(void *arg);
#endif


// EOF