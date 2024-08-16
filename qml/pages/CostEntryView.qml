/**
 * CarBudget, Sailfish application to manage car cost
 *
 * Copyright (C) 2014 Fabien Proriol, 2015 Thomas Michel
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
 * Authors: Thomas Michel
 */

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0
import "../js/util.js" as Util

Page {
        allowedOrientations: Orientation.All
        property Cost cost
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
                    onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"), { cost: cost })
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
                    title: qsTr("Cost")
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
                        text: (cost.distance/distanceunitfactor).toFixed(0)
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                        width:(parent.width-parent.spacing)/2
                    }
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
                        text: cost.date.toLocaleDateString(Qt.locale(),"yyyy/MM/dd")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                        width:(parent.width-parent.spacing)/2
                    }
                }
                Row {
                    id: costTyperow
                    width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                    spacing:Theme.paddingLarge
                    Text {
                        text: qsTr("Cost Type:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                        width:(parent.width-parent.spacing)/2
                    }
                    Text {
                        text: manager.car.getCostTypeName(cost.costType)
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
                        text: qsTr("Price:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                        width:(parent.width-parent.spacing)/2
                    }
                    Text {
                        text: Util.numberToString(cost.cost) + " " + manager.car.currency
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                        width:(parent.width-parent.spacing)/2
                    }
                }
                Text {
                    width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                    text: qsTr("Description:")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                }
                Text {
                    width: parent.width- Theme.paddingMedium - Theme.paddingMedium
                    text: cost.description
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    wrapMode:Text.Wrap
                }
           }
        }
}
