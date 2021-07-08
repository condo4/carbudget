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


Dialog {
    id: mountTire
    property Tire tire
    property date mountDate
    property date umountDate
    allowedOrientations: Orientation.All
    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: colum
            width: parent.width

            DialogHeader {
                title: (tire.mounted)?(qsTr("Umount Tire")):(qsTr("Mount Tire"));
            }

            ValueButton {
                function openDateDialog()
                {
                    var date = mountDate
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale())
                        mountDate = dialog.date
                    })
                }

                label: "Date: "
                value: mountDate.toLocaleDateString(Qt.locale())
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: kminput
                label: manager.car.distanceUnit
                placeholderText: label
                focus: true
                width: parent.width
                validator: RegExpValidator { regExp: /^[0-9]{1,7}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: accept()
            }

            TextSwitch {
                anchors { left: parent.left; right: parent.right }
                visible: tire.mounted
                id: totrashinput
                text: qsTr("To trash")
                checked: false
            }
        }
    }

    canAccept: kminput.acceptableInput

    onOpened: {
        mountDate = new Date()
    }

    onAccepted: {
        if(tire.mounted)
            manager.car.umountTire(mountDate, kminput.text * (manager.car.distanceUnit === "mi" ? 1.609 : 1.0), tire, totrashinput.checked)
        else
            manager.car.mountTire(mountDate, kminput.text * (manager.car.distanceUnit === "mi" ? 1.609 : 1.0), tire)
    }
}
