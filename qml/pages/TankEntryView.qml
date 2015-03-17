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
import harbour.carbudget 1.0


Page {
    property Tank tank

    SilicaFlickable {

        VerticalScrollDecorator {}

        PullDownMenu {
            MenuItem {
                text: qsTr("Modify")
                onClicked: pageStack.push(Qt.resolvedUrl("TankEntry.qml"), { tank: tank })
            }
        }

        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                title: qsTr("Tank")
            }
            Row {
                id: datarow
                width:parent.width
                spacing:Theme.paddingLarge
                Column {
                    id: labels
                    width: parent.width/2
                    spacing: Theme.paddingLarge
                    Text {
                        text: qsTr("Date:")
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        text: qsTr("ODO:")
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        text: qsTr("Quantity:")
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        text: qsTr("Total Price:")
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        text: qsTr("Unite Price:")
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: qsTr("Full tank:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: qsTr("Fuel Type:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: qsTr("Station:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: qsTr("Note:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                }
                Column {
                    id: values
                    width: parent.width/2
                    spacing: Theme.paddingLarge
                    Text {
                        text: tank.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        text: tank.distance
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        text: tank.quantity.toFixed(2)
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        text: tank.price
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        text: (tank.price / tank.quantity).toFixed(3)
                        anchors { left: parent.left; right: parent.right }
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }

                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: (tank.full)?(qsTr("Yes")):(qsTr("No"))
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: manager.car.getFueltypeName(tank.fueltype)
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: manager.car.getStationName(tank.station)
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                    Text {
                        anchors { left: parent.left; right: parent.right }
                        text: tank.note
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                    }
                }
            }
          }
    }
}
