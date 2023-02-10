#ifndef ARIA_CTR
#define ARIA_CTR

#include "../../ARIA-C/aria.h"
#include "../Util/util.h"


void counter_inc(unsigned char counter[16]);
void aria_ctr(unsigned char PT[], int pt_len, unsigned char mk[16], unsigned char CTR[16], unsigned char CT[], int round);

#endif


// EOF