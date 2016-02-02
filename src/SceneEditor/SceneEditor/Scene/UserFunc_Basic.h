#include "UserFunc.h"

class UseFunc_Basic : public UserFunc
{
public:
	UseFunc_Basic();
	~UseFunc_Basic();
	void OnItemChanged();
	void OnDelete();
	void OnCopy();
	void OnPaste();
private:
	void InitAction();
	void BindSignal();
private:
	QAction* mpDelete;
	QAction* mpCopy;
	QAction* mpPaste;
	class ItemClipboard* mpClipboard;
};