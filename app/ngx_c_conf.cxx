#include <fstream>
#include "ngx_c_conf.h"
#include "ngx_string.h"

CConfig *CConfig::m_instance = NULL;

bool CConfig::Load(std::string confName){
    std::ifstream fin;
    fin.open(confName);
    std::string lineContent;
    while (!fin.eof()){
        std::getline(fin,lineContent);
        removeInvalidChar(lineContent);
        // 注: 对注释行、空行进行处理
        if (lineContent.size() == 0 || lineContent[0] == '#' || lineContent[0] == '['){
            continue;
        }
        
        CConfItem configItem;
        size_t pos = lineContent.find_first_of('=');
        configItem.ItemName = lineContent.substr(0,pos);
        configItem.ItemContent = lineContent.substr(pos + 1);
        m_ConfigItemList.push_back(configItem);
    }

    return true;
}