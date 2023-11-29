format long
A = [4 0; 3 1]
[m, n] = size(A);
R = A;
Q = eye(m);

for k = 1:n
    x = R(k:m, k);
    v = x;
    v(1) = v(1) + sign(x(1)) * norm(x);
    v = v / norm(v);
    R(k:m, k:n) = R(k:m, k:n) - 2 * v * (v' * R(k:m, k:n));
    Q(k:m, :) = Q(k:m, :) - 2 * v * (v' * Q(k:m, :));
end
disp('Q=')
disp(Q)
disp('R=')
disp(R)
[Q1,R1] = qr(A)