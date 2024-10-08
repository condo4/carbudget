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
import "../js/util.js" as Util

Dialog {
    property Tank tank
    property date tank_date
    property int station
    property int fuelType
    property string distanceunit
    property real distanceunitfactor: 1
    allowedOrientations: Orientation.All
    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Manage stations")
                onClicked: pageStack.push(Qt.resolvedUrl("StationView.qml"))
            }
            MenuItem {
                text: qsTr("Manage fuel types")
                onClicked: pageStack.push(Qt.resolvedUrl("FueltypeView.qml"))
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
                        value = dialog.date.toLocaleDateString(Qt.locale())
                        tank_date = dialog.date
                        kminput.focus=true
                    })
                }

                label: qsTr("Date")
                value: tank_date.toLocaleDateString(Qt.locale())
                anchors { left: parent.left; right: parent.right }
                onClicked: openDateDialog()
            }

            TextField {
                id: kminput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: manager.car.distanceUnit
                placeholderText: qsTr("Odometer")

                validator: IntValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: quantityinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: quantityinput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Quantity")
                placeholderText: label

                validator: DoubleValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: priceinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: priceinput
                label: qsTr("Total Price")
                placeholderText: label
                anchors { left: parent.left; right: parent.right }
                validator: DoubleValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: cbfuelType.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: unitpriceinput
                label: qsTr("Unit Price")
                anchors { left: parent.left; right: parent.right }
                validator: DoubleValidator { bottom: 0; top: 99999999 }
                readOnly: true
                property var _unitPrice: Util.stringToNumber(priceinput.text) / Util.stringToNumber(quantityinput.text)
                text: (_unitPrice > 0 && _unitPrice < Infinity) ? Util.numberToString(_unitPrice, 3) : Util.numberToString(0, 3)
            }
            ComboBox {
                id: cbfuelType
                label: qsTr("Fuel Type")
                anchors { left: parent.left; right: parent.right }
                menu: ContextMenu {
                    Repeater {
                        id: fuelTypeslistrepeater
                        model: manager.car.fuelTypes
                        MenuItem {
                            property int dbid
                            id: fuelTypeListItem
                            text: modelData.name
                            dbid: modelData.id
                            onClicked:{
                                fuelType = modelData.id
                                cbstation.focus = true
                            }
                        }
                    }
                }
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
                            id: stationListItem
                            text: modelData.name
                            dbid: modelData.id
                            onClicked:{
                                station = modelData.id
                                noteinput.focus = true
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
                onCheckedChanged: missedinput.focus = true
            }

            TextSwitch {
                anchors { left: parent.left; right: parent.right }
                id: missedinput
                text: qsTr("Missed tank")
                checked: false
                onCheckedChanged: noteinput.focus = true
            }

            TextArea {
                anchors { left: parent.left; right: parent.right }
                id: noteinput
                placeholderText: qsTr("Note")
            }
        }
    }
    canAccept: kminput.acceptableInput && quantityinput.acceptableInput && priceinput.acceptableInput

    onOpened: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi" )
        {
            distanceunitfactor = 1.609
        }
        if(tank != undefined)
        {
            tank_date = tank.date
            kminput.text = (tank.distance / distanceunitfactor).toFixed(0)
            quantityinput.text = Util.numberToString(tank.quantity)
            priceinput.text = Util.numberToString(tank.price)
            fullinput.checked = tank.full
            missedinput.checked = tank.missed
            fuelType = tank.fuelType
            station = tank.station
            noteinput.text = tank.note
            for(var i=0; i<fuelTypeslistrepeater.count; i++)
            {
                if(fuelTypeslistrepeater.itemAt(i).dbid === tank.fuelType)
                {
                    cbfuelType.currentIndex = i
                    break
                }
            }
            for(var j=0; j<stationslistrepeater.count; j++)
            {
                if(stationslistrepeater.itemAt(j).dbid === tank.station)
                {
                    cbstation.currentIndex = j
                    break
                }
            }
        }
        else {
            tank_date = new Date()
            fuelType = manager.car.defaultFuelType
            station = manager.car.lastFuelStation
            for(var k=0; k<fuelTypeslistrepeater.count; k++)
            {
                if(fuelTypeslistrepeater.itemAt(k).dbid === fuelType)
                {
                    cbfuelType.currentIndex = k
                    break
                }
            }
            for(var l=0; l<stationslistrepeater.count; l++)
            {
                if(stationslistrepeater.itemAt(l).dbid === station)
                {
                    cbstation.currentIndex = l
                    break
                }
            }

        }
    }

    onAccepted: {
        if(tank == undefined)
        {
            manager.car.addNewTank(
                tank_date,
                kminput.text * distanceunitfactor,
                Util.stringToNumber(quantityinput.text),
                Util.stringToNumber(priceinput.text),
                fullinput.checked,
                missedinput.checked,
                fuelType,
                station,
                noteinput.text
            )
        }
        else
        {
            tank.date = tank_date
            tank.distance = kminput.text * distanceunitfactor
            tank.full = fullinput.checked
            tank.missed = missedinput.checked
            manager.car.modifyTank(
                tank,
                tank_date,
                kminput.text * distanceunitfactor,
                Util.stringToNumber(quantityinput.text),
                Util.stringToNumber(priceinput.text),
                fullinput.checked,
                missedinput.checked,
                fuelType,
                station,
                noteinput.text
            )
        }
    }
}
