import QtQuick 2.4
import QtQuick.Controls 2.2
import harbour.carbudget 1.0

TankEntryForm {
    id: tankEntry
    property Tank tank

    Menu {
        id: contextMenu
        MenuItem {
            text: qsTr("Manage stations")
            onClicked: {
                stackView.push("StationView.qml")
            }
        }
        MenuItem {
            text: qsTr("Manage fuel types")
            onClicked: {
                stackView.push("FueltypeView.qml")
            }
        }
    }

    Component.onCompleted: {
        if(!!tank)
        {
            console.log("MODIFY")
            kminput.text = tank.distance;
            quantityinput.text = tank.quantity;
            priceinput.text = tank.price;
            unitpriceinput.text = tank.pricePerUnit;
            fullinput.checked = tank.full
            noteinput.text = tank.note
            title = qsTr("Edit Tank");
        }
        else
        {
            console.log("CREATE")
            kminput.clear();
            quantityinput.clear();
            priceinput.clear();
            unitpriceinput.clear();
            cbfuelType.currentIndex = 0
            cbstation.currentIndex = 0
            cbstation2.text = manager.car.stations[0].name
            fullinput.checked = true
            noteinput.clear()
            title = qsTr("Add Tank");
        }
    }


    /*
    kminput {
        text: (tank.distance / manager.car.distanceunitfactor).toFixed(0)
    }

    quantityinput {
        text: tank.quantity
        onAccepted: {
            tank.quantity = text
        }
    }

    priceinput {
        text: tank.price
    }

    unitpriceinput {
        text: tank.pricePerUnit
    }
    */



    /*
    property alias quantityinput: quantityinput
    property alias priceinput: priceinput
    property alias unitpriceinput: unitpriceinput
    property alias cbfuelType: cbfuelType
    property alias cbstation: cbstation
    property alias fullinput: fullinput
    property alias noteinput: noteinput
    */
}
