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

void dft() {
    for(int k = 0; k < N; ++k) {
        X[k].r = 0.f;
        X[k].i = 0.f;
        for(int n = 0; n < N; ++n) {
            X[k].r += x[n] * cos(2 * E_PI * k * n / N);
            X[k].i -= x[n] * sin(2 * E_PI * k * n / N);
        }
    }
}

void idft() {
    for(int n = 0; n < N; ++n) {
        x_out[n] = 0.f;
        for(int k = 0; k < N; ++k) {
            x_out[n] += X[k].r * cos(2 * E_PI * k * n / N) - X[k].i * sin(2 * E_PI * k * n / N);
        }
        x_out[n] /= N;
    }
}

int main() {
    dft();
    idft();
    for(int k = 0; k < N; ++k) {
        printf("x[%d] = %.2f, X[%d] = %.2f + %.2fi, x_out[%d] = %.2f\n", k, x[k], k, X[k].r, X[k].i, k, x_out[k]);
    }
}
