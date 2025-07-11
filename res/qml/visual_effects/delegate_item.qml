import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtGraphicalEffects 1.15

import common 1.0

Rectangle {
  id: root
  property bool showLighting
  property bool isSelected: listView.currentIndex === index

  property alias muted: playClick.muted
  property alias text: titleText.text
  property alias source: imageItem.source
  property alias nMapSource: imageItem.nMapSource

  opacity: 1.0 - Math.abs((listView.currentIndex - index) * 0.25)

  height: 264
  width: 224

  anchors.horizontalCenter: parent.horizontalCenter

  onIsSelectedChanged: {
    isSelected ? playClick.play() : playClick.stop()
  }

  z: isSelected ? 1000 : -index
  rotation: isSelected ? 0 : -15
  scale: isSelected ? 1. : 0.7

  Behavior on rotation {
    NumberAnimation {
      duration: 500
      easing.type: Easing.OutBack
    }
  }
  Behavior on scale {
    NumberAnimation {
      duration: 1500
      easing.type: Easing.OutElastic
    }
  }
  Behavior on opacity {
    NumberAnimation {
      duration: 200
      easing.type: Easing.OutQuad
    }
  }

  ColumnLayout {
    visible: true
    id: delegeteColumnLayout
    spacing: 2
    anchors {
      margins: 2
      fill: parent
    }
    Text {
      id: titleText
      Layout.preferredWidth: parent.width * 0.75
      Layout.preferredHeight: 32
      Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

      wrapMode: Text.NoWrap
      horizontalAlignment: Text.AlignHCente
      verticalAlignment: Text.AlignVCenter

      font {
        family: AppSingleton.astraFont.name
        pointSize: AppSingleton.smallFontSize
        bold: isSelected
      }
      color: "#ffffff"
      style: Text.Outline
      styleColor: "#b0a030"
    }

    LightImage {
      id: imageItem
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      showLighting: root.showLighting
    }
  }
  // ----- no visual items
  SoundEffect {
    id: playClick
    source: "qrc:/res/sounds/click.wav"
  }
}
