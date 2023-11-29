format long
h = 10.^(-1:-1:-12);
f = @(x) sin(x) - cos(x);
df = @(x) cos(x) + sin(x);

exact = df(0);
approx = (f(0+h) - f(0-h)) ./ (2*h);
error = abs(exact - approx)