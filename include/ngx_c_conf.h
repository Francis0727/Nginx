#ifndef _NGX_C_CONF_H__
#define _NGX_C_CONF_H__

#include <vector>
#include <iostream>
#include <string>
#include "ngx_global.h"

class CConfig {
public:
    std::vector<CConfItem> m_ConfigItemList;
private:
    static CConfig* m_instance;

public:
    static CConfig* GetInstance(){
        if (m_instance == NULL){
            if (m_instance == NULL){
                m_instance = new CConfig();
                // 程序结束时,用于自动释放m_instance分配的内存
                static DestructInstance c1;
            }
        }
        return m_instance;
    }

    class DestructInstance{
    public:
        ~DestructInstance(){
            if (m_instance){
                delete CConfig::m_instance;
                CConfig::m_instance = NULL;
                std::cout <<"delete CConfig::m_instance..." << std::endl;
            }
        }
    };

public:
    bool Load(std::string confName);
};

#endif