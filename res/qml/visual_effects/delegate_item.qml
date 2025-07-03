import QtQuick 2.15

Image {
  property bool isSelected: listView.currentIndex === index

  anchors.horizontalCenter: parent.horizontalCenter
  width: parent ? parent.width - 40 : imageItem.width
  z: isSelected ? 1000 : -index
  rotation: isSelected ? 0 : -15
  scale: isSelected ? 1. : 0.7
  opacity: 1.0 - Math.abs((listView.currentIndex - index) * 0.25)

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
}
