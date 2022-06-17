import itertools

# Problem Statement
# Suppose you have 10 different products on the receipt
# Each product can have multiple prices, depending if it is discounted or not
# Find all possible combinations of prices between products that add up to total price

def get_valid_combination(max_perm, path_lengths):
    for permutation in itertools.product(range(max_perm), repeat=len(path_lengths)):
        valid_combination = True
        for (path_length, list_idx) in zip(path_lengths, permutation):
            # check if path is valid
            if list_idx > path_length:
                valid_combination = False
                break
        if valid_combination:
            yield permutation

def recursive_path_generator(traversal_path, total_price, max_depth, path_lengths):
    for permutation in get_valid_combination(max_perm=max_depth, path_lengths=path_lengths):
        traversal_costs = [traversal_path[j][i] for j, i in enumerate(permutation)]
        traversal_sum = sum(traversal_costs)
        if traversal_sum == total_price:
            yield traversal_costs

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

    traversal_path = list(item_dictionary.values())
    max_depth = 0
    path_lengths = []
    for element_list in traversal_path:
        len_element_list = len(element_list)
        path_lengths.append(len_element_list-1)
        if len_element_list > max_depth:
            max_depth = len_element_list

    item_keys = item_dictionary.keys()
    for valid_price_combination in recursive_path_generator(traversal_path, total_price, max_depth, path_lengths):
        print(list(zip(item_keys, valid_price_combination)), "    ----->    ", sum(valid_price_combination))


if __name__ == "__main__":
    price_combinations_algorithm()