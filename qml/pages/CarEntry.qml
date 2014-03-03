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


import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: carEntry

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Select another car")
                onClicked: pageStack.push(Qt.resolvedUrl("CarView.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
            }

            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("About.qml"))
            }
        }

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: carEntry.width
            spacing: Theme.paddingLarge
            PageHeader {
                height: 10
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Distance: %L1 km").arg(manager.car.distance)
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Consumption: %L1 l/100km").arg(manager.car.consumption.toFixed(2))
                font.pixelSize: Theme.fontSizeSmall
            }


            Row {
                spacing: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                width: 500

                Rectangle {
                    border.color : "black"
                    border.width : 5
                    width: 110
                    height: 110
                    radius: 10

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/Pump.png"
                        width: 100
                        height: 100
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("TankView.qml"))
                    }
                }

                Button {
                    text: qsTr("New Tank")
                    onClicked: pageStack.push(Qt.resolvedUrl("TankEntry.qml"))
                }
            }

            Row {
                spacing: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                width: 500

                Rectangle {
                    border.color : "black"
                    border.width : 5
                    width: 110
                    height: 110
                    radius: 10

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/Wrench.png"
                        width: 100
                        height: 100
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("CostView.qml"))
                    }
                }

                Button {
                    text: qsTr("New Cost")
                    onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"))
                }
            }

            Row {
                spacing: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                width: 500

                Rectangle {
                    border.color : "black"
                    border.width : 5
                    width: 110
                    height: 110
                    radius: 10

                    Image {
                        anchors.centerIn: parent
                        id: icon
                        source: "qrc:/Wheel.png"
                        width: 90
                        height: 90
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("TireView.qml"))
                    }
                }

                Button {
                    text: qsTr("Tire mounted: %1").arg(manager.car.tireMounted)
                    onClicked: pageStack.push(Qt.resolvedUrl("TireView.qml"))
                }
            }
            Row {
                spacing: Theme.paddingLarge
                anchors.horizontalCenter: parent.horizontalCenter
                width: 500

                Rectangle {
                    border.color : "black"
                    border.width : 5
                    width: 110
                    height: 110
                    radius: 10

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/Dollar.png"
                        width: 90
                        height: 90
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("BudgetView.qml"))
                    }
                }

                Button {
                    text: qsTr("Budget")
                    onClicked: pageStack.push(Qt.resolvedUrl("BudgetView.qml"))
                }
            }
        }
    }
}
