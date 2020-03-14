#include <iostream>
#include <cstring>
#include "ngx_string.h"
#include "ngx_global.h"

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

// 函数功能: 分配另一块内存用于存放环境变量
void ngx_init_setproctitle(){    
    for (size_t i = 0;environ[i]; i++){
        g_environlen += strlen(environ[i]) + 1;
    }
    // 分配新内存,用于存放环境变量
    gp_envmem = new char[g_environlen];
    memset(gp_envmem,0,g_environlen);

    char* ptmp = gp_envmem;
    for (size_t i = 0; environ[i]; i++){
        size_t size = strlen(environ[i]) + 1;
        strcpy(ptmp,environ[i]);
        environ[i] = ptmp;

        ptmp += size;
    }
    
    return ;
}

void ngx_setproctitle(std::string title){
    // 定义标题长度和argv占用字节数的变量
    size_t titleLen = strlen(title.c_str());
    size_t localEnvironLen = 0;

    for (int i = 0; g_os_argv[i]; ++i){
        localEnvironLen += strlen(g_os_argv[i]) + 1;
    }
    
    if (titleLen >= localEnvironLen + g_environlen){
        std::cout << "the title is too long..." << std::endl;
        return ;
    }
    
    char* ptmp = g_os_argv[0];
    memset(ptmp,0,localEnvironLen + g_environlen);
    g_os_argv[1] = NULL;
    strcpy(ptmp,title.c_str());
    
    return ;
}
