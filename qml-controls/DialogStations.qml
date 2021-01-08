import QtQuick 2.9
import QtQuick.Controls 2.2

DialogStationsForm {

    RoundButton {
        text: qsTr("+")
        highlighted: true
        anchors.margins: 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: {
            //tankDialog.createTank()
        }
    }
}
