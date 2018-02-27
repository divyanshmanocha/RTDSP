% 1:768
l_bound = 1;
u_bound = 600;

ap_y = allpass_gain(8);
a_y = actual_gain(8);
ap_x = allpass_gain(4);
a_x = allpass_gain(4);

gain_r_y = a_y{1}(l_bound:u_bound) - ap_y{1}(l_bound:u_bound);
gain_r_x = a_x{1}(l_bound:u_bound);

plot(gain_r_x, gain_r_y, gain_r_x, a_y{1}(l_bound:u_bound), gain_r_x, ap_y{1}(l_bound:u_bound));
xlabel('Frequency (Hz)') % x-axis label
ylabel('Amplitude (dB)') % y-axis label
title('Amplitude Spectrum (Linear plot)');
legend('Real gain','Measured Gain','Allpass gain');

phase_ap_y = allpass_phase(8);
phase_a_y = actual_phase(8);
phase_ap_x = allpass_phase(4);
phase_a_x = allpass_phase(4);


phase_r_y = phase_a_y{1}(l_bound:u_bound) - phase_ap_y{1}(l_bound:u_bound);
phase_r_x = phase_a_x{1}(l_bound:u_bound);
figure
plot(phase_r_x, phase_r_y, phase_r_x, phase_a_y{1}(l_bound:u_bound), phase_r_x, phase_ap_y{1}(l_bound:u_bound));
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Phase Spectrum (Linear plot)');
legend('Real phase','Measured phase','Allpass phase');


figure
plot(phase_r_x, phase_r_y);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Actual Phase Spectrum (Log plot)');

figure
plot(r_x, r_y);
xlabel('Frequency (Hz)') % x-axis label
ylabel('Phase (degrees)') % y-axis label
title('Actual Amplitude Spectrum (Log plot)');