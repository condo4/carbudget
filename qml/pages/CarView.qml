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
    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Create new car")
                onClicked: pageStack.push(Qt.resolvedUrl("CarCreate.qml"))
            }
        }

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Car List")
        }

        id: carView
        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model: manager.cars
        function select_car(data) {
            manager.selectCar(data)
            pageStack.clear()
            pageStack.push(Qt.resolvedUrl("CarEntry.qml"));
        }

        delegate: ListItem {
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            showMenuOnPressAndHold: true
            onClicked: carView.select_car(model.modelData)
            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Select")
                    onClicked: carView.select_car(model.modelData)
                }

                MenuItem {
                    text: qsTr("Remove")
                    onClicked: {
                        remorseAction("Deleting", function() {
                            manager.delCar(model.modelData)
                        })
                    }
                }
            }

            Column {
                width: parent.width

                Row {
                    width: parent.width
                    Text {
                        text : model.modelData
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
        }
    }
}
