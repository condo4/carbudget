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
        id:fueltypelist
        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        width: parent.width- Theme.paddingMedium - Theme.paddingMedium
        //contentHeight: allfields.height
        VerticalScrollDecorator {}
        header:  PageHeader {
                 title: qsTr("Costs by fuel type")
             }
        model:manager.car.fueltypes
        delegate: ListItem {
            height:dataRow.height
            contentHeight: dataRow.height
            Row {
                id:dataRow
                width: parent.width - Theme.paddingMedium - Theme.paddingMedium
                Text {
                    text: model.modelData.name;
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    width: parent.width/2
                }
                Text {
                    width:parent.width/2
                    text : manager.car.budget_fuel_total_byType(model.modelData.id).toFixed(2) + " " + manager.car.currency
                    font.family: "monospaced"
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                }
            }
     }
    }
}


