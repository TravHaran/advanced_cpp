#include <iostream>
#include "url.h"

int main() {
    URL url("http", "example.com/index.html");
    url.print();
}