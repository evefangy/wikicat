clc 
clear

opts.Colors     = get(groot,'defaultAxesColorOrder');
opts.saveFolder = '/Users/eve/Desktop/';
opts.width      = 15;
opts.height     = 10;

T = readtable('/Users/eve/Desktop/topical_measure.txt');
T.Properties.VariableNames = {'Art' 'Edit_Count' 'Top_m' 'Top'};

T2 = readtable('/Users/eve/Desktop/top_bar.txt');
T2.Properties.VariableNames = {'Topical_Measurement'};

fig = figure; clf
set(gcf,'color','w');
set(gca,'FontSize',14)

x1 = T.Edit_Count;
[a1,b1]=hist(x1, 4000);
x2 = T2.Topical_Measurement;
[a2,b2]=hist(x2, 4000);

upper_lim = 100;

x = a1-a2;
y = b1;
x = [a2', x'];
x = x(1:upper_lim, :);
y = y(1:upper_lim);


% x = log(x)

name = {};

gap = round(upper_lim/7);
for i=1:upper_lim
    name{i} = num2str(y(i));
end 

curr = 1;
for i=1:7
    name{i} = name{curr+(i-1)*gap};
end
bar(x, 'stacked')
% set(gca,'xticklabel',name)
xlabel('Number of Articles Edited')
ylabel('Number of Editors')
title('Number of Articles Edited by Editors')








% scaling
fig.Units               = 'centimeters';
fig.Position(3)         = opts.width;
fig.Position(4)         = opts.height;

% set text properties
set(fig.Children, ...
    'FontName',     'Times', ...
    'FontSize',     14);

% remove unnecessary white space
set(gca,'LooseInset',max(get(gca,'TightInset'), 0.02))

% export to png
fig.PaperPositionMode   = 'auto';
print([opts.saveFolder 'topical'], '-dpng', '-r600')