coefficients = [1 0 -14 0 20 -16];

% 定义多项式及其导数的函数句柄
f = @(x) polyval(coefficients, x);
df = @(x) polyval(polyder(coefficients), x);

% 设置迭代的最大次数和收敛阈值
maxIterations = 100;
tolerance = 1e-6;

% 初始化根的向量
roots = zeros(6, 1);

% 迭代求解复根
for i = 1:6
    % 随机选择一个初始猜测值
    x = randn() + 1i * randn();

    % 牛顿迭代
    for j = 1:maxIterations
        fx = f(x);
        dfx = df(x);
        x = x - fx / dfx;

        % 判断是否收敛
        if abs(fx) < tolerance
            roots(i) = x;
            break;
        end
    end
end
disp(roots)