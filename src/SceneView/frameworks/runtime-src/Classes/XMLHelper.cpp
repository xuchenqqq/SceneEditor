//XMLHelper.cpp

#include "tinyxml.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

void LuaXML_ParseNode (lua_State *L,TiXmlNode* pNode) {
    if (!pNode) return;
    // resize stack if neccessary
    luaL_checkstack(L, 5, "LuaXML_ParseNode : recursion too deep");
    
    // children
    TiXmlNode *pChild = pNode->FirstChild();
    if (pChild) {
        //int iChildCount = 0;
        for(;pChild;pChild = pChild->NextSibling()) {
            switch (pChild->Type()) {
                case TiXmlNode::TINYXML_DOCUMENT: break;
                case TiXmlNode::TINYXML_ELEMENT:
                    // normal element, parse recursive
                    lua_newtable(L);
                    LuaXML_ParseNode(L,pChild);
                    lua_setfield(L, -2, pChild->Value());
                    break;
                case TiXmlNode::TINYXML_COMMENT: break;
                case TiXmlNode::TINYXML_TEXT:
                    // plaintext, push raw
                    lua_pushstring(L,pChild->Value());
                    //lua_rawseti(L,-2,++iChildCount);
                    lua_setfield(L, -2, "Value");
                    break;
                case TiXmlNode::TINYXML_DECLARATION: break;
                case TiXmlNode::TINYXML_UNKNOWN: break;
            };
        }
    }
}

static int LuaXML_ParseFile (lua_State *L) {
    const char* sFileName = luaL_checkstring(L,1);
    TiXmlDocument doc(sFileName);
    doc.LoadFile();
    lua_newtable(L);
    LuaXML_ParseNode(L,&doc);
    return 1;
}

void RegisterLuaXML (lua_State *L) {
    lua_register(L,"LuaXML_ParseFile",LuaXML_ParseFile);
}