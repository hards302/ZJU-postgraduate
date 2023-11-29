% f = @(x)2*x*cos(x)-2*x+sin(x^3)
% inter = [-0.1,0.2]
% r = fzero(f, inter)
% ferror = abs(0-r)
% x = abs(r)
% berror = abs(2*x*cos(x)-2*x+sin(x^3))
format long
a = -0.1
b = 0.2
count = 0
while true
    c = (a+b)/2
    if func(c) == 0
        break
    end
    if b-a == 0
        break
    end
    if func(c) < 0
        a = c
    else
        b = c
    end
    count = count + 1
    if count > 100
        break
    end
end
function y = func(x)
y = 2*x*cos(x)-2*x+sin(x^3)
end