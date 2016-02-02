#include "PreHeader.h"
#include "SceneEditor.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle(QStyleFactory::create("Fusion"));

	SceneEditor w;
	w.show();
	return a.exec();
}

