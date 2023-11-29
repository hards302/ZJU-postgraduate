coefficients = [1 0 -14 0 20 -16];

% ��������Ϊ0
while coefficients(end) == 0
    coefficients = coefficients(1:end-1)
    coefficients(end) = -coefficients(end)
    disp('0')
end

% ����������
n = length(coefficients) - 1;
A = zeros(n);
A(1, :) = -coefficients(2:end) / coefficients(1);
for i = 2:n
    A(i, i-1) = 1;
end
% ��������ֵ
roots = eig(A);

disp(roots)