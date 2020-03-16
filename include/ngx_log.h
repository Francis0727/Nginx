#ifndef _NGX_LOG_H_
#define _NGX_LOG_H_

#include <string>

std::string ngx_log_stderr(int err, const char *fmt,va_list args);
std::string ngx_log_errno(int errCode);
void ngx_log_init();
void ngx_log_error_core(int level,  int err, const char *fmt, ...);

#endif