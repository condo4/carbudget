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
    property Tank tank
    property date tank_date
    property int station

    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Manage stations")
                onClicked: pageStack.push(Qt.resolvedUrl("StationView.qml"))
            }
        }

        VerticalScrollDecorator {}

        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge


        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader { title: {
                    if(tank != undefined) return qsTr("Modify Tank")
                    else return qsTr("New tank")
                }
            }

            ValueButton {
                function openDateDialog()
                {
                    var date = tank_date
                    var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: date })

                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        tank_date = dialog.date
                    })
                }

                label: qsTr("Date")
                value: tank_date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                anchors { left: parent.left; right: parent.right }
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
                EnterKey.onClicked: quanttityinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: quanttityinput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Quantity")
                placeholderText: qsTr("Quantity")

                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: priceinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: priceinput
                label: qsTr("Total Price")
                placeholderText: qsTr("Total Price")
                anchors { left: parent.left; right: parent.right }
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: cbstation.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: unitpriceinput
                label: qsTr("Unite Price")
                anchors { left: parent.left; right: parent.right }
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }
                readOnly: true
                text:  (priceinput.text.replace(",",".") / quanttityinput.text.replace(",",".")).toFixed(3) || 0
            }

            ComboBox {
                id: cbstation
                label: qsTr("Station")
                anchors { left: parent.left; right: parent.right }

                menu: ContextMenu {
                    Repeater {
                        id: stationslistrepeater
                        model: manager.car.stations
                        MenuItem {
                            property int dbid
                            id: stationlistItem
                            text: modelData.name
                            dbid: modelData.id
                            onClicked:{
                                station = modelData.id
                                fullinput.focus = true
                            }
                        }
                    }
                }
            }

            TextSwitch {
                anchors { left: parent.left; right: parent.right }
                id: fullinput
                text: qsTr("Full tank")
                checked: true
            }
        }
    }
    canAccept: kminput.acceptableInput && quanttityinput.acceptableInput && priceinput.acceptableInput

    onOpened: {
        if(tank != undefined)
        {
            tank_date = tank.date
            kminput.text = tank.distance
            quanttityinput.text = tank.quantity
            priceinput.text = tank.price
            fullinput.checked = tank.full
            station = tank.station
            for(var i=0; i<stationslistrepeater.count; i++)
            {
                if(stationslistrepeater.itemAt(i).dbid === tank.station)
                {
                    cbstation.currentIndex = i
                    break
                }
            }
        }
        else tank_date = new Date()
    }

    onAccepted: {
        if(tank == undefined)
        {
            manager.car.addNewTank(tank_date,kminput.text,quanttityinput.text.replace(",","."),priceinput.text.replace(",","."),fullinput.checked,station)
        }
        else
        {
            tank.date = tank_date
            tank.distance = kminput.text
            tank.quantity = quanttityinput.text.replace(",",".")
            tank.price = priceinput.text.replace(",",".")
            tank.full = fullinput.checked
            tank.station = station
            tank.save()
        }
    }
}
