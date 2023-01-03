import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material 2.4

ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Wetter-Station")
    Material.theme: Material.Light
    Material.accent: Material.Purple
    property string white: "#FFFFFF"
    property string blue: "#0A83E0"

    Drawer {
        id: drawer
        interactive: true
        edge: Qt.RightEdge
        height: mainWindow.height
        width: mainWindow.width * 0.45

        Optionen {}
        Button {
            id: drawerClose
            icon {
                source: "qrc:/images/right.png"
                width: 32.0
                height: 32.0
            }
            anchors {
                right: parent.right
                rightMargin: drawer.width
                verticalCenter: parent.verticalCenter
            }
            Material.background: blue
            Material.foreground: white
            onClicked: drawer.close()
        }
    }

    Wetter {}
    Button {
        id: drawerOpen
        icon {
            source: "qrc:/images/left.png"
            width: 32.0
            height: 32.0
        }
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        Material.background: blue
        Material.foreground: white
        onClicked: drawer.open()
    }
}
