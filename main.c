
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "common.h"
#include "types.h"
#include "calc.h"
#include "acorr.h"

extern float_t ecgdata[];
extern float_t timedata[ECGBIN_LENGTH + 1];
extern float_t modfreqdata[ECGBIN_LENGTH + 1];
extern float_t freqdata[ECGBIN_LENGTH + 1];
extern float_t psddata[ECGBIN_LENGTH + 1];

float_t compressedecgdata[ECGREC_LENGTH + 1];
float_t decompressedecgdata[ECGREC_LENGTH + 1];
int qrsdata[ECGREC_LENGTH + 1];
float_t acresult[ECGREC_LENGTH + 1];

acorr_data_t acorr_data;

int main(void) {
	unsigned int i;
	FILE *ofp;

	printf("\n\n 1-D DCT test\n\n");

	// Init PSD buffer
	calc_psdinit();
	// Transform-dismiss-inverse transform
	calc_compress_ecgdata();

	// Init ACF parameters
	acorr_data.acresult = acresult;
	acorr_data.ts = decompressedecgdata;
	acorr_data.maxlag = ECGREC_LENGTH;
	acorr_data.n = ECGREC_LENGTH + 1;
	// Calculate ACF
	acorr_calculate(&acorr_data);

	// Output result
	ofp = fopen("result_ch3.csv", "w");
	fprintf(ofp, "Original, DCT, iDCT, ACF\r\n");
	printf("Original\tDCT\tiDCT\tACF\r\n");

	for (i = 0; i < ECGREC_LENGTH; i++) {
		printf("%d: %f\t%f\t%f\t%f", i, ecgdata[i], compressedecgdata[i], decompressedecgdata[i], acresult[i]); 
		fprintf(ofp, "%f, %f, %f, %f", ecgdata[i], compressedecgdata[i], decompressedecgdata[i], acresult[i]);
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
