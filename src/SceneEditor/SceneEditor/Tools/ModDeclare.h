#ifndef MOD_DECLARE_H
#define MOD_DECLARE_H

//Error: 如果没用M_DECL_MOD定义模块，将链接不过。
template<typename T>
inline const char* __ModName__();

#define M_DECL_MOD(T) \
template<> \
inline const char* __ModName__<T>() {return #T;}

#endif
