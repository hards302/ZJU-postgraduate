format long
x = [0.6 0.7 0.8 0.9 1.0]
y = [1.433329 1.632316 1.896481 2.247908 2.718282]
y0=polyinterp(x,y,0.98)
error = exp(0.98^2)-y0
t = []
p = []
for s=0:0.01:2
    p(end+1) = exp(s^2)-polyinterp(x,y,s)
    t(end+1) = s
end
%t = [0.5 0.6 0.7 0.8 0.9 1.0]
%p = [-0.301158331225793 0.000041456034028 0.000021995537880 -0.000012069504840 -0.000001332352806 -0.000017154095433]
plot(t, p)
xlabel('t')
ylabel('p')
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