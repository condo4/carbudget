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
    property Cost cost
    property date cost_date

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
                //EnterKey.onClicked: descinput.focus = true
                //EnterKey.iconSource: "image://theme/icon-m-enter-next"
            }
        }
    }
    canAccept: currencyinput.acceptableInput

    onOpened: {
        currencyinput.text = manager.car.currency
        distanceunity.text = manager.car.distanceunity
    }

    onAccepted: {
        manager.car.currency      = currencyinput.text
        manager.car.distanceunity = distanceunity.text
    }
}
