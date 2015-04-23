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

Dialog {
    allowedOrientations: Orientation.All
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
                 title: qsTr("Costs by Type")
             }

        Canvas {
            id: pieChart
            width: { return parent.width < parent.height ? parent.width/2 : parent.height/2 }
            height: { return parent.width < parent.height ? parent.width/2 : parent.height/2 }
            anchors.top: header.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onPaint: {
                var ctx = pieChart.getContext('2d')
                ctx.clearRect(0,0,width,height)
                var centerX = (width/2).toFixed(0)
                var centerY = (height/2).toFixed(0)
                var radius = (0.95*width/2).toFixed(0)
                var startangle=0.0
                var endangle=0.0
                var total=0.0
                ctx.lineWidth = 1
                var i
                for (i =0; i < listModel.count;i++)
                {
                    total += listModel.get(i).total
                }
                var angle = 6.28/total
                for (i =0; i < listModel.count;i++)
                {
                    endangle = startangle + listModel.get(i).total * angle
                    ctx.fillStyle=listModel.get(i).color
                    ctx.beginPath()
                    ctx.moveTo(centerX,centerY)
                    ctx.arc(centerX,centerY,radius,startangle,endangle,false)
                    ctx.lineTo(centerX,centerY)
                    ctx.fill()
                    ctx.stroke()
                    startangle=endangle
                }
            }
        }
        SilicaListView{
            id:costlistView
            anchors.top: pieChart.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingMedium
            anchors.rightMargin: Theme.paddingMedium
            clip: true
            //width: parent.width- Theme.paddingMedium - Theme.paddingMedium
            VerticalScrollDecorator {}
            //model:manager.car.costtypes
            model:listModel
            delegate: ListItem {
                height:dataRow.height
                contentHeight: dataRow.height
                onClicked: pageStack.push(Qt.resolvedUrl("CostView.qml"), { filter: model.name , showDescription: true })
                Rectangle {
                    color:model.color
                    height:dataRow.height
                    width: parent.width //- Theme.paddingMedium - Theme.paddingMedium
                    Row {
                        id: dataRow
                        width: parent.width
                        Text {
                            text: model.name;
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            width: parent.width/2
                        }
                        Text {
                            width:parent.width/2
                            text : model.total.toFixed(2) + " " + manager.car.currency
                            font.family: "monospaced"
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
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
        var costlist = manager.car.costtypes;
        var color
        for (var i = 0;i < costlist.length ;i++)
        {
            color=(i+1)/(costlist.length+2)
            color=(255*color).toFixed(0)
            color=Number(color).toString(16).toUpperCase()
            /*
            /* Optional use color model
            color=(65536*color).toFixed(0)
            while (color.length < 4) {
                color = "0" + color;
            }
            var finalcolor = "#00"+color
            */
            var finalcolor = "#"+color+color+color
            listModel.append({id: costlist[i].id , name: costlist[i].name, total: manager.car.budget_cost_total_byType(costlist[i].id), color: finalcolor})
        }
    }
    onOpened:{fillListModel()}
}
