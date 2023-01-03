import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: optionen
    anchors {
        fill: parent
    }

    property string inputState: "Darmstadt"
    property var optionStates: [
        {
            name: "Datum",
            checked: true
        },
        {
            name: "Uhrzeit",
            checked: true
        },
        {
            name: "Standort",
            checked: true
        },
        {
            name: "Regenwahrscheinlichkeit",
            checked: true
        },
        {
            name: "Windgeschwindigkeit",
            checked: true
        },
        {
            name: "Luftfeuchte",
            checked: true
        },
        {
            name: "24-Stunden Vorschau",
            checked: true
        }
    ]


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
            text: inputState
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
            onTextChanged: inputState = text
        }

        ListModel{
            id: checkModel
            dynamicRoles: true
            Component.onCompleted: {
                for(const option of optionStates) {
                    checkModel.append({"name": option.name, "checked": option.checked})
                }
            }
        }

        ListView {
            id: checkList
            model: checkModel
            anchors {
                top: inputCity.bottom
                left: optionenContent.left
                right: optionenContent.right
                bottom: optionenContent.bottom
                topMargin: 10
            }

            delegate: SwitchDelegate {
                id: modelCheckBoxes
                checked: model.checked
                text: model.name
                anchors {
                    left: parent.left
                    right: parent.right
                }
                font.pixelSize: 15
                padding: 10
                leftPadding: 20
                rightPadding: 20
                onClicked: {
                    optionenTitle.visible = !optionenTitle.visible
                    model.checked = checked
                }
            }
        }

        Button {
            id: test
            text: "Read all"
            anchors {
                right: parent.right
                bottom: parent.bottom
            }
            onClicked: {
                console.log()
                console.log("Standort => " + inputState)
                for(const index in optionStates) {
                    console.log(checkModel.get(index).name + " => " + checkModel.get(index).checked)
                }
            }
        }
    }
}
