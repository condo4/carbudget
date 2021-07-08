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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0

Page {
    property bool backupOK
    allowedOrientations: Orientation.All
    SilicaFlickable {

        VerticalScrollDecorator {}

        anchors.fill: parent

        PageHeader {
            title: qsTr("Backup")
        }

        Label {
            anchors.fill: parent
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeLarge
            text: {
                if(backupOK == true)
                    return qsTr("Creating the backup was successful. The selected car has been exported to the Downloads directory.")
                else
                    return qsTr("There was an error during the backup operation.")
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}
