#include "ngx_c_conf.h"

int main() {
    CConfig* p_config = CConfig::GetInstance();
    if (p_config->Load("nginx.conf") == false){
        std::cout <<"读取配置文件失败..." << std::endl;
        exit(1);
    }
    
    return 0;
}