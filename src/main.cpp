#include <QtCore/QCoreApplication>

#include <QtCore/QTranslator>
#include <QtGui/QGuiApplication>

#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtGui/QScreen>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#endif

#ifdef QT_DEBUG
#include <QtCore/QDirIterator>
#include <QtCore/QLoggingCategory>

#endif

#include "hal.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    //QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

#ifdef QT_DEBUG
    QLocale::setDefault(QLocale::English);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));
#endif

    // Allocate [Hal] before the engine to ensure that it outlives it !!
    QScopedPointer<Hal> m_hal(new Hal);   
    m_hal->createAppFolder();

    QCoreApplication::setOrganizationName(PACKAGE_NAME_STR);
    QCoreApplication::setApplicationName(ACTIVITY_NAME_STR);
    QCoreApplication::setApplicationVersion(VERSION_STR);
    QGuiApplication app(argc, argv);

    QTranslator myappTranslator;
    if (  myappTranslator.load(QLocale(), QLatin1String("floodit"), QLatin1String("_"), QLatin1String(":/res/i18n")) ){
        app.installTranslator(&myappTranslator);
    }

    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/res/qml");
    QQmlContext *context = engine.rootContext();

#ifdef Q_OS_ANDROID    
    QtAndroid::hideSplashScreen();
#endif

    QScreen *screen = qApp->primaryScreen();
    m_hal->setDotsPerInch( screen->physicalDotsPerInch() );
    m_hal->setDevicePixelRatio( screen->devicePixelRatio() );

#ifdef QT_DEBUG
    qDebug() << "screen->devicePixelRatio():" << screen->devicePixelRatio();
    qDebug() << "screen->physicalSize():" <<screen->physicalSize();
    qDebug() << "screen->logicalDotsPerInch():" << screen->logicalDotsPerInch();
#endif

    context->setContextProperty("AppVersion",VERSION_STR);
    context->setContextProperty("isDebugMode", m_hal->getDebugMode() );           
    context->setContextProperty("isMobile",m_hal->isRunOnMobile());

    const QUrl url(QStringLiteral("qrc:/res/qml/main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](const QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    /**
 * @note Register the singleton type provider with QML by calling this function in an initialization function.
 * @sa  @link https://raymii.org/s/articles/Qt_QML_Integrate_Cpp_with_QML_and_why_ContextProperties_are_bad.html
 *
 */
    qmlRegisterSingletonInstance("io.github.zanyxdev.floodit.hal", 1, 0,"HAL", m_hal.get());
    engine.load(url);

    return app.exec();
}
