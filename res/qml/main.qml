import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2

import common 1.0
import visual_effects 1.0

import io.github.zanyxdev.floodit 1.0
import io.github.zanyxdev.floodit.hal 1.0

QQC2.ApplicationWindow {
  id: appWnd

  // Required properties should be at the top.
  readonly property int screenOrientation: Qt.PortraitOrientation
  readonly property bool appInForeground: Qt.application.state === Qt.ApplicationActive

  property bool appInitialized: false

  property var screenWidth: Screen.width
  property var screenHeight: Screen.height
  property var screenAvailableWidth: Screen.desktopAvailableWidth
  property var screenAvailableHeight: Screen.desktopAvailableHeight

  ///TODO use settings in cpp part to load|save mode
  property bool lightMode: dataManager.lightMode

  // ----- Signal declarations
  signal screenOrientationUpdated(int screenOrientation)


  /**
  * @brief
  * При работе с Android системами обычно выбирается базовый фрейм 360×640,
  * для адаптации под удлиненные экраны 18:9 можно использовать размер фрейма 360×720.
  * Размер фрейма для приложения на системе IOS чаще всего используется 375×812.
*/
  width: 360
  height: 640

  maximumHeight: height
  maximumWidth: width

  minimumHeight: height
  minimumWidth: width
  // ----- Then comes the other properties. There's no predefined order to these.
  visible: true
  visibility: (isMobile) ? Window.FullScreen : Window.Windowed
  flags: Qt.Dialog
  title: qsTr(" ")
  Screen.orientationUpdateMask: Qt.PortraitOrientation | Qt.LandscapeOrientation | Qt.InvertedPortraitOrientation
                                | Qt.InvertedLandscapeOrientation

  // ----- Then attached properties and attached signal handlers.

  // ----- States and transitions.
  // ----- Signal handlers
  Component.onCompleted: {

    // Write to model default values from QSetting
    ///TODO read from  dataManager (need use settings for read|save values)
    appSettingsModel.setProperty(0, "widthPortition", appWnd.width)
    appSettingsModel.setProperty(0, "heightPortition", appWnd.height)
    appSettingsModel.setProperty(0, "showFogParticles", true)
    appSettingsModel.setProperty(0, "showShootingStarParticles", true)
    appSettingsModel.setProperty(0, "showLighting", true)
    appSettingsModel.setProperty(0, "showColors", true)
    appSettingsModel.setProperty(0, "boardSize", 4)

    let infoMsg = `Screen.height[${Screen.height}], Screen.width[${Screen.width}]
    Screen [height ${height},width ${width}]
    Build with [${HAL.getAppBuildInfo()}]
    Available physical screens [${Qt.application.screens.length}]
    Available Resolution width: ${Screen.desktopAvailableWidth} height ${Screen.desktopAvailableHeight}
    `
    AppSingleton.toLog(infoMsg)

    if (!isMobile) {
      appWnd.moveToCenter()
    }
    // appWnd.boardSize = 6
    // dataManager.startNewGame(6, 4, false)
    //AppSingleton.toLog(`dataManager.boardModel.rowCount [${dataManager.boardModel.rowCount()}]`)
  }

  onAppInForegroundChanged: {
    if (appInForeground) {
      if (!appInitialized) {
        appInitialized = true

        //appCore.initialize()
      }
    } else {
      if (isDebugMode)
        console.log(
              "onAppInForegroundChanged-> [appInForeground:" + appInForeground + ", appInitialized:" + appInitialized + "]")
    }
  }

  // ----- Visual children
  Carusel {
    id: carusel

    anchors.fill: parent
    model: picturesModel
    settingModel: appSettingsModel
    Component.onCompleted: {
      if (isDebugMode) {
        AppSingleton.toTagLog("carusel", "completed")
        AppSingleton.toTagLog("carusel", "lightImage.y:" + lightImage.y)
        AppSingleton.toTagLog("carusel", "appWnd.height:" + appWnd.height)
      }
    }
  }
  AnimatedSprite {
    id: lightImage
    width: 64
    height: 64
    frameWidth: 128
    frameHeight: 128
    frameCount: 16
    frameRate: 15
    source: "qrc:/res/images/planet_sprite.png"
    interpolate: true
    loops: Animation.Infinite
    visible: appSettingsModel.get(0).showLighting
    running: true
    //running: !detailsView.isShown && !infoView.isShown && (settings.showLighting
    //                                                       || settings.showShootingStarParticles)
    onXChanged: {
      appSettingsModel.setProperty(0, "globalLightPosX", lightImage.x / appWnd.width)
    }
    onYChanged: {
      appSettingsModel.setProperty(0, "globalLightPosY", lightImage.y / appWnd.height)
    }
  }
  PathAnimation {
    target: lightImage
    duration: AppSingleton.timer5000
    orientation: PathAnimation.RightFirst
    anchorPoint: Qt.point(lightImage.width / 2, lightImage.height / 2)
    running: true

    //paused: detailsView.isShown || infoView.isShown || (!settings.showLighting && !settings.showShootingStarParticles)
    loops: Animation.Infinite

    path: Path {
      id: lightAnimPath
      startX: appWnd.width * 0.5
      startY: appWnd.height * 0.5

      PathCurve {
        x: appWnd.width * 0.75
        y: appWnd.height * 0.25
      }
      PathCurve {
        x: appWnd.width * 0.75
        y: appWnd.height * 0.75
      }
      PathCurve {
        x: appWnd.width * 0.25
        y: appWnd.height * 0.25
      }
      PathCurve {
        x: appWnd.width * 0.25
        y: appWnd.height * 0.75
      }
      PathCurve {
        x: appWnd.width * 0.5
        y: appWnd.height * 0.5
      } // замыкаем петлю
    }
  }

  // ----- Qt provided non-visual children
  DataManager {
    id: dataManager
  }

  GamePreviewModel {
    id: picturesModel
  }

  ListModel {
    id: appSettingsModel
    ListElement {
      globalLightPosX: 0
      globalLightPosY: 0
      showFogParticles: true
      showShootingStarParticles: true
      showLighting: true
      showColors: true
      widthPortition: 0
      heightPortition: 0
      boardSize: 4
    }
  }
  // ----- Custom non-visual children

  // ----- JavaScript functions
  function moveToCenter() {
    appWnd.y = (screenAvailableHeight / 2) - (height / 2)
    appWnd.x = (screenAvailableWidth / 2) - (width / 2)
  }
}
