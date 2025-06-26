#pragma once

#include <QObject>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#endif


class Hal : public QObject
{
    Q_OBJECT


    Q_PROPERTY(double devicePixelRatio
                   READ getDevicePixelRatio
                       NOTIFY devicePixelRatioChanged);
    Q_PROPERTY(QString appBuildInfo READ getAppBuildInfo
                   NOTIFY appBuildInfoChanged);

public:
    explicit Hal(QObject *parent = nullptr);

    double getDevicePixelRatio() const;
    bool getDebugMode() const;
    bool isRunOnMobile() const;

    void setDebugMode(bool newDebugmode);
    void setDotsPerInch(qreal m_dpi);
    void setDevicePixelRatio(qreal m_dpr);
    void createAppFolder();


public slots:
    void updateInfo();
    QString getAppBuildInfo();

signals:
    void upTimeChanged();
    void devicePixelRatioChanged();
    void appBuildInfoChanged();

private:
    double m_dpr; // DevicePixelRatio
    qreal m_physicalDotsPerInch;
    qreal m_devicePixelRatio;

    bool m_debugMode;
    bool m_runMobile;    

    QString m_appBuildInfo;
};

