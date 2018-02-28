function result = iir_filter(a, b, wave)

result = zeros(length(wave), 1);
x = zeros(length(a), 1);
y = zeros(length(a), 1);

for i = 1:length(wave)
    x(2) = x(1);
    x(1) = wave(i);
    y(2) = y(1);
    y(1) = b(1) * x(1) + b(2) * x(2) - a(2) * y(2);
    result(i) = y(1);
end