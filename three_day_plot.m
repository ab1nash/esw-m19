% Read count data from a ThingSpeak channel for three seperate days 
% and visualize the data in a single plot using the PLOT function. 

% Channel ID
readChannelID = 921115; 
% count Field ID
myFieldID = 1;
% One day date range
oneDay = [datetime('yesterday') datetime('today')];

% Channel Read API Key
% If your channel is private, then enter the read API key between the '' below: 
readAPIKey = ''; 

countDay1 = thingSpeakRead(readChannelID,'Fields',myFieldID, ...
                                 'dateRange', oneDay-days(1), 'ReadKey',readAPIKey); 
countDay2 = thingSpeakRead(readChannelID,'Fields',myFieldID, ...
                                 'dateRange',oneDay-days(2),'ReadKey',readAPIKey); 
countDay3 = thingSpeakRead(readChannelID,'Fields',myFieldID, ...
                                'dateRange', oneDay-days(3),'ReadKey',readAPIKey); 

myTimes1 = minutes(1:length(countDay1));
myTimes2 = minutes(1:length(countDay2));
myTimes3 = minutes(1:length(countDay3));

plot(myTimes1,countDay1, myTimes2,countDay2, myTimes3, countDay3);
legend({'Day1','Day2','Day3'});
xlabel('Minutes');
ylabel('Count');
title('3-Day Count Comparison');
