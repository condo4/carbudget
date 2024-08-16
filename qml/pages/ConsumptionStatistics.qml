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
 * Authors: Fabien Proriol, Thomas Michel
 */

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0
import "../js/util.js" as Util

Page {
    allowedOrientations: Orientation.All
    property real distanceunitfactor: 1
    property real consumptionfactor : 1.0
    Component.onCompleted: {
        if(manager.car.distanceUnit === "mi")
        {
            distanceunitfactor = 1.609
        }
        if(manager.car.consumptionUnit === 'mpg')
        {
            consumptionfactor = 4.546*100/1.609
        }
    }
    SilicaListView{
        id:fuelTypeList
        VerticalScrollDecorator {}
        header:  PageHeader {
                 title: qsTr("Consumption by fuel type")
             }
        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model:manager.car.fuelTypes
        delegate: ListItem {
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            height: dataColumn.height
            contentHeight: dataColumn.height
            Column {
                id: dataColumn
                width: parent.width
                Row {
                    width: parent.width
                    Text {
                        text: model.modelData.name;
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        font.bold: true
                        color: Theme.primaryColor
                        width: parent.width/2
                    }
                }
                Row {
                    width: parent.width
                    Text {
                        width:parent.width/2
                        text : qsTr("Average:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        text : if ( manager.car.consumptionUnit === 'l/100km') {
                            Util.numberToString(manager.car.budget_consumption_byType(model.modelData.id)) + " l";
                        }
                        else {
                            if ( manager.car.consumptionUnit === 'mpg') {
                                qsTr("%L1 mpg").arg(Util.numberToString(consumptionfactor / manager.car.budget_consumption_byType(model.modelData.id)));
                            }
                        }
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                }
                Row {
                    width:parent.width
                    Text {
                        width:parent.width/2
                        text : qsTr("Min:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        text : if ( manager.car.consumptionUnit === 'l/100km') {
                            Util.numberToString(manager.car.budget_consumption_min_byType(model.modelData.id)) + " l";
                        }
                        else {
                            if ( manager.car.consumptionUnit === 'mpg') {
                                qsTr("%L1 mpg").arg(Util.numberToString(consumptionfactor / manager.car.budget_consumption_min_byType(model.modelData.id)));
                            }
                        }
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                }
                Row {
                    width: parent.width
                    Text {
                        width:parent.width/2
                        text : qsTr("Max:")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        text : if ( manager.car.consumptionUnit === 'l/100km') {
                            Util.numberToString(manager.car.budget_consumption_max_byType(model.modelData.id)) + " l";
                        }
                        else {
                            if ( manager.car.consumptionUnit === 'mpg') {
                                qsTr("%L1 mpg").arg(Util.numberToString(consumptionfactor / manager.car.budget_consumption_max_byType(model.modelData.id)));
                            }
                        }
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}


