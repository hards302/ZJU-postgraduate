coefficients = [1 0 -14 0 20 -16];

% �������ʽ���䵼���ĺ������
f = @(x) polyval(coefficients, x);
df = @(x) polyval(polyder(coefficients), x);

% ���õ�������������������ֵ
maxIterations = 100;
tolerance = 1e-6;

% ��ʼ����������
roots = zeros(6, 1);

% ������⸴��
for i = 1:6
    % ���ѡ��һ����ʼ�²�ֵ
    x = randn() + 1i * randn();

    % ţ�ٵ���
    for j = 1:maxIterations
        fx = f(x);
        dfx = df(x);
        x = x - fx / dfx;

        % �ж��Ƿ�����
        if abs(fx) < tolerance
            roots(i) = x;
            break;
        end
    end
end
disp(roots)