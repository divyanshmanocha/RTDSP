function Hd = m_filter (Fs)
%FILTER Returns a discrete-time filter object.

% MATLAB Code
% Generated by MATLAB(R) 9.2 and the Signal Processing Toolbox 7.4.
% Generated on: 25-Jan-2018 13:45:46

% Equiripple Bandpass filter designed using the FIRPM function.

% All frequency values are in Hz.
Fstop1 = 355;              % First Stopband Frequency
Fpass1 = 415;              % First Passband Frequency
Fpass2 = 1200;             % Second Passband Frequency
Fstop2 = 1270;             % Second Stopband Frequency
Dstop1 = 0.0039810717055;  % First Stopband Attenuation
Dpass  = 0.028774368332;   % Passband Ripple
Dstop2 = 0.0039810717055;  % Second Stopband Attenuation
dens   = 20;               % Density Factor

% Calculate the order from the parameters using FIRPMORD.
[N, Fo, Ao, W] = firpmord([Fstop1 Fpass1 Fpass2 Fstop2]/(Fs/2), [0 1 ...
                          0], [Dstop1 Dpass Dstop2]);

% Calculate the coefficients using the FIRPM function.
b  = firpm(N, Fo, Ao, W, {dens});
Hd = b;

% [EOF]
