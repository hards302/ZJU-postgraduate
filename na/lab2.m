N = 20000000;
main_diag = ones(N,1);
A = spdiags([main_diag, 2*main_diag, main_diag], [-1, 0, 1], N, N);
b = ones(N,1);
x = LU_solve(A,b);
function x = LU_solve(A, b)
    N = size(A, 1);
    L = speye(N);
    U = sparse(N, N);

    U(1, 1) = A(1, 1);
    for i = 2:N
        L(i, i-1) = A(i, i-1) / U(i-1, i-1);
        U(i-1, i) = A(i-1, i);
        U(i, i) = A(i, i) - L(i, i-1) * U(i-1, i);
    end

    % Solve Ly = b using forward substitution
    y = zeros(N, 1);
    y(1) = b(1);
    for i = 2:N
        y(i) = b(i) - L(i, i-1) * y(i-1);
    end

    % Solve Ux = y using backward substitution
    x = zeros(N, 1);
    x(N) = y(N) / U(N, N);
    for i = N-1:-1:1
        x(i) = (y(i) - U(i, i+1) * x(i+1)) / U(i, i);
    end
end
function [L, U] = LU_Factorization_block(A, block_size)
    N = size(A, 1);
    num_blocks = ceil(N / block_size);
    L = speye(N);
    U = sparse(N, N);

    for k = 1:num_blocks
        start_idx = (k-1) * block_size + 1;
        end_idx = min(k * block_size, N);

        % LU分解子块
        [L(start_idx:end_idx, start_idx:end_idx), U(start_idx:end_idx, start_idx:end_idx)] = lu(A(start_idx:end_idx, start_idx:end_idx));
        
        % 更新下一个子块
        if k < num_blocks
            next_start_idx = end_idx + 1;
            next_end_idx = min((k+1) * block_size, N);
            A(next_start_idx:next_end_idx, start_idx:end_idx) = A(next_start_idx:next_end_idx, start_idx:end_idx) - L(next_start_idx:next_end_idx, start_idx:end_idx) * U(start_idx:end_idx, start_idx:end_idx);
        end
    end
end