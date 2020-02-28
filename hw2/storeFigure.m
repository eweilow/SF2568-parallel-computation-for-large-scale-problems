function storeFigure(name)
    [dir] = fileparts(name);
    if ~exist(dir, 'dir')
      mkdir(dir)
    end
    
    set(gca,'FontSize',14)
    %saveas(gcf,sprintf('%s.svg', name))
    saveas(gcf,sprintf('%s.png', name))
    
    fprintf("Saved to %s.png\n", name);
end

