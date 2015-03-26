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
                text: qsTr("Add cost")
                onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"))
            }
        }

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("Cost List")
        }

        anchors.fill: parent
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        model: manager.car.costs

        delegate: ListItem {
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            showMenuOnPressAndHold: true
            onClicked: pageStack.push(Qt.resolvedUrl("CostEntryView.qml"), { cost: model.modelData })

            menu: ContextMenu {
                MenuItem {
                    text: qsTr("Modify")
                    onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"), { cost: model.modelData })
                }

                MenuItem {
                    text: qsTr("Remove")
                    onClicked: {
                        remorseAction(qsTr("Deleting"), function() {
                            manager.car.delCost(model.modelData)
                        })
                    }
                }
            }

            Column {
                width: parent.width

                Row {
                    width: parent.width

                    Text {
                        text: model.modelData.distance + manager.car.distanceunity;
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        text: model.modelData.date.toLocaleDateString(Qt.locale(),"dd/MM/yyyy");
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
                        text: manager.car.getCosttypeName(model.modelData.costtype);
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignLeft
                    }

                    Text {
                        text: model.modelData.cost + manager.car.currency;
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        width: parent.width / 2
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }
        }
    }
}

