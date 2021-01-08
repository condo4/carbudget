import QtQuick 2.9
import QtQuick.Controls 2.2
//import QtQuick.VirtualKeyboard 2.2

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("CarBudget")

    header: ToolBar {
        id: toolBar
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        contentHeight: toolButtonParam.implicitHeight

        ToolButton {
            id: toolButton
            text: "\u25C0"
            visible: stackView.depth > 1
            anchors.left: parent.left
            anchors.rightMargin: 0
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if(!!stackView.currentItem.backhook)
                {
                    stackView.currentItem.backhook()
                }
                else
                {
                    stackView.pop()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }


        ToolButton {
            id: toolButtonParam
            visible: !!stackView.currentItem.contextMenu
            text: "\u2630"
            anchors.right: parent.right
            anchors.rightMargin: 0
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                stackView.currentItem.contextMenu.popup()
            }
        }
    }

    StackView {
        id: stackView
        initialItem:  {
            var pageName;

            if(manager.cars.length == 0)
                pageName = "CarView.qml"
            else
                pageName = "CarEntry.qml"

            return Qt.resolvedUrl(pageName)
        }
        anchors.fill: parent

        Keys.onBackPressed: {
            event.accepted = true
            if (stackView.depth > 1) {
                stackView.pop()
            }
        }
    }
/*
    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
*/
}
