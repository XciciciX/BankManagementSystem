// Project identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98
#include "bank.h"

int main(int argc, char** argv){
    std::ios_base::sync_with_stdio(false);
    Bank bank;
    bank.get_options(argc, argv);
    bank.operations();
    bank.query();
    return 0;
}