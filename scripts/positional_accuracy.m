%{
    Measure the positional difference results for the current PID
    configuration. Provides a plot over time of the desired and current
    position values. Requires a serial connection to the Arduino to run.

    Before running, modify the print settings under 'platform.h' and
    ensure the platform has been successfully reset and calibrated.
%}

%% Position configuration
DESIRED_POS = 200;                                                         % actuator positions in [0, 1024]
POS_THRESHOLD = [ 4, 4, 4, 4, 4, 4 ];

%% Feedback configuration (NOTE: manually inputted from Arduino code)
P = [ 3, 3, 3, 3, 3, 3 ];
I = [ 0.03, 0.035, 0.03, 0.03, 0.03, 0.035 ];
D = [ 0.025, 0.025, 0.025, 0.025, 0.025, 0.025 ];

pid_str = {
    ['P=' num2str(P(1)) '    I=' num2str(I(1)) '    D=' num2str(D(1))]
    ['P=' num2str(P(2)) '    I=' num2str(I(2)) '    D=' num2str(D(2))]
    ['P=' num2str(P(3)) '    I=' num2str(I(3)) '    D=' num2str(D(3))]
    ['P=' num2str(P(4)) '    I=' num2str(I(4)) '    D=' num2str(D(4))]
    ['P=' num2str(P(5)) '    I=' num2str(I(5)) '    D=' num2str(D(5))]
    ['P=' num2str(P(6)) '    I=' num2str(I(6)) '    D=' num2str(D(6))]
};

%% Time configuration
DURATION = 20;                                                             % time (in s) for the data acquisition to run
dt       = 0.01;                                                           % time difference (in s) between measuring the positions
nt       = DURATION / dt + 1;                                              % time steps to measure, starting after t = 0

%% Serial configuration
PORT        = 'COM7';
BAUD_RATE   = 115200;
DATA_BITS   = 8;
PARITY      = 'none';
STOP_BITS   = 1;

% Establish a new serial connection (clear previous connections)
delete(instrfindall);
ser = serial(      PORT,        ...
	'BaudRate',    BAUD_RATE,   ...
	'DataBits',    DATA_BITS,   ...
	'Parity',      PARITY,      ...
	'StopBits',    STOP_BITS	...
);
fopen(ser);

%% Data acquisition
% Initialize the position matrix
pos = zeros(nt, 6);

% Send desired positions
pos_str = num2str(DESIRED_POS);
fprintf(ser, [strjoin({pos_str, pos_str, pos_str, pos_str, pos_str, pos_str}), '\n']);

% Measure remaining values for the set duration
for i = 1 : nt
    % Find initial time of loop i
    previous_time = datetime;
    
    % Get latest current actuator positions from serial
    pos(i, :) = str2num(fscanf(ser));
     
    % Wait until dt has passed before finishing the time step
    current_time = datetime;
    while (seconds(current_time - previous_time) < dt)
        current_time = datetime;
    end
end

%% Plot results
% Initialize figure
figure;
shg;
hold on;

% Specify axis ranges
x = linspace(0, DURATION, nt);
dy = 10;

% Plot position vs. time for each actuator (against the desired position)
for i = 1 : 6
    subplot(2, 3, i);
    plot(x, pos(:, i), 'r-');
    axis([0 DURATION (DESIRED_POS - dy) (DESIRED_POS + dy)]);
    title({['Actuator ' num2str(i)]; cell2mat(pid_str(i))});
    xlabel('Time (s)');
    ylabel('Position');
    line([0, nt], [DESIRED_POS, DESIRED_POS], ...
         'Color','black', 'LineStyle', '-');
    line([0, nt], [DESIRED_POS + POS_THRESHOLD(i), DESIRED_POS + POS_THRESHOLD(i)], ...
         'Color','black', 'LineStyle', '--');
    line([0, nt], [DESIRED_POS - POS_THRESHOLD(i), DESIRED_POS - POS_THRESHOLD(i)], ...
         'Color','black', 'LineStyle', '--');
end

%% Output results to a CSV file
csvwrite([datestr(now,'yyyy-mm-dd__HH-MM-SS') '.csv'], pos);

%% End session (clean up serial objects)
fclose(ser);
delete(ser);
clear ser;
delete(instrfindall);
