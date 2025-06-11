#pragma once

#include <QObject>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QDebug>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QtAndroid>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#endif


class Hal : public QObject
{
    Q_OBJECT
    // Property indicating if the internal storage read / write permission has been granted
    Q_PROPERTY (bool externalStorageAccessGranted
                   READ externalStorageAccessGranted
                       NOTIFY externalStorageAccessGrantedChanged)

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
    bool externalStorageAccessGranted() const ;

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
    void externalStorageAccessGrantedChanged();
    void appBuildInfoChanged();

private:
    double m_dpr; // DevicePixelRatio
    qreal m_physicalDotsPerInch;
    qreal m_devicePixelRatio;   
    bool m_debugMode;
    bool m_externalStorageAccessGranted;
    bool m_runMobile;
    QString m_appBuildInfo;
};

