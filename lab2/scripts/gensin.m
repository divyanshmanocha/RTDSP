% Generates a discrete sine wave with the following properties

% A: Amplitude
% F0: frequency
% fs: sampling frequency
% theta: initial phase
% nstart: first sample index
% nend: last sample index
function x = gensin(A, F0, fs, theta, nstart, nend)

% zero the column vector to the right size
fullSeries = zeros(nend-nstart+1, 1);

% go through all the elements until the end
for i = 1:nend
    % apply the sine wave for every element
    fullSeries(i) = A*sin(2*pi*F0*i/fs + theta);
end

% set the result to the window that was passed in by the user
x = fullSeries(nstart:nend);

end