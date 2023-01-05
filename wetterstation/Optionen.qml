import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: optionen
    anchors {
        fill: parent
    }

    property string inputState: caller.getCity()
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
            name: "Temperatur",
            checked: true
        },
        {
            name: "Wetter Icon",
            checked: true
        },
        {
            name: "Wetter Code",
            checked: true
        },
        {
            name: "Nierderschlag",
            checked: true
        },
        {
            name: "Windgeschwindigkeit",
            checked: true
        },
        {
            name: "Windrichtung",
            checked: true
        },
        {
            name: "Bewölkung",
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
            font {
                pixelSize: 30
                family: "Verdana"
                weight: Font.DemiBold
            }
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
                topMargin: 10
                top: inputCity.bottom
                left: optionenContent.left
                right: optionenContent.right
                bottom: optionenContent.bottom
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
                padding: 5
                leftPadding: 20
                rightPadding: 20
                onClicked: {
                    //optionenTitle.visible = !optionenTitle.visible
                    model.checked = checked
                    caller.updateVisibility(model.name, model.checked)

                }
            }
        }

        TextField {
            id: inputCity
            objectName: "inputCity"
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
    }
}
