#include "PreHeader.h"
#include "Simulator.h"
#include "ActionMng.h"
#include "AppModAlias.h"
#include <QProcess>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QRegularExpression>
#include <QProcess>
#include "Macro.h"
#include <assert.h>
#include "SimConfigDlg.h"
#include "Setting.h"
#include "AppModAlias.h"
#include "Log.h"
#include <QtDebug>

extern bool coutFormat;

Simulator::Simulator(QObject* parent)
    : QObject(parent)
    , mpProc(new QProcess(this))
{
    Init();
    InitAction();
    BindSignal();
}
Simulator::~Simulator(){
}
void Simulator::Config(){

    /*
    SimConfigDlg dlg;
    if(dlg.exec() == QDialog::Accepted){
        mCmdLine = dlg.CmdLine();
        if(!mCmdLine.isEmpty()){
            mExeFilePath = mCmdLine;
            qDebug("x:%s",qPrintable(mExeFilePath));
            mpRun->setEnabled(true);
        }
    }
     */
    QString currentPath = QCoreApplication::applicationDirPath();
    QString cmd = currentPath.remove("SceneEditor.app/Contents/MacOS");
    mCmdLine = cmd.append("SceneView.app");
    mExeFilePath = mCmdLine;
    mpRun->setEnabled(true);
}
void Simulator::Run(){

#if defined(__APPLE__)
    if(mMapPath.isEmpty() || mAssertPath.isEmpty())
        return;
    QString destMapPath;
    QString strMapConfig;
    if(coutFormat){
        destMapPath = mExeFilePath + "/Contents/Resources/xml/";
        strMapConfig = "xml/"+ mMapName;
    }else{
        destMapPath = mExeFilePath + "/Contents/Resources/json/";
        strMapConfig = "json/"+ mMapName;
    }
    QString destImagePath = mExeFilePath + "/Contents/Resources/image/";
    QString destObjectPath = mExeFilePath+ "/Contents/Resources/object/";
    QString mapconfigFile = mExeFilePath + "/Contents/Resources/src/path.lua";

    QFile f(mapconfigFile);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outStream(&f);
    outStream << strMapConfig;

    f.close();

    if(!copyDirectoryFiles(mMapPath, destMapPath, true))
        return;
    if(!copyDirectoryFiles(mAssertPath, destImagePath, true))
        return;
    if(!copyDirectoryFiles(mObjectPath, destObjectPath, true))
        return;

    QString cmd = mCmdLine;
    mpProc->start(cmd);
    mpRun->setEnabled(false);
    mpStop->setEnabled(true);
#endif
    return;
}
void Simulator::Stop(){
    mpProc->kill();
    mpRun->setEnabled(true);
    mpStop->setEnabled(false);
}
void Simulator::HandleStdout(){
    Log::Info(QString::fromUtf8(mpProc->readAllStandardOutput()));
}
void Simulator::Init(){
    mCmdLine="";
    mExeFilePath = "";
}
void Simulator::InitAction(){
    ActionMng& actMng = sActionMng;
    actMng.BeginPart(QStringLiteral("模拟器"),true,true);
        mpConfig= actMng.AddAction(QIcon(":/Icon/config.png"),QStringLiteral("设置"));
        actMng.AddSeparator();
        mpRun   = actMng.AddAction(QIcon(":/Icon/play.png"),QStringLiteral("运行"));
        mpStop  = actMng.AddAction(QIcon(":/Icon/stop.png"),QStringLiteral("停止"));
    actMng.EndPart();
    
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
    mpConfig->setEnabled(false);
#endif
    mpRun->setEnabled(false);
    mpStop->setEnabled(false);
}
void Simulator::BindSignal(){
    bool res = false;
    res = connect(mpConfig,&QAction::triggered,this,&Simulator::Config); assert(res);
    res = connect(mpRun,&QAction::triggered,this,&Simulator::Run); assert(res);
    res = connect(mpStop,&QAction::triggered,this,&Simulator::Stop); assert(res);
    res = connect(mpProc,&QProcess::readyReadStandardOutput,
                  this,&Simulator::HandleStdout); assert(res);
}

void Simulator::SetMapName(const QString& filePath){
    mMapName = filePath;
    if(mMapName.isEmpty()){
        mpRun->setEnabled(false);
        mpStop->setEnabled(false);
    }else{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))

    QFile f("win32/res/mapname.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outStream(&f);
    outStream << mMapName;

    f.close();
    mpRun->setEnabled(false);
#else
        mpRun->setEnabled(true);

#endif
        mpStop->setEnabled(false);
    }
}

void Simulator::SetMapPath(const QString& filePath){
    mMapPath = filePath;
    if(mMapPath.isEmpty()){
        mpRun->setEnabled(false);
        mpStop->setEnabled(false);
    }else{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))

    QFile f("win32/res/mappath.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outStream(&f);
    outStream << mMapPath;

    f.close();
    mpRun->setEnabled(false);
#else
       mpRun->setEnabled(true);

#endif
        mpStop->setEnabled(false);
    }
}

void Simulator::SetAssertPath(const QString& filePath){
    mAssertPath = filePath;
    if(mAssertPath.isEmpty()){
        mpRun->setEnabled(false);
        mpStop->setEnabled(false);
    }else{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))

    QFile f("win32/res/assertpath.txt");
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream outStream(&f);
    outStream << mAssertPath;

    f.close();
    mpRun->setEnabled(false);
#else
        mpRun->setEnabled(true);

#endif
        mpStop->setEnabled(false);
    }
}


void Simulator::setObjectPath(const QString &filePath)
{
    mObjectPath = filePath;
    if(mObjectPath.isEmpty())
    {
        mpRun->setEnabled(false);
        mpStop->setEnabled(false);
    }
    else{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))

        QFile f("win32/res/objectpath.txt");
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream outStream(&f);
        outStream << mObjectPath;

        f.close();
        mpRun->setEnabled(false);
    #else
            mpRun->setEnabled(true);

    #endif
            mpStop->setEnabled(false);
    }
}

bool Simulator::copyDirectoryFiles(const QDir &fromDir, const QDir &toDir, bool coverFileIfExist)
{
    QDir sourceDir = fromDir;
    QDir targetDir = toDir;
    if(!targetDir.exists()){
        if(!targetDir.mkdir(toDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir()){
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName()),
                                   coverFileIfExist))
                return false;
        }
        else{
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}
