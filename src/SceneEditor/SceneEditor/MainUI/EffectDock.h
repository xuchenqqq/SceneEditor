#pragma once

#include <QDockWidget>
#include "ui_EffectDock.h"

class QAction;

class EffectDock : public QDockWidget
{
    Q_OBJECT
public:
    EffectDock(QWidget * parent);
    ~EffectDock();
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
private:
    QString mPath;
    class EffectModel* mpModel;
    Ui::EffectDock ui;
    QAction* mpAddFolder;
    QAction* mpDelFolder;
    QAction* mpExplore;
};

