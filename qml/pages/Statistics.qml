/**
 * CarBudget, Sailfish application to manage car cost
 *
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
 */


import QtQuick 2.0
import Sailfish.Silica 1.0
import jbQuick.Charts 1.0


Page {
    id: page

    Connections {
        target: manager.car
        onChartDataChanged: updatechart()
    }

    allowedOrientations: Orientation.All

    property int beginIndex: manager.car.beginIndex
    property int endIndex: manager.car.endIndex
    property real distanceunitfactor: 1
    property real distanceunit

    Component.onCompleted: {
        distanceunit = manager.car.distanceUnit
        if(distanceunit === "mi")
        {
            distanceunitfactor = 1.609
        }
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    SilicaFlickable {
        anchors.fill: parent

        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            enabled: true
            visible: true

            MenuItem {
                text: qsTr("Consumption")
                onClicked: {
                    manager.car.setChartTypeConsumption()
                    updatechart()
                }
            }

            MenuItem {
                text: qsTr("Costs")
                onClicked: {
                    manager.car.setChartTypeCosts()
                    updatechart()
                }
            }

            MenuItem {
                text: qsTr("Fuel price")
                onClicked: {
                    manager.car.setChartTypeOilPrice()
                    updatechart()
                }
            }
        }


        // Tell SilicaFlickable the height of its content.
        contentHeight: column.height

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Theme.paddingLarge
            PageHeader {
                title: qsTr("Statistics")
            }


            Label {
                x: Theme.paddingLarge
                text: manager.car.statisticType + " (" + qsTr("Distance") + ": " +
                       + (manager.car.tanks[endIndex].distance - manager.car.tanks[beginIndex].distance)/distanceunitfactor
                       + manager.car.distanceUnit + ")"
                font.pixelSize: Theme.fontSizeMedium
            }

            Chart {
                id: statisticsChart;
                width: page.width;
                height: page.height / 2;
                chartAnimated: false;
                chartOptions: ({scaleFontSize: 24, scaleFontColor: "#fff"});
                chartType: Charts.ChartType.LINE;
                chartData: manager.car.chartData;
            }

            Row {
                spacing: Theme.paddingSmall
                Button {
                    width: page.width / 3;
                    text: manager.car.tanks[beginIndex].date.toLocaleDateString(Qt.locale(),"yyyy/MM/dd");
                    onClicked: pageStack.push(Qt.resolvedUrl("SelectTankDate.qml"), { type: 0, theIndex:beginIndex })
                }

                Text{
                    width: page.width / 3.5;
                }

                Button {
                    width: page.width / 3;
                    text: manager.car.tanks[endIndex].date.toLocaleDateString(Qt.locale(),"yyyy/MM/dd");
                    onClicked: pageStack.push(Qt.resolvedUrl("SelectTankDate.qml"), { type: 1, theIndex:endIndex })
                }
            }
        }
    }

    function updatechart(){
          statisticsChart.chartData = manager.car.chartData;
          statisticsChart.update();
          statisticsChart.repaint();
    }
}
