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
    id: addTire
    property Tire tire
    property date buy_date
    property date trash_date

    SilicaFlickable {
        anchors.fill: parent

        Column {
            id: colum
            width: parent.width

            DialogHeader { title: {
                    if(tire != undefined) return qsTr("Modify Tire")
                    else return qsTr("New Tire")
                }
            }

            ValueButton {
                function openDateDialog()
                {
                    var date = buy_date
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        buy_date = dialog.date
                    })
                }

                label: qsTr("Buy date")
                value: buy_date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }

            ValueButton {
                function openDateDialog()
                {
                    var date = trash_date
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        trash_date = dialog.date
                    })
                }

                label: "Trash date"
                value: trash_date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: nameinput
                label: qsTr("Name")
                placeholderText: qsTr("Name")
                focus: true
                width: parent.width
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: manufacturerinput.focus = true
            }

            TextField {
                id: manufacturerinput
                label: qsTr("Manufacturer")
                placeholderText: qsTr("Manufacturer")
                width: parent.width
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: modelinput.focus = true
            }

            TextField {
                id: modelinput
                label: qsTr("Model")
                placeholderText: qsTr("Model")
                width: parent.width
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: priceinput.focus = true
            }


            TextField {
                id: priceinput
                label: qsTr("Price")
                placeholderText: qsTr("Price")
                width: parent.width
                validator: RegExpValidator { regExp: /^[0-9\.]{1,6}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: quantityinput.focus = true
            }

            TextField {
                id: quantityinput
                label: qsTr("Quantity")
                placeholderText: qsTr("Quantity")
                width: parent.width
                validator: RegExpValidator { regExp: /^[2,4]$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: quantityinput.focus = false
            }
        }
    }
    canAccept: priceinput.acceptableInput && modelinput.acceptableInput && manufacturerinput.acceptableInput && nameinput.acceptableInput

    onOpened: {
        if(tire != undefined)
        {
            buy_date = tire.buydate
            trash_date = tire.trashdate
            priceinput.text = tire.price
            quantityinput.text = tire.quantity
            modelinput.text = tire.modelname
            manufacturerinput.text = tire.manufacturer
            nameinput.text = tire.name
        }
        else
        {
            buy_date = new Date()
            trash_date = new Date()
        }
    }

    onAccepted: {
        if(tire == undefined)
        {
            manager.car.addNewTire(buy_date,trash_date,nameinput.text,manufacturerinput.text,modelinput.text,priceinput.text, quantityinput.text )
        }
        else
        {
            tire.buydate = buy_date
            tire.trashdate = trash_date
            tire.name = nameinput.text
            tire.manufacturer = manufacturerinput.text
            tire.modelname = modelinput.text
            tire.price = priceinput.text
            tire.quantity = quantityinput.text
            tire.save()
        }
    }
}
