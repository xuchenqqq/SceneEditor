#ifndef XMLWRITER_H
#define XMLWRITER_H

class ScreenMng;
class GSScreen;
class GSLayer;
class GSItem;

#include <QAction>
#include <QFile>
#include <QtXml/QDomDocument>

class XMLWriter
{
public:
    XMLWriter(const ScreenMng* pMng);
    ~XMLWriter();
    void Write(QFile& file);
private:
    void Head(QDomElement& root);
    void Body(QDomElement& root);
    void Scene(QDomElement& body);
    void Layer(QDomElement& scene,int num);
    void Item(QDomElement& layer,GSLayer* l);
    void createSizeElement(QDomElement& parent,int width,int height);
    void createPosElement(QDomElement& parent,qreal x,qreal y);
    void createCusElement(QDomElement& parent,GSItem* item);
private:
    const ScreenMng* mpMng;
    QDomDocument doc;
};

#endif // XMLWRITER_H
