clc 
clear

T = readtable('/Users/eve/Desktop/Two_Trees/BoW/output.txt');
T.Properties.VariableNames = {'Freq'};
myStd = std(T.Freq)
myMean = mean(T.Freq)
myMax = max(T.Freq)
hist(T.Freq)
% find a good cut-off from histogram and verify if any info may be missing

[N,edges] = histcounts(T.Freq); 
% plot(edges(101:895), N(100:894))

