#ifndef CHECKOUT_H_
#define CHECKOUT_H_

#include <map>
#include <stdexcept>
#include <string>

class Checkout {
   public:
    Checkout();
    virtual ~Checkout();

    void addItemPrice(std::string item, int price);
    void addItem(std::string item);
    void addDiscount(std::string item, int item_count, int discount_price);

    int calculateTotal();

   protected:
    struct Discount {
        int item_count;
        int discount_price;
    };
    std::map<std::string, int> prices;
    std::map<std::string, Discount> discounts;
    std::map<std::string, int> items;
    int total;

    void calculateItem(std::string item, int item_count);
    void calculateDiscount(std::string item, int item_count, Discount discount);
};

#endif /* CHECKOUT_H */