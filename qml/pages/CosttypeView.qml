/**
 * CarBudget, Sailfish application to manage car cost
 *
 * Copyright (C) 2014 Fabien Proriol, 2015 Thomaas Michel
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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0

Page {
    allowedOrientations: Orientation.All
    SilicaListView {
        PullDownMenu {
            MenuItem {
                text: qsTr("Add new cost type")
                onClicked: pageStack.push(Qt.resolvedUrl("CosttypeEntry.qml"))
            }
        }

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Cost Type List")
        }

        anchors.fill: parent
        model: manager.car.costTypes

        delegate: ListItem {
            width: parent.width
            showMenuOnPressAndHold: true

            menu: ContextMenu {
                MenuItem {
                    enabled: model.modelData.id > 0 ? true : false
                    visible: model.modelData.id > 0 ? true : false
                    text: qsTr("Modify")
                    onClicked: pageStack.push(Qt.resolvedUrl("CosttypeEntry.qml"), { costType: model.modelData })
                }

                MenuItem {
                    enabled: model.modelData.id > 0 ? true : false
                    visible: model.modelData.id > 0 ? true : false
                    text: qsTr("Delete")
                    onClicked: {
                        remorseAction(qsTr("Deleting"), function() {
                            manager.car.delCostType(model.modelData)
                        })
                    }
                }
            }

            Label {
                anchors.verticalCenter: parent.verticalCenter
                x: Theme.horizontalPageMargin
                text: model.modelData.name;
            }
        }
    }
}
