import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: root

    property string inConversationWith

    header: Label {
        padding: 10
        text: qsTr("Optionen")
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

        Button {
            icon.source: "qrc:/images/zurueck.png"
            icon.width: 32.0
            icon.height: 32.0
            onClicked: root.StackView.view.pop()
        }

}
