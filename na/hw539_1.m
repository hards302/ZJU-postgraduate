A = [12 -4 -2; 19 -19 -10; -35 52 27]; % 替换为你的矩阵
max_iterations = 100; % 最大迭代次数
tolerance = 1e-6; % 收敛容差

[lambda, v] = power_iteration(A, max_iterations, tolerance);

disp('主特征值:');
disp(lambda);
[reyv, reylambda] = eigs(A, 1, 'lm');
disp('rey lambda');
disp(reylambda);
function [lambda, v] = power_iteration(A, max_iterations, tolerance)
    % 随机生成一个初始向量
    n = size(A, 1);
    v = rand(n, 1);
    v = v / norm(v);

    % 迭代计算主特征值和特征向量
    for i = 1:max_iterations
        v_old = v;
        v = A * v;
        v = v / norm(v);
        
        % 计算特征值的估计值
        lambda = v' * A * v;
        
        % 检查收敛条件
        if norm(v - v_old) < tolerance
            break;
        end
    end
end