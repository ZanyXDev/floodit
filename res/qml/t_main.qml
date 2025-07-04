import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
  width: 400
  height: 300
  visible: true

  Text {
    id: titleText
    text: "Старый текст"
    font.pixelSize: 24
    anchors.centerIn: parent

    Behavior on text {
      SequentialAnimation {
        ParallelAnimation {
          NumberAnimation {
            property: "opacity"
            to: 0
            duration: 1000
          }
          NumberAnimation {
            property: "scale"
            to: 0.5
            duration: 1000
          }
        }
        PropertyAction {
          property: "text"
        }
        ScriptAction {
          script: console.log("Текст изменён")
        }
        ParallelAnimation {
          NumberAnimation {
            property: "opacity"
            to: 1
            duration: 1000
          }
          NumberAnimation {
            property: "scale"
            to: 1
            duration: 1000
          }
        }
      }
    }
  }

  Button {
    text: "Изменить текст"
    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    onClicked: {
      titleText.text = "Новый текст!"
    }
  }
}
