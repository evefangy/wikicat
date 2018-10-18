clc 
clear
% define figure properties
opts.Colors     = get(groot,'defaultAxesColorOrder');
opts.saveFolder = '/Users/eve/Desktop/';
opts.width      = 20;
opts.height     = 15;
% opts.fontType   = 'Times';
% opts.fontSize   = 20;

T = readtable('/Users/eve/Desktop/plot.txt');

% create new figure
fig = figure; clf

% hist([T.rev, T.rev_semi_random, T.complete_random])
hist([T.BoW, T.BoW_semi_random, T.complete_random])
set(gcf,'color','w');
% set(gca,'FontSize',14,'FontName','Times')

% legend({'Revision Tree', 'Semi-Random Revision Tree', 'Random Tree'},'FontSize',12)
legend({'BoW Tree', 'Semi-Random BoW Tree', 'Random BoW Tree'},'FontSize',12)

% title("Number of Paths vs. Avgerage Similarity Score")
title("Number of Paths vs. Best Similarity Score")

ylabel("Count")
% xlabel("Average Similarity Score")
xlabel("Best Similarity Score")


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
print([opts.saveFolder 'my_figure'], '-dpng', '-r600')