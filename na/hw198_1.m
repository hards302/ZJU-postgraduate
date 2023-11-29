A = [4 2 3 0; -2 3 -1 1; 1 3 -4 2;1 0 1 -1; 3 1 3 -2]
b = [10; 0; 2; 0; 5]
x = lsqnonneg(A,b)
error_norm = norm(A*x-b,2)