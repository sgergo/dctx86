#ifndef ACORR_H
#define ACORR_H

typedef struct {
	float_t *ts;
	float_t *acresult;
	float_t mean;
	float_t variance;
	int n;
	int maxlag;
}acorr_data_t;

void acorr_calculate(acorr_data_t *ads);

#endif