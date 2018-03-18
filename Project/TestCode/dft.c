#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define E_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#define N_DEF 8

typedef struct {
    float r;
    float i;
} Complex;

float x[] = {0, 0, 2, 3, 4, 0, 0, 0};

void dft(int N, Complex *X) {
    int i, n, k;
    Complex *x = malloc(N * sizeof(X[0]));
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
    free(x);
}

void idft(int N, Complex *X) {
    int i, n, k;
	Complex *x = malloc(N * sizeof(X[0]));
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
    free(x);
}

int main() {
    int k;
    Complex X_C[8];
    for(k = 0; k < N_DEF; ++k) {
        X_C[k].r = x[k];
        X_C[k].i = 0;
    }
    dft(N_DEF, X_C);
    for(k = 0; k < N_DEF; ++k) {
        printf("x[%d] = %.2f, X[%d] = %.2f + %.2fi\n", k, x[k], k, X_C[k].r, X_C[k].i);
    }
    idft(N_DEF, X_C);
    for(k = 0; k < N_DEF; ++k) {
        printf("x[%d] = %.2f, X[%d] = %.2f + %.2fi\n", k, x[k], k, X_C[k].r, X_C[k].i);
    }
}
