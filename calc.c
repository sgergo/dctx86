#include "common.h"
#include "types.h"
#include "fft4g.h"

extern float_t ecgdata[];
float_t timedata[ECGBIN_LENGTH + 1];
float_t modfreqdata[ECGBIN_LENGTH + 1];
float_t freqdata[ECGBIN_LENGTH + 1];

void calc_copy(float_t *dst, float_t *src, int length) {
	while(length--) {
		*dst = *src;
		dst++;
		src++; 
	}
}

void calc_dct(float_t *timedata) {

	int ip[NMAXSQRT + 2];
    float_t w[NMAX * 5 / 4];

    ip[0] = 0;
    ddct(ECGBIN_LENGTH, -1, timedata, ip, w);
}

void calc_idct(float_t *freqdata) {
	int j;
	int ip[NMAXSQRT + 2];
    float_t w[NMAX * 5 / 4];

	freqdata[0] *= 0.5;
    ddct(ECGBIN_LENGTH, 1, freqdata, ip, w);
    for (j = 0; j <= ECGBIN_LENGTH - 1; j++) {
        freqdata[j] *= 2.0 / ECGBIN_LENGTH;
    }
}