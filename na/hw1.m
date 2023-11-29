format long
x = 1.00001
y1 = 1
n = 0
while n < 50
    y1 = 1 + x*y1
    n = n+1
end
y2 = (x^51-1)/(x-1)
e = abs(y1-y2)