clc;clear;
sanityData = load('.\waveform_data.csv');
fs = 800000;
t = (0:length(sanityData)-1)/fs;
figure;
subplot(211);
plot(t.*1000, sanityData, 'k');
xlim([0 20]);
xlabel('Time (ms)');
ylabel('Amplitude (V)');
%
mainWinData= load('.\abnorm_AI_data.csv');
fs = 600000;
t = (0:length(mainWinData)-1)/fs;
subplot(212);
plot(t.*1000, mainWinData, 'k');
xlim([0 20]);
xlabel('Time (ms)');
ylabel('Amplitude (V)');