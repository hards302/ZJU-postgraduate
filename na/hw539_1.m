A = [12 -4 -2; 19 -19 -10; -35 52 27]; % �滻Ϊ��ľ���
max_iterations = 100; % ����������
tolerance = 1e-6; % �����ݲ�

[lambda, v] = power_iteration(A, max_iterations, tolerance);

disp('������ֵ:');
disp(lambda);
[reyv, reylambda] = eigs(A, 1, 'lm');
disp('rey lambda');
disp(reylambda);
function [lambda, v] = power_iteration(A, max_iterations, tolerance)
    % �������һ����ʼ����
    n = size(A, 1);
    v = rand(n, 1);
    v = v / norm(v);

    % ��������������ֵ����������
    for i = 1:max_iterations
        v_old = v;
        v = A * v;
        v = v / norm(v);
        
        % ��������ֵ�Ĺ���ֵ
        lambda = v' * A * v;
        
        % �����������
        if norm(v - v_old) < tolerance
            break;
        end
    end
end