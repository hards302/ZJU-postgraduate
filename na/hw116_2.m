n = 100
A = zeros(n, n)
A(1,1)=2; A(1,2)=1;
A(100,99)=1; A(100,100)=2
for i = 2: 99
    A(i, i-1)=1; A(i,i)=2; A(i, i+1)=1;
end
b = zeros(n, 1)
b(1, 1)=1; b(100, 1)=-1
x0 = zeros(n, 1)
eps = 0.005
D = diag(diag(A));
L = tril(A, -1);
U = triu(A, 1);
T = -inv(D) * (L + U);
C = inv(D) * b;
x = x0;
k = 0
while k > -1
    k = k+1
    x_old = x;
    x = T * x_old + C;
    if norm(x - x_old, inf) < eps
        break;
    end
end
disp(x)
disp(k)
backword_error = b-A*x
