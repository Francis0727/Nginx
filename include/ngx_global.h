#ifndef _NGX_GLOBAL_H__
#define _NGX_GLOBAL_H__

#include <string>

typedef struct {
    std::string ItemName;
    std::string ItemContent;
}CConfItem;

// 外部全局变量声明
extern char **environ;
extern int   g_environlen;
extern char  *gp_envmem;
extern char  **g_os_argv;

#endif