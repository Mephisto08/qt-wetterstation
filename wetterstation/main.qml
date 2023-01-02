import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.4

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    StackView {
           id: stackView
           anchors.fill: parent
           initialItem:  Wetter {}
       }


}
