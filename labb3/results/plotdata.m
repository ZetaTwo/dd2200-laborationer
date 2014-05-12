%% Import data

my1 = [];
my2 = [];
stock = [];

for i = 0:5
    raw = importfile(strcat('my1_', int2str(i), '.txt'));
    my1(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('my2_', int2str(i), '.txt'));
    my2(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

for i = 0:5
    raw = importfile(strcat('stock_', int2str(i), '.txt'));
    stock(:,:,i+1) = [ mean(raw); min(raw); max(raw) ];
end

x=[1 10 100 1000 10000 100000 1000000];

%% Plot data 1

figure;
hold on;
errorbar(log(x), log(my1(1, :, 1)), log(my1(1, :, 1))-log(my1(2, :, 1)), log(my1(3, :, 1))-log(my1(1, :, 1)), 'b');
errorbar(log(x), log(my1(1, :, 2)), log(my1(1, :, 2))-log(my1(2, :, 2)), log(my1(3, :, 2))-log(my1(1, :, 2)), 'r');
errorbar(log(x), log(my1(1, :, 3)), log(my1(1, :, 3))-log(my1(2, :, 3)), log(my1(3, :, 3))-log(my1(1, :, 3)), 'g');
errorbar(log(x), log(my1(1, :, 4)), log(my1(1, :, 4))-log(my1(2, :, 4)), log(my1(3, :, 4))-log(my1(1, :, 4)), 'y');
errorbar(log(x), log(my1(1, :, 5)), log(my1(1, :, 5))-log(my1(2, :, 5)), log(my1(3, :, 5))-log(my1(1, :, 5)), 'k');
errorbar(log(x), log(my1(1, :, 6)), log(my1(1, :, 6))-log(my1(2, :, 6)), log(my1(3, :, 6))-log(my1(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('My malloc with first fit');

%% Plot data 2

figure;
hold on;
errorbar(log(x), log(my2(1, :, 1)), log(my2(1, :, 1))-log(my2(2, :, 1)), log(my2(3, :, 1))-log(my2(1, :, 1)), 'b');
errorbar(log(x), log(my2(1, :, 2)), log(my2(1, :, 2))-log(my2(2, :, 2)), log(my2(3, :, 2))-log(my2(1, :, 2)), 'r');
errorbar(log(x), log(my2(1, :, 3)), log(my2(1, :, 3))-log(my2(2, :, 3)), log(my2(3, :, 3))-log(my2(1, :, 3)), 'g');
errorbar(log(x), log(my2(1, :, 4)), log(my2(1, :, 4))-log(my2(2, :, 4)), log(my2(3, :, 4))-log(my2(1, :, 4)), 'y');
errorbar(log(x), log(my2(1, :, 5)), log(my2(1, :, 5))-log(my2(2, :, 5)), log(my2(3, :, 5))-log(my2(1, :, 5)), 'k');
errorbar(log(x), log(my2(1, :, 6)), log(my2(1, :, 6))-log(my2(2, :, 6)), log(my2(3, :, 6))-log(my2(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('My malloc with best fit');

%% Plot data 3

figure;
hold on;
errorbar(log(x), log(stock(1, :, 1)), log(stock(1, :, 1))-log(stock(2, :, 1)), log(stock(3, :, 1))-log(stock(1, :, 1)), 'b');
errorbar(log(x), log(stock(1, :, 2)), log(stock(1, :, 2))-log(stock(2, :, 2)), log(stock(3, :, 2))-log(stock(1, :, 2)), 'r');
errorbar(log(x), log(stock(1, :, 3)), log(stock(1, :, 3))-log(stock(2, :, 3)), log(stock(3, :, 3))-log(stock(1, :, 3)), 'g');
errorbar(log(x), log(stock(1, :, 4)), log(stock(1, :, 4))-log(stock(2, :, 4)), log(stock(3, :, 4))-log(stock(1, :, 4)), 'y');
errorbar(log(x), log(stock(1, :, 5)), log(stock(1, :, 5))-log(stock(2, :, 5)), log(stock(3, :, 5))-log(stock(1, :, 5)), 'k');
errorbar(log(x), log(stock(1, :, 6)), log(stock(1, :, 6))-log(stock(2, :, 6)), log(stock(3, :, 6))-log(stock(1, :, 6)), 'm');
legend('0% realloc', '10% realloc', '20% realloc', '30% realloc', '40% realloc', '50% realloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('Reference malloc');

%% Plot data 4

figure;
hold on;
errorbar(log(x), log(my1(1, :, 6)), log(my1(1, :, 6))-log(my1(2, :, 6)), log(my1(3, :, 6))-log(my1(1, :, 6)), 'b');
errorbar(log(x), log(my2(1, :, 6)), log(my2(1, :, 6))-log(my2(2, :, 6)), log(my2(3, :, 6))-log(my2(1, :, 6)), 'r');
errorbar(log(x), log(stock(1, :, 6)), log(stock(1, :, 6))-log(stock(2, :, 6)), log(stock(3, :, 6))-log(stock(1, :, 6)), 'g');
legend('My malloc, first fit', 'My malloc, best fit', 'Reference malloc');
xlabel('Iterations (log(1))');
ylabel('Time (log(ms))');
title('Comparison at 50% realloc');

