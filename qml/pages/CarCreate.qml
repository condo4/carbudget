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
    allowedOrientations: Orientation.All
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingLarge

            DialogHeader {
                title: qsTr("Create new car")
            }

            TextField {
                id: inputname
                anchors { left: parent.left; right: parent.right }
                focus: true
                label: qsTr("Short Car name")
                placeholderText: qsTr("Short Car name")
                validator: RegExpValidator { regExp: /^[0-9A-Za-z_]{4,16}$/ }
                color: errorHighlight? "red" : Theme.primaryColor
                inputMethodHints: Qt.ImhNoPredictiveText
            }

        }
    }

    canAccept: inputname.acceptableInput

    onAccepted: {
        manager.createCar(inputname.text)
    }
}
