#include "../Util/util.h"
#include "../../ARIA-C/aria.h"
#include "../ARIA-CTR/aria_ctr.h"
#include "ghash.h"

/* GCM Return value, if failed then return -1 (declared in aria.c) */

int aria_gcm_enc(unsigned char pt[], int pt_len, unsigned char iv[16], unsigned char mk[16], unsigned char aad[], int aad_len, unsigned char ct[], unsigned char tag[16]);
int aria_gcm_dec(unsigned char ct[], int ct_len, unsigned char iv[16], unsigned char mk[16], unsigned char aad[], int aad_len, unsigned char pt[], unsigned char tag[16]);