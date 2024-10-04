// Project identifier: 292F24D17A4455C1B5133EDD8C7CEAA0C9570A98 
#include "bank.h"

inline std::string convert2interval(uint64_t t){
    // yy:mm:dd:hh:mm:ss
    std::string out = "\0";
    if(t/10000000000 != 0){
        if(t/10000000000 == 1){
            out = std::to_string(t/10000000000) + " year ";
        }
        else{
            out = std::to_string(t/10000000000) + " years ";
        }
        t %= 10000000000;
    }
    if(t/100000000 != 0){
        if(t/100000000 == 1){
            out = out + std::to_string(t/100000000) + " month ";
        }
        else{
            out = out + std::to_string(t/100000000) + " months ";
        }
        t %= 100000000;
    }
    if(t/1000000 != 0){
        if(t/1000000 == 1){
            out = out + std::to_string(t/1000000) + " day ";
        }
        else{
            out = out + std::to_string(t/1000000) + " days ";
        }
        t %= 1000000;
    }
    if(t/10000 != 0){
        if(t/10000 == 1){
            out = out + std::to_string(t/10000) + " hour ";
        }
        else{
            out = out + std::to_string(t/10000) + " hours ";
        }
        t %= 10000;
    }
    if(t/100 != 0){
        if(t/100 == 1){
            out = out + std::to_string(t/100) + " minute ";
        }
        else{
            out = out + std::to_string(t/100) + " minutes ";
        }
        t %= 100;
    }
    if(t != 0){
        if(t == 1){
            out = out + std::to_string(t) + " second ";
        }
        else{
            out = out + std::to_string(t) + " seconds ";
        }
    }
    if(!out.empty()){
        out.pop_back();
    }
    return out;
}

void Bank::query(){
    std::string first;
    while(std::cin >> first){
        if (std::cin.fail()) {
            std::cerr << "Error: Reading from cin has failed" << std::endl;
            exit(1);
        } 
        if(first == "l"){
            std::string t1;
            std::string t2;
            std::cin >> t1 >> t2;
            uint64_t starttime = convert2time(t1);
            uint64_t endtime = convert2time(t2);
            uint32_t cnt = 0;
            for(auto &value : exec_trans){
                if(value.exec_date >= starttime && value.exec_date < endtime){
                    cnt ++;
                    std::cout << value.ID << ": " << value.sender << " sent " << value.amount << ((value.amount == 1) ? (" dollar") : (" dollars")) << " to " << value.recipient << " at " << value.exec_date << ".\n";
                }
            }
            std::cout << "There" << ((cnt == 1) ? (" was ") : (" were ")) << cnt << ((cnt == 1) ? (" transaction") : (" transactions")) << " that" << ((cnt == 1) ? (" was ") : (" were ")) << "placed between time " << starttime << " to " << endtime << ".\n";
        }
        else if(first == "r"){
            std::string t1;
            std::string t2;
            std::cin >> t1 >> t2;
            uint64_t starttime = convert2time(t1);
            uint64_t endtime = convert2time(t2);
            uint64_t cnt = 0;
            for(auto &value : exec_trans){
                if(value.exec_date >= starttime && value.exec_date < endtime){
                    cnt += (value.recip_Fee+value.sender_Fee);
                }
            }
            std::cout << "281Bank has collected " << cnt << ((cnt == 1) ? (" dollar") : (" dollars")) << " in fees over "<< convert2interval(endtime-starttime) << ".\n";
        }
        else if(first == "h"){
            std::string name;
            std::cin >> name;
            int cnt_recip = 0;
            int cnt_send = 0;
            if(registration.count(name) == 1){
                std::vector<Transaction> tmp_trans_send;
                std::vector<Transaction> tmp_trans_recip;
                tmp_trans_send.reserve(10);
                tmp_trans_recip.reserve(10);
                for(size_t i = exec_trans.size(); i > 0; --i){
                    if(exec_trans[i-1].recipient == name ){
                        if(cnt_recip < 10){
                            tmp_trans_recip.push_back(exec_trans[i-1]);
                        }
                        cnt_recip++;
                    }
                    if(exec_trans[i-1].sender == name){
                        if( cnt_send < 10){
                            tmp_trans_send.push_back(exec_trans[i-1]);
                        }
                        cnt_send++;
                        
                    }
                }
                std::cout << "Customer " << name << " account summary:\n";
                std::cout << "Balance: $" << registration[name].start_balc << "\n";
                std::cout << "Total # of transactions: " << cnt_recip + cnt_send << "\n";
                std::cout << "Incoming " << cnt_recip << ":\n";
                while(!tmp_trans_recip.empty()){
                    Transaction value = tmp_trans_recip.back();
                    tmp_trans_recip.pop_back();
                    std::cout << value.ID << ": " << value.sender << " sent " << value.amount << ((value.amount == 1) ? (" dollar") : (" dollars")) << " to " << value.recipient << " at " << value.exec_date << ".\n";
                }
                std::cout << "Outgoing " << cnt_send << ":\n";
                while(!tmp_trans_send.empty()){
                    Transaction value = tmp_trans_send.back();
                    tmp_trans_send.pop_back();
                    std::cout << value.ID << ": " << value.sender << " sent " << value.amount << ((value.amount == 1) ? (" dollar") : (" dollars")) << " to " << value.recipient << " at " << value.exec_date << ".\n";
                }
            }
            else{
                std::cout << "User " << name << " does not exist.\n";
            }
        }
        else if(first == "s"){
            std::string t;
            std::cin >> t;
            uint64_t timestamp = convert2time(t);
            uint64_t day = timestamp / 1000000;
            uint64_t starttime = day * 1000000;
            uint64_t endtime = (day+1) * 1000000;
            int cnt = 0;
            uint64_t dollar = 0;
            std::cout << "Summary of [" << starttime << ", " << endtime << "):\n";
            for(auto &value : exec_trans){
                if(value.exec_date >= starttime && value.exec_date < endtime){
                    cnt ++;
                    dollar += (value.recip_Fee+value.sender_Fee);
                    std::cout << value.ID << ": " << value.sender << " sent " << value.amount << ((value.amount == 1) ? (" dollar") : (" dollars")) << " to " << value.recipient << " at " << value.exec_date << ".\n";
                }
            }
            std::cout << "There" << ((cnt == 1) ? (" was") : (" were")) << " a total of " << cnt << ((cnt == 1) ? (" transaction") : (" transactions")) << ", 281Bank has collected " << dollar << " dollars in fees.\n";
        }
        else{
            break;
        }

    }
}