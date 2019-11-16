/**
 * CarBudget, Sailfish application to manage car cost
 *
 * Copyright (C) 2018 Fabien Proriol
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
 * Authors: Matti Viljanen
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.carbudget 1.0

Page {
    property string filename
    property string newName

    Component.onCompleted: {
        newName=filename.substring(filename.lastIndexOf('/') + 1)
        newName=newName.replace(/_(\d){8}_(\d){6}/, "")
        newName=newName.replace(".cbg","")
        filename = filename.replace("file://","")
    }

        allowedOrientations: Orientation.All
        SilicaFlickable {
            VerticalScrollDecorator {}

            anchors.fill: parent
            contentHeight: column.height + Theme.paddingLarge

            Column {
                id: column
                width: parent.width
                spacing: Theme.paddingLarge

                PageHeader {
                    title: qsTr("Enter car name")
                }

                TextField {
                    id: newCarNameField
                    anchors { left: parent.left; right: parent.right }
                    focus: true

                    label: newName
                    placeholderText: label
                    text: label

                    validator: RegExpValidator { regExp: /^[0-9A-Za-z_-]{4,16}$/ }
                    EnterKey.enabled: text.length >= 4 && acceptableInput == true

                    onFocusChanged: if(focus) errorLabel.text = ""
                }

                Button {
                    text: qsTr("Import")
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: newCarNameField.acceptableInput
                    onClicked: {
                        var result = manager.importFromCarBudget(filename, newCarNameField.text+".cbg")
                        if(result === "OK")
                            pageStack.replaceAbove(null, Qt.resolvedUrl("CarView.qml"))
                        else if(result === "FILE_EXISTS")
                            errorLabel.text = qsTr("Could not import selected file, because the car name chosen already exists.")
                        else if(result === "DB_ERROR")
                            errorLabel.text = qsTr("Could not import selected file, because the file is not a valid CarBudet database file.")
                        else
                            errorLabel.text = qsTr("Could not import selected file. Unknown error.")
                    }
                }

                Label {
                    id: errorLabel
                    horizontalAlignment: Text.AlignHCenter
                    width: column.width
                    anchors.leftMargin: Theme.paddingLarge * 2
                    anchors.rightMargin: Theme.paddingLarge * 2
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeMedium
                    wrapMode: Text.Wrap
                }
            }
        }
}
