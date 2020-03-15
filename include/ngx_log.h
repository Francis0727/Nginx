#ifndef _NGX_LOG_H_
#define _NGX_LOG_H_

#include <string>

void ngx_log_stderr(int err, const char *fmt, ...);
std::string ngx_log_errno(int errCode);

#endif