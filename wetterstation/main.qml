import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

ApplicationWindow {
    id: main_window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

//    StackView {
//           id: stackView
//           width: main_window.width
//           height: main_window.height
//           anchors.fill: parent
//           property int param1: main_window.width
//           property int param2: main_window.height
//           initialItem:  Wetter { param1: stackView.param1
//               param2: stackView.param2}

//       }

    // Drawer
    Drawer {
            id: drawer
            width: 0.66 * window.width
            height: window.height
            interactive: false
            edge: Qt.RightEdge

            Label {
                    id: content

                    text: "Aa"
                    font.pixelSize: 96
                    anchors.fill: parent
                    verticalAlignment: Label.AlignVCenter
                    horizontalAlignment: Label.AlignHCenter

                    transform: Translate {
                        x: drawer.position * content.width * 0.33
                    }
}
                Button {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    icon.source: "qrc:/images/zurueck.png"
                    icon.width: 32.0
                    icon.height: 32.0
                    onClicked: drawer.close()
                }

        }

    // First Page

    Pane {
            id: loc_info
            anchors.right: parent.right
            RowLayout {
                id: info
                Text{
                    text: "Darmstadt" }
                ToolSeparator {

                }
                Text{
                    text: "02.01.2023" }
            }
        }


    Rectangle {
        id: rect
        anchors.verticalCenter: parent.verticalCenter
        width: main_window.width*0.90;
        height: main_window.height*0.50;
        Component {
            id: wetterDelegate
            Item {
                width: 180; height: 40
                Column {
                    Text { text: '<b>Name:</b> ' + name }
                    Text { text: '<b>Number:</b> ' + number }
                }
            }
        }

        ListView {
            anchors.fill: rect
            model: WetterModel {}
            delegate: wetterDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
            focus: true
        }
    }

    Button {
        id: options_button
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/images/optionen.png"
        icon.width: 32.0
        icon.height: 32.0
        onClicked: drawer.open()
    }

    ScrollView {
        anchors.bottom: parent.bottom
        ScrollBar.horizontal.interactive: true
        id: scrollBar
        contentWidth: main_window.width*0.90
        contentHeight: 50

        Rectangle {
            width: scrollBar.width
            height: 40

            ListView {
                anchors.fill: parent
                model: WetterModel {}
                delegate: wetterDelegate

                orientation: Qt.Horizontal
                focus: true

            }
        }
    }






}
