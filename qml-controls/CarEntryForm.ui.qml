import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    id: carEntryPage
    property string distanceunit
    property real distanceunitfactor: 1
    property real consumptionfactor: 1.0
    property int paddingLarge: 20
    property int paddingMedium: 10
    property real widthRatio: 1.5
    property alias distancetext: txtdist.text
    property alias constext: txtcons.text
    property alias lasttext: txtlast.text
    property alias lastcolor: txtlast.color

    property alias btnTank: btnTank
    property alias btnCost: btnCost
    property alias btnTire: btnTire
    property alias btnStats: btnStats

    Flickable {
        id: flickable
        anchors.fill: parent
        Rectangle {
            id: textColumn
            width: parent.width
            height: 75

            Label {
                id: txtdist
                x: paddingLarge
                height: 20
                text: "Distance: 100 ~ 2000 km"
                anchors.top: parent.top
                anchors.topMargin: 4
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                id: txtcons
                x: paddingLarge
                text: "Consumption: 5.3 l/100km"
                anchors.top: txtdist.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                id: txtlast
                x: paddingLarge
                text: "Last: 4.21 l/100km"
                anchors.top: txtcons.bottom
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter
                color: "green"
            }
        }

        Rectangle {
            id: rectangle
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: textColumn.bottom
            anchors.topMargin: 0

            Rectangle {
                border.color: "black"
                border.width: 5
                radius: 10
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 5
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/picture/Pump.png"
                    width: 100
                    height: 100
                }

                MouseArea {
                    id: btnTank
                    anchors.fill: parent
                }
            }

            Rectangle {
                border.color: "black"
                border.width: 5
                radius: 10
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 5
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.right: parent.right
                anchors.rightMargin: 5

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/picture/Wrench.png"
                    width: 100
                    height: 100
                }

                MouseArea {
                    id: btnCost
                    anchors.fill: parent
                }
            }

            Rectangle {
                border.color: "black"
                border.width: 5
                radius: 10
                anchors.top: parent.verticalCenter
                anchors.topMargin: 5
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5

                Image {
                    anchors.centerIn: parent
                    id: icon
                    source: "qrc:/picture/Wheel.png"
                    width: 90
                    height: 90
                }

                MouseArea {
                    id: btnTire
                    anchors.fill: parent
                }
            }

            Rectangle {
                border.color: "black"
                border.width: 5
                radius: 10
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.top: parent.verticalCenter
                anchors.topMargin: 5

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/picture/Dollar.png"
                    width: 90
                    height: 90
                }

                MouseArea {
                    id: btnStats
                    anchors.rightMargin: 210
                }
            }
        }
    }
}
