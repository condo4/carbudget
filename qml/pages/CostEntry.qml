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
    property Cost cost
    property date cost_date

    SilicaFlickable {

        VerticalScrollDecorator {}

        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader { title: {
                    if(cost != undefined) return qsTr("Modify Cost")
                    else return qsTr("New Cost")
                }
            }

            ValueButton {
                function openDateDialog()
                {
                    var date = cost_date
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        cost_date = dialog.date
                    })
                }

                label: qsTr("Date")
                value: cost_date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: kminput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Km")
                placeholderText: qsTr("Km")

                validator: RegExpValidator { regExp: /^[0-9]{1,7}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: descinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: descinput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Description")
                placeholderText: qsTr("description")

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: costinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: costinput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Price")
                placeholderText: qsTr("Price")

                validator: RegExpValidator { regExp: /^[0-9\.]{1,6}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: costinput.focus = false
            }
        }
    }
    canAccept: kminput.acceptableInput && descinput.acceptableInput && costinput.acceptableInput

    onOpened: {
        if(cost != undefined)
        {
            cost_date = cost.date
            kminput.text = cost.distance
            descinput.text = cost.description
            costinput.text = cost.cost
        }
        else cost_date = new Date()
    }

    onAccepted: {
        if(cost == undefined)
        {
            manager.car.addNewCost(cost_date,kminput.text,descinput.text,costinput.text)
        }
        else
        {
            cost.date = cost_date
            cost.distance = kminput.text
            cost.description = descinput.text
            cost.cost = costinput.text
            cost.save()
        }
    }
}
