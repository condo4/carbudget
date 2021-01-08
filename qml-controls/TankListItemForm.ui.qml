import QtQuick 2.4
import QtQuick.Layouts 1.1

Rectangle {
    id: rectangle
    width: 400
    height: 50
    color: "#c9c9c9"
    radius: 0
    border.color: "#ffffff"
    border.width: 2

    property alias distance: distance
    property alias date: date
    property alias unitPrice: unitPrice
    property alias quantity: quantity
    property alias price: price
    property alias consumtion: consumtion
    property alias zone: zone

    MouseArea {
        id: zone
        anchors.fill: parent
    }

    Text {
        id: distance
        text: "127344Km (+897Km)"
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    Text {
        id: date
        text: "2018/01/17"
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    Rectangle {
        id: rectangle1
        color: "transparent"
        anchors.top: parent.verticalCenter
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5

        Text {
            id: unitPrice
            width: parent.width / 4
            text: "1.40 €/l"
        }

        Text {
            id: quantity
            width: parent.width / 4
            text: "47.96l"
            anchors.left: unitPrice.right
            anchors.leftMargin: 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Text {
            id: price
            width: parent.width / 4
            text: "65.01€"
            anchors.left: quantity.right
            anchors.leftMargin: 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        Rectangle {
            id: rectangle2
            anchors.right: parent.right
            width: consumtion.width + 6
            height: consumtion.height + 6
            color: "#000000"
            radius: 7
            Text {
                id: consumtion
                text: "5.35l/100km"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "red"
                Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            }
        }
    }
}
