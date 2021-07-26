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


Dialog {
    property TireMount tireMount
    property date mountDate
    property date unmountDate
    allowedOrientations: Orientation.All
    SilicaFlickable {
        VerticalScrollDecorator {}

        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                title: qsTr("Modify Tire Mount")
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
                        mountDistance.focus=true
                    })
                }

                label: qsTr("Mount date")
                value: mountDate.toLocaleDateString(Qt.locale())
                width: parent.width
                onClicked: openDateDialog()
            }
            TextField {
                id: mountDistance
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: manager.car.distanceUnit
                placeholderText: label

                validator: IntValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            ValueButton {
                id: unmountDatebutton
                function openDateDialog()
                {
                    var date = unmountDate
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        unmountDate = dialog.date
                        unmountDistance.focus=true
                    })
                }

                label: qsTr("Unmount date")
                value: unmountDate.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }
            TextField {
                id: unmountDistance
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: manager.car.distanceUnit
                placeholderText: label
                validator: IntValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
         }
    }
    canAccept: mountDistance.acceptableInput && unmountDistance.acceptableInput

    onOpened: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi" )
        {
            distanceunitfactor = 1.609
        }
        if(tireMount != undefined)
        {
            mountDate = tireMount.mountDate
            mountDistance.text = (tireMount.mountDistance / distanceunitfactor).toFixed(0)
            unmountDate = tireMount.unmountDate
            unmountDistance.text = (tireMount.unmountDistance / distanceunitfactor).toFixed(0)
            if (tireMount.unmountDistance==0)
            {
                unmountDistance.visible=false;
                unmountDatebutton.visible=false;
            }
        }
        else
        {
            //This should never happen
            mountDate = new Date()
            unmountDate = new Date()
        }
    }
    onAccepted: {
        tireMount.mountDate = mountDate
        tireMount.mountDistance = mountDistance.text * distanceunitfactor
        if (tireMount.unmountDistance > 0)
        {
            tireMount.unmountDate = unmountDate
            tireMount.unmountDistance = unmountDistance.text * distanceunitfactor
        }
        tireMount.save()
    }
}

