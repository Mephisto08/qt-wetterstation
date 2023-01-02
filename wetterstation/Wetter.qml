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




    ColumnLayout{
        id: outer_column
        Layout.alignment: Qt.AlignCenter
        spacing: 20
        width: root.width
        Pane {
            id: loc_info
            width: outer_column.width
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
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

            RowLayout{
                id: raws
                width: outer_column.width
                Layout.alignment: Qt.AlignTop
                spacing: 10
                Layout.fillWidth: true

                Rectangle {
                    id: rect
                    width: raws.width*0.90;
                    height: 200
                    Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter
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
                        anchors.fill: parent
                        model: WetterModel {}
                        delegate: wetterDelegate
                        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                        focus: true
                    }
                }

                Button {
                    id: options_button
                    Layout.alignment: Qt.AlignRight | Qt.AlignHCenter
                    Layout.leftMargin: root.width*0.02
                    icon.source: "qrc:/images/optionen.png"
                    icon.width: 32.0
                    icon.height: 32.0
                    onClicked: root.StackView.view.push("Optionen.qml")
                }


        }

            ScrollView {
                Layout.alignment: Qt.AlignCenter | Qt.AlignBottom
                ScrollBar.horizontal.interactive: true
                //anchors.fill: parent
                id: scrollBar
//                property bool showScrollBar: position < maximum - pageSize
//                visible: showScrollBar
                contentWidth: root.width
                contentHeight: 50

                Rectangle {
                    width: root.width; height: 40
                    Layout.alignment: Qt.AlignLeft | Qt.AlignHCenter


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
}
