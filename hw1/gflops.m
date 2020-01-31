clf;
clear
N = 1:100;


kappa_s = 0.1;

sigma_s = 2;

kappa_p = 1 - kappa_s;
sigma_p = sigma_s ./ (kappa_s + kappa_p ./ N);

plot(N, sigma_p, 'LineWidth', 2, 'DisplayName', 'Parallelized FLOPS')
hold on;
plot([min(N) max(N)], [sigma_s sigma_s], 'LineWidth', 2, 'DisplayName', 'Serial FLOPS')
plot(N, N.*sigma_s, 'LineWidth', 2, 'DisplayName', 'Linear speedup')
ylim([0, max(sigma_p)]);
xlabel("processor count");
ylabel("GFLOPS");
legend('show', 'Location', 'Best');

sigma_p(end)

set(gca, 'FontSize', 14);

saveas(gcf,"./plot.png")