A = [1 -1 1 2;0 2 1 0;1 3 4 4; 0 2 1 -1]
n = 4
L = eye(n);
U = A;
for k = 1:n-1
    for j = k+1:n
        L(j,k) = U(j,k) / U(k,k);
        U(j,k:n) = U(j,k:n) - L(j,k) * U(k,k:n);
    end
end
disp(L)
disp(U)