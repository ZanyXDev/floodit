import QtQuick 2.15
import common 1.0

Text {
  id: root

  verticalAlignment: Text.AlignVCenter
  horizontalAlignment: Text.AlignRight

  font {
    pointSize: AppSingleton.smallFontSize
    family: AppSingleton.digitalFont.name
  }
}
