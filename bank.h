// Project identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98 
#ifndef BANK
#define BANK

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
struct User{
    uint64_t reg_time;
    unsigned int pin;
    uint32_t start_balc;
};

struct Transaction{
    uint64_t timestamp;
    uint64_t exec_date;
    unsigned int ID;
    uint32_t amount;
    char type;
    std::string sender;
    std::string recipient;
    uint32_t sender_Fee;
    uint32_t recip_Fee;
};

class TransFirst {
    public:
        bool operator()(const Transaction& one, const Transaction& two) {
            if(one.exec_date == two.exec_date){
                return one.ID > two.ID;
            }
            return one.exec_date > two.exec_date;
        }

};


class Bank{
    public:
        Bank():modeV(false){
            exec_trans.reserve(30);
        }
        void read_reg();
        void get_options(int argc, char** argv);
        void operations();
        void query();

    private:
        bool modeV;
        std::string reg_File;
        std::unordered_map<std::string, User> registration;
        std::unordered_map<std::string, std::unordered_set<std::string>> IP_list;
        std::priority_queue<Transaction, std::vector<Transaction>, TransFirst> transactions;
        std::vector<Transaction> exec_trans;
        uint64_t convert2time(std::string v){
            return std::stoull(v.substr(0,2))*10000000000 
                    + std::stoull(v.substr(3,2))*100000000
                    + std::stoull(v.substr(6,2))*1000000 
                    + std::stoull(v.substr(9,2))*10000
                    + std::stoull(v.substr(12,2))*100
                    + std::stoull(v.substr(15,2))*1;  
        }
        //std::string convert2interval(uint64_t t);
        void proceedFee(uint64_t timestamp);
};  

#endif