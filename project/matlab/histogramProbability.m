clf
clear
ages = [];

center = 6*28;
span = 0:center*2;
width = 1;

sigmoid = @(x, center) exp((x - center) / sqrt(center*width)) ./ (exp((x - center)/sqrt(center*width)) + 1.0) / sqrt(center*width);

clf
ages = span;
probability = sigmoid(ages, center);
plot(ages, probability, 'LineWidth', 2)
hold on
lims = ylim;
plot([center center], lims, '--', 'LineWidth', 2);
title("Probability of death");
xlabel("Age (days)");
ylabel("Probability to die at given age");

storeFigure("./prob1");

clf
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
xlabel("Age of death (days)");
ylabel("Frequency");
title("Age of death");
storeFigure("./prob2");