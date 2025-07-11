pragma Singleton

import QtQuick 2.15
import QtQuick.Window 2.15

QtObject {
  id: root
  readonly property real _scaling_factor: Math.min(1.8, Screen.devicePixelRatio)

  readonly property real extraLargeFontSize: _scaling_factor * 48
  readonly property real largeFontSize: _scaling_factor * 36
  readonly property real middleFontSize: _scaling_factor * 24
  readonly property real averageFontSize: _scaling_factor * 16
  readonly property real smallFontSize: _scaling_factor * 12

  /// TODO document better
  // This is for smartphone's and small screen, If they have a screen width less than this
  // we adjust the layout for that kind of display
  readonly property int min_width_threshold: 700

  property FontLoader astraFont: FontLoader {
    id: astraFont
    source: "qrc:/res/fonts/PT-Astra-Serif_Regular.ttf"
  }
  property FontLoader digitalFont: FontLoader {
    id: digitalFont
    source: "qrc:/res/fonts/681-font.otf"
  }

  /* This is msecs. Half of second is enough for smooth animation. */
  readonly property int timer16: 16
  readonly property int timer150: 150
  readonly property int timer200: 200
  readonly property int timer500: 500
  readonly property int timer1500: 1500
  readonly property int timer2000: 2000
  readonly property int timer5000: 5000

  function toLog(msg) {
    console.log(`${msg}`)
  }

  function toTagLog(tag, msg) {
    console.log(`[${tag}]:${msg}`)
  }

  /** Checks to see if a given width is considered "small" for the theme */
  function is_width_small(width) {
    return (width < root.min_width_threshold)
  }
}
