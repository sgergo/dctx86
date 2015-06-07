/*
You specified the following parameters:
filtertype	=	Butterworth
passtype	=	Lowpass
ripple	=	
order	=	3
samplerate	=	500
corner1	=	36
corner2	=	
adzero	=	
logmin	=	


#define NZEROS 3
#define NPOLES 3
#define GAIN   1.295012829e+02

static float xv[NZEROS+1], yv[NPOLES+1];

static void filterloop()
  { for (;;)
      { xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; 
        xv[3] = `next input value' / GAIN;
        yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; 
        yv[3] =   (xv[0] + xv[3]) + 3 * (xv[1] + xv[2])
                     + (  0.4013417275 * yv[0]) + ( -1.5651374543 * yv[1])
                     + (  2.1020202770 * yv[2]);
        `next output value' = yv[3];
      }
  }

  */

