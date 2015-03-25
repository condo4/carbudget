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
import Qt.labs.folderlistmodel 2.1

Page {
    allowedOrientations: Orientation.All
    SilicaListView {

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("XML Files")
        }

        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model:folderModel

        FolderListModel {
            id: folderModel
            folder: "file:///$HOME"
            nameFilters: ["*.xml"]
            showDirs: false
        }

        delegate: ListItem {
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            showMenuOnPressAndHold: true
            onClicked: pageStack.push(Qt.resolvedUrl("MyCarImportMainview.qml"), { filename: filename.text })

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Import")
                    onClicked: pageStack.push(Qt.resolvedUrl("MyCarImportMainview.qml"), { filename: filename.text })
                }

            }

            Column {
                width: parent.width

                Row {
                    width: parent.width

                    Text {
                        id: filename
                        text: fileName
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

