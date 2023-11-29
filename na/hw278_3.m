format long
n = 4; % 高斯点的数量
f = @(x) exp(x); % 定义函数

[x, w] = gauss(n) % 获取高斯点和权重

% 计算高斯逼近的积分
Is = sum(w .* f(x)')
exact = f(1)-f(-1)
error = Is-exact

% fprintf('The integral is: %f\n', I); % 输出结果
function [x, w] = gauss(n)
    beta = 0.5 ./ sqrt(1 - (2*(1:n-1)).^(-2)); % 3-term recurrence coeffs
    T = diag(beta, 1) + diag(beta, -1); % Jacobi matrix
    [V, D] = eig(T); % compute eigenvectors and eigenvalues
    x = diag(D); % Gauss nodes
    w = 2 * V(1, :).^2; % Gauss weights
end