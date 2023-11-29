
n = ceil(8*log10(10)/log10(2))-1
a1 = 0
b1 = 1
a2 = -1
b2 = 0
a3 = 1
b3 = 2
ans1 = 0
ans2 = 0
ans3 = 0
while n > 0
    c1 = (a1+b1)/2
    if c1^5 + c1 - 1 < 0
        a1 = c1
    elseif c1^5 + c1 - 1 > 0
            b1 = c1
    else
            ans1 = c1
    end
    c2 = (a2+b2)/2
    if sin(c2) - 6*c2 - 5 < 0
        b2 = c2
    elseif sin(c2) - 6*c2 - 5 > 0
            a2 = c2
    else
            ans2 = c2
    end
    c3 = (a3+b3)/2
    if log10(c3) + c3^2 -3 < 0
        a3 = c3
    elseif log10(c3) + c3^2 -3 > 0
            b3 = c3
    else
            ans3 = c3
    end
    n = n-1
end
if ans1 == 0
    ans1 = (a1+b1)/2
end
if ans2 == 0
    ans2 = (a2+b2)/2
end
if ans3 == 0
    ans3 = (a3+b3)/2
end