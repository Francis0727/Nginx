#include <iostream>
#include "ngx_string.h"

void removeInvalidChar(std::string& str){
    size_t validChar = 0;
    for (size_t cnt = 0; cnt < str.size();++cnt){
        if (str[cnt] != ' ' && (int)str[cnt] != 13){
            str[validChar] = str[cnt];
            ++validChar;
        }
    }
    str = str.substr(0, validChar);
}