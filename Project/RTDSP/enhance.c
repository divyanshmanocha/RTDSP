/*************************************************************************************
			       DEPARTMENT OF ELECTRICAL AND ELECTRONIC ENGINEERING
					   		     IMPERIAL COLLEGE LONDON 

 				      EE 3.19: Real Time Digital Signal Processing
					       Dr Paul Mitcheson and Daniel Harvey

				        		 PROJECT: Frame Processing

 				            ********* ENHANCE. C **********
							 Shell for speech enhancement 

  		Demonstrates overlap-add frame processing (interrupt driven) on the DSK. 

 *************************************************************************************
 				             By Danny Harvey: 21 July 2006
							 Updated for use on CCS v4 Sept 2010
 ************************************************************************************/
/*
 *	You should modify the code so that a speech enhancement project is built 
 *  on top of this template.
 */
/**************************** Pre-processor statements ******************************/
//  library required when using calloc
#include <stdlib.h>
//  Included so program can make use of DSP/BIOS configuration tool.  
#include "dsp_bios_cfg.h"

/* The file dsk6713.h must be included in every program that uses the BSL.  This 
   example also includes dsk6713_aic23.h because it uses the 
   AIC23 codec module (audio interface). */
#include "dsk6713.h"
#include "dsk6713_aic23.h"

// math library (trig functions)
#include <math.h>

/* Some functions to help with Complex algebra and FFT. */
#include "cmplx.h"      
#include "fft_functions.h"  

// Some functions to help with writing/reading the audio ports when using interrupts.
#include <helper_functions_ISR.h>

#define WINCONST 0.85185			/* 0.46/0.54 for Hamming window */
#define FSAMP 8000.0		/* sample frequency, ensure this matches Config for AIC */
#define FFTLEN 256					/* fft length = frame length 256/8000 = 32 ms*/
#define NFREQ (1+FFTLEN/2)			/* number of frequency bins from a real FFT */
#define OVERSAMP 4					/* oversampling ratio (2 or 4) */  
#define FRAMEINC (FFTLEN/OVERSAMP)	/* Frame increment */
#define CIRCBUF (FFTLEN+FRAMEINC)	/* length of I/O buffers */
#define FRAME_TIME 2.5
#define MAX_COUNT 20000
#define MAX_FLOAT 3.4E+38
#define OUTGAIN 16000.0				/* Output gain for DAC */
#define INGAIN  (1.0/16000.0)		/* Input gain for ADC  */
#define NUM_M 4
// PI defined here for use in your code 
#define PI 3.141592653589793
#define TFRAME FRAMEINC/FSAMP       /* time between calculation of each frame */

/******************************* Global declarations ********************************/

/* Audio port configuration settings: these values set registers in the AIC23 audio 
   interface to configure it. See TI doc SLWS106D 3-3 to 3-10 for more info. */
DSK6713_AIC23_Config Config = { \
			 /**********************************************************************/
			 /*   REGISTER	            FUNCTION			      SETTINGS         */ 
			 /**********************************************************************/\
    0x0017,  /* 0 LEFTINVOL  Left line input channel volume  0dB                   */\
    0x0017,  /* 1 RIGHTINVOL Right line input channel volume 0dB                   */\
    0x01f9,  /* 2 LEFTHPVOL  Left channel headphone volume   0dB                   */\
    0x01f9,  /* 3 RIGHTHPVOL Right channel headphone volume  0dB                   */\
    0x0011,  /* 4 ANAPATH    Analog audio path control       DAC on, Mic boost 20dB*/\
    0x0000,  /* 5 DIGPATH    Digital audio path control      All Filters off       */\
    0x0000,  /* 6 DPOWERDOWN Power down control              All Hardware on       */\
    0x0043,  /* 7 DIGIF      Digital audio interface format  16 bit                */\
    0x008d,  /* 8 SAMPLERATE Sample rate control        8 KHZ-ensure matches FSAMP */\
    0x0001   /* 9 DIGACT     Digital interface activation    On                    */\
			 /**********************************************************************/
};

// Codec handle:- a variable used to identify audio interface  
DSK6713_AIC23_CodecHandle H_Codec;

float *inbuffer, *outbuffer;   		/* Input/output circular buffers */
float *inframe, *outframe;          /* Input and output frames */
float *inwin, *outwin;              /* Input and output windows */
float ingain, outgain;				/* ADC and DAC gains */ 
float cpufrac; 						/* Fraction of CPU time used */
complex *fft_out;						/* FFT output */
float *noise;
float *power_in;
float *mag_in;
float* lpf;
volatile int io_ptr=0;              /* Input/ouput pointer for circular buffers */
volatile int frame_ptr=0;           /* Frame pointer */
volatile int frame_ctr = 0;
volatile int m_ptr = 0;
float lambda = 0.05;
float alpha = 100;
double avg = 0;
float *M[NUM_M];
float mag_N_X;
float K;
static float time_constant = 40e-3;		/* Time constant in ms */
int started = 0;
 /******************************* Function prototypes *******************************/
