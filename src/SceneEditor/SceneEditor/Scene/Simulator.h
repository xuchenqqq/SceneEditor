#pragma once

#include <QObject>
#include <QDir>
#include <QFile>

class QProcess;
class QAction;

class Simulator : public QObject
{
    Q_OBJECT
public:
    Simulator(QObject* parent);
    ~Simulator();
public slots:
    void SetMapName(const QString& filePath);
    void SetMapPath(const QString& filePath);
    void SetAssertPath(const QString& filePath);
    void setObjectPath(const QString& filePath);
//    void SetEffectPath(const QString& filePath);
private slots:
    void Config();
    void Run();
    void Stop();
    void HandleStdout();
private:
    void Init();
    void InitAction();
    void BindSignal();
    bool copyDirectoryFiles(const QDir &fromDir, const QDir &toDir, bool coverFileIfExist);
private:
    QProcess* mpProc;
    QAction* mpConfig;
    QAction* mpRun;
    QAction* mpStop;
    QString mExeFilePath;
    QString mMapPath;
    QString mAssertPath;
    QString mMapName;
//    QString mEffectPath;
    QString mObjectPath;
    QString mCmdLine;
};
