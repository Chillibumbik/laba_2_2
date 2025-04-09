#include "errors.h"
#include <iostream>

const char* errors[] = {
    "Error: Index out of range\n",
    "Error: Immutable object\n",
    "Error: Empty array\n",
    "Error: Empty list\n",
    "Error: Invalid argument\n",
    "Error: Incompatible types\n",
    "Error: Empty value\n",
    "Error: Invalid size\n",
}

void checkErrors(int errorIndex){
    std::cout << errors[errorIndex]; 
}