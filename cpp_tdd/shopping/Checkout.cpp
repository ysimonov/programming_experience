#include "Checkout.h"

Checkout::Checkout() : total(0) {}

Checkout::~Checkout() {}

void Checkout::addItemPrice(std::string item, int price) {
    prices[item] = price;
}

void Checkout::addItem(std::string item) {
    std::map<std::string, int>::iterator priceIter = prices.find(item);
    if (priceIter == prices.end()) {
        throw std::invalid_argument("Invalid item. No price.");
    }
    items[item]++;
}

void Checkout::addDiscount(std::string item, int item_count,
                           int discount_price) {
    Discount discount;
    discount.item_count = item_count;
    discount.discount_price = discount_price;
    discounts[item] = discount;
}

int Checkout::calculateTotal() {
    total = 0;
    for (std::map<std::string, int>::iterator itemIter = items.begin();
         itemIter != items.end(); ++itemIter) {
        std::string item = itemIter->first;
        int item_count = itemIter->second;
        calculateItem(item, item_count);
    }
    return total;
}

void Checkout::calculateItem(std::string item, int item_count) {
    std::map<std::string, Discount>::iterator discountIter;
    discountIter = discounts.find(item);
    if (discountIter != discounts.end()) {
        Discount discount = discountIter->second;
        calculateDiscount(item, item_count, discount);
    } else {
        total += item_count * prices[item];
    }
}

void Checkout::calculateDiscount(std::string item, int item_count,
                                 Discount discount) {
    if (item_count >= discount.item_count) {
        int number_of_discounts = item_count / discount.item_count;
        total += number_of_discounts * discount.discount_price;
        int remaining_items = item_count % discount.item_count;
        total += remaining_items * prices[item];
    } else {
        total += item_count * prices[item];
    }
}