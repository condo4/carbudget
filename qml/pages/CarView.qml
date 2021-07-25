/**
 * CarBudget, Sailfish application to manage car cost
 *
 * Copyright (C) 2014 Fabien Proriol
 *
 * This file is part of CarBudget.
 *
 * CarBudget is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * CarBudget is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with CarBudget. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Fabien Proriol
 */


import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0


Page {
    allowedOrientations: Orientation.All

    property int numCars: manager.cars.length

    SilicaFlickable {
        id: welcomeFlickable
        enabled: numCars == 0
        visible: numCars == 0
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Import Car")
                onClicked: pageStack.push(Qt.resolvedUrl("ImportHelp.qml"))
            }
            MenuItem {
                text: qsTr("Create new car")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"), { newCarMode: true })
            }
        }
        Label {
            id: welcomeTextA
            anchors.top: parent.top
            width: parent.width - Theme.paddingLarge*2
            height: parent.height / 2
            x: Theme.paddingLarge
            font.pixelSize: Theme.fontSizeHuge
            color: Theme.highlightColor
            text: qsTr("Welcome to CarBudget!")
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Label {
            id: welcomeTextB
            anchors.top: welcomeTextA.bottom
            anchors.bottom: parent.bottom
            width: parent.width - Theme.paddingLarge*2
            height: parent.height / 2
            x: Theme.paddingLarge
            font.pixelSize: Theme.fontSizeLarge
            color: Theme.highlightColor
            text: qsTr("Please create a new car or import data from another application using the pulley menu.")
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignTop
        }
    }

    SilicaListView {
        id: carView
        enabled: numCars > 0
        visible: numCars > 0
        anchors.fill: parent
        model: manager.cars

        PullDownMenu {
            MenuItem {
                text: qsTr("Import Car")
                onClicked: pageStack.push(Qt.resolvedUrl("ImportHelp.qml"))
            }
            MenuItem {
                text: qsTr("Create new car")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"), { newCarMode: true })
            }
        }

        //VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Car List")
        }



        delegate: ListItem {
            id: carItem
            width: parent.width
            showMenuOnPressAndHold: true

            onClicked: function() {
                manager.selectCar(model.modelData)
                pageStack.replace(Qt.resolvedUrl("CarEntry.qml"))
            }
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Backup")
                    onClicked: function() {
                        var successful = manager.backupCar(model.modelData)
                        pageStack.push(Qt.resolvedUrl("BackupNotification.qml"), {backupOK: successful})
                    }
                }
                MenuItem {
                    text: qsTr("Remove")
                    onClicked: {
                        Remorse.itemAction(carItem, "Deleting", function() {
                            manager.delCar(model.modelData)
                        })
                    }
                }
            }
            Row {
                anchors.fill: parent
                spacing: Theme.paddingMedium
                Rectangle {
                    height: parent.height
                    width: height
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Theme.rgba(Theme.primaryColor, 0.1) }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        source: "image://theme/icon-m-car"
                    }
                }
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    text : model.modelData
                }
            }
        }
    }
}
