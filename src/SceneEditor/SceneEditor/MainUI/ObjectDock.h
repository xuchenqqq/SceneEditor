#ifndef OBJECTDOCK_H
#define OBJECTDOCK_H

#include <QDockWidget>
#include <QFileInfo>
class QAction;

namespace Ui {
class ObjectDock;
}

class ObjectDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit ObjectDock(QWidget *parent = 0);
    ~ObjectDock();
public slots:
    void SetPath(const QString& path);
private slots:
    void AddFolder();
    void DelFolder();
    void Explore();
    void CurrentChanged(const QModelIndex&,const QModelIndex&);
private:
    QString CurrentPath();
    void Init();
    void InitView();
    void BindSignal();
    void InitAction();
    void UpdateAction();

    void Preview(const QString& path);
    void LoadingImage(const QSize& size);
private:
    Ui::ObjectDock *ui;
    QString mPath;
    QString mFilePath;
    class ObjectModel* mpModel;
    QAction* mpAddFolder;
    QAction* mpDelFolder;
    QAction* mpExplore;
};

#endif // OBJECTDOCK_H
