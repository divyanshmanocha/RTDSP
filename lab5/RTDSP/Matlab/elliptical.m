function [b,a] = elliptical (Fs)

order = 4; % Order of the bandpass filter
startband = 270*2/Fs; % Normalised startband frequency
stopband = 450*2/Fs; % Normalised stopband frequency
ripple = 0.3; % The passband ripple (in dB)
stopband_atten = 20; % The stopband attenuation (in dB)

% Call the eliptical filter function
[b,a] = ellip(order/2,ripple,stopband_atten,[startband, stopband]);
