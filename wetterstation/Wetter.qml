import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Page {
    id: wetter
    anchors.fill: parent

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
