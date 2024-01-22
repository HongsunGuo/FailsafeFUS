% Define the x values
x = 0:0.1:100;

% Parameters for multiple Gaussian peaks
% For each peak: [height, center, width]
peaks = [5, 20, 3;  % Peak 1
         7, 50, 5;  % Peak 2
         4, 75, 2]; % Peak 3

% Initialize the y values
y = zeros(size(x));

% Add each Gaussian peak to the y values
for i = 1:size(peaks, 1)
    a = peaks(i, 1);
    b = peaks(i, 2);
    c = peaks(i, 3);
    y = y + a * exp(-(x - b).^2 / (2 * c^2));
end

% Add Gaussian noise
noise_amplitude = 0.3;  % Adjust this value for more or less noise
noise = noise_amplitude * randn(size(x));
y_noisy = y + noise;

% Add a monotonically increasing baseline
baseline_slope = 0.06;  % Adjust this value to control the slope of the baseline
baseline = baseline_slope * x;
y_monotonic_baseline = y_noisy + baseline;

% Plotting the curve with noise and monotonic baseline increase
plot(x, y_monotonic_baseline);
xlabel('x');
ylabel('y with noise and monotonic baseline increase');
title('Curve with Multiple Gaussian Peaks, Noise, and Monotonic Baseline Increase');

N = 10;             % Filter order, choose based on your requirements
Fc = 0.05;           % Cutoff frequency (normalized from 0 to 1, where 1 is half the sampling rate)
windowType = 'low'; % Window type, e.g., Hamming, Hann, Blackman, etc.

% Designing the FIR filter
b = fir1(N, Fc, windowType);
% Apply the FIR filter to the signal
% y_denoise = filtfilt(b, 1, y_monotonic_baseline);

y_denoise = movmean(y_monotonic_baseline, 20);
%find peaks
nHalfWin = 30;
peakXind = [];
for k=1 + nHalfWin:length(y_denoise) - nHalfWin
    lo = k - nHalfWin;
    if lo <= 1
        lo = 1;
    end
    up = k + nHalfWin;
    if(k>= length(y_denoise))
        up = length(y_denoise);
    end
    found = true;
    while(lo < up)
        if y_denoise(lo) > y_denoise(k)
            found = false;
            break;
        end
        lo = lo + 1;
    end
    if found
        xline(k./10);
        peakXind = [peakXind; k];
    end
end



% yy = y_denoise;
% x_tang = [];
% nStep = 5;
% for k=2:length(yy)
%     slope = yy(k) - yy(k-1);
%     lo = k - nStep;
%     if lo < 1
%         lo = 1;
%     end
%     
%     up = k+ nStep;
%     if up > length(yy)
%         up = length(yy);
%     end
%     
%     isT = 1;
%     while(lo < up)
%         if(yy(lo) < slope * (lo - k) + yy(k) )
%             isT = 0;
%             break;
%         end
%         lo = lo + 1;
%     end
%     if isT == 1
%         x_tang = [x_tang; k];
%         hold on;
%         xline(k./10);
%     end
%     
% end
% hold on;
yline(0);


% splineFit = spline(x, y_denoise);
% y_fittedCurve = ppval(splineFit, x);

% Evaluate spline
hold on;
% plot(x, y_fittedCurve, 'r', 'linewidth', 2);
plot(x, y_denoise, 'r', 'linewidth', 2);

%y_corrected = detrend(y_denoise);

nStep = 1;
% y_denoise = y_fittedCurve;
yyy = y_denoise(nStep + 1:end) - y_denoise(1:end-nStep);
% plot(x(2:end), 5.*(y_denoise) - 1), 'b', 'linewidth', 2);
% yyy = movmean(yyy, 10);
x1 = x(nStep + 1:end);
% yyy = downsample(yyy, 3);
% xxx = downsample(x1, 3);
yyy = movmean(yyy, 10);
plot(x1, 20* yyy, 'k', 'linewidth', 1);


% % Step 1: Identify tangent points
% signal = y_denoise;
% [~, minLocations] = findpeaks(-signal);
% 
% % Step 2: Interpolate to estimate the baseline
% baseline = interp1(minLocations, signal(minLocations), 1:length(signal), 'linear', 'extrap');
% 
% % Step 3: Subtract the baseline
% y_corrected = signal - baseline;

% hold on;
% plot(x, y_corrected, 'k', 'linewidth', 2);


% Open a file for writing
fileID = fopen('xy_data.txt', 'w');

% Check if the file was opened successfully
if fileID == -1
    error('File could not be opened.');
end

% Write each pair of x and y values to the file
for i = 1:length(x)
    fprintf(fileID, '%f %f\n', x(i), y_monotonic_baseline(i));
end

% Close the file
fclose(fileID);

