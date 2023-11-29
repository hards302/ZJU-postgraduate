a = [2 1 -4;1 -1 1;-1 3 -2]
x = zeros(1,3)
b = [-7; -2; 6]
n = 3
for j = 1 : n-1
    if abs(a(j,j))<eps; error('zero pivot encoutered'); end
    for i = j+1 : n
        mult = a(i,j)/a(j,j)
        for k = j+1:n
            a(i,k)=a(i,k)-mult*a(j,k);
        end
        b(i)=b(i)-mult*b(j);
    end
end
for i = n : -1 : 1
    for j = i+1 : n
        b(i) = b(i) - a(i,j) *x(j);
    end
    x(i) = b(i)/a(i,i)
end