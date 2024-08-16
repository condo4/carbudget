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


import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0
import "../js/util.js" as Util

Page {
    allowedOrientations: Orientation.All
    property string distanceunit
    property real distanceunitfactor: 1

    Component.onCompleted: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
    }

    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Create new tire")
                onClicked: pageStack.push(Qt.resolvedUrl("TireEntry.qml"))
            }
            MenuItem {
                text: qsTr("Show history")
                onClicked: pageStack.push(Qt.resolvedUrl("TiremountView.qml"))
            }
        }

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Tire List")
        }

        anchors.fill: parent
        model: manager.car.tires

        delegate: ListItem {
            width: parent.width
            showMenuOnPressAndHold: true
            contentHeight: tireColumn.height
            opacity: (model.modelData.mounted)?(1):((model.modelData.mountable)?(1):(0.4))

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Modify")
                    visible: !model.modelData.trashed
                    onClicked: pageStack.push(Qt.resolvedUrl("TireEntry.qml"), { tire: model.modelData })
                }
                MenuItem {
                    text: qsTr("Untrash")
                    visible: model.modelData.trashed
                    onClicked: manager.car.untrashTire(model.modelData)
                }
                MenuItem {
                    text: qsTr("Delete")
                    visible: !model.modelData.mounted && model.modelData.trashed
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
                id: tireColumn
                width: parent.width

                Row {
                    x: Theme.paddingMedium
                    width: parent.width - Theme.paddingMedium - Theme.paddingMedium

                    Text {
                        text: model.modelData.manufacturer + " " + model.modelData.modelname;
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        text: (model.modelData.distance/distanceunitfactor).toFixed(0) + manager.car.distanceUnit;
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Row {
                    x: Theme.paddingMedium
                    width: parent.width - Theme.paddingMedium - Theme.paddingMedium

                    Text {
                        text: model.modelData.name + " (x" + model.modelData.quantity + ")"
                        font.bold: model.modelData.mounted

                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor

                        width: parent.width / 2
                    }
                    Text {
                        text: Util.numberToString(model.modelData.price) + " " + manager.car.currency;
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Row {
                    x: Theme.paddingMedium
                    width: parent.width - Theme.paddingMedium - Theme.paddingMedium
                    Text {
                        text: model.modelData.buyDate.toLocaleDateString(Qt.locale(),"yyyy/MM/dd");
                        font.bold: model.modelData.mounted
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                    }
                    Text {
                        text: (model.modelData.trashed ? qsTr("Trashed") :
                               model.modelData.mounted ? qsTr("Mounted") : " ")
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}
