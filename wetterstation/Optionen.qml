import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: optionen
    anchors {
        fill: parent
    }

    Rectangle {
        id: optionenContent
        color: 'white'
        anchors {
            fill: parent
        }

        Label {
            id: optionenTitle
            text: qsTr("Optionen")
            anchors {
                horizontalCenter: parent.horizontalCenter
            }
            topPadding: 10
            bottomPadding: 20
            font.pixelSize: 30
        }

        TextField {
            id: inputCity
            anchors {
                top: optionenTitle.bottom
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width * 0.85
            placeholderText: qsTr("Standort")
            font.pixelSize: 15
            leftPadding: 10
            rightPadding: 10
            topPadding: 5
            bottomPadding: 5
        }

        ButtonGroup {
           id: checkGroup
           exclusive: false

           Component.onCompleted: {
               console.log("log completed")
           }
        }

        ListView {
            id: checkList
            anchors {
                top: inputCity.bottom
                left: optionenContent.left
                right: optionenContent.right
                bottom: optionenContent.bottom
                topMargin: 10
            }
            model: ["Datum", "Uhrzeit", "Standort", "Regenwahrscheinlichkeit", "Windgeschwindigkeit", "Luftfeuchte", "24-Stunden Vorschau"]
            delegate: SwitchDelegate {
                id: checkBoxes
                text: modelData
                checked: true
                ButtonGroup.group: checkGroup
                anchors {
                    left: parent.left
                    right: parent.right
                }
                font.pixelSize: 15
                padding: 10
                leftPadding: 20
                rightPadding: 20
            }
        }
    }
}
