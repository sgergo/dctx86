#ifndef COMMON_H_
#define COMMON_H_

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef NMAX
#define NMAX 8192
#define NMAXSQRT 64
#endif

#define ECGBIN_LENGTH 512
#define ECGREC_LENGTH 4096

#define LOWPASS 64
#define HIGHPASS 2
#define THRESHOLD_RANGE 200
#define FRAME 256
// void calc_qrs_complex({0.01...0.1}, {0.15...0.2});
#define ALPHA 0.03f
#define GAMMA 0.10f

#endif
