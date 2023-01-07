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
        objectName: "pageBackground"
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

    // Mittig
    Image {
        id: wetterIcon
        objectName: "wetterIcon"
        source: "qrc:/images/wetterIconExample.png"
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.horizontalCenter
            rightMargin: 50
        }
    }
    Label {
        id: wetterTemperatur
        objectName: "wetterTemperatur"
        text: qsTr("Temp. °C")
        anchors {
            verticalCenter: parent.verticalCenter
            left: wetterIcon.right
            leftMargin: 50
        }
        font {
            pixelSize: 30
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterCode
        objectName: "wetterCode"
        text: qsTr("wetterCode")
        anchors {
            top: wetterIcon.bottom
            topMargin: 0
            horizontalCenter: parent.horizontalCenter
        }
        font {
            pixelSize: 30
            family: "Verdana"
            weight: Font.DemiBold
        }
    }

    // Links oben
    /*
    Label {
        id: wetterRegenwahrschienlichkeit
        objectName: "wetterRegenwahrschienlichkeit"
        text: qsTr("Regenwahrsch. %")
        anchors {
            top: parent.top
            left: parent.left
            topMargin: parent.height * 0.20
            leftMargin: parent.width * 0.10
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    */
    Label {
        id: wetterRegenInMM
        objectName: "wetterRegenInMM"
        text: qsTr("wetterRegenInMM. mm")
        anchors {
            top: parent.top
            left: parent.left
            topMargin: parent.height * 0.20
            leftMargin: parent.width * 0.10
            /* Wenn wetterRegenwahrschienlichkeit einkommetiert ist
            top: wetterRegenwahrschienlichkeit.bottom
            left: parent.left
            topMargin: 7
            leftMargin: parent.width * 0.10
            */
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }

    // Rechts oben
    Label {
        id: wetterWindgeschindigkeit
        objectName: "wetterWindgeschindigkeit"
        text: qsTr("Windgeschwind. km/h")
        anchors {
            top: parent.top
            right: parent.right
            topMargin: parent.height * 0.20
            rightMargin: parent.width * 0.10
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }
    Label {
        id: wetterWindRichtung
        objectName: "wetterWindRichtung"
        text: qsTr("Windrichtung")
        anchors {
            top: wetterWindgeschindigkeit.bottom
            right: parent.right
            topMargin: 10
            rightMargin: parent.width * 0.10
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }

    // Links unten
    Label {
        id: wetterWolkendichte
        objectName: "wetterWolkendichte"
        text: qsTr("wetterWolkendichte %")
        anchors {
            top: parent.top
            left: parent.left
            topMargin: parent.height * 0.70
            leftMargin: parent.width * 0.10
        }
        font {
            pixelSize: 20
            family: "Verdana"
            weight: Font.DemiBold
        }
    }


    // Rechts unten
    // ...


    // Unten
    ScrollView {
        id: scrollBar
        objectName: "scrollBar"
        ScrollBar.horizontal.interactive: true
        background: null
        contentHeight: 50
        contentWidth: contentChildren
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            leftMargin: 5
            rightMargin: 5
            bottomMargin: 5
        }

        GridLayout{
            columns: 24
            columnSpacing: 0
            anchors.fill: parent
            Repeater {
                model: 24
                delegate: Item {
                    width: 90
                    height: 50
                    Rectangle{
                        width: 90
                        height: 50
                        color: "transparent"
                        border.color: "black"
                        Column {
                            Text { text: "       " + caller.time24hours[index] + "Uhr" }
                            Text { text: "          " +caller.temp24hours[index]+ "°C" }
                        }
                    }
                }
            }
        }
    }
}
