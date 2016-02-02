local GameScene = class("GameScene",function (  )
	return cc.Scene:create()
end)

function GameScene.create(  )
	local scene = GameScene.new()
	scene:addChild(scene:createLayer())
	return scene
end

function GameScene:ctor(  )
end

function GameScene:createLayer(  )
    local winSize = cc.Director:getInstance():getWinSize()
	local mapLayer = cc.Layer:create()
	local mapNode = cc.ParallaxNode:create()
	mapLayer:addChild(mapNode)

    local xml = LuaXML_ParseFile("res/map/xml/mountain_map.xml");
    local head = xml.map.Head
    local body = xml.map.Body
    
    local scene_count = tonumber(head.SceneCount.Value)
    local scene_width = tonumber(head.Size.Width.Value)
    local scene_height = tonumber(head.Size.Height.Value)
    local scene_dir = tonumber(head.SceneDir.Value)

    local sceneOffsetX = 0
    local sceneOffsetY = 0
    local contentSize = nil
    local  go = nil

    local moveVel_X = 500
    local moveVel_Y = 500

    if scene_dir == 0 then   -- right
        sceneOffsetX = scene_width
        sceneOffsetY = 0
        contentSize = cc.size(scene_count * scene_width, scene_height)
        --go = cc.MoveBy:create(scene_count * scene_width/moveVel_X, cc.p(-scene_count * scene_width,0) )
        go = cc.MoveBy:create(scene_count * scene_width/moveVel_X, cc.p(-48000,0) )
    

    elseif scene_dir == 1 then  -- top
        sceneOffsetX = 0
        sceneOffsetY = scene_height
        contentSize = cc.size(scene_width,scene_count * scene_height)
        go = cc.MoveBy:create(scene_count * scene_height/moveVel_Y, cc.p(0,-scene_count * scene_height) )
        
    elseif scene_dir == 2 then -- bottom
        sceneOffsetX = 0
        sceneOffsetY = -scene_height
        mapNode:setPosition(0, 640 - scene_count * scene_height)
        contentSize = cc.size(scene_width,scene_count * scene_height)
        go = cc.MoveBy:create(scene_count * scene_height/moveVel_Y, cc.p(0,scene_count * scene_height) )
    end

    local  goBack = go:reverse()
    local  seq = cc.Sequence:create( go, goBack)
    mapNode:runAction( (cc.RepeatForever:create(seq) ))

    for i,scene in pairs(body) do
    	for j,layer in pairs(scene) do
    		if string.sub(j,0,5) == "Layer" then
	    		local view = cc.Node:create()
	    		local zorder = tonumber(layer.ZOrder.Value)
	    		local speed = tonumber(layer.Speed.Value)
	    		for k,sprite in pairs(layer) do
	    			if string.sub(k,0,6) == "Sprite" then
	    				local sp = cc.Sprite:create("map/image/"..sprite.Name.Value)
	    				local offx = tonumber(sprite.Pos.X.Value)
	    				local offy = tonumber(sprite.Pos.Y.Value)
	    				local flipx = tonumber(sprite.FlipX.Value)
	    				local flipy = tonumber(sprite.FlipY.Value)
	    				local scale = tonumber(sprite.Scale.Value)
	    				local zOrder = tonumber(sprite.ZOrder.Value)
	    				sp:setPosition(cc.p(offx,offy))
                        if flipx == 1 then
	    				   sp:setFlippedX(true)
                        end
                        if flipy == 1 then
                            sp:setFlippedY(true)
                        end
	    				sp:setScale(scale)
	    				view:addChild(sp,zOrder)
	    			end
	    		end
	    		mapNode:addChild(view, zorder, cc.p(speed/10,1), cc.p(0,0))
	    	end
    	end
    end
    return mapLayer
end

return GameScene