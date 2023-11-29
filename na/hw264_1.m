format long
f = @(x) x .* exp(x);
low = 0;
high = 1;
exact = integral(f, low, high)
for m = [16, 32]
    h = (high - low)/m;
    x = low:h:high;
    y = f(x);
    I = h * (0.5*y(1) + sum(y(2:end-1)) + 0.5*y(end));
    fprintf('m=%d: I=%f\n', m, I)
    error = exact-I
end