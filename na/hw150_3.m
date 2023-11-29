x = [0 2 3]
y = [1 2 4]
y0=polyinterp(x,y,6);
function y0 = polyinterp(x, y, x0)
    n = length(x)
    c = newtdd(x, y, n)
    y0 = nest(n-1, c, x0, x)
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