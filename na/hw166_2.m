t = []
p = []
for m = -100:0.1:100
    t(end+1)=m
    p(end+1)=cos2(m)
end
plot(t,p)
function y=cos2(x)
n=10
b=pi/4+(pi/4)*cos((1:2:2*n-1)*pi/(2*n))
yb=cos(b)
c=newtdd(b,yb,n);
s=1
x1=mod(x,2*pi)
if x1>pi
    x1 = 2*pi-x1
end
if x1>pi/2
    x1 = pi-x1;
    s = -1
end
y = s*nest(n-1,c,x1,b)
end
function c = newtdd(x,y,n)
    for j=1:n
        v(j,1)=y(j)
    end
    for i=2:n
        for j=1:n+1-i
            v(j,i)=(v(j+1,i-1)-v(j,i-1))/(x(j+i-1)-x(j));
        end
    end
    for i=1:n
        c(i)=v(1,i);
    end
end
function y=nest(d,c,x,b)
    if nargin<4, b=zeros(d,1);end
    y=c(d+1)
    for i=d:-1:1
        y = y.*(x-b(i))+c(i);
    end
end