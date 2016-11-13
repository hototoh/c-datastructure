#ifndef LOG_H
#define LOG_H

#include<stdarg.h>
#define MAX_LOG_LEN 512

#ifdef NDEBUG
#define debug( fmt, ... ) ((void)0)
#else
#include <stdio.h>
#define debug( fmt, ... )                       \
    fprintf( stderr,                            \
             "[%s] %s:%s:%u # " fmt "\n",       \
             __DATE__, __FILE__, __func__,      \
             __LINE__, ##__VA_ARGS__            \
             )
#endif

#define LOG_TEMPLATE(out, in)                   \
    sprintf( out,                               \
             "[%s] %s:%s:%u # %s\n",            \
             __DATE__, __FILE__,                \
             __func__, __LINE__, in             \
             )

static inline void __log_error(char* fmt, ...) {
    char string[MAX_LOG_LEN];
    va_list ap;

    va_start(ap, fmt);    
    vsprintf(string, fmt, ap);
    va_end(ap);

    printf("%s\n", string);
}
#define log_error( fmt, ...) ({                                     \
            char string[MAX_LOG_LEN];                               \
            sprintf(string, "[%s] %s:%s:%u # %s\n",                 \
                    __DATE__, __FILE__, __func__, __LINE__, fmt);   \
            __log_error(string, ##__VA_ARGS__); })

static inline void __log_perror(char* fmt, ...) {    
    char tmp[MAX_LOG_LEN];
    char string[MAX_LOG_LEN];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(tmp, fmt, ap);
    va_end(ap);
    
    perror(string);
}
#define log_perror( fmt, ...) ({                                    \
            char string[MAX_LOG_LEN];                               \
            sprintf(string, "[%s] %s:%s:%u # %s\n",                 \
                    __DATE__, __FILE__, __func__, __LINE__, fmt);   \
            __log_perror(string, ##__VA_ARGS__); })

#endif
