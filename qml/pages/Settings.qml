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
import harbour.carbudget 1.0


Dialog {
    property date buying_date
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
                title: qsTr("Settings")
            }


            TextField {
                id: currencyinput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Currency")
                placeholderText: qsTr("Currency")

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                //EnterKey.onClicked: descinput.focus = true
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: distanceunity
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Distance Unity")
                placeholderText: qsTr("Km or Mile")

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: nbtire.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: nbtire
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Number of wheels")
                placeholderText: qsTr("2, 4, 6 or 8")
                validator: RegExpValidator { regExp: /^[2,4,6,8]$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: buyingdate.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            ValueButton {
                function openDateDialog()
                {
                    var date = buying_date
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        buying_date = dialog.date
                        buyingprice.focus=true
                    })
                }

                label: qsTr("Buying date")
                value: buying_date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: buyingprice
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Buying Price")
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: sellingprice.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: sellingprice
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Selling Price (est.)")
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: lifetime.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: lifetime
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Lifetime (in months, est.)")
                validator: RegExpValidator { regExp: /^[0-9]{1,4}$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                //EnterKey.onClicked: descinput.focus = true
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
        }
    }
    canAccept: currencyinput.acceptableInput

    onOpened: {
        currencyinput.text = manager.car.currency
        distanceunity.text = manager.car.distanceunity
        nbtire.text        = manager.car.nbtire
        buying_date        = manager.car.buyingdate
        buyingprice.text   = manager.car.buyingprice
        sellingprice.text  = manager.car.sellingprice
        lifetime.text      = manager.car.lifetime
    }

    onAccepted: {
        manager.car.currency      = currencyinput.text
        manager.car.distanceunity = distanceunity.text
        manager.car.nbtire        = nbtire.text
        manager.car.buyingdate    = buying_date
        manager.car.buyingprice   = buyingprice.text
        manager.car.sellingprice  = sellingprice.text
        manager.car.lifetime      = lifetime.text
    }
}
