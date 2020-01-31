fid = fopen("./data/data.bin",'r');
format = 'int64';
size = fread(fid,1,format); % this one works
data = fread(fid,size*size,format); % this one works
fclose(fid);

C = reshape(data, [size size]);
%%
clf

Cdata = log(C+10);
Cmax = max(max(Cdata))
Cmin = min(min(Cdata))


Scaled = (Cdata - Cmin) / (Cmax - Cmin);

imagesc(Scaled * max(data))
caxis([0 max(data)]);

colorbar
axis equal;
%%
imwrite(Scaled * length(colormap),colormap,"./img.png")