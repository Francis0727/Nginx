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
    String = 3, // %s
    Pid = 4,    // %P，支持打印进程ID
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
                    break;
            }
        }
        
    }
    
    return ret;
}

std::string ngx_log_errno(int errCode){
    char* perrorInfo = std::strerror(errCode);
    std::string leftstr = std::string("(") + std::to_string(errCode);
    std::string codeInfo = std::string(perrorInfo);
    std::string rightStr = std::string(")");
    return  leftstr + codeInfo + rightStr;
}

void ngx_log_stderr(int err, const char *fmt, ...){
    va_list args;
    va_start(args,fmt);

    size_t pos = 0;
    std::string errStr = "";
    std::vector<PosInfos> infos = GetPosInfos(fmt);
    // 格式字符串中无表示数据类型的字符
    if (infos.size() == 0){
        errStr = std::string(fmt);
        return ;
    }
    
    std::string strFmt = std::string(fmt);
    size_t position = 0;    // 记录'f'字符的位置
    size_t digitNum;
    std::string readStr;    // 记录读取到的浮点数
    for (std::vector<PosInfos>::iterator ite = infos.begin();ite != infos.end();++ite){
        errStr += strFmt.substr(pos,ite->pos - pos);
        switch (ite->type){
            case Type::Int:
                pos = ite->pos + 2;
                errStr +=  std::to_string(va_arg(args,int));
                break;
            case Type::Uint:
                pos = ite->pos + 2;
                errStr += std::to_string(va_arg(args,unsigned int));
                break;
            case Type::Float:
                pos = ite->pos + 2;
                errStr += std::to_string(va_arg(args,double));
                break;
            case Type::String:
                pos = ite->pos + 2;
                errStr += std::string(va_arg(args,const char*));
                break;
            case Type::Pid:
                pos = ite->pos + 2;
                errStr += std::to_string(va_arg(args,unsigned int));
                break;
        } 
    }

    if (pos != strFmt.size()){
        errStr += strFmt.substr(pos,strFmt.size() - pos);
    }
    
    va_end(args);
    errStr += ngx_log_errno(err);
    std::cout << "errMessage: " << errStr << std::endl;
}