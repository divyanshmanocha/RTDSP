#include <math.h>
#include <stdio.h>

#define E_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#define N 8

typedef struct {
    float r;
    float i;
} Complex;

float x[] = {0, 0, 2, 3, 4, 0, 0, 0};
Complex X[N];
float x_out[N];

void dft(int N, Complex *X) {
    int i, n, k;
	Complex x[N];
	for(i = 0; i < N; ++i) {
		x[i] = X[i];
	}
    for(k = 0; k < N; ++k) {
        X[k].r = 0.f;
        X[k].i = 0.f;
        for(n = 0; n < N; ++n) {
            X[k].r += x[n].r * cos(2 * E_PI * k * n / N);
            X[k].i -= x[n].r * sin(2 * E_PI * k * n / N);
        }
    }
}

void idft(int N, Complex *X) {
    int i, n, k;
	Complex x[N];
	for(i = 0; i < N; ++i) {
		x[i] = X[i];
	}
    for(n = 0; n < N; ++n) {
        X[n].i = 0.f;
		X[n].r = 0.f;
        for(k = 0; k < N; ++k) {
            X[n].r += x[k].r * cos(2 * E_PI * k * n / N) - x[k].i * sin(2 * E_PI * k * n / N);
        }
        X[n].r /= N;
    }
}

int main() {
    int k;
    dft();
    idft();
    for(k = 0; k < N; ++k) {
        printf("x[%d] = %.2f, X[%d] = %.2f + %.2fi, x_out[%d] = %.2f\n", k, x[k], k, X[k].r, X[k].i, k, x_out[k]);
    }
}