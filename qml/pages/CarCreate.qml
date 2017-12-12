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


Dialog {
    id: dialog
    allowedOrientations: Orientation.All
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: flow.height + Theme.paddingLarge

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Flow {
            id: flow
            spacing: Theme.paddingLarge
            width: parent.width

            DialogHeader {
                title: qsTr("Create new car")
            }

            TextField {
                id: name
                width: (isPortrait ? parent.width : (parent.width / 2 - Theme.paddingLarge))
                focus: true
                label: qsTr("Short car name")
                placeholderText: qsTr("Short car name")
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_]{4,16}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                inputMethodHints: Qt.ImhNoPredictiveText
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: make.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: make
                width: (isPortrait ? parent.width : (parent.width / 2 - Theme.paddingLarge))
                label: qsTr("Car manufacturer")
                placeholderText: qsTr("Car manufacturer")
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: model.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: model
                width: (isPortrait ? parent.width : (parent.width / 2 - Theme.paddingLarge))
                label: qsTr("Car model")
                placeholderText: qsTr("Car model")
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: year.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: year
                width: (isPortrait ? parent.width : (parent.width / 2 - Theme.paddingLarge))
                label: qsTr("Car manufacture year")
                placeholderText: qsTr("Car manufacture year")
                validator: IntValidator { bottom: 1000; top: 9999 }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                inputMethodHints: Qt.ImhDigitsOnly
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: licencePlate.focus = true
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
            TextField {
                id: licencePlate
                width: (isPortrait ? parent.width : (parent.width / 2 - Theme.paddingLarge))
                label: qsTr("Licence plate")
                placeholderText: qsTr("Licence plate")
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_]{1,32}$/ }
                color: errorHighlight ? Theme.highlightColor : Theme.primaryColor
                inputMethodHints: Qt.ImhNoPredictiveText
                EnterKey.enabled: text.length > 0 && acceptableInput == true
                EnterKey.onClicked: dialog.canAccept ? dialog.accept() : function() {}
            }
        }
    }

    canAccept: {
        return name.acceptableInput
                && make.acceptableInput
                && model.acceptableInput
                && year.acceptableInput
                && licencePlate.acceptableInput
    }

    onAccepted: {
        manager.createCar(name.text)
        manager.selectCar(name.text)
        manager.car.make = make.text
        manager.car.model = model.text
        manager.car.year = year.text
        manager.car.licensePlate = licencePlate.text
    }
}
