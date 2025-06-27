import QtQuick 2.15

Item {
  id: root
  // Required properties should be at the top.
  // Set this to blur the mainView when showing something on top of it
  property real blurAmount: 0
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
  // ----- Custom non-visual children
  // ----- JavaScript functions

  /**
    * @note Updates the blur shader source, best called right before
    * adding blurAmount
    */
  function scheduleUpdate() {
    mainContentSource.scheduleUpdate()
  }
}

/**

  ListView {
    id: listView
    anchors.fill: parent
    spacing: -60
    model: picturesModel
    highlightFollowsCurrentItem: true
    highlightRangeMode: ListView.StrictlyEnforceRange
    highlightMoveDuration: 400
    preferredHighlightBegin: appWnd.height * 0.5 - 140
    preferredHighlightEnd: appWnd.height * 0.5 - 140
    cacheBuffer: 4000
    delegate: Image {
      source: "image://dynamic_image/" + dataManager.lightMode + "/true/" + model.image
    }
    Component.onCompleted: {
      AppSingleton.toLog(`picturesModel.count ${picturesModel.count}`)
    }
  }
  */
