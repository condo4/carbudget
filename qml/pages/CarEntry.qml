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
    id: carEntryPage
    allowedOrientations: Orientation.All
    property string distanceunit
    property real distanceunitfactor: 1
    property real consumptionfactor : 1.0

    Component.onCompleted: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
        if(manager.car.consumptionUnit === "mpg")
        {
            consumptionfactor  = 4.546*100/1.609
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // Tell SilicaFlickable the height of its content.
        contentHeight: textColumn.height + flowElement.height

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

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: textColumn
            width: parent.width
            spacing: Theme.paddingMedium
            PageHeader {
                id: header
                title: {
                    if(manager.car.make.length > 1)
                        return (manager.car.make + " " + manager.car.model)
                    else
                        return manager.car.name
                }
            }

            Label {
                x: Theme.paddingLarge
                text: qsTr("Distance: %L1 ~ %L2 %3").arg((manager.car.minDistance/distanceunitfactor).toFixed(0)).arg((manager.car.maxDistance/distanceunitfactor).toFixed(0)).arg(manager.car.distanceUnit)
            }

            Label {
                x: Theme.paddingLarge
                text:
                    if ( manager.car.consumptionUnit === "l/100km" ) {
                        qsTr("Consumption: %L1 l/100km").arg(manager.car.consumption.toFixed(2))
                    }
                    else if ( manager.car.consumptionUnit === "mpg" ) {
                        qsTr("Consumption: %L1 mpg").arg((consumptionfactor * 1/manager.car.consumption).toFixed(2))
                    }
            }
            Label {
                x: Theme.paddingLarge
                text:
                    if ( manager.car.consumptionUnit === "l/100km" ) {
                        qsTr("Last: %L1 l/100km").arg(manager.car.consumptionLast.toFixed(2))
                    }
                    else if ( manager.car.consumptionUnit === "mpg" ) {
                        qsTr("Last: %L1 mpg").arg(((consumptionfactor * 1/manager.car.consumptionLast)).toFixed(2))
                    }
                color: {
                    if(manager.car.consumptionLast === 0) return Theme.primaryColor

                    var cLast = manager.car.consumptionLast
                    var cAvg  = manager.car.consumption
                    if(cLast < cAvg * 0.92) return "#00FF00"
                    if(cLast < cAvg * 0.94) return "#40FF00"
                    if(cLast < cAvg * 0.96) return "#80FF00"
                    if(cLast < cAvg * 0.98) return "#C0FF00"
                    if(cLast < cAvg * 1.00) return "#FFFF00"
                    if(cLast < cAvg * 1.02) return "#FFC000"
                    if(cLast < cAvg * 1.04) return "#FF8000"
                    if(cLast < cAvg * 1.06) return "#FF4000"
                    if(cLast < cAvg * 1.08) return "#FF2000"
                    return "#FF0000"
                }
            }
            Rectangle {
                width: parent.width
                height: Theme.paddingLarge
                color: "transparent"
            }
        }
        Flow {
            id: flowElement
            anchors.top: textColumn.bottom
            width: parent.width
                Column {
                id: buttonColumn1
                width: (carEntryPage.width < carEntryPage.height ? parent.width : parent.width / 2)
                spacing: Theme.paddingLarge
                Row {
                    spacing: Theme.paddingLarge
                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        border.color : "black"
                        border.width : 5 * Screen.widthRatio
                        width: 110 * Screen.widthRatio
                        height: 110 * Screen.widthRatio
                        radius: 10 * Screen.widthRatio

                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/Pump.png"
                            width: 100 * Screen.widthRatio
                            height: 100 * Screen.widthRatio
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: pageStack.push(Qt.resolvedUrl("TankView.qml"))
                        }
                    }

                    Button {
                        width: 300 * Screen.widthRatio
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("New Tank")
                        onClicked: pageStack.push(Qt.resolvedUrl("TankEntry.qml"))
                    }
                }

                Row {
                    spacing: Theme.paddingLarge
                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        border.color : "black"
                        border.width : 5 * Screen.widthRatio
                        width: 110 * Screen.widthRatio
                        height: 110 * Screen.widthRatio
                        radius: 10 * Screen.widthRatio

                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/Wrench.png"
                            width: 100 * Screen.widthRatio
                            height: 100 * Screen.widthRatio
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: pageStack.push(Qt.resolvedUrl("CostView.qml"))
                        }
                    }

                    Button {
                        width: 300 * Screen.widthRatio
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("New Cost")
                        onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"))
                    }
                }
                Row {
                    Rectangle {
                        width: buttonColumn2.width
                        height: (carEntryPage.width < carEntryPage.height ? 1 : 0)
                        color: "transparent"
                    }
                }
            }
            Column {
                id: buttonColumn2
                width: (carEntryPage.width < carEntryPage.height ? parent.width : parent.width / 2)
                spacing: Theme.paddingLarge

                Row {
                    spacing: Theme.paddingLarge
                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        border.color : "black"
                        border.width : 5 * Screen.widthRatio
                        width: 110 * Screen.widthRatio
                        height: 110 * Screen.widthRatio
                        radius: 10 * Screen.widthRatio

                        Image {
                            anchors.centerIn: parent
                            id: icon
                            source: "qrc:/Wheel.png"
                            width: 90 * Screen.widthRatio
                            height: 90 * Screen.widthRatio
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: pageStack.push(Qt.resolvedUrl("TireView.qml"))
                        }
                    }

                    Button {
                        width: 300 * Screen.widthRatio
                        anchors.verticalCenter: parent.verticalCenter
                        text: (manager.car.tireMounted < manager.car.numTires)?(qsTr("Tires mounted: %1/%2").arg(manager.car.tireMounted).arg(manager.car.numTires)):(qsTr("Tires mounted"))
                        color: (manager.car.tireMounted < manager.car.numTires)?(Theme.highlightColor):(Theme.primaryColor)
                        onClicked: pageStack.push(Qt.resolvedUrl("TireView.qml"))
                    }
                }
                Row {
                    spacing: Theme.paddingLarge
                    anchors.horizontalCenter: parent.horizontalCenter

                    Rectangle {
                        border.color : "black"
                        border.width : 5 * Screen.widthRatio
                        width: 110 * Screen.widthRatio
                        height: 110 * Screen.widthRatio
                        radius: 10 * Screen.widthRatio

                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/Dollar.png"
                            width: 90 * Screen.widthRatio
                            height: 90 * Screen.widthRatio
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: pageStack.push(Qt.resolvedUrl("BudgetView.qml"))
                        }
                    }

                    Button {
                        width: 300 * Screen.widthRatio
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Statistics")
                        onClicked: pageStack.push(Qt.resolvedUrl("BudgetView.qml"))
                    }
                }
                Row {
                    Rectangle {
                        width: buttonColumn2.width
                        height: (carEntryPage.width < carEntryPage.height ? 1 : 0)
                        color: "transparent"
                    }
                }
            }
        }
    }
}