void init_hardware(void);    	/* Initialize codec */ 
void init_HWI(void);            /* Initialize hardware interrupts */
void ISR_AIC(void);             /* Interrupt service routine for codec */
void process_frame(void);       /* Frame processing routine */
void write_spectrum(void);
void get_noise(void);
void low_pass_filter(void);           
/********************************** Main routine ************************************/
void main()
{      

  	int k; // used in various for loops
  
/*  Initialize and zero fill arrays */  

	inbuffer	= (float *) calloc(CIRCBUF, sizeof(float));	/* Input array */
    outbuffer	= (float *) calloc(CIRCBUF, sizeof(float));	/* Output array */
	inframe		= (float *) calloc(FFTLEN, sizeof(float));	/* Array for processing*/
    outframe	= (float *) calloc(FFTLEN, sizeof(float));	/* Array for processing*/
    inwin		= (float *) calloc(FFTLEN, sizeof(float));	/* Input window */
    outwin		= (float *) calloc(FFTLEN, sizeof(float));	/* Output window */
    fft_out		= (complex *) calloc(FFTLEN, sizeof(complex));	/* FFT Output */
    power_in	= (float *) calloc(FFTLEN, sizeof(float));	/* Output window */
	lpf 		= (float *) calloc(FFTLEN, sizeof(float));	/* Output window */
    mag_in	= (float *) calloc(FFTLEN, sizeof(float));	/* Output window */
    noise	= (float *) calloc(FFTLEN, sizeof(float));	/* Output window */
	
	/* initialize board and the audio port */
  	init_hardware();
  
  	/* initialize hardware interrupts */
  	init_HWI();    
  
/* initialize algorithm constants */  
                       
  	for (k=0; k<FFTLEN; ++k)
	{                           
		inwin[k] = sqrt((1.0-WINCONST*cos(PI*(2*k+1)/FFTLEN))/OVERSAMP);
		outwin[k] = inwin[k]; 
	} 
  	ingain=INGAIN;
  	outgain=OUTGAIN;

  	for (k = 0; k < NUM_M; ++k) {
  		M[k] = (float *) calloc(FFTLEN, sizeof(float));
  	}

 	K = exp(-TFRAME/time_constant);			
  	/* main loop, wait for interrupt */  
  	while(1) 	process_frame();
}
    
/********************************** init_hardware() *********************************/  
void init_hardware()
{
    // Initialize the board support library, must be called first 
    DSK6713_init();
    
    // Start the AIC23 codec using the settings defined above in config 
    H_Codec = DSK6713_AIC23_openCodec(0, &Config);

	/* Function below sets the number of bits in word used by MSBSP (serial port) for 
	receives from AIC23 (audio port). We are using a 32 bit packet containing two 
	16 bit numbers hence 32BIT is set for  receive */
	MCBSP_FSETS(RCR1, RWDLEN1, 32BIT);	

	/* Configures interrupt to activate on each consecutive available 32 bits 
	from Audio port hence an interrupt is generated for each L & R sample pair */	
	MCBSP_FSETS(SPCR1, RINTM, FRM);

	/* These commands do the same thing as above but applied to data transfers to the 
	audio port */
	MCBSP_FSETS(XCR1, XWDLEN1, 32BIT);	
	MCBSP_FSETS(SPCR1, XINTM, FRM);	
	

}
/********************************** init_HWI() **************************************/ 
void init_HWI(void)
{
	IRQ_globalDisable();			// Globally disables interrupts
	IRQ_nmiEnable();				// Enables the NMI interrupt (used by the debugger)
	IRQ_map(IRQ_EVT_RINT1,4);		// Maps an event to a physical interrupt
	IRQ_enable(IRQ_EVT_RINT1);		// Enables the event
	IRQ_globalEnable();				// Globally enables interrupts

}

// Spectrum calculations for the new values
void write_spectrum(void) {
	unsigned int k;
	for(k = 0; k < FFTLEN; ++k) {
		if(power_in[k] < M[m_ptr][k] || M[m_ptr][k] == 0) {
			M[m_ptr][k] = power_in[k];
		}
	}
}

