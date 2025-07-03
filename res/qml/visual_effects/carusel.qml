import QtQuick 2.15
import QtQuick.Particles 2.15
import QtGraphicalEffects 1.15
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

    anchors.fill: parent
    spacing: -60
    highlightFollowsCurrentItem: true
    highlightRangeMode: ListView.StrictlyEnforceRange
    highlightMoveDuration: 400
    preferredHighlightBegin: root.height * 0.5 - 140
    preferredHighlightEnd: root.height * 0.5 - 140
    cacheBuffer: 4000

    delegate: DelegateItem {
      source: "image://dynamic_image/" + listView.lightMode + "_true_" + model.source
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
