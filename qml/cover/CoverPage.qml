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
import "../js/util.js" as Util

CoverBackground {
    // Show main content only if the car exists
    property bool nullCar: (manager.car == null ||
                            (   manager.car.name  === ""
                             && manager.car.make  === ""
                             && manager.car.model === ""))
    Column {
        enabled: !nullCar
        visible: !nullCar
        x: Theme.paddingMedium
        y: Theme.paddingMedium
        width: parent.width - 2*x
        spacing: Theme.paddingMedium
        Label {
            width: parent.width
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            text: {
                if(nullCar)
                    return ""
                else if(manager.car.make.length > 0)
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

        // Show the car details only if the car exists.
        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: {
                if(nullCar)
                    return ""
                else
                    return ("%L1 %2").arg((manager.car.maxDistance - manager.car.minDistance).toFixed(0)).arg(manager.car.distanceUnit)
            }
        }
        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: {
                if(nullCar)
                    return ""
                else
                   ("%1 %2 / 100 %3").arg(Util.numberToString(manager.car.budget)).arg(manager.car.currency).arg(manager.car.distanceUnit)
            }
            font.pixelSize: Theme.fontSizeSmall
        }
        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: {
                if(nullCar)
                    return ""
                else
                    ("%1l / 100%2").arg(Util.numberToString(manager.car.consumption)).arg(manager.car.distanceUnit)
            }
            font.pixelSize: Theme.fontSizeSmall
        }
    }

    CoverPlaceholder {
        enabled: nullCar
        visible: nullCar
        text: "CarBudget"
        icon.source: "qrc:/harbour-carbudget.png"
    }

    // Show only proper cover actions
    CoverActionList {
        enabled: !nullCar
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
    CoverActionList {
        enabled: nullCar
        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                app.activate()
                pageStack.push(Qt.resolvedUrl("../pages/Settings.qml"), { newCarMode: true })
            }
        }
    }
}


