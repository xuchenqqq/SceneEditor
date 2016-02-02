//
//  XMLHelper.hpp
//  MyLuaGame
//
//  Created by xuchen on 15/12/11.
//
//

#ifndef XMLHelper_hpp
#define XMLHelper_hpp

#include <stdio.h>

class lua_State;

//Register parser functions to lua
void RegisterLuaXML(lua_State* L);

#endif /* XMLHelper_hpp */
