from price_combination_finder import generate_price_combinations
import time

price_list = [
    [3, 7, 5, 8, 1],
    [4],
    [2, 4, 5],
    [10, 9],
    [1, 20],
    [3, 7, 6],
    [4, 6, 13, 25],
    [1, 5, 10],
    [6, 8],
    [22, 35, 20],
    [12, 2, 10]
]

total = 100

t0 = time.perf_counter()
price_combinations = generate_price_combinations(price_list, total)
t1 = time.perf_counter()

print("Number of combinations: ", len(price_combinations))
print("Elapsed Time: ", t1-t0)