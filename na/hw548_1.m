a = [-7 -8 1; 17 18 -1; -8 -8 2]
lam = shifted_qr(a,1e-14);
disp(lam)
function lam = shifted_qr(a, tol)
    m=size(a,1);lam=zeros(m,1);
    n=m;
    kounttol=500;
    while n>1
        kount=0;
        while max(abs(a(n,1:n-1)))>tol & kount<kounttol
            kount=kount+1; % keep track of number of qr��s
            mu=a(n,n); % shift is mu
            [q,r]=qr(a-mu*eye(n));
            a=r*q+mu*eye(n);
        end
        if kount<kounttol % have isolated 1x1 block
            lam(n)=a(n,n); % declare eigenvalue
            n=n-1;
            a=a(1:n,1:n); % deflate by 1
        else % have isolated 2x2 block
            disc=(a(n-1,n-1)-a(n,n))^2+4*a(n,n-1)*a(n-1,n);
            lam(n)=(a(n-1,n-1)+a(n,n)+sqrt(disc))/2;
            lam(n-1)=(a(n-1,n-1)+a(n,n)-sqrt(disc))/2;
            n=n-2;
            a=a(1:n,1:n); % deflate by 2
        end
    end
    if n>0;lam(1)=a(1,1);end
end