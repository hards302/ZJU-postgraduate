format long
n = 10
A = zeros(n,n)
for i = 1 : n
    for j = 1 : n
        A(i,j)=5/(i+2*j-1);
    end
end
x = ones(n,1)
b = A*x
xc = A\b
forward_error = xc-x
inf_norm = norm(forward_error, inf)
rela_fe = norm(forward_error, inf)/norm(x, inf)
rela_be = norm(b-A*xc, inf)/norm(b, inf)
error_mag_fac = rela_fe/rela_be
condtion_num = cond(A)