import QtQuick 2.9
import QtQuick.Controls 2.2

CarEntryForm {
    id: carEntryPage
    property alias contextMenu: contextMenu

    title: {
        if (manager.car.make.length > 1)
            return (manager.car.make + " " + manager.car.model)
        else
            return manager.car.name
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: qsTr("Settings")
            onClicked: {
                stackView.push("Settings.qml")
            }
        }
        MenuItem {
            text: qsTr("Select another car")
            onClicked: {
                stackView.push("CarView.qml")
            }
        }
        MenuItem {
            text: qsTr("About")
            onClicked: {
                stackView.push("About.qml")
            }
        }
    }

    distancetext: qsTr("Distance: %L1 ~ %L2 %3").arg(
                      (manager.car.minDistance / manager.car.distanceunitfactor).toFixed(
                          0)).arg(
                      (manager.car.maxDistance / manager.car.distanceunitfactor).toFixed(
                          0)).arg(manager.car.distanceUnit)

    constext: if (manager.car.consumptionUnit === "l/100km") {
                  qsTr("Consumption: %L1 l/100km").arg(
                              manager.car.consumption.toFixed(2))
              } else if (manager.car.consumptionUnit === "mpg") {
                  qsTr("Consumption: %L1 mpg").arg(
                              (manager.car.consumptionfactor * 1 / manager.car.consumption).toFixed(
                                  2))
              }

    lasttext: if (manager.car.consumptionUnit === "l/100km") {
                  qsTr("Last: %L1 l/100km").arg(
                              manager.car.consumptionLast.toFixed(2))
              } else if (manager.car.consumptionUnit === "mpg") {
                  qsTr("Last: %L1 mpg").arg(
                              ((manager.car.consumptionfactor * 1
                                / manager.car.consumptionLast)).toFixed(
                                  2))
              }
    lastcolor: {
        if (manager.car.consumptionLast === 0)
            return primaryColor

        var cLast = manager.car.consumptionLast
        var cAvg = manager.car.consumption
        if (cLast < cAvg * 0.92)
            return "#00FF00"
        if (cLast < cAvg * 0.94)
            return "#40FF00"
        if (cLast < cAvg * 0.96)
            return "#80FF00"
        if (cLast < cAvg * 0.98)
            return "#C0FF00"
        if (cLast < cAvg * 1.00)
            return "#FFFF00"
        if (cLast < cAvg * 1.02)
            return "#FFC000"
        if (cLast < cAvg * 1.04)
            return "#FF8000"
        if (cLast < cAvg * 1.06)
            return "#FF4000"
        if (cLast < cAvg * 1.08)
            return "#FF2000"
        return "#FF0000"
    }


    btnTank {
        onClicked: stackView.push("TankView.qml")
    }

    btnCost {
        onClicked: stackView.push("CostView.qml")
    }

    btnTire {
        onClicked: stackView.push("TireView.qml")
    }

    btnStats {
        //color: (manager.car.tireMounted < manager.car.numTires)?(Theme.highlightColor):(Theme.primaryColor)
        onClicked: stackView.push("TireView.qml")
    }
}
