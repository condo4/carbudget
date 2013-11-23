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
import libcar 1.0


Dialog {
    id: addStation
    property Station station

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: colum
            width: parent.width

            DialogHeader { title: {
                    if(station != undefined) return qsTr("Modify Station")
                    else return qsTr("New Station")
                }
            }

            TextField {
                id: nameinput
                label: qsTr("Name")
                placeholderText: qsTr("Name")
                focus: true
                width: parent.width
            }
        }
    }
    canAccept: nameinput.acceptableInput

    onOpened: {
        if(station != undefined)
        {
            nameinput.text = station.name
        }
    }

    onAccepted: {
        if(station == undefined)
        {
            manager.car.addNewStation(nameinput.text)
        }
        else
        {
            station.name = nameinput.text
            station.save()
        }
    }
}
