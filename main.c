
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "types.h"
#include "calc.h"

extern float_t ecgdata[];
extern float_t timedata[ECGBIN_LENGTH + 1];
extern float_t modfreqdata[ECGBIN_LENGTH + 1];
extern float_t freqdata[ECGBIN_LENGTH + 1];

int main(void) {
	unsigned int i;
	FILE *ofp;

	printf("\n\n 1-D DCT test\n\n");

	// Copy original data into working buffer
	calc_copy(timedata, ecgdata, ECGBIN_LENGTH);
	// Calculate 1-D DCT and put everything back to working buffer
	calc_dct(timedata);

	// timedata now contains frequency domain values
	// Copy those values into freqdata buffer
	calc_copy(freqdata, timedata, ECGBIN_LENGTH);

	// Dismiss some values
	for (i = 0; i < 2; i++)
		freqdata[i] = 0.00;
	for (i = 80; i < ECGBIN_LENGTH; i++)
		freqdata[i] = 0.00;

	// Freqdata contains frequency domain data from timedata with some values dismissed
	// Copy those values into modfreqdata buffer
	calc_copy(modfreqdata, freqdata, ECGBIN_LENGTH);

	// Calculate inverse transform
	calc_idct(freqdata);

	ofp = fopen("result.csv", "w");
	fprintf(ofp, "Original, Transformed, Inverse-Transformed\r\n");
	printf("Original\tTransformed\tInverse-Transformed\r\n");

	for (i = 0; i < ECGBIN_LENGTH; i++) {
		printf("%f\t%f\t%f\t%f\r\n", ecgdata[i], timedata[i], modfreqdata[i], freqdata[i]); 
		fprintf(ofp, "%f, %f, %f \r\n", ecgdata[i], timedata[i], freqdata[i]);
	}
	fclose(ofp);
	return 0;
}
