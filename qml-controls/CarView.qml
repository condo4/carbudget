import QtQuick 2.4
import QtQuick.Controls 2.2

CarViewForm {
    property alias contextMenu: contextMenu

    Menu {
        id: contextMenu
        MenuItem {
            text: qsTr("Import Car")
            onClicked: {
                stackView.push("ImportHelp.qml")
            }
        }
        MenuItem {
            text: qsTr("Create new car")
            onClicked: {
                stackView.push("CarCreate.qml")
            }
        }
    }

}
