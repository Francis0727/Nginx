#include "ngx_c_conf.h"
#include "ngx_string.h"
#include "ngx_log.h"
#include <unistd.h>

char  **g_os_argv;
char *gp_envmem = NULL;      
int  g_environlen = 0;     

int main(int argc, char *const *argv) {
    g_os_argv = (char **) argv;
    ngx_init_setproctitle();
    ngx_setproctitle("master_process");

    CConfig* p_config = CConfig::GetInstance();
    if (p_config->Load("nginx.conf") == false){
        std::cout <<"读取配置文件失败..." << std::endl;
        exit(1);
    }

    ngx_log_init();
    ngx_log_error_core(1,2,"th erro code: %d", 12);
    if (gp_envmem){
        delete [] gp_envmem;
        gp_envmem = NULL;
    }
        
    return 0;
}