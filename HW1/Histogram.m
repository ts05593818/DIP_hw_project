clc;
clear;
[~, ~, raw] = xlsread('D:\Dropbox\DIP_Program\HW1_2018\Histogram','工作表1','F1:F256');
raw(cellfun(@(x) ~isempty(x) && isnumeric(x) && isnan(x),raw)) = {''};
R = cellfun(@(x) ~isnumeric(x) && ~islogical(x),raw); % Find non-numeric cells
raw(R) = {NaN}; % Replace non-numeric cells
Untitled1 = reshape([raw{:}],size(raw));
clearvars raw R;
GRAY_Oringinal=Untitled1;
set(gca,'XLim',[0 255]);
bar(GRAY_Oringinal);
title('Fig 1');
xlabel('Gray Level');
ylabel('Number of Pixels');

figure(2);
clc;
clear;
[~, ~, raw] = xlsread('D:\Dropbox\DIP_Program\HW1_2018\Histogram','工作表1','G1:G256');
raw(cellfun(@(x) ~isempty(x) && isnumeric(x) && isnan(x),raw)) = {''};
R = cellfun(@(x) ~isnumeric(x) && ~islogical(x),raw); % Find non-numeric cells
raw(R) = {NaN}; % Replace non-numeric cells
Untitled1 = reshape([raw{:}],size(raw));
clearvars raw R;
GRAY_r05=Untitled1;
set(gca,'XLim',[0 255]);
bar(GRAY_r05);
title('Fig 2-2');
xlabel('Gray Level');
ylabel('Number of Pixels');

figure(3);
clc;
clear;
[~, ~, raw] = xlsread('D:\Dropbox\DIP_Program\HW1_2018\Histogram','工作表1','H1:H256');
raw(cellfun(@(x) ~isempty(x) && isnumeric(x) && isnan(x),raw)) = {''};
R = cellfun(@(x) ~isnumeric(x) && ~islogical(x),raw); % Find non-numeric cells
raw(R) = {NaN}; % Replace non-numeric cells
Untitled1 = reshape([raw{:}],size(raw));
clearvars raw R;
GRAY_r05=Untitled1;
set(gca,'XLim',[0 255]);
bar(GRAY_r05);
title('Fig 2-4');
xlabel('Gray Level');
ylabel('Number of Pixels');

figure(4);
clc;
clear;
[~, ~, raw] = xlsread('D:\Dropbox\DIP_Program\HW1_2018\Histogram','工作表1','I1:I256');
raw(cellfun(@(x) ~isempty(x) && isnumeric(x) && isnan(x),raw)) = {''};
R = cellfun(@(x) ~isnumeric(x) && ~islogical(x),raw); % Find non-numeric cells
raw(R) = {NaN}; % Replace non-numeric cells
Untitled1 = reshape([raw{:}],size(raw));
clearvars raw R;
GRAY_r05=Untitled1;
set(gca,'XLim',[0 255]);
bar(GRAY_r05);
title('Fig 3-2');
xlabel('Gray Level');
ylabel('Number of Pixels');

figure(5);
clc;
clear;
[~, ~, raw] = xlsread('D:\Dropbox\DIP_Program\HW1_2018\Histogram','工作表1','J1:J256');
raw(cellfun(@(x) ~isempty(x) && isnumeric(x) && isnan(x),raw)) = {''};
R = cellfun(@(x) ~isnumeric(x) && ~islogical(x),raw); % Find non-numeric cells
raw(R) = {NaN}; % Replace non-numeric cells
Untitled1 = reshape([raw{:}],size(raw));
clearvars raw R;
GRAY_r05=Untitled1;
set(gca,'XLim',[0 255]);
bar(GRAY_r05);
title('Fig 3-4');
xlabel('Gray Level');
ylabel('Number of Pixels');

