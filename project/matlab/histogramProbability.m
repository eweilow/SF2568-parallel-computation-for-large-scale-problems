clf
clear
ages = [];

center = 150;
span = 0:center*2;
width = 5;

sigmoid = @(x, center) exp((x - center) / sqrt(center*width)) ./ (exp((x - center)/sqrt(center*width)) + 1.0) / sqrt(center*width);

subplot(2,1,1);
ages = span;
probability = sigmoid(ages, center);
plot(ages, probability)
hold on
lims = ylim;
plot([center center], lims, '--', 'LineWidth', 2);

sample = @(age) sigmoid(age, center);

for i = 1:10000
    for age=1:center*2
        if(rand() < sample(age))
            ages(end+1) = age;
            break
        end
    end
end

histogram(ages)
xlim([0, center*2])
hold on
lims = ylim;
plot([center center], lims, '--', 'LineWidth', 2);