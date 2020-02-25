clear

clc

inputData = "./data/data.bin";
fid = fopen(inputData,'r');
iterInterval = fread(fid, 1, "int64")
countIters = fread(fid, 1, "int64")
xv = fread(fid, 1, "float64")
convData = fread(fid, countIters, "float64")';


N = fread(fid, 1, "int64");
iters = fread(fid, 1, "int64");
data = fread(fid, N, "float64")';
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


analytic = @(x) -10.*x.*(x-1).*(x-0.5);
u = analytic(x);

figure(3)
clf
errx = 0:iterInterval:iters-iterInterval;
err = abs(convData - analytic(xv));
semilogy(errx, err, '-');

figure(1);
clf
title(sprintf('iters = %d', iters));
S = floor(linspace(1,length(x),18));
S = S(2:end-1);
plot(x(S), u(S), 'o', 'DisplayName', 'Real')
hold on
% plot(xt, ut(:,1), 'DisplayName', 'ODE45')
plot(x, data, 'DisplayName', 'Our code')

legend('show', 'Location', 'best');


figure(2);
clf
title(sprintf('iters = %d', iters));
semilogy(x, abs(u - data), 'DisplayName', 'Real')