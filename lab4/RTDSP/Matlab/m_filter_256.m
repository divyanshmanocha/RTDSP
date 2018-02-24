function Hd = m_filter_256
%M_FILTER_256 Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.0 and the Signal Processing Toolbox 7.2.
% Generated on: 22-Feb-2018 12:05:24

% Equiripple Bandpass filter designed using the FIRPM function.

% All frequency values are in Hz.
Fs = 8000;  % Sampling Frequency

N      = 255;   % Order
Fstop1 = 355;   % First Stopband Frequency
Fpass1 = 415;   % First Passband Frequency
Fpass2 = 1200;  % Second Passband Frequency
Fstop2 = 1270;  % Second Stopband Frequency
Wstop1 = 10;    % First Stopband Weight
Wpass  = 1.38;  % Passband Weight
Wstop2 = 10;    % Second Stopband Weight
dens   = 20;    % Density Factor

% Calculate the coefficients using the FIRPM function.
Hd  = firpm(N, [0 Fstop1 Fpass1 Fpass2 Fstop2 Fs/2]/(Fs/2), [0 0 1 1 0 ...
           0], [Wstop1 Wpass Wstop2], {dens});

% [EOF]
