# distutils: language = c++
# distutils: sources = priceCombinationFinder.cpp
 
from libcpp.vector cimport vector

cdef extern from "priceCombinationFinder.hpp":
    cdef vector[vector[int]] priceCombinationsGenerator(vector[vector[int]] item_prices, int total_price)

cpdef get_price_combinations(list item_price_list, int total_price):
    cdef:
        vector[vector[int]] item_price_vector
        vector[int] item_prices
        vector[vector[int]] price_combinations

    for item_price_combination in item_price_list:
        item_prices.clear()
        if not isinstance(item_price_combination, (list, tuple)):
            raise ValueError("item_price_list must have type List[List[int]]")
        for item_price in item_price_combination:
            if isinstance(item_price, int):
                item_prices.push_back(item_price)
            else:
                raise ValueError("item_price_list must have type List[List[int]]")
        item_price_vector.push_back(item_prices)

    price_combinations = priceCombinationsGenerator(item_price_vector, total_price)
    return price_combinations



    