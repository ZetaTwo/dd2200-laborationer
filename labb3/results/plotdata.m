%% Import data

my1_t = [];
my2_t = [];
stock_t = [];

my1_mem = [];
my2_mem = [];
stock_mem = [];

for i = 0:5
    raw = importfile(strcat('my1_t_', int2str(i), '.txt'));
    my1_t(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('my2_t_', int2str(i), '.txt'));
    my2_t(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('stock_t_', int2str(i), '.txt'));
    stock_t(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('my1_mem_', int2str(i), '.txt'));
    my1_mem(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('my2_mem_', int2str(i), '.txt'));
    my2_mem(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('stock_mem_', int2str(i), '.txt'));
    stock_mem(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

x=[1 10 100 1000 10000 100000 1000000];

%% Plot data 1 (My 1 T)

figure;
hold on;
errorbar(log(x), log(my1_t(1, :, 1)), log(my1_t(1, :, 1))-log(my1_t(2, :, 1)), log(my1_t(3, :, 1))-log(my1_t(1, :, 1)), 'b');
errorbar(log(x), log(my1_t(1, :, 2)), log(my1_t(1, :, 2))-log(my1_t(2, :, 2)), log(my1_t(3, :, 2))-log(my1_t(1, :, 2)), 'r');
errorbar(log(x), log(my1_t(1, :, 3)), log(my1_t(1, :, 3))-log(my1_t(2, :, 3)), log(my1_t(3, :, 3))-log(my1_t(1, :, 3)), 'g');
errorbar(log(x), log(my1_t(1, :, 4)), log(my1_t(1, :, 4))-log(my1_t(2, :, 4)), log(my1_t(3, :, 4))-log(my1_t(1, :, 4)), 'y');
errorbar(log(x), log(my1_t(1, :, 5)), log(my1_t(1, :, 5))-log(my1_t(2, :, 5)), log(my1_t(3, :, 5))-log(my1_t(1, :, 5)), 'k');
errorbar(log(x), log(my1_t(1, :, 6)), log(my1_t(1, :, 6))-log(my1_t(2, :, 6)), log(my1_t(3, :, 6))-log(my1_t(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('My malloc with first fit');

%% Plot data 2 (My 2 T)

figure;
hold on;
errorbar(log(x), log(my2_t(1, :, 1)), log(my2_t(1, :, 1))-log(my2_t(2, :, 1)), log(my2_t(3, :, 1))-log(my2_t(1, :, 1)), 'b');
errorbar(log(x), log(my2_t(1, :, 2)), log(my2_t(1, :, 2))-log(my2_t(2, :, 2)), log(my2_t(3, :, 2))-log(my2_t(1, :, 2)), 'r');
errorbar(log(x), log(my2_t(1, :, 3)), log(my2_t(1, :, 3))-log(my2_t(2, :, 3)), log(my2_t(3, :, 3))-log(my2_t(1, :, 3)), 'g');
errorbar(log(x), log(my2_t(1, :, 4)), log(my2_t(1, :, 4))-log(my2_t(2, :, 4)), log(my2_t(3, :, 4))-log(my2_t(1, :, 4)), 'y');
errorbar(log(x), log(my2_t(1, :, 5)), log(my2_t(1, :, 5))-log(my2_t(2, :, 5)), log(my2_t(3, :, 5))-log(my2_t(1, :, 5)), 'k');
errorbar(log(x), log(my2_t(1, :, 6)), log(my2_t(1, :, 6))-log(my2_t(2, :, 6)), log(my2_t(3, :, 6))-log(my2_t(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('My malloc with best fit');

%% Plot data 3 (Ref T)

figure;
hold on;
errorbar(log(x), log(stock_t(1, :, 1)), log(stock_t(1, :, 1))-log(stock_t(2, :, 1)), log(stock_t(3, :, 1))-log(stock_t(1, :, 1)), 'b');
errorbar(log(x), log(stock_t(1, :, 2)), log(stock_t(1, :, 2))-log(stock_t(2, :, 2)), log(stock_t(3, :, 2))-log(stock_t(1, :, 2)), 'r');
errorbar(log(x), log(stock_t(1, :, 3)), log(stock_t(1, :, 3))-log(stock_t(2, :, 3)), log(stock_t(3, :, 3))-log(stock_t(1, :, 3)), 'g');
errorbar(log(x), log(stock_t(1, :, 4)), log(stock_t(1, :, 4))-log(stock_t(2, :, 4)), log(stock_t(3, :, 4))-log(stock_t(1, :, 4)), 'y');
errorbar(log(x), log(stock_t(1, :, 5)), log(stock_t(1, :, 5))-log(stock_t(2, :, 5)), log(stock_t(3, :, 5))-log(stock_t(1, :, 5)), 'k');
errorbar(log(x), log(stock_t(1, :, 6)), log(stock_t(1, :, 6))-log(stock_t(2, :, 6)), log(stock_t(3, :, 6))-log(stock_t(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('Reference malloc');

%% Plot data 4 (Comp T)

figure;
hold on;
errorbar(log(x), log(my1_t(1, :, 6)), log(my1_t(1, :, 6))-log(my1_t(2, :, 6)), log(my1_t(3, :, 6))-log(my1_t(1, :, 6)), 'b');
errorbar(log(x), log(my2_t(1, :, 6)), log(my2_t(1, :, 6))-log(my2_t(2, :, 6)), log(my2_t(3, :, 6))-log(my2_t(1, :, 6)), 'r');
errorbar(log(x), log(stock_t(1, :, 6)), log(stock_t(1, :, 6))-log(stock_t(2, :, 6)), log(stock_t(3, :, 6))-log(stock_t(1, :, 6)), 'g');
legend('My malloc, first fit', 'My malloc, best fit', 'Reference malloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('Comparison at 50% realloc');

%% Plot data 5 (My 1 Mem)

figure;
hold on;
errorbar(log(x), log(my1_mem(1, :, 1)), log(my1_mem(1, :, 1))-log(my1_mem(2, :, 1)), log(my1_mem(3, :, 1))-log(my1_mem(1, :, 1)), 'b');
errorbar(log(x), log(my1_mem(1, :, 2)), log(my1_mem(1, :, 2))-log(my1_mem(2, :, 2)), log(my1_mem(3, :, 2))-log(my1_mem(1, :, 2)), 'r');
errorbar(log(x), log(my1_mem(1, :, 3)), log(my1_mem(1, :, 3))-log(my1_mem(2, :, 3)), log(my1_mem(3, :, 3))-log(my1_mem(1, :, 3)), 'g');
errorbar(log(x), log(my1_mem(1, :, 4)), log(my1_mem(1, :, 4))-log(my1_mem(2, :, 4)), log(my1_mem(3, :, 4))-log(my1_mem(1, :, 4)), 'y');
errorbar(log(x), log(my1_mem(1, :, 5)), log(my1_mem(1, :, 5))-log(my1_mem(2, :, 5)), log(my1_mem(3, :, 5))-log(my1_mem(1, :, 5)), 'k');
errorbar(log(x), log(my1_mem(1, :, 6)), log(my1_mem(1, :, 6))-log(my1_mem(2, :, 6)), log(my1_mem(3, :, 6))-log(my1_mem(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('My malloc with first fit');

%% Plot data 6 (My 2 Mem)

figure;
hold on;
errorbar(log(x), log(my2_mem(1, :, 1)), log(my2_mem(1, :, 1))-log(my2_mem(2, :, 1)), log(my2_mem(3, :, 1))-log(my2_mem(1, :, 1)), 'b');
errorbar(log(x), log(my2_mem(1, :, 2)), log(my2_mem(1, :, 2))-log(my2_mem(2, :, 2)), log(my2_mem(3, :, 2))-log(my2_mem(1, :, 2)), 'r');
errorbar(log(x), log(my2_mem(1, :, 3)), log(my2_mem(1, :, 3))-log(my2_mem(2, :, 3)), log(my2_mem(3, :, 3))-log(my2_mem(1, :, 3)), 'g');
errorbar(log(x), log(my2_mem(1, :, 4)), log(my2_mem(1, :, 4))-log(my2_mem(2, :, 4)), log(my2_mem(3, :, 4))-log(my2_mem(1, :, 4)), 'y');
errorbar(log(x), log(my2_mem(1, :, 5)), log(my2_mem(1, :, 5))-log(my2_mem(2, :, 5)), log(my2_mem(3, :, 5))-log(my2_mem(1, :, 5)), 'k');
errorbar(log(x), log(my2_mem(1, :, 6)), log(my2_mem(1, :, 6))-log(my2_mem(2, :, 6)), log(my2_mem(3, :, 6))-log(my2_mem(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('My malloc with best fit');

%% Plot data 7 (Ref Mem)

figure;
hold on;
errorbar(log(x), log(stock_mem(1, :, 1)), log(stock_mem(1, :, 1))-log(stock_mem(2, :, 1)), log(stock_mem(3, :, 1))-log(stock_mem(1, :, 1)), 'b');
errorbar(log(x), log(stock_mem(1, :, 2)), log(stock_mem(1, :, 2))-log(stock_mem(2, :, 2)), log(stock_mem(3, :, 2))-log(stock_mem(1, :, 2)), 'r');
errorbar(log(x), log(stock_mem(1, :, 3)), log(stock_mem(1, :, 3))-log(stock_mem(2, :, 3)), log(stock_mem(3, :, 3))-log(stock_mem(1, :, 3)), 'g');
errorbar(log(x), log(stock_mem(1, :, 4)), log(stock_mem(1, :, 4))-log(stock_mem(2, :, 4)), log(stock_mem(3, :, 4))-log(stock_mem(1, :, 4)), 'y');
errorbar(log(x), log(stock_mem(1, :, 5)), log(stock_mem(1, :, 5))-log(stock_mem(2, :, 5)), log(stock_mem(3, :, 5))-log(stock_mem(1, :, 5)), 'k');
errorbar(log(x), log(stock_mem(1, :, 6)), log(stock_mem(1, :, 6))-log(stock_mem(2, :, 6)), log(stock_mem(3, :, 6))-log(stock_mem(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('Reference malloc');

%% Plot data (Comp Mem)

figure;
hold on;
errorbar(log(x), log(my1_mem(1, :, 6)), log(my1_mem(1, :, 6))-log(my1_mem(2, :, 6)), log(my1_mem(3, :, 6))-log(my1_mem(1, :, 6)), 'b');
errorbar(log(x), log(my2_mem(1, :, 6)), log(my2_mem(1, :, 6))-log(my2_mem(2, :, 6)), log(my2_mem(3, :, 6))-log(my2_mem(1, :, 6)), 'r');
errorbar(log(x), log(stock_mem(1, :, 6)), log(stock_mem(1, :, 6))-log(stock_mem(2, :, 6)), log(stock_mem(3, :, 6))-log(stock_mem(1, :, 6)), 'g');
legend('My malloc, first fit', 'My malloc, best fit', 'Reference malloc');
xlabel('Iterations (log(1))');
ylabel('Memory (log(byte))');
title('Comparison at 50% realloc');

