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

foxesDeadIndex = find(~sum(foxCount));

tspan = timesteps;
if ~isempty(foxesDeadIndex)
  %tspan = foxesDeadIndex(1)
end
xlims = [0 tspan]

indices = 1:timesteps;

clf
subplot(2,3,1);
plot(indices, sum(rabbitCount) / tileCount, 'LineWidth', 2, 'DisplayName', 'Rabbits')
% plot(sum(rabbitCount), 'DisplayName', 'Rabbits')
% hold on
% plot(sum(foxCount), 'DisplayName', 'Foxes')
title("Average rabbits per tile");
legend('show', 'Location', 'Best')
xlim(xlims)

subplot(2,3,2);
% plot(indices, sum(rabbitCount) / tileCount, 'LineWidth', 1, 'DisplayName', 'Rabbits')
plot(indices, sum(foxCount) / tileCount, 'LineWidth', 2, 'DisplayName', 'Foxes')
title("Average foxes per tile");
legend('show', 'Location', 'Best')
xlim(xlims)

subplot(2,3,3);
totalRabbitAgePerTile = sum(totalRabbitAge)./max(1, sum(rabbitCount));
plot(totalRabbitAgePerTile, 'LineWidth', 1,  'DisplayName', 'Rabbits')
hold on
totalFoxAgePerTile = sum(totalFoxAge)./max(1, sum(foxCount));
plot(totalFoxAgePerTile, 'LineWidth', 1,  'DisplayName', 'Foxes')
title("Age");
legend('show', 'Location', 'Best')
xlim(xlims)

subplot(2,3,4);
plot(sum(totalFoxHunger)./max(1, sum(foxCount)), 'LineWidth', 1,  'DisplayName', 'Foxes')
title("Average hunger");
legend('show', 'Location', 'Best')
xlim(xlims)

subplot(2,3,5);
plot(sum(vegetation) / tileCount)
ylim([0 1.1])
xlim(xlims)
title("Vegetation");

subplot(2,3,6);
plot(max(maxRabbitAge), 'LineWidth', 1,  'DisplayName', 'Rabbits')
hold on
plot(max(maxFoxAge), 'LineWidth', 1,  'DisplayName', 'Foxes')
title("Max age");
legend('show', 'Location', 'Best')
xlim(xlims)