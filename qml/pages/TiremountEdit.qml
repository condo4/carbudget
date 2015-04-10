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


Dialog {
    property Tiremount tiremount
    property date mountdate
    property date unmountdate

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
                    var date = mountdate
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        mountdate = dialog.date
                        mountdistance.focus=true
                    })
                }

                label: qsTr("Mount date")
                value: mountdate.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }
            TextField {
                id: mountdistance
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: manager.car.distanceunity
                placeholderText: manager.car.distanceunity

                validator: RegExpValidator { regExp: /^[0-9]{1,7}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            ValueButton {
                id: unmountdatebutton
                function openDateDialog()
                {
                    var date = unmountdate
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        unmountdate = dialog.date
                        unmountdistance.focus=true
                    })
                }

                label: qsTr("Unmount date")
                value: unmountdate.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }
            TextField {
                id: unmountdistance
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: manager.car.distanceunity
                placeholderText: manager.car.distanceunity
                validator: RegExpValidator { regExp: /^[0-9]{1,7}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
         }
    }
    canAccept: mountdistance.acceptableInput && unmountdistance.acceptableInput

    onOpened: {
        if(tiremount != undefined)
        {
            mountdate = tiremount.mountdate
            mountdistance.text = tiremount.mountdistance
            unmountdate = tiremount.unmountdate
            unmountdistance.text = tiremount.unmountdistance
            if (tiremount.unmountdistance==0)
            {
                unmountdistance.visible=false;
                unmountdatebutton.visible=false;
            }
        }
        else
        {
            //This should never happen
            mountdate = new Date()
            unmountdate = new Date()
        }
    }
    onAccepted: {
        tiremount.mountdate = mountdate
        tiremount.mountdistance = mountdistance.text
        if (tiremount.unmountdistance > 0)
        {
            tiremount.unmountdate = unmountdate
            tiremount.unmountdistance = unmountdistance.text
        }
        tiremount.save()
    }
}

