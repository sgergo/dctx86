#ifndef CALC_H
#define CALC_H

void calc_copy(float_t *dst, float_t *src, int length);
void calc_dct(float_t *timedata);
void calc_idct(float_t *freqdata);


#endif