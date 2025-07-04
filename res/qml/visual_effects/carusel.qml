import QtQuick 2.15
import QtQuick.Particles 2.15

import common 1.0

Item {
  id: root
  // Required properties should be at the top.
  // Set this to blur the mainView when showing something on top of it
  property real blurAmount: 0
  // Item lightImage used for lighting image
  property Item lightImage
  property alias showLighting: listView.showLighting
  property alias model: listView.model
  property alias lightMode: listView.lightMode

  // ----- Signal declarations
  // ----- Then comes the other properties. There's no predefined order to these.
  anchors.fill: parent
  // ----- Then attached properties and attached signal handlers.
  // ----- States and transitions.
  // ----- Signal handlers
  // Update blur shader source when width/height changes
  onHeightChanged: {
    root.scheduleUpdate()
  }
  onWidthChanged: {
    root.scheduleUpdate()
  }
  // ----- Visual children
  ListView {
    id: listView
    property bool showLighting: false
    property bool lightMode: true
    focus: true // важно!
    // Включаем обработку стрелок
    clip: true
    anchors.fill: parent
    spacing: -42
    highlightFollowsCurrentItem: true
    highlightRangeMode: ListView.StrictlyEnforceRange
    highlightMoveDuration: 400
    preferredHighlightBegin: root.height * 0.35
    preferredHighlightEnd: root.height * 0.35
    cacheBuffer: 4000

    delegate: DelegateItem {
      // Чтобы делегат мог получать фокус
      focus: true
      muted: true ///TODO extract to settings
      text: model.name
      source: "image://dynamic_image/" + listView.lightMode + "_true_" + model.source
      nMapSource: "image://dynamic_image/" + listView.lightMode + "_false_" + model.source
      // Обработка нажатия Enter или Space
      Keys.onPressed: {
        console.log("event.key :", event.key)
        if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return || event.key === Qt.Key_Space) {
          console.log("Selected item:", index)
        }
      }
    }
  }
  // ----- Custom non-visual children
  // ----- JavaScript functions


  /**
    * @note Updates the blur shader source, best called right before
    * adding blurAmount
    */
  function scheduleUpdate() {//mainContentSource.scheduleUpdate()
  }
}
