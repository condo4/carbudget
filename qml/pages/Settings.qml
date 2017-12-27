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
    property date buyingDate
    property string consumptionUnit
    property string distanceUnit

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
                id: makeInput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Car manufacturer")
                placeholderText: label

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: modelInput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: modelInput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Car model")
                placeholderText: label

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: yearInput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: yearInput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Model year")
                placeholderText: label
                validator: IntValidator { bottom: 1000; top: 9999 }
                inputMethodHints: Qt.ImhDigitsOnly

                EnterKey.enabled: text.length > 4 && acceptableInput == true
                EnterKey.onClicked: licensePlateInput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: licensePlateInput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("License plate number")
                placeholderText: label

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: currencyInput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: currencyInput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Currency")
                placeholderText: qsTr("Currency")

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: distanceUnitInput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            ComboBox {
                id: distanceUnitInput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Distance Unit")
                menu: ContextMenu {
                    MenuItem {
                        text: "km"
                        property string value: "km"
                    }
                    MenuItem {
                        text: "mile"
                        property string value: "mi"
                    }
                }
                onCurrentItemChanged: {
                    consumptionUnitInput.focus = true
                    distanceUnit = currentItem.value
                }
            }

            ComboBox {
                id: consumptionUnitInput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Consumption Unit")

                menu: ContextMenu {
                    MenuItem {
                        text: "l/100km"
                        property string value: "l/100km"
                    }
                    MenuItem {
                        text: "mpg"
                        property string value: "mpg"
                    }

                }
                onCurrentItemChanged: {
                    consumptionUnit = currentItem.value
                    numTiresInput.focus = true
                }
            }

            TextField {
                id: numTiresInput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Number of wheels")
                placeholderText: qsTr("2, 4, 6 or 8")
                validator: RegExpValidator { regExp: /^[2,4,6,8]$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: buyingDateInput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            ValueButton {
                id: buyingDateInput
                function openDateDialog()
                {
                    var date = buyingDate
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale())
                        buyingDate = dialog.date
                        buyingPrice.focus=true
                    })
                }

                label: qsTr("Buying date")
                value: buyingDate.toLocaleDateString(Qt.locale())
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: buyingPrice
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Buying Price")
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: sellingPrice.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: sellingPrice
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Selling Price (est.)")
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: lifeTime.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: lifeTime
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
    canAccept: currencyInput.acceptableInput

    onOpened: {
        makeInput.text         = manager.car.make
        modelInput.text        = manager.car.model
        yearInput.text         = manager.car.year
        licensePlateInput.text = manager.car.licensePlate
        currencyInput.text     = manager.car.currency
        numTiresInput.text     = manager.car.nbtire
        buyingDate             = manager.car.buyingdate
        buyingPrice.text       = manager.car.buyingprice
        sellingPrice.text      = manager.car.sellingprice
        lifeTime.text          = manager.car.lifetime
        consumptionUnit        = manager.car.consumptionunit
        distanceUnit           = manager.car.distanceunity

        // I don't know why, but there is no easy way to set these...
        if(distanceUnit == "km")   { distanceUnitInput.currentIndex = 0 }
        if(distanceUnit == "mi") { distanceUnitInput.currentIndex = 1 }
        if(consumptionUnit == "l/100km") { consumptionUnitInput.currentIndex = 0 }
        if(consumptionUnit == "mpg")     { consumptionUnitInput.currentIndex = 1 }
    }

    onAccepted: {
        manager.car.make            = makeInput.text
        manager.car.model           = modelInput.text
        manager.car.year            = yearInput.text
        manager.car.licensePlate    = licensePlateInput.text
        manager.car.currency        = currencyInput.text
        manager.car.nbtire          = numTiresInput.text
        manager.car.buyingdate      = buyingDate
        manager.car.buyingprice     = buyingPrice.text
        manager.car.sellingprice    = sellingPrice.text
        manager.car.lifetime        = lifeTime.text
        manager.car.consumptionunit = consumptionUnit
        manager.car.distanceunity   = distanceUnit
    }
}
