function [ ] = ProbCutStats( filename, depth )
%SDFSD Summary of this function goes here
%   Detailed explanation goes here

file = fopen(filename);
a = NaN(depth+1,depth+1);
b = NaN(depth+1,depth+1);
R_sq = NaN(depth+1,depth+1);
sigma = NaN(depth+1,depth+1);

% Loop over uninteresting stuff
line = fgetl(file);
while (length(line) < 12) || (~strcmp(line(1:12), '############'))
    line = fgetl(file);
end


for i = 0:depth
    for j = (i+1):depth
        line = fgetl(file);
        line = fgetl(file);
        disp(line);
        tmp = strsplit(line, ' ');
        SmallDepth = str2double(tmp(2));
        BigDepth = str2double(tmp(3));
        tmp = str2double(strsplit(fgetl(file), ' '));
        a(SmallDepth+1,BigDepth+1) = tmp(2);
        tmp = str2double(strsplit(fgetl(file), ' '));
        b(SmallDepth+1,BigDepth+1) = tmp(2);
        tmp = str2double(strsplit(fgetl(file), ' '));
        R_sq(SmallDepth+1,BigDepth+1) = tmp(2);
        tmp = str2double(strsplit(fgetl(file), ' '));
        sigma(SmallDepth+1,BigDepth+1) = tmp(2);
    end
end
plot(sigma)

line = fgetl(file);
line = fgetl(file);

for i = 0:depth
    for j = (i+1):depth
        line = fgetl(file);
        line = fgetl(file);
        disp(line);
        tmp = strsplit(line, ' ');
        SmallDepth = tmp(2);
        BigDepth = tmp(3);

        A = zeros(129,129);
        for l = 1:129
            tmp = str2double(strsplit(fgetl(file), '\t'));
            A(l,:) = tmp(1:129);
        end

        figure
        colormap('hot')
        imagesc([-64 64], [-64 64], A);
%         title(strjoin(['Depth ' SmallDepth ' vs ' BigDepth]))
        axis([-64 64 -64 64]);
%         xlabel(strjoin(['Depth ' SmallDepth]))
%         ylabel(strjoin(['Depth ' BigDepth]))
    end
end

fclose(file);

end

