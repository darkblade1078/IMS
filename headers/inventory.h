#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

class Inventory {
    private:
        int id;
        std::string itemName;
        int quantity;

    public:
        Inventory(int id, std::string itemName, int quantity);
        int getId();
        std::string getItemName();
        int getQuantity();
};

#endif