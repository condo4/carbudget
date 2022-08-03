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
        property Tank tank
        property string distanceunit
        property real distanceunitfactor: 1

        Component.onCompleted: {
            distanceunit = manager.car.distanceUnit
            if(distanceunit == "mi")
            {
                distanceunitfactor = 1.609
            }
        }


        SilicaFlickable {


        PullDownMenu {
            MenuItem {
                text: qsTr("Modify")
                onClicked: pageStack.push(Qt.resolvedUrl("TankEntry.qml"), { tank: tank })
            }
        }

        VerticalScrollDecorator {}
        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        width: parent.width- Theme.paddingMedium - Theme.paddingMedium

        Column {
            id: column
            width: parent.width- Theme.paddingMedium - Theme.paddingMedium
            spacing: Theme.paddingLarge
            anchors.fill: parent

            PageHeader {
                title: qsTr("Tank")
            }


            Row {
                id: daterow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing: parent.spacing
                Text {
                    text: qsTr("Date:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }

                Text {
                    text: tank.date.toLocaleDateString(Qt.locale(),"yyyy/MM/dd")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row
            {
                id: odorow
                spacing: parent.spacing
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                Text {
                    text: qsTr("Odometer:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: (tank.distance/distanceunitfactor).toFixed(0)
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: quantityrow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Quantity:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: tank.quantity.toLocaleString(Qt.locale(),'f',2)
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: pricerow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Total Price:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: tank.price.toLocaleString(Qt.locale(),'f',2)
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: unitpricerow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Unit Price:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: (tank.price / tank.quantity).toLocaleString(Qt.locale(),'f',3)
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: stationrow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Station:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: manager.car.getStationName(tank.station)
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: fuelTyperow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Fuel Type:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: manager.car.getFuelTypeName(tank.fuelType)
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: fulltankrow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Full tank:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: (tank.full)?(qsTr("Yes")):(qsTr("No"))
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Row {
                id: missedtankrow
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                spacing:Theme.paddingLarge
                Text {
                    text: qsTr("Missed tank:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                    width:(parent.width-parent.spacing)/2
                }
                Text {
                    text: (tank.missed)?(qsTr("Yes")):(qsTr("No"))
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                    width:(parent.width-parent.spacing)/2
                }
            }

            Text {
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                text: qsTr("Note:")
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
            }
            Text {
                width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                text: tank.note
                font.family: Theme.fontFamily
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.primaryColor
                wrapMode:Text.Wrap
            }

       }
    }
}
