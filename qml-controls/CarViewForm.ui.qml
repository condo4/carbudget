import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    title: qsTr("Car List")
    property int numCars: manager.cars.length

    width: 400
    height: 400

    Flickable {
        id: welcomeFlickable
        enabled: numCars == 0
        visible: numCars == 0
        anchors.fill: parent

        Label {
            id: welcomeTextA
            anchors.top: parent.top
            width: parent.width
            height: parent.height / 2
            text: qsTr("Welcome to CarBudget!")
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Label {
            id: welcomeTextB
            anchors.top: welcomeTextA.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height / 2
            text: qsTr("Please create a new car or import data from another application using settings menu.")
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }
    }

    ListView {
        id: carView
        enabled: numCars > 0
        visible: numCars > 0
        anchors.fill: parent
        model: manager.cars

        delegate: Item {
            id: carItem
            width: parent.width
            /*
            onClicked: {
                manager.selectCar(model.modelData)
                pageStack.replace(Qt.resolvedUrl("CarEntry.qml"))
            }
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Backup")
                    onClicked: function () {
                        var successful = manager.backupCar(model.modelData)
                        pageStack.push(Qt.resolvedUrl(
                                           "BackupNotification.qml"), {
                                           backupOK: successful
                                       })
                    }
                }
                MenuItem {
                    text: qsTr("Remove")
                    onClicked: {
                        Remorse.itemAction(carItem, "Deleting", function () {
                            manager.delCar(model.modelData)
                        })
                    }
                }
            }*/
            Row {
                anchors.fill: parent
                spacing: Theme.paddingMedium
                Rectangle {
                    height: parent.height
                    width: height

                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        source: "image://theme/icon-m-car"
                    }
                }
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    text: model.modelData
                }
            }
        }
    }
}
