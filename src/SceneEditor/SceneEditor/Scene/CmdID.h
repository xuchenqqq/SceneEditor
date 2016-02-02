#ifndef CMD_ID_H
#define CMD_ID_H

namespace CmdID
{
	enum{
		OP_NULL = 0,
		//景区相关操作
		LAYER_INSERT,
		LAYER_REMOVE,
		LAYER_ORDER,
		//图层相关操作
		ITEM_INSERT,
		ITEM_REMOVE,
		ITEM_SCALE,
		ITEM_MOVE,
		ITEM_ORDER,
		ITEM_IMAGE,
		ITEM_FLIP,
	};
}

#endif