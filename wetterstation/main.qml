import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Controls.Material 2.4

ApplicationWindow {
    id: mainWindow
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    maximumWidth: 640
    maximumHeight: 480
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
        width: 275
        onClosed: {
            caller.setEditingOptions(false)
            caller.setCity(optionen.inputState)
            caller.triggerUpdate()
        }

        Optionen {
            id: optionen
        }

        Button {
            id: drawerClose
            font.pixelSize: 30
            width: 22
            text: '❯'
            anchors {
                right: parent.right
                rightMargin: drawer.width
                verticalCenter: parent.verticalCenter
            }
            Material.background: blue
            Material.foreground: white
            onClicked: {
                drawer.close()
            }
        }
    }

    Wetter {
        id: wetter
    }

    Button {
        id: drawerOpen
        objectName: "drawerOpen"
        font.pixelSize: 30
        width: 22
        text: '❮'
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        Material.background: blue
        Material.foreground: white
        onClicked: {
            caller.setEditingOptions(true)
            drawer.open()
        }
    }
}
