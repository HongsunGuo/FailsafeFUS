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
noise_amplitude = 0.5;  % Adjust this value for more or less noise
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

