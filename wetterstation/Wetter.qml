import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Layouts



Page {
    id: root
    anchors.fill: parent
    header: Label {
        padding: 10
        text: qsTr("Wetter")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }




    ColumnLayout{
        Layout.alignment: Qt.AlignCenter
        spacing: 20
        Pane {
            id: loc_info
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
                Layout.alignment: Qt.AlignTop
                spacing: 20
                Layout.fillWidth: true

                Rectangle {
                    width: 540; height: 200
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
                    Layout.leftMargin: 40
                    icon.source: "qrc:/images/optionen.png"
                    icon.width: 32.0
                    icon.height: 32.0
                    onClicked: root.StackView.view.push("Optionen.qml")
                }


        }

    }
}
