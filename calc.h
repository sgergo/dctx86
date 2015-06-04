#ifndef CALC_H
#define CALC_H

void calc_psdinit(void);
void calc_copy(float_t *dst, float_t *src, int dstindex, int srcindex, int length);
void calc_dct(float_t *timedata);
void calc_idct(float_t *freqdata);
void calc_compress_ecgdata(void);
void calc_qrs_complex(float alpha, float gamma);


#endif