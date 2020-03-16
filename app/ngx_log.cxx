#include <string>
#include <stdarg.h>
#include <vector>
#include <cstring>
#include <sys/types.h>    
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "iostream"
#include "ngx_log.h"
#include "ngx_c_conf.h"
#include "ngx_macro.h"


ngx_log_t   ngx_log;

enum class Type{
    Int = 0,    // %d
    Uint = 1,   // %ud
    Float = 2,  // %f 
    String = 3, // %s
    Pid = 4,    // %P，支持打印进程ID
};

std::string err_levels[] = {"stderr","emerg","alert","crit",
    "error","warn","notice","info","debug"
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
    std::string leftstr = std::string("(") + std::to_string(errCode) +": ";
    std::string codeInfo = std::string(perrorInfo);
    std::string rightStr = std::string(")");
    return  leftstr + codeInfo + rightStr;
}

std::string ngx_log_stderr(int err, const char *fmt,va_list args){
    size_t pos = 0;
    std::string errStr = "";
    std::vector<PosInfos> infos = GetPosInfos(fmt);
    // 格式字符串中无表示数据类型的字符
    if (infos.size() == 0){
        errStr = std::string(fmt);
        return errStr;
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
    
    if (err){
        errStr += ngx_log_errno(err);
    }
    
    return errStr;
}

void ngx_log_init(){
    CConfig* p_config = CConfig::GetInstance();
    // 获得日志文件所在位置
    std::string logName = p_config->GetString("Log");
    if (logName.size() == 0){
        logName = std::string(NGX_ERROR_LOG_PATH);
    }
    
    ngx_log.log_level = p_config->GetIntDefault("LogLevel",NGX_LOG_NOTICE);
    ngx_log.fd = open(logName.c_str(),O_WRONLY|O_APPEND|O_CREAT,0644);
    if (ngx_log.fd == -1){
        std::cout << "open log file falled..." << std::endl;
    }
    return ;
}

void ngx_log_error_core(int level,  int err, const char *fmt, ...){
    struct tm tm;
    struct timeval tv;
    memset(&tm,0,sizeof(struct timeval));
    memset(&tv,0,sizeof(struct tm));

    gettimeofday(&tv,NULL);
    time_t sec = tv.tv_sec;
    localtime_r(&sec,&tm); 
    tm.tm_mon++;
    tm.tm_year += 1900;

    std::string timeStr = std::to_string(tm.tm_year) + "/" + \
        std::to_string(tm.tm_mon) + "/" + \
        std::to_string(tm.tm_mday) + " " + \
        std::to_string(tm.tm_hour) + ":" + \
        std::to_string(tm.tm_min) + ":" + std::to_string(tm.tm_sec);
    std::string pidStr = std::to_string(getpid());
    va_list args;
    va_start(args,fmt);
    std::string errStr = ngx_log_stderr(err,fmt,args);
    va_end(args);

    std::string errMessage = timeStr + "[" + err_levels[level] + "]" + \
        pidStr + ":" + errStr;
    if (level <= ngx_log.log_level){
        write(ngx_log.fd,errMessage.c_str(),errMessage.size());
    }
}