#include "hal.h"

Hal::Hal(QObject *parent)
    : QObject{parent}
    , m_dpr(1)
    , m_physicalDotsPerInch(0)
    , m_devicePixelRatio(0)
    , m_debugMode(false)
    , m_externalStorageAccessGranted(false)
    , m_runMobile(false)
    , m_appBuildInfo(QString())
{
#ifdef QT_DEBUG
    m_debugMode = true;
#endif

#ifdef Q_OS_ANDROID
    m_externalStorageAccessGranted = true;
    m_runMobile = true;
#endif
    m_appBuildInfo= QString(tr("This program uses Qt version %1.")).arg(QT_VERSION_STR);
}


void Hal::updateInfo(){

}

double Hal::getDevicePixelRatio() const{
    int density = 0;
#ifdef Q_OS_ANDROID
    QAndroidJniEnvironment env;

    //  BUG with dpi on some androids: https://bugreports.qt-project.org/browse/QTBUG-35701
    QAndroidJniObject qtActivity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return EXIT_FAILURE;
    }

    QAndroidJniObject resources = qtActivity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return EXIT_FAILURE;
    }

    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return EXIT_FAILURE;
    }

    density = displayMetrics.getField<int>("densityDpi");
    qDebug() << "density:" << density;
#else
    density = m_physicalDotsPerInch * m_devicePixelRatio;
#endif
    return   density >= 480 ? 3 :
               density >= 320 ? 2 :
               density >= 240 ? 1.5 : 1;
}

bool Hal::getDebugMode() const
{
    return m_debugMode;
}

bool Hal::isRunOnMobile() const
{
    return m_runMobile;
}

bool Hal::externalStorageAccessGranted() const
{
    return m_externalStorageAccessGranted;
}

void Hal::setDebugMode(bool newDebugmode){
    if (m_debugMode == newDebugmode){
        return;
    }
    m_debugMode = newDebugmode;
}

void Hal::setDotsPerInch(qreal m_dpi)
{
    if (m_physicalDotsPerInch == m_dpi){
        return;
    }
    m_physicalDotsPerInch = m_dpi;
}

void Hal::setDevicePixelRatio(qreal m_dpr)
{
    if (m_devicePixelRatio == m_dpr){
        return;
    }
    m_devicePixelRatio = m_dpr;
}

/*!
 * \brief Make docs encourage readers to query locale right
 * \sa https://codereview.qt-project.org/c/qt/qtdoc/+/297560
 */
void Hal::createAppFolder(){
    ///TODO check permissions
    QDir dirAppConfig( QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) );
    if (dirAppConfig.exists() == false) {
        dirAppConfig.mkpath(dirAppConfig.path());
    }
    QDir dirAppData( QStandardPaths::writableLocation(QStandardPaths::AppDataLocation ) );
    if (dirAppData.exists() == false) {
        dirAppData.mkpath(dirAppData.path());
    }
#ifdef QT_DEBUG
    qDebug() <<"dirAppConfig.path():" <<dirAppConfig.path();
    qDebug() <<"dirAppData.path():" <<dirAppData.path();
#endif
}

QString Hal::getAppBuildInfo() {
    return m_appBuildInfo;
}
