clear

clc


P = 4;

for p = 0:(P-1)
  inputData = sprintf("../data_p%d.red.bin", p);
  
  fid = fopen(inputData,'r');
  reduced = fread(fid, 1, "int64");
  rank = fread(fid, 1, "int64");
  tileCount = fread(fid, 1, "int64");
  tileSize = fread(fid, 1, "double");
  timesteps = fread(fid, 1, "int64");
  rabbitSize = fread(fid, 1, "int64");
  foxSize = fread(fid, 1, "int64");
    
  if p == 0
      vegetation = zeros(tileCount, timesteps);
      foxCount = zeros(tileCount, timesteps);
      rabbitCount = zeros(tileCount, timesteps);
      totalFoxHunger = zeros(tileCount, timesteps);
      totalFoxAge = zeros(tileCount, timesteps);
      totalRabbitAge = zeros(tileCount, timesteps);
      maxFoxAge = zeros(tileCount, timesteps);
      maxRabbitAge = zeros(tileCount, timesteps);
  end
  
  for i = 1:tileCount
    x = fread(fid, 1, "double");
    y = fread(fid, 1, "double");
    id = fread(fid, 1, "uint64");
    tileProcess = fread(fid, 1, "int64");
    isOwnedByThisProcess = fread(fid, 1, "int8");
    isWaterTile = fread(fid, 1, "int8");

    if isOwnedByThisProcess
      historicalDataCount = fread(fid, 1, "int64");

      N = historicalDataCount;
      tile_vegetation = zeros(1, historicalDataCount);
      tile_foxCount = zeros(1, historicalDataCount);
      tile_rabbitCount = zeros(1, historicalDataCount);
      tile_totalFoxHunger = zeros(1, historicalDataCount);
      tile_totalFoxAge = zeros(1, historicalDataCount);
      tile_totalRabbitAge = zeros(1, historicalDataCount);
      tile_maxFoxAge = zeros(1, historicalDataCount);
      tile_maxRabbitAge = zeros(1, historicalDataCount);

      for j = 1:historicalDataCount
        tile_vegetation(j) = fread(fid, 1, "double");
        tile_foxCount(j) = fread(fid, 1, "int64");
        tile_rabbitCount(j) = fread(fid, 1, "int64");
        tile_totalFoxHunger(j) = fread(fid, 1, "double");
        tile_totalFoxAge(j) = fread(fid, 1, "int64");
        tile_totalRabbitAge(j) = fread(fid, 1, "int64");
        tile_maxFoxAge(j) = fread(fid, 1, "int64");
        tile_maxRabbitAge(j) = fread(fid, 1, "int64");
      end

      vegetation(i, :) = tile_vegetation;
      foxCount(i, :) = tile_foxCount;
      rabbitCount(i, :) = tile_rabbitCount;
      totalFoxHunger(i, :) = tile_totalFoxHunger;
      totalFoxAge(i, :) = tile_totalFoxAge;
      totalRabbitAge(i, :) = tile_totalRabbitAge;
      maxFoxAge(i, :) = tile_maxFoxAge;
      maxRabbitAge(i, :) = tile_maxRabbitAge;
    end
  end

  fclose(fid);
end


%%

%%

runName = "run1";

foxesDeadIndex = find(~sum(foxCount));

tspan = timesteps;
if ~isempty(foxesDeadIndex)
  %tspan = foxesDeadIndex(1)
end
xlims = [0 tspan] / 7;

indices = 1:timesteps;
indices = indices / 7;

close all

semilogy(indices, sum(rabbitCount) / tileCount, 'LineWidth', 2, 'DisplayName', 'Rabbits')
hold on
semilogy(indices, sum(foxCount) / tileCount, 'LineWidth', 2, 'DisplayName', 'Foxes')
% plot(sum(rabbitCount), 'DisplayName', 'Rabbits')
% hold on
% plot(sum(foxCount), 'DisplayName', 'Foxes')
xlabel("Week");
title("Average count of animals per tile");
legend('show', 'Location', 'Best')
xlim(xlims)
storeFigure(sprintf("%s-averageAnimals", runName));
clf

semilogy(indices, sum(rabbitCount) / tileCount, 'LineWidth', 2, 'DisplayName', 'Rabbits')
% plot(sum(rabbitCount), 'DisplayName', 'Rabbits')
% hold on
% plot(sum(foxCount), 'DisplayName', 'Foxes')
xlabel("Week");
title("Average rabbits per tile");
xlim(xlims)
storeFigure(sprintf("%s-averageRabbits", runName));
clf

% plot(indices, sum(rabbitCount) / tileCount, 'LineWidth', 1, 'DisplayName', 'Rabbits')
plot(indices, sum(foxCount) / tileCount, 'LineWidth', 2, 'DisplayName', 'Foxes')
xlabel("Week");
title("Average foxes per tile");
xlim(xlims)
storeFigure(sprintf("%s-averageFoxes", runName));
clf

totalRabbitAgePerTile = sum(totalRabbitAge)./max(1, sum(rabbitCount));
plot(indices, totalRabbitAgePerTile, 'LineWidth', 2,  'DisplayName', 'Rabbits')
hold on
totalFoxAgePerTile = sum(totalFoxAge)./max(1, sum(foxCount));
plot(indices, totalFoxAgePerTile, 'LineWidth', 2,  'DisplayName', 'Foxes')
xlabel("Week");
title("Average age");
legend('show', 'Location', 'Best')
storeFigure(sprintf("%s-averageAge", runName));
clf

plot(indices, sum(totalFoxHunger)./max(1, sum(foxCount)), 'LineWidth', 2,  'DisplayName', 'Foxes')
title("Average hunger");
% legend('show', 'Location', 'Best')
xlim(xlims)
xlabel("Week");
storeFigure(sprintf("%s-averageHunger", runName));
clf

plot(indices, sum(vegetation) / tileCount, 'LineWidth', 2)
ylim([0 1.1])
xlim(xlims)
title("Vegetation");
xlabel("Week");
storeFigure(sprintf("%s-vegetation", runName));
clf
% 
% subplot(2,3,6);
% plot(max(maxRabbitAge), 'LineWidth', 1,  'DisplayName', 'Rabbits')
% hold on
% plot(max(maxFoxAge), 'LineWidth', 1,  'DisplayName', 'Foxes')
% title("Max age");
% legend('show', 'Location', 'Best')
% xlim(xlims)
% storeFigure(sprintf("%s-vegetation", runName));