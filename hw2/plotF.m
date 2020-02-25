clear

inputData = "./data/data.bin";
fid = fopen(inputData,'r');
N = fread(fid, 1, "int64"); % this one works
iters = fread(fid, 1, "int64"); % this one works
data = fread(fid, N, "float64")'; % this one works
fclose(fid);


% %%
% 
% r = @(x) -x;
% f = @(x) 2.0 - x * r(x) * (x - 1.0);
% 
% dydt = @(t, x) [ x(2); (f(x(1)) - r(x(1)*x(1))) ];
%     
% [xt, ut] = ode45(dydt, [0, 1], [0 0]);

%%
clc
x = linspace(0, 1, N+2);
x = x(2:end-1);


u = -10.*x.*(x-1).*(x-0.5);


figure(1);
clf
title(sprintf('iters = %d', iters));
plot(x, u, 'DisplayName', 'Real')
hold on
% plot(xt, ut(:,1), 'DisplayName', 'ODE45')
plot(x, data, 'DisplayName', 'Our code')

legend('show', 'Location', 'best');


figure(2);
clf
title(sprintf('iters = %d', iters));
semilogy(x, abs(u - data), 'DisplayName', 'Real')