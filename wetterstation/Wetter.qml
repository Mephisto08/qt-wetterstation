import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts



Page {
    id: root
    property int param1
    property int param2
    width: param1
    height: param2
    anchors.fill: parent
    header: Label {
        padding: 10
        text: qsTr("Wetter")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

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
        width: root.width*0.90;
        height: root.height*0.50;
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
        onClicked: root.StackView.view.push("Optionen.qml")
    }

    ScrollView {
        anchors.bottom: parent.bottom
        ScrollBar.horizontal.interactive: true
        id: scrollBar
        contentWidth: root.width*0.90
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
