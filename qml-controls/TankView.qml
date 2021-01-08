import QtQuick 2.9
import QtQuick.Controls 2.2
import harbour.carbudget 1.0


TankViewForm {
    property alias contextMenu: contextMenu
    property int currentTank: -1

    Menu {
        id: contextMenu
        MenuItem {
            text: qsTr("New Tank")
            onClicked: {
                stackView.push("TankEntry.qml")
            }
        }
    }

    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            stackView.push("TankEntry.qml")
        }
    }
}
