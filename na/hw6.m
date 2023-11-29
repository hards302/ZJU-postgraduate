format long
n = 10
a0 = 1
a1 = 2
b0 = 1
b1 = 2
c0 = 1
c1 = 2
while n > 0
    n = n-1
    if fa(a1)-fa(a0) ~= 0
        a2 = sfa(a1, a0)
        a0 = a1
        a1 = a2
    end
    if fb(b1)-fb(b0) ~= 0
        b2 = sfb(b1, b0)
        b0 = b1
        b1 = b2
    end
    if fc(c1)-fc(c0) ~= 0
        c2 = sfc(c1, c0)
        c0 = c1
        c1 = c2
    end
end

function y = sfa(x1, x0)
y = x1 - fa(x1)*(x1-x0)/(fa(x1)-fa(x0))
end
function y = sfb(x1, x0)
y = x1 - fb(x1)*(x1-x0)/(fb(x1)-fb(x0))
end
function y = sfc(x1, x0)
y = x1 - fc(x1)*(x1-x0)/(fc(x1)-fc(x0))
end
function y = fa(x)
y = x^3-2*x-2
end
function y = fb(x)
y = exp(x)+x-7
end
function y = fc(x)
y = exp(x)+sin(x)-4
end