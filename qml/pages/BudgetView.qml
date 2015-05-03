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
    Drawer {
        id: budgetviewDrawer
        anchors.fill: parent
        dock: Dock.Top
        open: false
        //backgroundSize: budgetView.contentHeight
    }
    PageHeader {
            id: header
             title: qsTr("Statistics")
         }
    Canvas {
        id: pieChart
        width: { return parent.width < parent.height ? parent.width/2 : parent.height/2 }
        height: { return parent.width < parent.height ? parent.width/2 : parent.height/2 }
        //width: budgetPage.width/2
        //height:budgetPage.height/2
        anchors.top: header.bottom
        anchors.left: parent.left
        onPaint: {
            var ctx = pieChart.getContext('2d')
            ctx.clearRect(0,0,width,height)
            var centerX = (width/2).toFixed(0)
            var centerY = (height/2).toFixed(0)
            var radius = (0.95*width/2).toFixed(0)
            var startangle=0.0
            var endangle=0.0
            var total = manager.car.budget_total
            var angle = 6.28/total
            ctx.lineWidth = 1
            endangle = manager.car.budget_cost_total * angle
            ctx.fillStyle = "darkgrey"
            ctx.beginPath()
            ctx.moveTo(centerX,centerY)
            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
            ctx.lineTo(centerX,centerY)
            ctx.fill()
            ctx.stroke()
            startangle=endangle
            endangle = startangle+manager.car.budget_fuel_total*angle
            ctx.fillStyle = "lightgrey"
            ctx.beginPath()
            ctx.moveTo(centerX,centerY)
            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
            ctx.lineTo(centerX,centerY)
            ctx.fill()
            ctx.stroke()
            startangle=endangle
            endangle = startangle+manager.car.budget_tire_total*angle
            ctx.fillStyle = "black"
            ctx.beginPath()
            ctx.moveTo(centerX,centerY)
            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
            ctx.lineTo(centerX,centerY)
            ctx.fill()
            ctx.stroke()
            startangle=endangle
            endangle = startangle+manager.car.budget_invest_total*angle
            ctx.fillStyle = "grey"
            ctx.beginPath()
            ctx.moveTo(centerX,centerY)
            ctx.arc(centerX,centerY,radius,startangle,endangle,false)
            ctx.lineTo(centerX,centerY)
            ctx.fill()
            ctx.stroke()
        }
    }
    Rectangle {
        id: pieChartLegend
        width: { return parent.width < parent.height ? parent.width/2 : parent.height/2 }
        height: { return parent.width < parent.height ? parent.width/2 : parent.height/2 }
        anchors.top: header.bottom
        anchors.right:  parent.right
        color: "Transparent"
        Column {
            anchors.centerIn: pieChartLegend
            width: parent.width- Theme.paddingMedium - Theme.paddingMedium
            Row {
                width:parent.width
                Rectangle {
                    color:"darkgrey"
                    height:billLegend.height
                    width:parent.width
                    Text {
                        id:billLegend
                        text : qsTr("Bills:") + " " + (manager.car.budget_cost_total*100/manager.car.budget_total ).toFixed(2) + "%"
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                }

            }
            Row {
                width:parent.width
                Rectangle {
                    color: "lightgrey"
                    width:parent.width
                    height:fuelLegend.height
                    Text {
                        id:fuelLegend
                        text : qsTr("Fuel:") + " " + (manager.car.budget_fuel_total*100/manager.car.budget_total).toFixed(2) + "%"
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
            Row {
                width:parent.width
                Rectangle {
                    color: "black"
                    width:parent.width
                    height:fuelLegend.height
                    Text {
                        id:tireLegend
                        text : qsTr("Tires:") + " " + (manager.car.budget_tire_total*100/manager.car.budget_total).toFixed(2) + "%"
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
            Row {
                width:parent.width
                Rectangle {
                    color: "grey"
                    width:parent.width
                    height:investLegend.height
                    Text {
                        id:investLegend
                        text : qsTr("Invest:") + " " + (manager.car.budget_invest_total*100/manager.car.budget_total).toFixed(2) + "%"
                        font.family: Theme.fontFamily
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
        }

    }
    SilicaFlickable {
        id: budgetView
        VerticalScrollDecorator {}
        anchors.top: pieChart.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        leftMargin: Theme.paddingMedium
        rightMargin: Theme.paddingMedium
        contentHeight: dataColumn.height
        Column {
            id: dataColumn
            width: parent.width- Theme.paddingMedium - Theme.paddingMedium
            Row {
                id:odoRow
                width: parent.width
                Text {
                    width:parent.width/2
                    text : qsTr("ODO ")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                }
                Text {
                    width:parent.width/2
                    text :  manager.car.maxdistance
                    font.family: "monospaced"
                    font.pixelSize: Theme.fontSizeSmall
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
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                }
                Text {
                    width:parent.width/2
                    text :  manager.car.maxdistance - manager.car.mindistance
                    font.family: "monospaced"
                    font.pixelSize: Theme.fontSizeSmall
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
                    color: "Transparent"
                    Column {
                        id: consumptionTable
                        width:parent.width
                        Row {
                            id: fuelRow
                            width: parent.width
                            Text {
                                text : qsTr("Fuel")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeMedium
                                font.bold: true
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                        Row {
                            id: fuelTotalRow
                            width: parent.width
                            Text {
                                width:parent.width/2
                                text : qsTr("Total: ")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                text :  manager.car.fueltotal.toFixed(2) + " l"
                                font.family: "monospaced"
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                        Row {
                            id:fuelAverageRow
                            width: parent.width
                            Text {
                                width:parent.width/2
                                text : qsTr("Average: ")
                                font.family: Theme.fontFamily
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                text :  manager.car.consumption.toFixed(2) + " l"
                                font.family: "monospaced"
                                font.pixelSize: Theme.fontSizeSmall
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
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                text :  manager.car.consumptionmin.toFixed(2) + " l"
                                font.family: "monospaced"
                                font.pixelSize: Theme.fontSizeSmall
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
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignLeft
                            }
                            Text {
                                width:parent.width/2
                                text :  manager.car.consumptionmax.toFixed(2) + " l"
                                font.family: "monospaced"
                                font.pixelSize: Theme.fontSizeSmall
                                color: Theme.primaryColor
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                    MouseArea {
                        id:consumptionMouse
                        anchors.fill:parent
                        onClicked: pageStack.push(Qt.resolvedUrl("ConsumptionStatistics.qml"))
                    }
                }
            }
            Row {
                id:costsRow
                width: parent.width
                Text {
                    text : qsTr("Costs")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                }
            }
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        anchors.right:parent.right
                        width:parent.width/2
                        text : manager.car.budget_fuel_total.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                    MouseArea {
                        id:fuelcostsMouse
                        anchors.fill:parent
                        onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"),{per100:false, type:"fuel"})
                    }
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        anchors.right:parent.right
                        text : manager.car.budget_cost_total.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                    MouseArea {
                        id: billcostsMouse
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"), {per100: false, type:"costs"})
                    }
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        anchors.right:parent.right
                        width:parent.width/2
                        text : manager.car.budget_tire_total.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                    /*
                    MouseArea {
                        id:tirecostsMouse
                        anchors.fill:parent
                        onClicked: pageStack.push(Qt.resolvedUrl("FuelStatistics.qml"))
                    }
                    */
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        anchors.right:parent.right
                        width:parent.width/2
                        text : manager.car.budget_invest_total.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
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
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                }
                Text {
                    width:parent.width/2
                    text : manager.car.budget_total.toFixed(2) + " " + manager.car.currency
                    font.family: "monospaced"
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                }
            }
            Row {
                id: per100Row
                width: parent.width
                Text {
                    text : qsTr("Costs per 100 Km")
                    font.family: Theme.fontFamily
                    font.pixelSize: Theme.fontSizeMedium
                    font.bold: true
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                }
            }
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        anchors.right:parent.right
                        text : manager.car.budget_fuel.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                    MouseArea {
                        id:fuelper100Mouse
                        anchors.fill:parent
                        onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"),{per100:true, type:"fuel"})
                    }
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        anchors.right:parent.right
                        text : manager.car.budget_cost.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                    MouseArea {
                        id: billsper100Mouse
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"), {per100: true})
                    }
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        anchors.right:parent.right
                        text : manager.car.budget_tire.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignRight
                    }
                    /*
                    MouseArea {
                        id: tiresper100Mouse
                        anchors.fill: parent
                        onClicked: pageStack.push(Qt.resolvedUrl("CostStatistics.qml"), {per100: true})
                    }
                    */
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
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        horizontalAlignment: Text.AlignLeft
                    }
                    Text {
                        width:parent.width/2
                        anchors.right:parent.right
                        text : manager.car.budget_invest.toFixed(2) + " " + manager.car.currency
                        font.family: "monospaced"
                        font.pixelSize: Theme.fontSizeSmall
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
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignLeft
                }
                Text {
                    width:parent.width/2
                    text : manager.car.budget.toFixed(2) + " " + manager.car.currency
                    font.family: "monospaced"
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }
}
