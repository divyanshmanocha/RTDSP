[h, w] = freqz([0.125/2.125, 0.125/2.125], [1, -1.875/2.125]);

figure;
semilogx(h);

R = 1000; C = 1 * 10^(-6);

G = tf(1, [R*C 1]);
sysd = c2d(G, 1/8000, 'tustin');

figure;
bode(G, '-', sysd, '--');