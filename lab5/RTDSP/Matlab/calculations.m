% 1:768

load('all_pass_gain.mat');
load('all_pass_phase.mat');

allpass_gain = Gain;
allpass_phase = Phase;

load('actual_phase.mat');
load('actual_gain.mat');
actual_gain = Gain;
actual_phase = Phase;

l_bound = 1;
u_bound = 200;

ap_y = allpass_gain(8);
a_y = actual_gain(8);
ap_x = allpass_gain(4);
a_x = allpass_gain(4);

gain_r_y = a_y{1}(l_bound:u_bound) - ap_y{1}(l_bound:u_bound);
gain_r_x = a_x{1}(l_bound:u_bound);

plot(gain_r_x, gain_r_y, gain_r_x, a_y{1}(l_bound:u_bound), gain_r_x, ap_y{1}(l_bound:u_bound), 'LineWidth',2);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Amplitude (dB)') % y-axis label
title('Amplitude Spectrum (Log plot)');
legend('Real gain','Measured Gain','Allpass gain');
set(gca, 'XScale', 'log');
set(gca,'FontSize',16);

phase_ap_y = allpass_phase(8);
phase_a_y = actual_phase(8);
phase_ap_x = allpass_phase(4);
phase_a_x = allpass_phase(4);


phase_r_y = phase_a_y{1}(l_bound:u_bound) - phase_ap_y{1}(l_bound:u_bound);
phase_r_x = phase_a_x{1}(l_bound:u_bound);
figure
plot(phase_r_x, phase_r_y, phase_r_x, phase_a_y{1}(l_bound:u_bound), phase_r_x, phase_ap_y{1}(l_bound:u_bound), 'LineWidth',2);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Phase Spectrum (Log plot)');
legend('Real phase','Measured phase','Allpass phase');
set(gca, 'XScale', 'log');
set(gca,'FontSize',16);


figure
plot(phase_r_x, phase_r_y, 'LineWidth',2);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Actual Phase Spectrum (Log plot)');
set(gca, 'XScale', 'log');
set(gca,'FontSize',16);

figure
plot(gain_r_x, gain_r_y, 'LineWidth',2);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Actual Amplitude Spectrum (Log plot)');
set(gca, 'XScale', 'log');
set(gca,'FontSize',16);