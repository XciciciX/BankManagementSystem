// Project identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98 
#include "bank.h"

void Bank::proceedFee(uint64_t timestamp){
    
    if(!transactions.empty()){
        Transaction t = transactions.top();
        while(!transactions.empty() && timestamp >= t.exec_date){
            uint32_t transFee =  t.amount / 100;
            if(transFee > 450){
                transFee = 450;
            }
            else{
                if(transFee < 10){
                    transFee = 10;
                }
            }
            if(t.exec_date > 50000000000 + registration[t.sender].reg_time){
                transFee = (transFee * 3) / 4;
            }
            uint32_t sender_fee = 0;
            uint32_t recip_fee = 0;
            if(t.type == 's'){
                if(transFee % 2 == 1){
                    sender_fee = transFee / 2 + 1;
                    recip_fee = transFee / 2;
                }
                else{
                    sender_fee = transFee / 2;
                    recip_fee = transFee / 2;
                }
            }
            else{
                sender_fee = transFee;
            }
            t.sender_Fee = sender_fee;
            t.recip_Fee = recip_fee;

            if(registration[t.recipient].start_balc >= recip_fee
            && registration[t.sender].start_balc >= sender_fee+t.amount){
                registration[t.sender].start_balc -= (sender_fee+t.amount);
                registration[t.recipient].start_balc -= recip_fee;
                registration[t.recipient].start_balc += t.amount;
                exec_trans.push_back(t);
                if(modeV){
                    std::cout << "Transaction executed at " << t.exec_date << ": $" << t.amount << " from " << t.sender << " to " << t.recipient <<  ".\n";
                }
            }
            else{
                if(modeV){
                    std::cout << "Insufficient funds to process transaction " << t.ID << ".\n";
                }
                
            }
            transactions.pop();
            if(!transactions.empty()){
                t = transactions.top();
            }            
        }
    }
}

void Bank::operations(){
    std::string first;
    unsigned int trans_ID = 0;
    uint64_t prev_place_time = 0;
    while(std::cin >> first){
        if (std::cin.fail()) {
            std::cerr << "Error: Reading from cin has failed" << std::endl;
            exit(1);
        }
        if (first[0] == '#'){
            std::getline(std::cin, first);
        }
        else{
            if(first == "login"){
                std::string name;
                unsigned int pin;
                std::string ip;
                std::cin >> name >> pin >> ip;
                auto it = registration.find(name);
                if(it != registration.end()){
                    if(it->second.pin == pin){
                        IP_list[name].insert(ip);
                        if(modeV){
                            std::cout << "User " << name << " logged in.\n"; 
                        }
                    }
                    else{
                        if(modeV){
                            std::cout << "Failed to log in " << name << ".\n"; 
                        }  
                    }
                }
                else{
                    if(modeV){
                        std::cout << "Failed to log in " << name << ".\n"; 
                    }
                }
            }
            else if(first == "out"){
                std::string name;
                std::string ip;
                std::cin >> name >> ip;
                bool flag = true;
                if( registration.count(name) == 1){
                    //find the name
                    if(IP_list.count(name) == 1){
                        if(IP_list[name].count(ip) == 1){
                            IP_list[name].erase(ip);
                            if(IP_list[name].empty()){
                                IP_list.erase(name);
                            }
                            if(modeV){
                                std::cout << "User " << name << " logged out.\n"; 
                            }
                        }
                        else{
                            flag = false;
                        }
                    }
                    else{
                        flag = false;
                    }
                }
                else{
                    flag = false;
                }
                if(!flag && modeV){
                    std::cout << "Failed to log out " << name << ".\n"; 
                }
            }
            else if(first == "place"){
                //place <TIMESTAMP> <IP> <SENDER> <RECIPIENT> <AMOUNT> <EXEC_DATE> <o/s>
                std::string t;
                std::string ip;
                std::string sender;
                std::string recipient;
                std::string a;
                std::string e;
                char fee_method; 
                std::cin >> t >> ip >> sender >> recipient >> a >> e >> fee_method;
                uint64_t timestamp = convert2time(t);
                uint32_t amount = std::stoi(a);
                uint64_t exec_date = convert2time(e);
                //ERROR
                if(timestamp < prev_place_time){
                    std::cerr << "Invalid decreasing timestamp in 'place' command.";
                    exit(1);
                }
                //ERROR
                if(timestamp > exec_date){
                    std::cerr<<"You cannot have an execution date before the current timestamp.";
                    exit(1);
                }
                
                prev_place_time = timestamp;
                if(exec_date > 3000000 + timestamp){ // yy:mm:dd:hh:mm:ss
                    if(modeV){
                        std::cout << "Select a time less than three days in the future.\n";
                    }
                    continue;
                }
                auto it_s = registration.find(sender);
                auto it_r = registration.find(recipient);
                if(it_s == registration.end()){
                    if(modeV){
                        std::cout << "Sender " << sender << " does not exist.\n";
                    }
                    continue;
                }
                if(it_r == registration.end()){
                    if(modeV){
                        std::cout << "Recipient " << recipient  << " does not exist.\n";
                    }
                    continue;
                }
                if(exec_date <= it_s->second.reg_time || exec_date <= it_r->second.reg_time){
                    if(modeV){
                        std::cout << "At the time of execution, sender and/or recipient have not registered.\n";
                    }
                    continue;
                }
                if(IP_list.count(sender) != 1){
                    if(modeV){
                        std::cout << "Sender " << sender << " is not logged in.\n";
                    }
                    continue;
                } 
                if(IP_list[sender].count(ip) == 1){
                    proceedFee(timestamp);
                    Transaction t = {timestamp, exec_date , trans_ID, amount, fee_method, sender, recipient, 0, 0};
                    transactions.push(t);
                    if(modeV){
                        std::cout << "Transaction placed at " << timestamp << ": $" << amount << " from " << sender << " to " << recipient << " at " << exec_date << ".\n";
                    }
                    trans_ID++;
                }
                else{
                    if(modeV){
                        std::cout << "Fraudulent transaction detected, aborting request.\n";
                    }
                }
            }
            else if(first == "$$$"){
                break;
            }
        }
    }
    proceedFee(1000000000000);
}
