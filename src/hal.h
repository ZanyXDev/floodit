#pragma once

#include <QObject>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QSettings>

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

    // Property read/write ligth mode  (dark/ligth) value save to Settings
    Q_PROPERTY (bool lightMode
                   READ getLightMode
                       WRITE setLightMode
                           NOTIFY lightModeChanged)
    Q_PROPERTY(double devicePixelRatio
                   READ getDevicePixelRatio
                       NOTIFY devicePixelRatioChanged);
    Q_PROPERTY(QString appBuildInfo READ getAppBuildInfo
                   NOTIFY appBuildInfoChanged);

public:
    explicit Hal(QObject *parent = nullptr, QSettings *settings = nullptr);

    double getDevicePixelRatio() const;
    bool getDebugMode() const;
    bool isRunOnMobile() const;

    void setDebugMode(bool newDebugmode);
    void setDotsPerInch(qreal m_dpi);
    void setDevicePixelRatio(qreal m_dpr);
    void createAppFolder();

    bool getLightMode() const;
    void setLightMode(bool newLightMode);
public slots:
    void updateInfo();
    QString getAppBuildInfo();

signals:
    void upTimeChanged();
    void devicePixelRatioChanged();
    void appBuildInfoChanged();
    void lightModeChanged();

private:
    double m_dpr; // DevicePixelRatio
    qreal m_physicalDotsPerInch;
    qreal m_devicePixelRatio;

    bool m_debugMode;
    bool m_runMobile;    
    bool m_lightMode;

    QString m_appBuildInfo;
    QSettings *m_settings;
};

