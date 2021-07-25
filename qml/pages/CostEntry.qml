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
    property Cost cost
    property date cost_date
    property int costType
    property string distanceunit
    property real distanceunitfactor: 1
    allowedOrientations: Orientation.All
    SilicaFlickable {
        PullDownMenu {
            MenuItem {
                text: qsTr("Manage cost types")
                onClicked: pageStack.push(Qt.resolvedUrl("CosttypeView.qml"))
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
                        value = dialog.date.toLocaleDateString(Qt.locale())
                        cost_date = dialog.date
                        kminput.focus=true
                    })
                }

                label: qsTr("Date")
                value: cost_date.toLocaleDateString(Qt.locale())
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: kminput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: manager.car.distanceUnit
                placeholderText: label

                validator: IntValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction

                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: cbcostType.clicked(0)
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            ComboBox {
                id: cbcostType
                label: qsTr("Cost Type")
                anchors { left: parent.left; right: parent.right }

                menu: ContextMenu {
                    Repeater {
                        id: costTypeslistrepeater
                        model: manager.car.costTypes
                        MenuItem {
                            property int dbid
                            id: costTypeListItem
                            text: modelData.name
                            dbid: modelData.id
                            onClicked:{
                                costType = modelData.id
                                costinput.focus = true
                            }
                        }
                    }
                }
            }

            TextField {
                id: costinput
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Price")
                placeholderText: label

                validator: DoubleValidator { bottom: 0; top: 99999999 }
                inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhNoPrediction
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: descinput.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextArea {
                anchors { left: parent.left; right: parent.right }
                id: descinput
                placeholderText: qsTr("Description")
            }
        }
    }
    canAccept: kminput.acceptableInput && costinput.acceptableInput

    onOpened: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi" )
        {
            distanceunitfactor = 1.609
        }
        if(cost != undefined)
        {
            cost_date = cost.date
            kminput.text = (cost.distance / distanceunitfactor)
            costType = cost.costType
            descinput.text = cost.description
            costinput.text = cost.cost
            for(var i=0; i<costTypeslistrepeater.count; i++)
            {
                if(costTypeslistrepeater.itemAt(i).dbid === cost.costType)
                {
                    cbcostType.currentIndex = i
                    break
                }
            }
        }
        else cost_date = new Date()
    }

    onAccepted: {
        if(cost == undefined)
        {
            manager.car.addNewCost(cost_date,kminput.text * distanceunitfactor,costType,descinput.text,costinput.text.replace(",","."))
        }
        else
        {
            cost.date = cost_date
            cost.distance = kminput.text * distanceunitfactor
            cost.costType = costType
            cost.description = descinput.text
            cost.cost = costinput.text.replace(",",".")
            cost.save()
        }
    }
}
