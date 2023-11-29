n = 3
A = [1 -1 -1; -1 2 1; -1 1 2]
n = size(A, 1);
R = zeros(n, n);
for i=1:n
    for k=1:i-1
        R(i, i) = R(i, i) + R(i, k)^2;
    end
    R(i, i) = sqrt(A(i, i) - R(i, i));
    for j=i+1:n
        for k=1:i-1
            R(j, i) = R(j, i) + R(j, k)*R(i, k);
        end
        R(j, i) = (A(j, i) - R(j, i))/R(i, i);
    end
end
disp('R =')
disp(R)