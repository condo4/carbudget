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
    property string filter: ""
    // When in statistics drilldown, show description instead of cost type
    property bool showDescription: false
    allowedOrientations: Orientation.All
    property string distanceunit
    property real distanceunitfactor

    Component.onCompleted: {
        distanceunit = manager.car.distanceunity
        if(distanceunit == "km")
        {
            distanceunitfactor = 1
        }
        else if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
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
        interactive: !costlistView.flicking
        pressDelay: 0
        anchors.fill: parent
        PageHeader {
            id: header
            title: qsTr("Cost List")
        }
        PullDownMenu {
            MenuItem {
                text: qsTr("Add cost")
                onClicked: pageStack.push(Qt.resolvedUrl("CostEntry.qml"))
            }
        }
        SilicaListView {
            VerticalScrollDecorator {}
            id:costlistView
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingSmall
            anchors.rightMargin: Theme.paddingSmall
            clip: true
            leftMargin: Theme.paddingMedium
            rightMargin: Theme.paddingMedium
            onModelChanged: fillListModel()
            model: listModel
            delegate: ListItem {
                width: parent.width - Theme.paddingMedium - Theme.paddingMedium
                height: dataColumn.height
                contentHeight: dataColumn.height
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
                    id: dataColumn
                    width: parent.width
                    Row {
                        width: parent.width
                        Text {
                            text: (model.modelData.distance/distanceunitfactor).toFixed(0) + manager.car.distanceunity;
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
                            text: {return showDescription ? model.modelData.description : manager.car.getCosttypeName(model.modelData.costtype)}
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeExtraSmall
                            color: Theme.secondaryColor
                            width: parent.width / 2
                            horizontalAlignment: Text.AlignLeft
                            clip: true
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
    ListModel {
        id:listModel
    }

    // Fill list model
    function fillListModel()
    {
        var costlist = manager.car.costs;
        for (var i = 0;i < costlist.length ;i++)
        {
            if ((filter=="")||(manager.car.getCosttypeName(costlist[i].costtype)==filter))
                listModel.append({"cost" : costlist[i]})
        }
        console.log("List Model filled")
    }
}

