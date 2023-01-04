import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts

Page {
    id: wetter
    objectName: "wetterPage"
    anchors.fill: parent
    property var wetterModel: WetterModel {}

    background: Image {
        source: "qrc:/images/background.jpeg"
    }

    Label {
        id: wetterTitle
        objectName: "wetterTitle"
        text: caller.getCity()
        anchors {
            bottom: seperator.top
            horizontalCenter: parent.horizontalCenter
        }
        font {
            pixelSize: 30
            family: "Verdana"
            weight: Font.ExtraBold
            capitalization: Font.AllUppercase
        }
    }

    Label {
        id: wetterDate
        objectName: "wetterDate"
        text: qsTr("2. Nov 2022")
        anchors {
            bottom: seperator.top
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }

    Label {
        id: wetterTime
        objectName: "wetterTime"
        text: qsTr("18:00 Uhr")
        anchors {
            bottom: seperator.top
            right: parent.right
            rightMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }

    Rectangle {
        id: seperator
        color: "black"
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            topMargin: 50
            leftMargin: 5
            rightMargin: 5
        }
        height: 3
    }

    Label {
        id: wetterTemperatur
        objectName: "wetterTemperatur"
        text: qsTr("wetterTemperatur")
        anchors {
            top: seperator.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterRegenwahrschienlichkeit
        objectName: "wetterRegenwahrschienlichkeit"
        text: qsTr("wetterRegenwahrschienlichkeit")
        anchors {
            top: wetterTemperatur.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterWindgeschindigkeit
        objectName: "wetterWindgeschindigkeit"
        text: qsTr("wetterWindgeschindigkeit")
        anchors {
            top: wetterRegenwahrschienlichkeit.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterRichtung
        objectName: "wetterRichtung"
        text: qsTr("wetterRichtung")
        anchors {
            top: wetterWindgeschindigkeit.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterLuftfeuchte
        objectName: "wetterLuftfeuchte"
        text: qsTr("wetterLuftfeuchte")
        anchors {
            top: wetterRichtung.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterCode
        objectName: "wetterCode"
        text: qsTr("wetterCode")
        anchors {
            top: wetterLuftfeuchte.bottom
            left: parent.left
            leftMargin: 10
            bottomMargin: 5
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }



    ScrollView {
        id: scrollBar
        ScrollBar.horizontal.interactive: true
        background: null
        contentHeight: 50
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: 5
            rightMargin: 5
        }

        Rectangle {
            color: "transparent"
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                top: parent.top
            }

            /*
            ListView {
                anchors.fill: parent
                model: WetterModel {}
                delegate: wetterDelegate
                orientation: Qt.Horizontal
                focus: true
            }
            */
        }
    }
}
