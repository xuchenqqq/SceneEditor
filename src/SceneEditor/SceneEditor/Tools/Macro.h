#ifndef MACRO_H
#define MACRO_H

#define LEN_OF_ARRAY(array) int(sizeof(array)/sizeof(array[0]))
#define FAIL_RET_X(act,x)  if(!(act))return x;
#define FAIL_RET_VOID(act) if(!(act))return;
#define FAIL_RET(act)   FAIL_RET_X(act,false);
#define FAIL_BREAK(act)    if(!(act))break;
#define SUCC_RET_X(act,x)  if(act)return x;
#define SUCC_RET_VOID(act)  if(act)return;
#define STR(x) #x
#define FAIL_GOTO(cond,errSec) if(!(cond)) goto errSec;
#define ASSERT_MSG(cond,msg) assert((cond)&&("原因: " msg));
#define ASSERT_MSG_RET(cond,msg) ASSERT_MSG(cond,msg);FAIL_RET(cond);
#define ASSERT_MSG_RET_X(cond,msg,x) ASSERT_MSG(cond,msg);FAIL_RET_X(cond,x);
#define ASSERT_MSG_RET_VOID(cond,msg) ASSERT_MSG(cond,msg); FAIL_RET_VOID(cond);
#define CLR_BIT(var,bit) (var)&=(~(bit))
#define SET_BIT(var,bit) (var)|=(bit)

#endif
