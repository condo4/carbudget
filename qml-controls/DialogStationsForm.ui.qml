import QtQuick 2.4

ListView {
    id: listView
    model: manager.car.stations

    delegate: Rectangle {
        width: parent.width

        Text {
            text: model.modelData.name
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
        }
    }
}
