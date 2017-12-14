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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.carbudget 1.0

Page {
    allowedOrientations: Orientation.All
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
                        text :  manager.car.budget_consumption_byType(model.modelData.id).toFixed(2) + " l";
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
                        text :  manager.car.budget_consumption_min_byType(model.modelData.id).toFixed(2) + " l";
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
                        text :  manager.car.budget_consumption_max_byType(model.modelData.id).toFixed(2) + " l";
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