// Noise estimataion
void get_noise(void) {
	int k, i, min_i;
	float min_val;

	for(k = 0; k < FFTLEN; ++k) {
		min_i = 0;
		min_val = M[0][k];
		for(i = 1; i < NUM_M; ++i) {
			if (M[i][k] < min_val && M[i][k]!= 0) {
				min_val = M[i][k];
				min_i = i;
			}
		}
		noise[k] = alpha * M[min_i][k];
	}
}


// Low pass filter X(w)
void low_pass_filter(void) {
	int w;
	float temp;
	for (w = 0; w < FFTLEN; ++w) {
		temp = power_in[w];
		power_in[w] = (1-K)*power_in[w] + K*lpf[w];
		lpf[w] = temp;
	}
}
        
/******************************** process_frame() ***********************************/  
void process_frame(void)
{
	int k, m; 
	int io_ptr0;
	/* work out fraction of available CPU time used by algorithm */    
	cpufrac = ((float) (io_ptr & (FRAMEINC - 1)))/FRAMEINC;  
		
	/* wait until io_ptr is at the start of the current frame */ 	
	while((io_ptr/FRAMEINC) != frame_ptr); 
	
	/* then increment the framecount (wrapping if required) */ 
	if (++frame_ptr >= (CIRCBUF/FRAMEINC)) frame_ptr=0;
 	
 	/* save a pointer to the position in the I/O buffers (inbuffer/outbuffer) where the 
 	data should be read (inbuffer) and saved (outbuffer) for the purpose of processing */
 	io_ptr0=frame_ptr * FRAMEINC;
	
	/* copy input data from inbuffer into inframe (starting from the pointer position) */ 
	 
	m=io_ptr0;
    for (k=0;k<FFTLEN;k++)
	{                           
		inframe[k] = inbuffer[m] * inwin[k]; 
		if (++m >= CIRCBUF) m=0; /* wrap if required */
	} 
	
	/************************* DO PROCESSING OF FRAME  HERE **************************/
	
	// Initialise the array fft_out for FFT
	for (k = 0; k < FFTLEN; ++k) {
		fft_out[k] = cmplx(inframe[k], 0.0);
	}
	
	// Perform the FFT
	fft(FFTLEN, fft_out);
	
	// calculate the power spectrum
	for (k = 0; k < FFTLEN; ++k) {
		power_in[k] = sqrt(fft_out[k].r * fft_out[k].r + fft_out[k].i * fft_out[k].i);
	}
	
	//low_pass_filter();

	// Get average of fft_out and write to Spectrum
	write_spectrum();

	// Set the noise
	get_noise();
	
	if(frame_ctr > MAX_COUNT-1) {
		int i;
		frame_ctr = 0;
		if(++m_ptr == NUM_M) m_ptr = 0;
  		for(i = 0; i < FFTLEN; ++i) {
  			M[m_ptr][i] = power_in[i];
  		}
	}

	// max(lambda, |N(w)/g(w)|
	for (k = 0; k < FFTLEN; ++k) {
		float g;
		mag_N_X = 1 - noise[k]/power_in[k];
		g = mag_N_X > lambda ? mag_N_X : lambda;
		fft_out[k] = rmul(g, fft_out[k]);
	}
	
	// Back into time domain
	ifft(FFTLEN, fft_out);
	
	for (k = 0; k < FFTLEN; ++k) {
		outframe[k] = fft_out[k].r;
	}
	/********************************************************************************/
	
    /* multiply outframe by output window and overlap-add into output buffer */  
                           
	m=io_ptr0;
    
    for (k=0;k<(FFTLEN-FRAMEINC);k++) 
	{    										/* this loop adds into outbuffer */                       
	  	outbuffer[m] = outbuffer[m]+outframe[k]*outwin[k];   
		if (++m >= CIRCBUF) m=0; /* wrap if required */
	}         
    for (;k<FFTLEN;k++) 
	{                           
		outbuffer[m] = outframe[k]*outwin[k];   /* this loop over-writes outbuffer */        
	    m++;
	}	                                 
}        
/*************************** INTERRUPT SERVICE ROUTINE  *****************************/

// Map this to the appropriate interrupt in the CDB file
   
void ISR_AIC(void)
{       
	short sample;
	/* Read and write the ADC and DAC using inbuffer and outbuffer */
	sample = mono_read_16Bit();
	inbuffer[io_ptr] = ((float)sample)*ingain;
		/* write new output data */
	mono_write_16Bit((int)(outbuffer[io_ptr]*outgain)); 
	
	/* update io_ptr and check for buffer wraparound */    
	
	if (++io_ptr >= CIRCBUF) io_ptr=0;
	frame_ctr++;
	started = 1;
}

/************************************************************************************/
