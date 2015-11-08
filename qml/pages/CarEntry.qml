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
    allowedOrientations: Orientation.All
    property string distanceunit
    property real distanceunitfactor

    Component.onCompleted: {
        distanceunit = manager.car.distanceunity
        if(distanceunit == "km")
        {
            distanceunitfactor = 1
        }
        else if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
    }

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
                title: manager.car.name
            }

            Label {
                x: Theme.paddingLarge
                text: qsTr("Distance: %L1 ~ %L2 %3").arg(manager.car.mindistance/distanceunitfactor).arg(manager.car.maxdistance/distanceunitfactor).arg(manager.car.distanceunity)
                font.pixelSize: Theme.fontSizeSmall
            }

            Label {
                x: Theme.paddingLarge
                text: qsTr("Consumption: %L1 l/100%2").arg(manager.car.consumption.toFixed(2)).arg(manager.car.distanceunity)
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Last: %L1 l/100%2").arg(manager.car.consumptionlast.toFixed(2)).arg(manager.car.distanceunity)
                font.pixelSize: Theme.fontSizeSmall
                color: {
                    if(manager.car.consumptionlast < manager.car.consumption * 0.92) return "#00FF00"
                    if(manager.car.consumptionlast < manager.car.consumption * 0.94) return "#40FF00"
                    if(manager.car.consumptionlast < manager.car.consumption * 0.96) return "#80FF00"
                    if(manager.car.consumptionlast < manager.car.consumption * 0.98) return "#C0FF00"
                    if(manager.car.consumptionlast < manager.car.consumption * 1.00) return "#FFFF00"
                    if(manager.car.consumptionlast < manager.car.consumption * 1.02) return "#FFC000"
                    if(manager.car.consumptionlast < manager.car.consumption * 1.04) return "#FF8000"
                    if(manager.car.consumptionlast < manager.car.consumption * 1.06) return "#FF4000"
                    if(manager.car.consumptionlast < manager.car.consumption * 1.08) return "#FF2000"
                    return "#FF0000"
                }            }
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
                    text: (manager.car.tireMounted < manager.car.nbtire)?(qsTr("Tires mounted: %1/%2").arg(manager.car.tireMounted).arg(manager.car.nbtire)):(qsTr("Tires mounted"))
                    color: (manager.car.tireMounted < manager.car.nbtire)?(Theme.highlightColor):(Theme.primaryColor)
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
