#ifndef MSG_H_INCLUDED
#define MSG_H_INCLUDED

void errmsg(int line, const char* fmt, ...);
void setDebugFlag(void);
void debmsg(const char* fmt, ...);
int isDebug(void);
int errorcount(void);
void reset_error_count(void);

#endif

