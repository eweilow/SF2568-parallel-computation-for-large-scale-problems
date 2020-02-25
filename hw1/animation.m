
clc
close all
clear


iLog = linspace(-1, 10, 200);
val = round(10.^iLog, 4)

for i = 1:length(val)
    fprintf("mpirun --allow-run-as-root -np 8 main zoom1_%d 2000 -0.9973 -0.2875 %.4f && \\\n", i, val(i));
end

for i = 1:length(val)
    outputImage(sprintf("./data/zoom1_%d.bin", i), sprintf("./zoom1_%d.png", i), 1);
    drawnow
end
