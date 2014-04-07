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

Page {
    allowedOrientations: Orientation.All
    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Create new tire")
                onClicked: pageStack.push(Qt.resolvedUrl("TireEntry.qml"))
            }
        }

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Tire List")
        }

        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model: manager.car.tires

        delegate: ListItem {
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            showMenuOnPressAndHold: true
            contentHeight: Theme.itemSizeExtraLarge
            opacity: (model.modelData.mounted)?(1):((model.modelData.mountable)?(1):(0.4))

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Modify")
                    onClicked: pageStack.push(Qt.resolvedUrl("TireEntry.qml"), { tire: model.modelData })
                }

                MenuItem {
                    text: qsTr("Remove")
                    visible: !model.modelData.mounted
                    onClicked: {
                        remorseAction(qsTr("Deleting"), function() {
                            manager.car.delTire(model.modelData)
                        })
                    }
                }
                MenuItem {
                    text: (model.modelData.mounted)?(qsTr("Umount")):(qsTr("Mount"))
                    visible: model.modelData.mounted || model.modelData.mountable
                    onClicked: pageStack.push(Qt.resolvedUrl("TireMount.qml"), { tire: model.modelData })
                }
            }

            Column {
                width: parent.width

                Row {
                    width: parent.width

                    Text {
                        text: model.modelData.manufacturer + " (" + model.modelData.distance + qsTr("km") + ")";
                        font.bold: model.modelData.mounted

                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor

                        width: parent.width / 2
                    }
                    Text {
                        text: model.modelData.modelname + " x" +  model.modelData.quantity;
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Row {
                    width: parent.width
                    Text {
                        text: model.modelData.name;
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: 0
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        text: model.modelData.price + manager.car.currency;
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: 0
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Row {
                    width: parent.width
                    Text {
                        text: model.modelData.buydate.toLocaleDateString(Qt.locale(),"dd/MM/yyyy");
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                    }
                    Text {
                        text: model.modelData.trashdate.toLocaleDateString(Qt.locale(),"dd/MM/yyyy");
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                        visible: model.modelData.trashed
                    }
                }
            }
        }
    }
}
