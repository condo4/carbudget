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

Page {
    property string filter: ""
    // When in statistics drilldown, show description instead of cost type
    property bool showDescription: false
    allowedOrientations: Orientation.All
    property string distanceunit
    property real distanceunitfactor: 1
    property variant costList: manager.car.costs

    onCostListChanged: fillListModel()

    Component.onCompleted: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
        if(listModel.count === 0)
            fillListModel()
    }

    Drawer {
        id: costviewDrawer
        anchors.fill: parent
        dock: Dock.Top
        open: false
        backgroundSize: costview.contentHeight
    }
    SilicaFlickable {
        id:costview
        interactive: !costListView.flicking
        anchors.fill: parent
        PageHeader {
            id: header
            title: qsTr("Cost List")
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("Add Cost")
                onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"))
            }
        }
        SilicaListView {
            VerticalScrollDecorator {}
            id:costListView
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            clip: true
            onModelChanged: fillListModel()
            model: listModel
            delegate: ListItem {
                width: parent.width
                showMenuOnPressAndHold: true
                onClicked: pageStack.push(Qt.resolvedUrl("CostEntryView.qml"), { cost: model.modelData })
                contentHeight: itemTexts.height + Theme.paddingSmall
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

                Item {
                    id: itemTexts
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        topMargin: Theme.paddingSmall
                        bottomMargin: Theme.paddingSmall
                        leftMargin: Theme.paddingMedium
                        rightMargin: Theme.paddingMedium
                    }
                    height: tCostType.y + tCostType.height + Theme.paddingSmall
                    Label {
                        id: tDistance
                        anchors.top: parent.top
                        anchors.left: parent.left
                        text: (model.modelData.distance/distanceunitfactor).toFixed(0) + manager.car.distanceUnit;
                        font.family: Theme.fontFamily
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                    }
                    Label {
                        id: tDate
                        anchors.top: parent.top
                        anchors.right: parent.right
                        text: model.modelData.date.toLocaleDateString(Qt.locale(),"yyyy/MM/dd");
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }

                    Label {
                        id: tCostType
                        anchors.top: tDistance.bottom
                        anchors.left: tDistance.left
                        text: {return showDescription ? model.modelData.description : manager.car.getCostTypeName(model.modelData.costType)}
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        clip: true
                    }
                    Label {
                        id: tPrice
                        anchors.top: tDate.bottom
                        anchors.right: tDate.right
                        text: model.modelData.cost.toLocaleString(Qt.locale(),'f',2) + " " + manager.car.currency;
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                }

            }
        }

    }
    ListModel {
        id:listModel
    }

    // Fill list model
    function fillListModel()
    {
        listModel.clear()
        for (var i = 0; i < costList.length; i++)
        {
            if(filter === "" || filter === costList[i].costType)
                listModel.append({"cost" : costList[i]})
        }
        console.log("List Model filled")
    }
}

