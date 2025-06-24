import QtQuick 2.15

// Размерность: 8x8, 12x12, 16x16, 20x20 и 24x24.
// Для каждой размерности поля есть три режима: 4 цвета, 6 цветов и 8 цветов. Итого 15 режимов.
/// TOOD Моедль перенести в срр код добавить сохранение рекорда
ListModel {
  id: gamePreviewModel
  ListElement {
    name: qsTr("Board size 8x8. 4 colors")
    nmap: "8x8x4.nmap"
    image: "8x8x4.picture"
    overview: "Very simple game. Board size 8x8 cells, only 4 colors."
  }
  ListElement {
    name: qsTr("Board size 8x8. 6 colors")
    nmap: "8x8x6.nmap"
    image: "8x8x6.picture"
    overview: "Very simple game. Board size 8x8 cells, only 6 colors."
  }
  ListElement {
    name: qsTr("Board size 8x8. 8 colors")
    nmap: "8x8x8.nmap"
    image: "8x8x8.picture"
    overview: "Very simple game. Board size 8x8 cells, only 8 colors."
  }

  ListElement {
    name: qsTr("Board size 12x12. 4 colors")
    nmap: "12x12x4.nmap"
    image: "12x12x4.picture"
    overview: "Simple game. Board size 12x12 cells, only 4 colors."
  }
  ListElement {
    name: qsTr("Board size 12x12 6 colors")
    nmap: "12x12x6.nmap"
    image: "12x12x6.picture"
    overview: "Simple game. Board size 12x12 cells, only 6 colors."
  }
  ListElement {
    name: qsTr("Board size 12x12. 8 colors")
    nmap: "12x12x8.nmap"
    image: "12x12x8.picture"
    overview: "Simple game. Board size 12x12 cells, only 8 colors."
  }

  ListElement {
    name: qsTr("Board size 16x16. 4 colors")
    nmap: "16x16x4.nmap"
    image: "16x16x4.picture"
    overview: "Normal game. Board size 16x16 cells, only 4 colors."
  }
  ListElement {
    name: qsTr("Board size 16x16 6 colors")
    nmap: "16x16x6.nmap"
    image: "16x16x6.picture"
    overview: "Normal game. Board size 16x16 cells, only 6 colors."
  }
  ListElement {
    name: qsTr("Board size 16x16. 8 colors")
    nmap: "16x16x8.nmap"
    image: "16x16x8.picture"
    overview: "Normal game. Board size 16x16 cells, only 8 colors."
  }

  ListElement {
    name: qsTr("Board size 20x20. 4 colors")
    nmap: "20x20x4.nmap"
    image: "20x20x4.picture"
    overview: "Hard game. Board size 20x20 cells, only 4 colors."
  }
  ListElement {
    name: qsTr("Board size 20x20 6 colors")
    nmap: "20x20x6.nmap"
    image: "20x20x6.picture"
    overview: "Hard game. Board size 20x20 cells, only 6 colors."
  }
  ListElement {
    name: qsTr("Board size 20x20. 8 colors")
    nmap: "20x20x8.nmap"
    image: "20x20x8.picture"
    overview: "Hard game. Board size 20x20 cells, only 8 colors."
  }
}
