[h, w] = freqz([0.125/2.125, 0.125/2.125], [1, -1.875/2.125]);

figure;
semilogx(h);

R = 1000; C = 1 * 10^(-6);

G = tf(1, [R*C 1]);
sysd = c2d(G, 1/8000, 'tustin');

figure;
bode(G, '-', sysd, '--');
grid on;

figure;
bode(G);
grid on;
title('RC Circuit bode plot (R = 1000\Omega, C = 1\muF)');

%%
% Generate a square wave

Fs = 8000;
freq = 10;
rep = Fs/freq/2;
num = 5;

squareWave = repmat([ones(1, rep),zeros(1, rep)], [1, num]);
figure;
plot(iir_filter([1, -0.882], [0.0588, 0.0588], squareWave));
hold on;
plot(squareWave);
title('10 Hz Discrete Square Wave at Fs = 8kHz');
legend('Output of RC circuit', 'Square wave input');
xlabel('Sample');
ylabel('Amplitude');
ylim([-0.2, 1.2]);
grid on;