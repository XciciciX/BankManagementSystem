// Project identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98 
#include "bank.h"
#include <getopt.h> 
#include <fstream>
#include <iostream>

void Bank::read_reg(){
    std::ifstream inputFile(reg_File);
    std::string v;
    std::string name;
    User ur;
    //timestamp
    while(getline(inputFile, v, '|')){
    
        ur.reg_time = convert2time(v);
        //name
        getline(inputFile, name, '|');
        //pin
        getline(inputFile, v, '|');
        ur.pin = std::stoi(v);
        //account balance
        getline(inputFile, v);
        ur.start_balc = std::stoi(v);
        //ur.is_login = false;
        registration[name] = ur;
    }

    // Check if the file is open
    //if (inputFile.is_open()) {
    inputFile.close();
}

void Bank::get_options(int argc, char** argv){
    int option_index = 0, option = 0;
    
    opterr = false;

    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                                { "file", required_argument, nullptr, 'f'},
                                { "verbose", no_argument, nullptr, 'v'},
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((option = getopt_long(argc, argv, "hf:v", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'h':
                exit(0);
                break;

            case 'f':
                reg_File = optarg;
                break;

            case 'v':
                modeV = true;
                break;

            default:
                std::cerr <<"Wrong input."; 
                exit(1);           
        }
    }
    read_reg();
}
