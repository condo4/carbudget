import QtQuick 2.6
import Sailfish.Silica 1.0
import harbour.carbudget 1.0

//import "functions.js" as Functions
//import "../components"

Page {
    id: page
    allowedOrientations: Orientation.All
    property string dir: "/"
    property bool initial: false // this is set to true if the page is initial page
    property bool remorsePopupActive: false // set to true when remorsePopup is active
    property bool remorseItemActive: false // set to true when remorseItem is active (item level)

    FileModel {
        id: fileModel
        dir: page.dir
        // page.status does not exactly work - root folder seems to be active always??
        active: page.status === PageStatus.Active
    }



    SilicaListView {
        id: fileList
        anchors.fill: parent
        clip: true

        model: fileModel

        VerticalScrollDecorator { flickable: fileList }

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        header: PageHeader {
            title: qsTr("Import File")
        }

        delegate: ListItem {
            id: fileItem
            menu: contextMenu
            width: ListView.view.width
            contentHeight: listLabel.height
            Row {
                Label {
                    id: listLabel
                    anchors.fill: parent
                    text: filename
                    color: Theme.primaryColor
                }
/*
                onClicked: {
                    if (model.isDir)
                        pageStack.push(Qt.resolvedUrl("DirectoryPage.qml"),
                                       { dir: fileModel.appendPath(listLabel.text) });
                    else
                        pageStack.push(Qt.resolvedUrl("FilePage.qml"),
                                       { file: fileModel.appendPath(listLabel.text) });
                }
                */
                MouseArea {
                    width: 90
                    height: parent.height
                    onClicked: {
                        fileModel.toggleSelectedFile(index);
                        selectionPanel.open = (fileModel.selectedFileCount > 0);
                        selectionPanel.overrideText = "";
                    }
                }


            }


            // context menu is activated with long press
            Component {
                 id: contextMenu
                 ContextMenu {
                     MenuItem {
                         text: qsTr("Cut")
                         onClicked: engine.cutFiles([ fileModel.fileNameAt(index) ]);
                     }
                 }
             }
        }

        // text if no files or error message
        Text {
            width: parent.width
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            horizontalAlignment: Qt.AlignHCenter
            y: -fileList.contentY + 100
            visible: fileModel.fileCount === 0 || fileModel.errorMessage !== ""
            text: fileModel.errorMessage !== "" ? fileModel.errorMessage : qsTr("No files")
            color: Theme.highlightColor
        }
    }

}
