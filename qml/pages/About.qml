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
        /*
        PullDownMenu {
            MenuItem {
                text: qsTr("Simulation")
                onClicked: {
                    manager.car.simulation()
                }
            }
        }
        */

        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: "CarBudget 0.12"
            }

            Label {
                x: Theme.paddingLarge
                text: qsTr("License: GPLv3")
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                x: Theme.paddingLarge
                text: qsTr("Created by condo4 (Fabien Proriol)")
                font.pixelSize: Theme.fontSizeSmall
            }

            Label {
                x: Theme.paddingLarge
                text: qsTr("Credits to:<br\>- Lorenzo Facca (Italian translation)<br\>- Alois Spitzbart (German translation)<br\>- Michal Hrusecky (Many improvments)<br\>- Denis Fedoseev (Russion translation)<br \>- Thomas Michel (Many improvments)")
                font.pixelSize: Theme.fontSizeSmall
            }

            Button {
               id: homepage
               anchors.horizontalCenter: parent.horizontalCenter
               text: "<a href=\"https://github.com/condo4/carbudgetr\">Sourcecode on Github</a>"
               onClicked: {
                   Qt.openUrlExternally("https://github.com/condo4/carbudget")
               }
            }
        }
    }
}
