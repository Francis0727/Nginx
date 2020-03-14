#include <string>
#include <stdarg.h>
#include <vector>
#include <cstring>
#include "iostream"
#include "ngx_log.h"

enum class Type{
    Int = 0,    // %d
    Uint = 1,   // %ud
    Float = 2,  // %f 
    Frac = 3,   // %.3f
    String = 4, // %s
    Pid = 5,    // %P，支持打印进程ID
};

struct PosInfos{
    int pos;
    Type type;
};

std::vector<PosInfos>  GetPosInfos(const char* fmt) {
    size_t pos = 0;
    PosInfos element ;
    std::vector<PosInfos> ret;

    for (size_t cnt = 0; cnt < strlen(fmt); ++cnt){
        if (fmt[cnt] == '%'){
            switch (fmt[cnt + 1]){
                case 'd':
                    element.pos = cnt;
                    element.type = Type::Int;
                    ret.push_back(element);
                    cnt += 2;
                    break;
                case 'u':
                    element.pos = cnt;
                    element.type = Type::Uint;
                    ret.push_back(element);
                    cnt += 2;
                    break;
                case 'f':
                    element.pos = cnt;
                    element.type = Type::Float;
                    ret.push_back(element);
                    cnt += 2;
                    break;
                case 's':
                    element.pos = cnt;
                    element.type = Type::String;
                    ret.push_back(element);
                    cnt += 2;
                    break;
                case 'p':
                    element.pos = cnt;
                    element.type = Type::Pid;
                    ret.push_back(element);
                    cnt += 2;
                case '.':
                    pos = cnt + 2;
                    for (; pos < strlen(fmt); ++pos){
                        if ('0' <= fmt[pos] && fmt[pos] <= '9'){
                            continue;
                        }
                        
                        if (fmt[pos] == 'f'){
                            element.pos = cnt;
                            element.type = Type::Frac;
                            ret.push_back(element);
                            cnt = pos + 1;
                        }else {
                            cnt = pos;
                            break;
                        }
                    }
                    
            }
        }
        
    }
    
    return ret;
}

void ngx_log_stderr(int err, const char *fmt, ...){
    va_list args; 
    std::string errStr;
    std::vector<PosInfos> vec1 = GetPosInfos(fmt);
    auto ite = vec1.begin();
    for (;ite != vec1.end();++ite){
        std::cout << ite->pos <<" " << (int)ite->type << std::endl;
    }
}