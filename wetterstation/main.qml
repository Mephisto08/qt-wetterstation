import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: main_window
    width: 0
    height: 0
    visible: true
    title: qsTr("Hello World")

    StackView {
           id: stackView
           width: main_window.width
           height: main_window.height
           anchors.fill: parent
           property int param1: main_window.width
           property int param2: main_window.height
           initialItem:  Wetter { param1: stackView.param1
               param2: stackView.param2}

       }


}
