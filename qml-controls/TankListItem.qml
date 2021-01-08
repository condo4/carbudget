import QtQuick 2.4
import QtQuick.Controls 2.2
import harbour.carbudget 1.0

TankListItemForm {
    property Tank tank
    property variant consumptionAvg :  [manager.car.consumption * 0.92,
                                        manager.car.consumption * 0.94,
                                        manager.car.consumption * 0.96,
                                        manager.car.consumption * 0.98,
                                        manager.car.consumption * 1.00,
                                        manager.car.consumption * 1.02,
                                        manager.car.consumption * 1.04,
                                        manager.car.consumption * 1.06,
                                        manager.car.consumption * 1.08]

    zone {
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressAndHold: {
            contextMenu.popup()
        }
        onClicked: {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }
    }

    Menu
    {
         id: contextMenu
         MenuItem {
             text: "Modify";
             onPressed: {
                 stackView.push("TankEntry.qml", {tank: tank})
             }
         }
         MenuItem { text: "Delete (long press)"; onPressAndHold: {manager.car.delTank(tank)}}
    }

    distance {
        text:  (tank.distance / manager.car.distanceunitfactor).toFixed(0) + ((tank.newDistance > 0)?(manager.car.distanceUnit + " (+" + (tank.newDistance / manager.car.distanceunitfactor).toFixed(0)+manager.car.distanceUnit+")"):(manager.car.distanceUnit));
    }

    date {
        text: tank.date.toLocaleDateString(Qt.locale(),"yyyy/MM/dd");
    }

    unitPrice {
        text: tank.pricePerUnit.toFixed(3)+manager.car.currency + "/l";
    }

    quantity {
        text: tank.quantity + "l"
    }

    price {
        text: tank.price + manager.car.currency;
    }

    consumtion {
        text: ( manager.car.consumptionUnit === "mpg")?(("%L1 mpg").arg((consumptionfactor * 1/model.modelData.consumption).toFixed(2))):(tank.consumption.toFixed(2) +  manager.car.consumptionUnit)
        visible: tank.consumption > 0
        color: {
            if(tank.consumption < consumptionAvg[0]) return "#00FF00"
            if(tank.consumption < consumptionAvg[1]) return "#40FF00"
            if(tank.consumption < consumptionAvg[2]) return "#80FF00"
            if(tank.consumption < consumptionAvg[3]) return "#C0FF00"
            if(tank.consumption < consumptionAvg[4]) return "#FFFF00"
            if(tank.consumption < consumptionAvg[5]) return "#FFC000"
            if(tank.consumption < consumptionAvg[6]) return "#FF8000"
            if(tank.consumption < consumptionAvg[7]) return "#FF4000"
            if(tank.consumption < consumptionAvg[8]) return "#FF2000"
            return "#FF0000"
        }
    }
}
