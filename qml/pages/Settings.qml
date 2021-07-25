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
    property date buyingDate: new Date()
    property string consumptionUnit
    property string distanceUnit
    property int fuelType

    // Enter "Settings" mode by default
    property bool newCarMode: false

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
                title: newCarMode
                       ? qsTr("Create new car")
                       : qsTr("Settings")
            }

            TextField {
                id: name
                focus: true
                label: qsTr("Short car name")
                placeholderText: label
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_\- ]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: make.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"

                // TODO Changing car short name is essentially renaming the file.
                Component.onCompleted: {
                    if(!newCarMode) {
                        name.height = 0
                        name.enabled = false
                        name.visible = false
                    }
                    name._editor.touched = true
                }
            }

            TextField {
                id: make
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Car manufacturer")
                placeholderText: label
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_-]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: model.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: make._editor.touched = true
            }

            TextField {
                id: model
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Car model")
                placeholderText: label
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_-]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: year.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: model._editor.touched = true
            }

            TextField {
                id: year
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Car manufacture year")
                placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1000; top: 9999 }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: licensePlate.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: year._editor.touched = true
            }

            TextField {
                id: licensePlate
                anchors { left: parent.left; right: parent.right }
                label: qsTr("License plate number")
                placeholderText: label
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_-]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: numTires.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: licensePlate._editor.touched = true
            }

            TextField {
                id: numTires
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Number of wheels")
                placeholderText: label
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 99 }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: {
                    if(newCarMode) defaultFuelTypeText.focus = true
                    else {
                        numTires.focus = false
                        defaultFuelType.clicked(0)
                    }
                }
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: numTires._editor.touched = true
            }

            ComboBox {
                id: defaultFuelType
                label: qsTr("Primary Fuel Type")
                anchors { left: parent.left; right: parent.right }
                menu: ContextMenu {
                    Repeater {
                        id: fuelTypeRepeater
                        model: manager.car.fuelTypes
                        MenuItem {
                            id: fuelTypeListItem
                            property int dbid
                            text: modelData.name
                            dbid: modelData.id
                            onClicked: {
                                fuelType = dbid
                                defaultFuelType.focus = false
                                distanceUnitInput.clicked(0)
                            }
                        }
                    }
                }
            }

            TextField {
                id: defaultFuelTypeText
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Primary Fuel Type")
                placeholderText: label
                validator: RegExpValidator { regExp: /^.+$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: {
                    defaultFuelTypeText.focus = false
                    distanceUnitInput.clicked(0)
                }
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: defaultFuelTypeText._editor.touched = true
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
                        onClicked: {
                            distanceUnitInput.focus = false
                            consumptionUnitInput.clicked(0)
                        }
                    }
                    MenuItem {
                        text: "mi"
                        property string value: "mi"
                        onClicked: {
                            distanceUnitInput.focus = false
                            consumptionUnitInput.clicked(0)
                        }
                    }
                }
                onCurrentItemChanged: {
                    distanceUnit = currentItem.value
                }
                Component.onCompleted: {
                    if(!newCarMode)
                        return

                    if(systemDistanceUnit === "km")
                        distanceUnitInput.currentIndex = 0
                    else
                        distanceUnitInput.currentIndex = 1
                }
            }

            ComboBox {
                id: consumptionUnitInput
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Consumption Unit")

                menu: ContextMenu {
                    id: consumptionUnitInputMenu
                    MenuItem {
                        text: "l/100km"
                        property string value: "l/100km"
                        onClicked: buyingPrice.focus = true

                    }
                    MenuItem {
                        text: "mpg"
                        property string value: "mpg"
                        onClicked: buyingPrice.focus = true
                    }

                }
                onCurrentItemChanged: {
                    consumptionUnit = currentItem.value
                }
                Component.onCompleted: {
                    if(!newCarMode)
                        return

                    if(systemDistanceUnit === "km")
                        consumptionUnitInput.currentIndex = 0
                    else
                        consumptionUnitInput.currentIndex = 1
                }
            }

            TextField {
                id: buyingPrice
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Buying Price")
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 99999999 }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: currency.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: buyingPrice._editor.touched = true
            }

            TextField {
                id: currency
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Currency")
                placeholderText: label
                validator: RegExpValidator { regExp: /^.+$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                text: newCarMode ? systemCurrencySymbol : label
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: {
                    buyingDateButton.openDateDialog()
                }
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: currency._editor.touched = true
            }

            ValueButton {
                id: buyingDateButton
                function openDateDialog()
                {
                    var date = buyingDate
                    var dialog = pageStack.push(datePickerDialog, { date: date })

                    // There seems to be no way to scroll ValueButton into view,
                    // so the next best option is focuing the next item,
                    // wether the dialog is accepted or rejected.
                    dialog.accepted.connect(function()
                    {
                        value = dialog.date.toLocaleDateString(Qt.locale())
                        buyingDate = dialog.date
                        lifeTime.focus = true
                    })
                    dialog.rejected.connect(function() {
                        lifeTime.focus = true
                    })
                }
                DatePickerDialog {
                    id: datePickerDialog
                }

                label: qsTr("Buying date")
                value: buyingDate.toLocaleDateString(Qt.locale())
                width: parent.width
                onClicked: openDateDialog()
            }

            TextField {
                id: lifeTime
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Lifetime (in months, est.)")
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 9999 }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: sellingPrice.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: lifeTime._editor.touched = true
            }

            TextField {
                id: sellingPrice
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Selling Price (est.)")
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 0; top: 99999999 }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: acceptableInput
                EnterKey.onClicked: sellingPrice.focus = false
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                Component.onCompleted: {
                    if(newCarMode) {
                        sellingPrice.text = 0
                        sellingPrice.height = 0
                        sellingPrice.enabled = false
                        sellingPrice.visible = false
                    }
                    sellingPrice._editor.touched = true
                }
            }
        }
    }

    canAccept: name.acceptableInput &&
               make.acceptableInput &&
               model.acceptableInput &&
               year.acceptableInput &&
               licensePlate.acceptableInput &&
               numTires.acceptableInput &&
               (newCarMode ? defaultFuelTypeText.acceptableInput : true) &&
               buyingPrice.acceptableInput &&
               currency.acceptableInput &&
               lifeTime.acceptableInput &&
               sellingPrice.acceptableInput

    onOpened: {
        if(!newCarMode) {
            name.text         = manager.car.name
            make.text         = manager.car.make
            model.text        = manager.car.model
            year.text         = manager.car.year
            licensePlate.text = manager.car.licensePlate
            numTires.text     = manager.car.numTires
            fuelType          = manager.car.defaultFuelType
            distanceUnit      = manager.car.distanceUnit
            consumptionUnit   = manager.car.consumptionUnit
            buyingPrice.text  = manager.car.buyingPrice
            currency.text     = manager.car.currency
            buyingDate        = manager.car.buyingDate
            lifeTime.text     = manager.car.lifetime
            sellingPrice.text = manager.car.sellingPrice

            distanceUnitInput.currentIndex = (distanceUnit === "km" ? 0 : 1)
            consumptionUnitInput.currentIndex = (consumptionUnit == "l/100km" ? 0 : 1)

            for(var i=0; i<fuelTypeRepeater.count; i++) {
                if(fuelTypeRepeater.itemAt(i).dbid === manager.car.defaultFuelType) {
                    defaultFuelType.currentIndex = i
                    break
                }
            }
        }

        // "Not set" counts as one
        if(fuelTypeRepeater.count <= 1 || newCarMode) {
            defaultFuelType.enabled = false
            defaultFuelType.visible = false
            defaultFuelType.height = 0
        }
        else {
            defaultFuelTypeText.enabled = false
            defaultFuelTypeText.visible = false
            defaultFuelTypeText.height = 0
        }
    }

    onAccepted: {
        if(newCarMode) {
            manager.createCar(name.text)
            manager.selectCar(name.text)
        }
        manager.car.make            = make.text
        manager.car.model           = model.text
        manager.car.year            = year.text
        manager.car.licensePlate    = licensePlate.text
        manager.car.numTires        = numTires.text
        manager.car.distanceUnit    = distanceUnit
        manager.car.consumptionUnit = consumptionUnit
        manager.car.buyingPrice     = (buyingPrice.text.length > 0 ? parseFloat(buyingPrice.text) : 0)
        manager.car.currency        = currency.text
        manager.car.buyingDate      = buyingDate
        manager.car.lifetime        = (lifeTime.text.length > 0 ? parseInt(lifeTime.text) : 0)
        manager.car.sellingPrice    = (sellingPrice.text.length > 0 ? parseFloat(sellingPrice.text) : 0)

        if(fuelTypeRepeater.count <= 1 || newCarMode) {
            manager.car.addNewFuelType(defaultFuelTypeText.text)
            manager.car.defaultFuelType = 1
        }
        else {
            manager.car.defaultFuelType = defaultFuelType.currentIndex
        }
    }

    Component.onCompleted: {
        if(newCarMode) name.focus = true
        else make.focus = true
    }
}
