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

import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0
import "../js/util.js" as Util

Page {
    allowedOrientations: Orientation.All
    id:coststatisticsPage
    property bool  per100: false
    property string type: "costs"
    property real distanceunitfactor: 1
    Component.onCompleted: {
        if(manager.car.distanceUnit === "mi")
        {
            distanceunitfactor = 1.609
        }
    }

    PageHeader {
            id: header

             title: {
                 if (type=="costs")
                 {
                     if (per100)
                         return  qsTr("Bills per 100 %1 by type").arg(manager.car.distanceUnit)
                     return qsTr("Bills by Type")
                 }
                 else
                 {
                     if (per100)
                         return  qsTr("Fuel per 100 %1 by type").arg(manager.car.distanceUnit)
                     return qsTr("Fuel by Type")
                 }
             }
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
        id:costListView
        anchors.top:pieChart.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        contentHeight: parent.height
        width:parent.width
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        clip: true
        VerticalScrollDecorator {}
        model:listModel
        delegate: ListItem {
            height:dataRow.height
            width: parent.width - Theme.paddingMedium - Theme.paddingMedium
            contentHeight: dataRow.height
            onClicked: {
                if (type=="costs")
                    pageStack.push(Qt.resolvedUrl("CostView.qml"), { filter: model.name , showDescription: true })
                else pageStack.push(Qt.resolvedUrl("TankView.qml"), { filter: model.name , showDescription: true })
            }
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
                        text: Util.numberToString(model.total) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
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
        var costList = manager.car.costTypes
        var fuelTypeList = manager.car.fuelTypes
        var color
        listModel.clear()
        var count = (type=="costs" ? costList.length : fuelTypeList.length)
        for (var i = 0;i < count ;i++)
        {
            color=(i+1)/(count+2)
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
            var price
            var name
            var id
            if (type=="costs")
            {
                name = costList[i].name
                id = costList[i].id
                if (per100)
                    price = manager.car.budgetCost_byType(costList[i].id)*distanceunitfactor
                else price = manager.car.budgetCostTotal_byType(costList[i].id)
            }
            else
            {
                name = fuelTypeList[i].name
                id = fuelTypeList[i].id
                if (per100)
                    price = manager.car.budgetFuel_byType(fuelTypeList[i].id)*distanceunitfactor
                else price = manager.car.budgetFuelTotal_byType(fuelTypeList[i].id)
            }
            listModel.append({id: id, name: name, total: price, color: finalcolor})
        }
    }
    onVisibleChanged: {fillListModel()}
}


