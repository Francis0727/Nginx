#ifndef _NGX_STRING_H
#define _NGX_STRING_H

#include <string>

void removeInvalidChar(std::string& str);
void ngx_init_setproctitle();
void ngx_setproctitle(std::string title);

#endif