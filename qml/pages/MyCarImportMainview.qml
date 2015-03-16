/**
 * CarBudget, Sailfish application to manage car cost
 *
 * Copyright (C) 2015 Thomas Michel
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
 * Authors: Thomas Michel
 * Provides list view of all cars to import from a mycar xml backup file
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.carbudget 1.0
import QtQuick.XmlListModel 2.0



Page {
    allowedOrientations: Orientation.All
    XmlListModel {
        id: xmlCars
        source: "/home/nemo/mycar_data.xml"
        query: "/mycar/car"
        XmlRole { name: "name"; query: "name/string()" }
    }


    SilicaListView {

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Cars in myCar")
        }

        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model: xmlCars

        delegate: ListItem {
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            showMenuOnPressAndHold: true
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Import")
                    onClicked: manager.importFromMyCar(name)
                }
            }

            Column {
                width: parent.width

                Row {
                    width: parent.width

                    Text {
                        text: name
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        width: parent.width
                        horizontalAlignment: Text.AlignLeft

                    }

            }
            }
        }
     }
}
