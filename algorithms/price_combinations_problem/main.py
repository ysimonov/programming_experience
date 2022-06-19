import time 

# Problem Statement
# Suppose you have 10 different products on the receipt
# Each product can have multiple prices, depending if it is discounted or not
# Find all possible combinations of prices between products that add up to total price

def get_price_combination(list_of_items):
    if not list_of_items:
        yield ()
    else:
        for item_price in list_of_items[0]:
            for product in get_price_combination(list_of_items[1:]):
                yield (item_price, ) + product

def price_combinations_algorithm():

    total_price = 100
    item_dictionary = {
        "Cheeseburger": [3, 7, 5, 8, 1],
        "Coca-Cola": [4, 4],
        "Mac'n Cheese": [2, 4, 5],
        "Hawaiian Pizza": [10, 9],
        "Salty Popcorn": [1, 20],
        "Bubble Tea": [3, 7, 6],
        "Chocolate Protein Balls": [4, 6, 13, 25],
        "Weet-Bix": [1, 5, 10],
        "Curry Chicken": [6, 8],
        "Tuna Sandwich": [22, 35, 20],
        "Banana Bread": [12, 2, 10]
    }

    traversal_path = [list(set(values)) for values in list(item_dictionary.values())]
    item_keys = list(item_dictionary.keys())

    t0 = time.perf_counter()
    for combination in get_price_combination(traversal_path):
        total_price_combination = sum(combination)
        if sum(combination) == total_price:
            print(list(zip(item_keys, combination)), "    ----->    ", total_price_combination)
    t1 = time.perf_counter()
    print("Execution time: ", t1-t0)


if __name__ == "__main__":
    price_combinations_algorithm()