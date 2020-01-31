clear
fid = fopen("./data/data.bin",'r');
format = 'int64';
xlims = fread(fid,2,'float64'); % this one works
ylims = fread(fid,2,'float64'); % this one works
sizez = fread(fid,1,format); % this one works
data = fread(fid,sizez*sizez,format); % this one works
fclose(fid);

S = [sizez sizez];
C = reshape(data, S);
%%
clf

xlims
ylims

Cdata = log(C+10);
Cmax = max(max(Cdata))
Cmin = min(min(Cdata))

clipX = 1;
clipY = 1;

rY = (1 - clipX)/2;
rX = (1 - clipY)/2;

round(S(1)*(1 - rX))

Cdata2 = Cdata; %Cdata(...
  %  round(S(1)*rX):round(S(1)*(1 - rX)),...
  %  round(S(2)*rY):round(S(2)*(1 - rY)));

S2 = size(Cdata)

Scaled = (Cdata2 - Cmin) / (Cmax - Cmin);

N = 5;
imagesc(Scaled * max(data))
caxis([0 max(data)]);
xticks(linspace(0, S2(1), N));
yticks(linspace(0, S2(2), N));
xticklabels(linspace(xlims(1), xlims(2), N))
yticklabels(linspace(ylims(1), ylims(2), N))

colorbar
axis equal;
%%
imwrite(Scaled * length(colormap),colormap,"./img.png")