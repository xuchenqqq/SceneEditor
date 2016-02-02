#ifndef XMLREADER_H
#define XMLREADER_H

#include<QtXml/QDomDocument>

class QFile;
class ScreenMng;
class GSLayer;
class GSScreen;

class XMLReader
{
public:
    XMLReader(ScreenMng* pMng);
    ~XMLReader();
    bool Read(QFile& file);
private:
    bool ParseHead(const QDomElement& root);
    bool ParseBody(const QDomElement& root);
    QDomNodeList NodeList(const QDomNode& parent);
    bool CreateScrren(const QDomNode& scene,int idx);
    bool CreateLayer(const QDomNode& layer,GSScreen* pScreen);
    bool CreateItem(const QDomNode& item,GSLayer* pLayer);
private:
    QDomDocument doc;
    ScreenMng* mpMng;
};

#endif // XMLREADER_H
