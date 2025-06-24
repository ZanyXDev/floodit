import QtQuick 2.15

Item {
  id: root


  /**
    * @brief Для использования в качестве источника света lightImage
             Каждому классу 8x8,12x12,16x16,20x20 изображений
             сторится своя карта нормалей
  */
  //property real globalLightPosX: lightImage.x / root.width
  //property real globalLightPosY: lightImage.y / root.height
  ShaderEffectSource {
    id: coverNmapSource
    sourceItem: Image {
      // "dynamic" — это ID, зарегистрированный в addImageProvider
      source: "image://dynamic/cover_nmap_" + model.nmap
    }
    hideSource: true
    visible: false
  }
}
