%{
    Measure the positional difference results for the current PID
    configuration. Provides a plot over time of the desired and current
    position values. Requires a serial connection to the Arduino to run.
%}

%% Position configuration
DESIRED_POS = [                                                            % actuator positions in [0, 1024]
    500
    500
    500
    500
    500
    500
].';
POS_THRESHOLD = 2;

%% Feedback configuration (NOTE: manually inputted from Arduino code)
P = 20;
I = 0.01;
D = 0.01;
pid_str = ['P=' num2str(P) '    I=' num2str(I) '    D=' num2str(D)];

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
fprintf(ser, strjoin([string(DESIRED_POS), '\n']));

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
    axis([0 DURATION (DESIRED_POS(:, i) - dy) (DESIRED_POS(:, i) + dy)]);
    title(['Actuator ' num2str(i)]);
    xlabel('Time (s)');
    ylabel('Position');
    line([0, nt], [DESIRED_POS(:, i), DESIRED_POS(:, i)], ...
         'Color','black', 'LineStyle', '--');
    line([0, nt], [DESIRED_POS(:, i) + POS_THRESHOLD, DESIRED_POS(:, i) + POS_THRESHOLD], ...
         'Color','black', 'LineStyle', '-');
    line([0, nt], [DESIRED_POS(:, i) - POS_THRESHOLD, DESIRED_POS(:, i) - POS_THRESHOLD], ...
         'Color','black', 'LineStyle', '-');
end

% Set the title for all plots (with PID values)
suptitle(pid_str);

%% Output results to a CSV file
csvwrite([pid_str '_' num2str(DURATION) '.csv'], pos);

%% End session (clean up serial objects)
fprintf(ser, strjoin([string([0 0 0 0 0 0]), '\n']));
fclose(ser);
delete(ser);
clear ser;
delete(instrfindall);
