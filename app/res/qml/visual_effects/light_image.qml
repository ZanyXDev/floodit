import QtQuick 2.15
import QtGraphicalEffects 1.15

import common 1.0

Item {
  id: root
  property alias source: imageItem.source
  property alias nMapSource: imageNMap.source

  property real widthPortition
  // listView.widthPortition / imageItem.width
  property real heightPortition
  // listView.heightPortion / imageItem.height
  property real heightNorm
  // root.y / imageItem.height - listView.contentY / imageItem.height
  property real lightPosX
  //listView.globalLightPosX * widthPortition - widthNorm
  property real lightPosY
  //listView.globalLightPosY * heightPortition - heightNorm
  property bool showLighting
  property int idx

  signal clicked(int idx)

  Image {
    id: imageItem
    anchors {
      horizontalCenter: parent.horizontalCenter
      verticalCenter: parent.verticalCenter
    }
    smooth: true
    mipmap: true
  }
  // Normal-mapped cover shared among delegates
  ShaderEffectSource {
    id: coverNmapSource
    sourceItem: Image {
      id: imageNMap
    }
    hideSource: true
    visible: false
  }
  ShaderEffectSource {
    id: imageItemSource
    sourceItem: imageItem
    hideSource: root.showLighting
    visible: root.showLighting
  }

  ShaderEffect {
    id: shaderEffect
    anchors.fill: imageItem
    property variant src: imageItemSource
    property variant srcNmap: coverNmapSource

    property real widthPortition: root.widthPortition
    property real heightPortition: root.heightPortition
    property real widthNorm: root.widthPortition * 0.5 - 0.5
    property real heightNorm: root.heightNorm
    property real lightPosX: root.globalLightPosX
    property real lightPosY: root.globalLightPosY
    visible: root.showLighting

    fragmentShader: "
uniform sampler2D src;
uniform sampler2D srcNmap;
uniform lowp float qt_Opacity;
varying highp vec2 qt_TexCoord0;
uniform highp float lightPosX;
uniform highp float lightPosY;
void main() {
highp vec4 pix = texture2D(src, qt_TexCoord0.st);
highp vec4 pix2 = texture2D(srcNmap, qt_TexCoord0.st);
highp vec3 normal = normalize(pix2.rgb * 2.0 - 1.0);
highp vec3 light_pos = normalize(vec3(qt_TexCoord0.x - lightPosX, qt_TexCoord0.y - lightPosY, 0.8 ));
highp float diffuse = max(dot(normal, light_pos), 0.2);

// boost a bit
diffuse *= 2.5;

highp vec3 color = diffuse * pix.rgb;
gl_FragColor = vec4(color, pix.a) * qt_Opacity;
}
"
    Component.onCompleted: {
      if (isDebugMode) {

        console.log("------------ ShaderEffect ----------")
        console.log("parent.height:" + parent.height)
        console.log("parent.width:" + parent.width)

        for (var prop in shaderEffect) {
          print(prop += " (" + typeof (shaderEffect[prop]) + ") = " + shaderEffect[prop])
        }
      }
    }
  }
}
