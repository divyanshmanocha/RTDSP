% 1:768

%Obtain Matlab coefficients
Fs = 8000;
[b,a] = elliptical (Fs);
[exp_y, exp_x] = freqz(b, a, 2000,8000);


load('all_pass_gain_linear.mat');
load('all_pass_phase_linear.mat');

allpass_gain = Gain;
allpass_phase = Phase;

load('actual_phase_linear.mat');
load('actual_gain_linear.mat');
actual_gain = Gain;
actual_phase = Phase;

l_bound = 1;
u_bound = 1200;

ap_y = allpass_gain(8);
a_y = actual_gain(8);
ap_x = allpass_gain(4);
a_x = allpass_gain(4);

gain_r_y = a_y{1}(l_bound:u_bound) - ap_y{1}(l_bound:u_bound);
gain_r_x = a_x{1}(l_bound:u_bound);

plot(gain_r_x, gain_r_y, gain_r_x, a_y{1}(l_bound:u_bound), gain_r_x, ap_y{1}(l_bound:u_bound), 'LineWidth',1);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Amplitude (dB)') % y-axis label
title('Amplitude Spectrum (Linear plot)');
legend('Real gain','Measured Gain','Allpass gain');
%set(gca, 'XScale', 'log');
set(gca,'FontSize',16);
grid on;

phase_ap_y = allpass_phase(8);
phase_a_y = actual_phase(8);
phase_ap_x = allpass_phase(4);
phase_a_x = allpass_phase(4);


phase_r_y = phase_a_y{1}(l_bound:u_bound) - phase_ap_y{1}(l_bound:u_bound);
phase_r_x = phase_a_x{1}(l_bound:u_bound);
figure
plot(phase_r_x, phase_r_y, phase_r_x, phase_a_y{1}(l_bound:u_bound), phase_r_x, phase_ap_y{1}(l_bound:u_bound), 'LineWidth',1);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Phase Spectrum (Linear plot)');
legend('Real phase','Measured phase','Allpass phase');
%set(gca, 'XScale', 'log');
set(gca,'FontSize',16);
grid on;


figure
plot(phase_r_x, phase_r_y, 'LineWidth',1);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Actual Phase Spectrum (Linear plot)');
%set(gca, 'XScale', 'log');
set(gca,'FontSize',16);
grid on;


figure
plot(gain_r_x, gain_r_y, 'LineWidth',1);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Actual Amplitude Spectrum (Linear plot)');
%set(gca, 'XScale', 'log');
set(gca,'FontSize',16);
grid on;


figure
plot(gain_r_x, gain_r_y, exp_x, 20*log10(abs(exp_y)));
legend('Real gain','Expected gain');
xlabel('Frequency (Hz)') % x-axis label
ylabel('Amplitude (dB)') % y-axis label
title('Comparison of the Matlab filter and the actual gain response');
set(gca,'FontSize',16);
grid on;


figure
plot(phase_r_x, phase_r_y, exp_x, rad2deg(angle(exp_y)));
legend('Real Phase','Expected Phase');
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Comparison of the Matlab filter and the actual phase response');
set(gca,'FontSize',16);
grid on;
