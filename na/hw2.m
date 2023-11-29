format long
x = 1
while x > 10^(-14)
    x = x/10
    y1 = (1-sec(x))/((tan(x))^2)
    y2 = -1/(1+(1/cos(x)))
    y3 = (1-(1-x)^3)/x
    y4 = x^2-3*x+3
end
