clear

inputData = "./data/data.bin";
fid = fopen(inputData,'r');
N = fread(fid, 1, "int64"); % this one works
iters = fread(fid, 1, "int64"); % this one works
data = fread(fid, N, "float64")'; % this one works
fclose(fid);


%%

x = linspace(0, 1, N+2);
x = x(2:end-1);

u = x.*(x-1);

figure(1);
clf
title(sprintf('iters = %d', iters));
plot(x, u, 'DisplayName', 'Real')
hold on
plot(x, data, 'DisplayName', 'Our code')

legend('show', 'Location', 'best');


figure(2);
clf
title(sprintf('iters = %d', iters));
semilogy(x, abs(u - data), 'DisplayName', 'Real')