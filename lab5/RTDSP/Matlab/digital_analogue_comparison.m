R = 1000; C = 1 * 10^(-6);

G = tf(1, [R*C 1]);
sysd = c2d(G, 1/8000, 'tustin');

figure;
%h = bodeplot(G, '-', sysd, 'r--')
[mag,phase,wout] = bode(G);
semilogx(wout,20*log10(squeeze(mag)));
hold on;
semilogx(wout,phase);
grid on;
legend('Analogue RC circuit', 'Mapped Digital response');
% a = getoptions(h);
% a.YLim{1} = [-60, 0];
% setoptions(h, a);

% 
% figure;
% bode(G);
% grid on;
% title('RC Circuit bode plot (R = 1000\Omega, C = 1\muF)');