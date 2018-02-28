%comparinson

l_bound = 1;
u_bound = 500;

%Load the actual data
load('actual_phase_linear.mat');
load('actual_gain_linear.mat');
actual_gain = Gain;
actual_phase = Phase;
a_y = actual_gain(8);
a_x = actual_gain(4);

Fs = 8000;
[b,a] = elliptical (Fs);

%Plot it with the Matlab data
[exp_y, exp_x] = freqz(b, a, 2000,8000);

gain_y = a_y{1}(l_bound:u_bound);
gain_x = a_x{1}(l_bound:u_bound);


hold on;
plot(gain_x, gain_y, exp_x, rad2deg(angle(exp_y)));
%legend('Real gain','Expected Gain');
