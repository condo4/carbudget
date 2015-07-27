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


Page {
    id: page
    allowedOrientations: Orientation.All
    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Import Car")
            }


            Text {
                x: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge - Theme.paddingLarge
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("Cars can be imported from Android app My Cars or from Nokia app Fuelpad.")
                font.pixelSize: Theme.fontSizeSmall
            }
            Text {
                x: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge - Theme.paddingLarge
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("My Cars import file must be XML Export from My Cars.")
                font.pixelSize: Theme.fontSizeSmall
            }
            Text {
                x: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge - Theme.paddingLarge
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                text: qsTr("Fuelpad import file must be a db file.")
                font.pixelSize: Theme.fontSizeSmall
            }

            Button {
               id: btnImport
               anchors.horizontalCenter: parent.horizontalCenter
               text: "Select Import File"
               onClicked: pageStack.push(Qt.resolvedUrl("SelectImportFile.qml"))
            }
        }
    }
}
