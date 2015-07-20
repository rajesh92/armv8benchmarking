#include <fftw3.h>
#define NUM_POINTS 32	// N x N 2D array // input N 
#define runs 10000

#include <stdio.h>
#include <math.h>
#include <time.h>

#define REAL 0
#define IMAG 1

clock_t begin,end,planstart,planstop;
double time_spent,plan_time;

void acquire_signal(fftw_complex* signal) {
	// Generates sine waves of different frequencies and amplitudes. 
	long long i,j=0;
	for(j=0;j<NUM_POINTS;++j){
	for (i = 0; i < NUM_POINTS; ++i) {
	long double theta = (long double)((j*NUM_POINTS)+i) / ((long double)NUM_POINTS * NUM_POINTS) * M_PI;

	signal[(j*NUM_POINTS)+i][REAL] = 1.0 * cos(10.0 * theta) +
 								     0.5 * cos(25.0 * theta);

	signal[(j*NUM_POINTS)+i][IMAG] = 1.0 * sin(10.0 * theta) +
									 0.5 * sin(25.0 * theta);
	// printf("%f \t",signal[(j*NUM_POINTS)+i][REAL]);
} }
}

void do_something_with(fftw_complex* result) {
	long long i,j;
	for(j=0;j<NUM_POINTS;++j){
	for (i = 0; i < NUM_POINTS; ++i) {
	double mag = sqrt(result[(j*NUM_POINTS)+i][REAL] * result[(j*NUM_POINTS)+i][REAL] +
	result[(j*NUM_POINTS)+i][IMAG] * result[(j*NUM_POINTS)+i][IMAG]);

	// printf("%g\n", mag);
 } }
}

int main() {
	FILE *p;
	p = fopen("wisdom","r");
	fftw_import_wisdom_from_file(p);
	fftw_complex signal[NUM_POINTS*NUM_POINTS];
	fftw_complex result[NUM_POINTS*NUM_POINTS];

	planstart = clock();
	fftw_plan plan = fftw_plan_dft_2d(NUM_POINTS,NUM_POINTS,
									  signal,
									  result,
									  FFTW_FORWARD,
									  FFTW_PATIENT);
	planstop = clock();
	acquire_signal(signal);

	int i; double mflops;
	begin = clock();
	for(i=0;i<runs;i++)
		fftw_execute(plan);  //repeat 'runs' times
 	end = clock();
	do_something_with(result);
	
	fftw_destroy_plan(plan);
	plan_time = (double)(planstop-planstart)/CLOCKS_PER_SEC;
	printf("Time spent planning for %d = %f\n",NUM_POINTS,plan_time);
	time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	printf("Time spent on %d = %f\nAverage time for one execution = %f\n",NUM_POINTS,time_spent,time_spent/runs);
	mflops = (5*NUM_POINTS*NUM_POINTS * (log(NUM_POINTS*NUM_POINTS)/log(2)))/(time_spent*(1000000/runs));
	printf("MFLOPS = %f\n",mflops);

	return 0;
}
