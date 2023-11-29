format long
n = 4
a = 2
b = 2
c = 2
while n > 0
    a = fa(a)
    b = fb(b)
    c = fc(c)
    n = n-1
end
function y = fa(x)
y = x- (x^3-2*x-2)/(3*x^2-2)
end
function y = fb(x)
y = x - (exp(x)+x-7)/(exp(x)+1)
end
function y = fc(x)
y = x - (exp(x)+sin(x)-4)/(exp(x)+cos(x))
end