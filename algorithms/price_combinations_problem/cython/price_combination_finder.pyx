import cython

@cython.wraparound(False)
@cython.boundscheck(False)
@cython.cdivision(True)
def get_price_combination(list list_of_items):
    cdef:
        Py_ssize_t item_price
        tuple product
    if not list_of_items:
        yield ()
    else:
        for item_price in list_of_items[0]:
            for product in get_price_combination(list_of_items[1:]):
                yield (item_price, ) + product

@cython.wraparound(False)
@cython.boundscheck(False) 
@cython.cdivision(True)
cpdef generate_price_combinations(list price_list, int total):
    cdef:
        Py_ssize_t price
        Py_ssize_t total_check
        tuple price_combination
        list valid_combinations = []
    for price_combination in get_price_combination(price_list):
        total_check = 0
        for price in price_combination:
            total_check += price
        if total_check == total:
            valid_combinations.append(price_combination)
    return valid_combinations