#include "common.h"
#include "types.h"
#include "fft4g.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

extern float_t ecgdata[];
float_t timedata[ECGBIN_LENGTH + 1];
float_t modfreqdata[ECGBIN_LENGTH + 1];
float_t freqdata[ECGBIN_LENGTH + 1];
float_t psddata[ECGBIN_LENGTH + 1];

float_t compressedecgdata[ECGREC_LENGTH + 1];
float_t decompressedecgdata[ECGREC_LENGTH + 1];
int qrsdata[ECGREC_LENGTH + 1];

static void calc_copy(float_t *dst, float_t *src, int dstindex, int srcindex, int length) {
	// Add offset
	dst +=dstindex;
	src +=srcindex;

	while(length--) {
		*dst = *src;
		dst++;
		src++; 
	}
}

void calc_psdinit(void) {
	int i;

	for (i = 0; i < ECGBIN_LENGTH; i++)
		psddata[i] = 0;
}

static void calc_add(float_t *dst, float_t *src, int dstindex, int srcindex, int length) {
	// Add offset
	dst +=dstindex;
	src +=srcindex;

	while(length--) {
		*dst += fabs(*src);
		dst++;
		src++; 
	}
}

static void calc_dct(float_t *timedata) {

	int ip[NMAXSQRT + 2];
    float_t w[NMAX * 5 / 4];

    ip[0] = 0;
    ddct(ECGBIN_LENGTH, -1, timedata, ip, w);
}

static void calc_idct(float_t *freqdata) {
	int j;
	int ip[NMAXSQRT + 2];
    float_t w[NMAX * 5 / 4];

	freqdata[0] *= 0.5;
    ddct(ECGBIN_LENGTH, 1, freqdata, ip, w);
    for (j = 0; j <= ECGBIN_LENGTH - 1; j++) {
        freqdata[j] *= 2.0 / ECGBIN_LENGTH;
    }
}

void calc_compress_ecgdata(void) {
	int num_dctbins_in_ecgdata;
	int i, j;
    int index_of_nextbin;

	num_dctbins_in_ecgdata = ECGREC_LENGTH / ECGBIN_LENGTH;

	for (j = 0; j < num_dctbins_in_ecgdata; j++) {
		index_of_nextbin = (ECGBIN_LENGTH * j);
		printf("Iteration: %d, Bin index: %d\n", j, index_of_nextbin);
		
		// Copy original data into working buffer
		calc_copy(timedata, ecgdata, 0, index_of_nextbin, ECGBIN_LENGTH);
		// Calculate 1-D DCT and put everything back to working buffer
		calc_dct(timedata);

		// timedata now contains frequency domain values
		// Copy those values into freqdata buffer
		calc_copy(freqdata, timedata, 0, 0, ECGBIN_LENGTH);
		calc_add(psddata, timedata, 0, 0, ECGBIN_LENGTH);

		// Dismiss some values
		// for (i = 0; i < HIGHPASS; i++)
		// 	freqdata[i] = 0.00;
		for (i = LOWPASS; i < ECGBIN_LENGTH; i++)
			freqdata[i] = 0.00;

		// Freqdata contains frequency domain data from timedata with some values dismissed
		// Add those values into compressedecgdata buffer
		calc_copy(compressedecgdata, freqdata, index_of_nextbin, 0, ECGBIN_LENGTH);

		// Calculate inverse transform
		calc_idct(freqdata);

		// Freqdata contains inverse transformed data from freqdata
		// Add those values into decompressedecgdata buffer
		calc_copy(decompressedecgdata, freqdata, index_of_nextbin, 0, ECGBIN_LENGTH);
	} 	
}

// calc_qrs_complex({0.01...0.1}, {0.15...0.2});
void calc_qrs_complex(float alpha, float gamma) {
	float threshold = 0;
	int i, j;
	// int frame;
	float max;
	int index;

	for (i = 0; i < THRESHOLD_RANGE; i++) {
		if (decompressedecgdata[i] > threshold)
			threshold = decompressedecgdata[i];
	}

	for(i = 0; i < (ECGREC_LENGTH + 1); i+=FRAME) {
        max = 0;
        index = 0;

        if(i + FRAME > (ECGREC_LENGTH + 1)) {
            index = (ECGREC_LENGTH + 1);
        } else {
            index = i + FRAME;
        }

        for(j = i; j < index; j++) {
            if(decompressedecgdata[j] > max)
            	max = decompressedecgdata[j];
        }

        bool added = false;

        for(j = i; j < index; j++) {
            if(decompressedecgdata[j] > threshold && !added) {
                qrsdata[j] = 1;
                added = true;
            }
            else {
                qrsdata[j] = 0;
            }                
        }
        threshold = alpha * gamma * max + (1 - alpha) * threshold;
    }
}

/*

public static int[] QRS(float[] lowPass, int nsamp) {
    int[] QRS = new int[nsamp];

    double treshold = 0;

    for(int i=0; i<200; i++) {
        if(lowPass[i] > treshold) {
            treshold = lowPass[i];
        }
    }

    int frame = 250;

    for(int i=0; i<lowPass.length; i+=frame) {
        float max = 0;
        int index = 0;
        if(i + frame > lowPass.length) {
            index = lowPass.length;
        }
        else {
            index = i + frame;
        }
        for(int j=i; j<index; j++) {
            if(lowPass[j] > max) max = lowPass[j];
        }
        boolean added = false;
        for(int j=i; j<index; j++) {
            if(lowPass[j] > treshold && !added) {
                QRS[j] = 1;
                added = true;
            }
            else {
                QRS[j] = 0;
            }                
        }

        double gama = (Math.random() > 0.5) ? 0.15 : 0.20;
        double alpha = 0.01 + (Math.random() * ((0.1 - 0.01)));

        treshold = alpha * gama * max + (1 - alpha) * treshold;

    }

    return QRS;
}*/