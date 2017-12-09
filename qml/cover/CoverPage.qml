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

CoverBackground {
    Column {
        x: Theme.paddingMedium
        y: Theme.paddingMedium
        width: parent.width - 2*x
        spacing: Theme.paddingMedium
        Label {
            width: parent.width
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            text: {
                if(manager.car.make.length > 0)
                    return (manager.car.make + "<br />" + manager.car.model)
                else
                    return manager.car.name
            }
        }

        Rectangle {
            // The easiest way to insert a spacer in a column...
            width: parent.width
            height: Theme.paddingSmall
            color: "transparent"
        }

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: ("%L1 %2").arg(manager.car.maxdistance - manager.car.mindistance).arg(manager.car.distanceunity)
        }
        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: manager.car.budget.toFixed(2)+manager.car.currency+qsTr(" / 100")+manager.car.distanceunity
            font.pixelSize: Theme.fontSizeSmall
        }
        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: manager.car.consumption.toFixed(2)+qsTr("l / 100")+manager.car.distanceunity
            font.pixelSize: Theme.fontSizeSmall
        }
    }
    CoverActionList {
        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                app.activate()
                pageStack.push(Qt.resolvedUrl("../pages/TankEntry.qml"))
            }
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: {
                app.activate()
                pageStack.push(Qt.resolvedUrl("../pages/CarView.qml"))
            }
        }
    }
}


