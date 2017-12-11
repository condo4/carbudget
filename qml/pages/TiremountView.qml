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
 * Authors: Fabien Proriol, Thomas Michel
 */

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.carbudget 1.0

Page {
    allowedOrientations: Orientation.All
    property string distanceunit
    property real distanceunitfactor: 1

    Component.onCompleted: {
        distanceunit = manager.car.distanceunity
        if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
    }

    SilicaListView {

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Tire Mounts")
        }

        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model: manager.car.tiremounts

        delegate: ListItem {
            width: parent.width
            showMenuOnPressAndHold: true

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Modify")
                    onClicked: pageStack.push(Qt.resolvedUrl("TiremountEdit.qml"), { tirmount: model.modelData })
                }
            }

            Column {
                width: parent.width
                Row {
                    width: parent.width

                    Text {
                        text:  model.modelData.tirename;
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        width: parent.width
                        horizontalAlignment: Text.AlignLeft                   }
                }
                 Row {
                    width: parent.width
                    Text {
                        text: (model.modelData.mountdistance/distanceunitfactor).toFixed(0) + manager.car.distanceunity + ((model.modelData.unmountdistance === 0) ? "" :  " - " + (model.modelData.unmountdistance/distanceunitfactor).toFixed(0) + manager.car.distanceunity)
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.primaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        text: model.modelData.mountdate.toLocaleDateString(Qt.locale(),"yyyy/MM/dd") + ((model.modelData.unmountdistance === 0) ? "" : " - " + model.modelData.unmountdate.toLocaleDateString(Qt.locale(),"yyyy/MM/dd"))
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.primaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}
