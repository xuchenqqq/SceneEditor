
cc.FileUtils:getInstance():setPopupNotify(false)
cc.FileUtils:getInstance():addSearchPath("src/")
cc.FileUtils:getInstance():addSearchPath("res/")

require "config"
require "cocos.init"

local function main()
    local scene = require("GameScene.lua")
    local gameScene = scene.create()

    if cc.Director:getInstance():getRunningScene() then
    	cc.Director:getInstance():replaceScene(gameScene)
    else
    	cc.Director:getInstance():runWithScene(gameScene)
    end
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    print(msg)
end
