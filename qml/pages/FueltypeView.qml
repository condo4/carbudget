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
    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Add new fuel type")
                onClicked: pageStack.push(Qt.resolvedUrl("FueltypeEntry.qml"))
            }
        }

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Fuel Type List")
        }

        anchors.fill: parent
        model: manager.car.fueltypes

        delegate: ListItem {
            showMenuOnPressAndHold: true

            menu: ContextMenu {
                MenuItem {
                    enabled: model.modelData.id > 0 ? true : false
                    visible: model.modelData.id > 0 ? true : false
                    text: qsTr("Modify")
                    onClicked: pageStack.push(Qt.resolvedUrl("FueltypeEntry.qml"), { fueltype: model.modelData })
                }
                MenuItem {
                    enabled: model.modelData.id > 0 ? true : false
                    visible: model.modelData.id > 0 ? true : false
                    text: qsTr("Remove")
                    onClicked: {
                        remorseAction(qsTr("Deleting"), function() {
                            manager.car.delFueltype(model.modelData)
                        })
                    }
                }
            }

            MenuItem {
                anchors.verticalCenter: parent.verticalCenter
                x: Theme.horizontalPageMargin
                text: model.modelData.name;
            }
        }
    }
}
