
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "types.h"
#include "calc.h"

extern float_t ecgdata1[];
extern float_t ecgdata2[];
extern float_t ecgdata3[];
extern float_t timedata[ECGBIN_LENGTH + 1];
extern float_t modfreqdata[ECGBIN_LENGTH + 1];
extern float_t freqdata[ECGBIN_LENGTH + 1];
extern float_t psddata[ECGBIN_LENGTH + 1];

float_t compressedecgdata[ECGREC_LENGTH + 1];
float_t decompressedecgdata[ECGREC_LENGTH + 1];
int qrsdata[ECGREC_LENGTH + 1];

int main(void) {
	unsigned int i;
	FILE *ofp;

	printf("\n\n 1-D DCT test\n\n");

	// // Copy original data into working buffer
	// calc_copy(timedata, ecgdata, ECGBIN_LENGTH);
	// // Calculate 1-D DCT and put everything back to working buffer
	// calc_dct(timedata);

	// // timedata now contains frequency domain values
	// // Copy those values into freqdata buffer
	// calc_copy(freqdata, timedata, ECGBIN_LENGTH);

	// // Dismiss some values
	// for (i = 0; i < 2; i++)
	// 	freqdata[i] = 0.00;
	// for (i = 60; i < ECGBIN_LENGTH; i++)
	// 	freqdata[i] = 0.00;

	// // Freqdata contains frequency domain data from timedata with some values dismissed
	// // Copy those values into modfreqdata buffer
	// calc_copy(modfreqdata, freqdata, ECGBIN_LENGTH);

	// // Calculate inverse transform
	// calc_idct(freqdata);
	calc_psdinit();
	calc_compress_ecgdata();

	// alpha = {0.01...0.1}
	// gamma = {0.15...0.2}
	// void calc_qrs_complex({0.01...0.1}, {0.15...0.2});
	calc_qrs_complex(ALPHA, GAMMA);

	ofp = fopen("result_ch1.csv", "w");
	fprintf(ofp, "Original, Transformed, Inverse-Transformed\r\n");
	printf("Original\tTransformed\tModified-Transformed\tInverse-Transformed\r\n");

	// for (i = 0; i < ECGBIN_LENGTH; i++) {
	// 	printf("%f\t%f\t%f\t%f\r\n", ecgdata[i], timedata[i], modfreqdata[i], freqdata[i]); 
	// 	fprintf(ofp, "%f, %f, %f \r\n", ecgdata[i], timedata[i], freqdata[i]);
	// }

	for (i = 0; i < ECGREC_LENGTH; i++) {
		printf("%d: %f\t%f\t%f\t%d", i, ecgdata1[i], compressedecgdata[i], decompressedecgdata[i], qrsdata[i]); 
		fprintf(ofp, "%f, %f, %f, %d", ecgdata1[i], compressedecgdata[i], decompressedecgdata[i], qrsdata[i]);
		if (i < ECGBIN_LENGTH) {
			printf("\t%f\n", psddata[i]);
			fprintf(ofp, ", %f\n", psddata[i]);
		} else {
			printf("\n");
			fprintf(ofp, "\n");
		}
	}
	fclose(ofp);
	return 0;
}
