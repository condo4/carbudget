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
    id: budgetPage
    property string distanceunit
    property real distanceunitfactor: 1
    property real consumptionfactor : 1.0
    property variant chartColor: [ Theme.secondaryColor,
        Theme.secondaryHighlightColor,
        Theme.highlightColor,
        Theme.highlightDimmerColor ]

    Component.onCompleted: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit == "mi")
        {
            distanceunitfactor = 1.609
        }
        if(manager.car.consumptionunit == 'mpg')
        {
            consumptionfactor = 4.546*100/1.609
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: flowElement.height + header.height + Theme.paddingSmall
        PullDownMenu {
            enabled: true
            visible: true

            MenuItem {
                text: qsTr("Consumption")
                onClicked: {
                    manager.car.setChartTypeConsumption()
                    pageStack.push(Qt.resolvedUrl("Statistics.qml"))
                }
            }

            MenuItem {
                text: qsTr("Costs")
                onClicked: {
                    manager.car.setChartTypeCosts()
                    pageStack.push(Qt.resolvedUrl("Statistics.qml"))
                }
            }

            MenuItem {
                text: qsTr("Fuel price")
                onClicked: {
                    manager.car.setChartTypeOilPrice()
                    pageStack.push(Qt.resolvedUrl("Statistics.qml"))
                }
            }
        }
        PageHeader {
            id: header
            title: qsTr("Statistics")
        }

        Flow {
            id: flowElement
            width: parent.width
            anchors.top: header.bottom
            Column {
                id: pieColumn
                width: (budgetPage.width < budgetPage.height ? parent.width : parent.width / 2 )
                Row {
                    width: parent.width
                    Rectangle {
                        height: 10;
                        width: 0.175 * parent.width
                        color: "transparent"
                    }

                    Canvas {
                        id: pieChart
                        height: parent.width * 0.65
                        width: parent.width * 0.65
                        onPaint: {
                            var ctx = pieChart.getContext('2d')
                            var centerX = (width/2).toFixed(0)
                            var centerY = (height/2).toFixed(0)
                            var radius = (0.95*width/2).toFixed(0)
                            var startangle=0.0
                            var endangle=0.0
                            var total = manager.car.budgetTotal
                            var angle = Math.PI * 2 / total

                            ctx.clearRect(0,0,width,height)
                            ctx.lineWidth = 2.0 * Screen.widthRatio

                            startangle=endangle
                            endangle = manager.car.budgetCostTotal * angle
                            ctx.fillStyle = chartColor[3]
                            ctx.beginPath()
                            ctx.moveTo(centerX,centerY)
                            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
                            ctx.fill()

                            startangle=endangle
                            endangle = startangle+manager.car.budgetFuelTotal*angle
                            ctx.fillStyle = chartColor[2]
                            ctx.beginPath()
                            ctx.moveTo(centerX,centerY)
                            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
                            ctx.fill()

                            startangle=endangle
                            endangle = startangle+manager.car.budgetTireTotal*angle
                            ctx.fillStyle = chartColor[1]
                            ctx.beginPath()
                            ctx.moveTo(centerX,centerY)
                            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
                            ctx.fill()

                            startangle=endangle
                            endangle = startangle+manager.car.budgetInvestTotal*angle
                            ctx.fillStyle = chartColor[0]
                            ctx.beginPath()
                            ctx.moveTo(centerX,centerY)
                            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
                            ctx.fill()

                            startangle = 0.0
                            endangle = Math.PI * 2
                            ctx.beginPath()
                            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
                            ctx.stroke()
                        }
                    }
                }

                Row {
                    width: parent.width
                    height: 60 * Screen.widthRatio
                    spacing: Theme.paddingMedium
                    Rectangle {
                        height: 10;
                        width: 0.175 * parent.width
                        color: "transparent"
                    }
                    Rectangle {
                        id: chartColorBox1
                        color: chartColor[3]
                        width: 50 * Screen.widthRatio
                        height: 50 * Screen.widthRatio
                        border.color: "black"
                        border.width: 2 * Screen.widthRatio
                        radius: 3 * Screen.widthRatio
                        anchors.margins: 5 * Screen.widthRatio;
                    }
                    Text {
                        width: 0.5 * parent.width
                        height: parent.height
                        text : qsTr("Bills: %1%").arg((manager.car.budgetCostTotal*100/manager.car.budgetTotal).toFixed(2))
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row {
                    width: parent.width
                    height: 60 * Screen.widthRatio
                    spacing: Theme.paddingMedium
                    Rectangle {
                        height: 10;
                        width: 0.175 * parent.width
                        color: "transparent"
                    }
                    Rectangle {
                        color: chartColor[2]
                        width: 50 * Screen.widthRatio
                        height: 50 * Screen.widthRatio
                        border.color: "black"
                        border.width: 2 * Screen.widthRatio
                        radius: 3 * Screen.widthRatio
                        anchors.margins: 5 * Screen.widthRatio;
                    }
                    Text {
                        width: 0.5 * parent.width
                        height: parent.height
                        text : qsTr("Fuel: %1%").arg((manager.car.budgetFuelTotal*100/manager.car.budgetTotal).toFixed(2))
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row {
                    width: parent.width
                    height: 60 * Screen.widthRatio
                    spacing: Theme.paddingMedium
                    Rectangle {
                        height: 10;
                        width: 0.175 * parent.width
                        color: "transparent"
                    }
                    Rectangle {
                        color: chartColor[1]
                        width: 50 * Screen.widthRatio
                        height: 50 * Screen.widthRatio
                        border.color: "black"
                        border.width: 2 * Screen.widthRatio
                        radius: 3 * Screen.widthRatio
                        anchors.margins: 5 * Screen.widthRatio;
                    }
                    Text {
                        width: 0.5 * parent.width
                        height: parent.height
                        text : qsTr("Tires: %1%").arg((manager.car.budgetTireTotal*100/manager.car.budgetTotal).toFixed(2))
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                Row {
                    width: parent.width
                    height: 60 * Screen.widthRatio
                    spacing: Theme.paddingMedium
                    Rectangle {
                        height: 10;
                        width: 0.175 * parent.width
                        color: "transparent"
                    }
                    Rectangle {
                        color: chartColor[0]
                        width: 50 * Screen.widthRatio
                        height: 50 * Screen.widthRatio
                        border.color: "black"
                        border.width: 2 * Screen.widthRatio
                        radius: 3 * Screen.widthRatio
                        anchors.margins: 5 * Screen.widthRatio;
                    }
                    Text {
                        width: 0.5 * parent.width
                        height: parent.height
                        text : qsTr("Invest: %1%").arg((manager.car.budgetInvestTotal*100/manager.car.budgetTotal).toFixed(2))
                        font.pixelSize: Theme.fontSizeMedium
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            Column {
                id: dataColumn
                width: pieColumn.width
                //anchors.top: (Screen.width < Screen.width ? pieColumn.bottom : header.bottom)
                //x: (Screen.width < Screen.width ? parent.x : pieColumn.x)
                Flow {
                    x: Theme.paddingMedium
                    width: parent.width - 2*Theme.paddingMedium
                    Row {
                        id:odoRow
                        width: parent.width
                        Text {
                            width:parent.width/2
                            text : qsTr("Odometer")
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            font.bold: true
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignLeft
                        }
                        Text {
                            width:parent.width/2
                            text :  (manager.car.maxDistance/distanceunitfactor).toFixed(0)
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                    Row {
                        width: parent.width
                        Text {
                            width:parent.width/2
                            text : qsTr("In Budget")
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            font.bold: true
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignLeft
                        }
                        Text {
                            width:parent.width/2
                            text :  ((manager.car.maxDistance - manager.car.minDistance)/distanceunitfactor).toFixed(0)
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                    Row {
                        id:consumptonRow
                        width: parent.width
                        Rectangle {
                            width: parent.width
                            height:consumptionTable.height
                            color: "transparent"
                            Column {
                                id: consumptionTable
                                width:parent.width
                                Row { width: parent.height; height: Theme.paddingMedium; }
                                Row {
                                    id: fuelRow
                                    width: parent.width
                                    Rectangle {
                                        height: 5
                                        width: Theme.paddingLarge
                                        color: "transparent"
                                    }
                                    Text {
                                        text : qsTr("Fuel")
                                        font.family: Theme.fontFamily
                                        font.pixelSize: Theme.fontSizeMedium
                                        font.bold: true
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                }
                                Row { width: parent.height; height: Theme.paddingMedium; }
                                Row {
                                    id: fuelTotalRow
                                    width: parent.width
                                    Text {
                                        width:parent.width/2
                                        text : qsTr("Total:")
                                        font.family: Theme.fontFamily
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Text {
                                        width:parent.width/2
                                        text :  manager.car.fuelTotal.toFixed(2) + " l"
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                                Row {
                                    id:fuelAverageRow
                                    width: parent.width
                                    Text {
                                        width:parent.width/2
                                        text : qsTr("Average:")
                                        font.family: Theme.fontFamily
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Text {
                                        width:parent.width/2
                                        text: if ( manager.car.consumptionunit == 'l/100km') {
                                            manager.car.consumption.toFixed(2)+ " l";
                                        }
                                        else {
                                            if ( manager.car.consumptionunit == 'mpg') {
                                                qsTr("%L1 mpg").arg((consumptionfactor * 1/manager.car.consumption).toFixed(2))
                                            }
                                        }
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                                Row {
                                    id:fuelMinRow
                                    width: parent.width
                                    Text {
                                        width:parent.width/2
                                        text : qsTr("Min:")
                                        font.family: Theme.fontFamily
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Text {
                                        width:parent.width/2
                                        text : if (manager.car.consumptionunit == 'l/100km') {
                                            manager.car.consumptionMin.toFixed(2) + " l"
                                        }
                                        else {
                                            if ( manager.car.consumptionunit == 'mpg') {
                                                qsTr("%L1 mpg").arg((consumptionfactor * 1/manager.car.consumptionMin).toFixed(2))
                                            }
                                        }
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                                Row {
                                    id: fuelMaxRow
                                    width: parent.width
                                    Text {
                                        width:parent.width/2
                                        text : qsTr("Max:")
                                        font.family: Theme.fontFamily
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Text {
                                        width:parent.width/2
                                        text :  if ( manager.car.consumptionunit == 'l/100km') {
                                            manager.car.consumptionMax.toFixed(2) + " l"
                                        }
                                        else {
                                            if ( manager.car.consumptionunit == 'mpg') {
                                                qsTr("%L1 mpg").arg((consumptionfactor * 1/manager.car.consumptionMax).toFixed(2))
                                            }
                                        }
                                        font.pixelSize: Theme.fontSizeMedium
                                        color: Theme.primaryColor
                                        horizontalAlignment: Text.AlignRight
                                    }
                                }
                            }
                            //MouseArea {
                            //    id:consumptionMouse
                            //    anchors.fill:parent
                            //    onClicked: pageStack.push(Qt.resolvedUrl("ConsumptionStatistics.qml"))
                            //}
                        }
                    }
                    Row { width: parent.height; height: Theme.paddingMedium; }
                    Row {
                        id:costsRow
                        width: parent.width
                        Rectangle {
                            height: 5
                            width: Theme.paddingLarge
                            color: "transparent"
                        }
                        Text {
                            text : qsTr("Costs")
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            font.bold: true
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                    Row { width: parent.height; height: Theme.paddingMedium; }
                    Row {
                        id:fuelcostsRow
                        width: parent.width
                        Rectangle {
                            height: fueltext.height
                            width: parent.width
                            color: "transparent"
                            Text {
                                id: fueltext
                                width:parent.width/2
                                text : qsTr("Fuel:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                anchors.right:parent.right
                                width:parent.width/2
                                text : manager.car.budgetFuelTotal.toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                            //MouseArea {
                            //    id:fuelcostsMouse
                            //    anchors.fill:parent
                            //    onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"),{per100:false, type:"fuel"})
                            //}
                        }
                    }
                    Row {
                        id: billcostsRow
                        width: parent.width
                        Rectangle {
                            height: billtext.height
                            width: parent.width
                            color: "transparent"

                            Text {
                                id: billtext
                                width:parent.width/2
                                text : qsTr("Bills:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                anchors.right:parent.right
                                text : manager.car.budgetCostTotal.toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                            //MouseArea {
                            //    id: billcostsMouse
                            //    anchors.fill: parent
                            //    onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"), {per100: false, type:"costs"})
                            //}
                        }
                    }
                    Row {
                        id:tirecostsRow
                        width: parent.width
                        Rectangle {
                            height: tiretext.height
                            width: parent.width
                            color: "transparent"
                            Text {
                                id: tiretext
                                width:parent.width/2
                                text : qsTr("Tires:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                anchors.right:parent.right
                                width:parent.width/2
                                text : manager.car.budgetTireTotal.toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                            //    MouseArea {
                            //        id:tirecostsMouse
                            //        anchors.fill:parent
                            //        onClicked: pageStack.push(Qt.resolvedUrl("FuelStatistics.qml"))
                            //    }
                        }
                    }
                    Row {
                        id:buyingcostsRow
                        width: parent.width
                        Rectangle {
                            height: fueltext.height
                            width: parent.width
                            color: "transparent"
                            Text {
                                id: buyingcosttext
                                width:parent.width/2
                                text : qsTr("Invest:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                anchors.right:parent.right
                                width:parent.width/2
                                text : manager.car.budgetInvestTotal.toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                    Row {
                        id: totalcostsRow
                        width: parent.width
                        Text {
                            width:parent.width/2
                            text : qsTr("Total:")
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignLeft
                        }
                        Text {
                            width:parent.width/2
                            text : manager.car.budgetTotal.toFixed(2) + " " + manager.car.currency
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                    Row { width: parent.height; height: Theme.paddingMedium; }
                    Row {
                        id: per100Row
                        width: parent.width
                        Rectangle {
                            height: 5
                            width: Theme.paddingLarge
                            color: "transparent"
                        }
                        Text {
                            text : qsTr("Costs per 100 %1").arg(distanceunit)
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            font.bold: true
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                    Row { width: parent.height; height: Theme.paddingMedium; }
                    Row {
                        id:fuelper100Row
                        width: parent.width
                        Rectangle {
                            height: fuelbtext.height
                            width: parent.width
                            color: "transparent"

                            Text {
                                id: fuelbtext
                                width:parent.width/2
                                text : qsTr("Fuel:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                anchors.right:parent.right
                                text : (manager.car.budgetFuel*distanceunitfactor).toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                            //MouseArea {
                            //    id:fuelper100Mouse
                            //    anchors.fill:parent
                            //    onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"),{per100:true, type:"fuel"})
                            //}
                        }
                    }
                    Row {
                        id:billsper100Row
                        width: parent.width
                        Rectangle {
                            height: billsper100text.height
                            width: parent.width
                            color: "transparent"

                            Text {
                                id: billsper100text
                                width:parent.width/2
                                text : qsTr("Bills:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                anchors.right:parent.right
                                text : (manager.car.budgetCost*distanceunitfactor).toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                            //MouseArea {
                            //    id: billsper100Mouse
                            //    anchors.fill: parent
                            //    onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"), {per100: true})
                            //}
                        }
                    }
                    Row {
                        id:tiresper100Row
                        width: parent.width
                        Rectangle {
                            height: tiresper100text.height
                            width: parent.width
                            color: "transparent"

                            Text {
                                id: tiresper100text
                                width:parent.width/2
                                text : qsTr("Tires:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                anchors.right:parent.right
                                text : (manager.car.budgetTire*distanceunitfactor).toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                            //    MouseArea {
                            //        id: tiresper100Mouse
                            //        anchors.fill: parent
                            //        onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"), {per100: true})
                            //    }
                        }
                    }
                    Row {
                        id:buyingper100Row
                        width: parent.width
                        Rectangle {
                            height: buyingper100text.height
                            width: parent.width
                            color: "transparent"

                            Text {
                                id: buyingper100text
                                width:parent.width/2
                                text : qsTr("Buying:")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                anchors.right:parent.right
                                text : (manager.car.budgetInvest*distanceunitfactor).toFixed(2) + " " + manager.car.currency
                                font.pixelSize: Theme.fontSizeMedium
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                    Row {
                        id: totalper100Row
                        width: parent.width
                        Text {
                            width:parent.width/2
                            text : qsTr("Total:")
                            font.family: Theme.fontFamily
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignLeft
                        }
                        Text {
                            width:parent.width/2
                            text : (manager.car.budget*distanceunitfactor).toFixed(2) + " " + manager.car.currency
                            font.pixelSize: Theme.fontSizeMedium
                            color: Theme.primaryColor
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
    }
}
