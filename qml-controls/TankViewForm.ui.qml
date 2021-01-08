import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    title: qsTr("Tank List")

    Flickable {
        anchors.fill: parent

        ListView {
            id: tanklistView
            spacing: 4
            anchors.fill: parent
            clip: true
            model: manager.car.tanks

            delegate: TankListItem {
                width: parent.width
                radius: 5
                tank: model.modelData
            }
        }
    }
}
