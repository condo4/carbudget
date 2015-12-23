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
    property int fueltype
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
                        value = dialog.date.toLocaleDateString(Qt.locale(),"d MMM yyyy")
                        tank_date = dialog.date
                        kminput.focus=true
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
                label: manager.car.distanceunity
                placeholderText: qsTr("ODO")

                validator: RegExpValidator { regExp: /^[0-9]{1,7}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: quantityinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: quantityinput
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
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,7}$/ }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: cbfueltype.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }

            TextField {
                id: unitpriceinput
                label: qsTr("Unite Price")
                anchors { left: parent.left; right: parent.right }
                validator: RegExpValidator { regExp: /^[0-9\.,]{1,6}$/ }
                readOnly: true
                text:  (priceinput.text.replace(",",".") / quantityinput.text.replace(",",".")).toFixed(3) || 0
            }
            ComboBox {
                id: cbfueltype
                label: qsTr("Fuel Type")
                anchors { left: parent.left; right: parent.right }
                menu: ContextMenu {
                    Repeater {
                        id: fueltypeslistrepeater
                        model: manager.car.fueltypes
                        MenuItem {
                            property int dbid
                            id: fueltypelistItem
                            text: modelData.name
                            dbid: modelData.id
                            onClicked:{
                                fueltype = modelData.id
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
                            id: stationlistItem
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
                onCheckedChanged: noteinput.focus = true
            }

            TextArea {
                anchors { left: parent.left; right: parent.right }
                id: noteinput
                placeholderText: qsTr("description")
            }
        }
    }
    canAccept: kminput.acceptableInput && quantityinput.acceptableInput && priceinput.acceptableInput

    onOpened: {
        distanceunit = manager.car.distanceunity
        if(distanceunit == "mi" )
        {
            distanceunitfactor = 1.609
        }
        if(tank != undefined)
        {
            tank_date = tank.date
            kminput.text = (tank.distance / distanceunitfactor).toFixed(0)
            quantityinput.text = tank.quantity
            priceinput.text = tank.price
            fullinput.checked = tank.full
            fueltype = tank.fueltype
            station = tank.station
            noteinput.text = tank.note
            for(var i=0; i<fueltypeslistrepeater.count; i++)
            {
                if(fueltypeslistrepeater.itemAt(i).dbid === tank.fueltype)
                {
                    cbfueltype.currentIndex = i
                    break
                }
            }
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
            manager.car.addNewTank(tank_date,kminput.text * distanceunitfactor,quantityinput.text.replace(",","."),priceinput.text.replace(",","."),fullinput.checked, fueltype, station, noteinput.text)
        }
        else
        {
            tank.date = tank_date
            tank.distance = kminput.text * distanceunitfactor
            tank.quantity = quantityinput.text.replace(",",".")
            tank.price = priceinput.text.replace(",",".")
            tank.full = fullinput.checked
            tank.fueltype = fueltype
            tank.station = station
            tank.note = noteinput.text
            tank.save()
        }
    }
}
