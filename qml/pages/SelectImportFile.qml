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
/* Commented out, FolderListModel being in harbour.carbudget namespace. */
/* import Qt.labs.folderlistmodel 2.1 */

Page {
    id: page
    property string folderName
    property bool showFolderUp: false
    allowedOrientations: Orientation.All
    SilicaListView {
        anchors.fill: parent

        VerticalScrollDecorator {}

        header: PageHeader {
            title: qsTr("File to import")
        }

        model: folderModel

        FolderListModel {
            id: folderModel
            folder: folderName.length > 0 ? folderName : "file:///home/nemo"
            nameFilters: ["*.xml", "*.db", "*.cbg"]
            showDirsFirst: true
            showDotAndDotDot: showFolderUp
        }

        delegate: ListItem {
            id: fileDelegate
            enabled: fileName == "." ? false : true
            visible: fileName == "." ? false : true
            height: fileName == "." ? 0 : Theme.itemSizeMedium

            Row {
                anchors.fill: parent
                spacing: Theme.paddingMedium
                Rectangle {
                    height: parent.height
                    width: height
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Theme.rgba(Theme.primaryColor, 0.1) }
                        GradientStop { position: 1.0; color: "transparent" }
                    }
                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.PreserveAspectFit
                        source: {
                            var iconName
                            if(folderModel.isFolder(index)) {
                                iconName = "image://theme/icon-m-folder"
                            }
                            else {
                                iconName = "image://theme/icon-m-document"
                            }
                            return iconName
                        }
                    }
                }

                Label {
                    id: fileLabel
                    anchors.verticalCenter: parent.verticalCenter
                    text: fileName
                }
            }
            onClicked: {
                if(fileName == ".") {
                    // Just to be sure...
                }
                else if(fileName == "..") {
                    pageStack.navigateBack()
                }
                else if (folderModel.isFolder(index)) {
                    console.log("Clicked dir: " + fileName + " index: " + index)
                    //folderModel.folder = folderModel.folder + "/" + fileName
                    pageStack.push(Qt.resolvedUrl("SelectImportFile.qml"), {folderName: folderModel.folder + "/" + fileName, showFolderUp: true})
                }
                else {
                    console.log("Selectedted file: " + fileName + " index: " + index)
                    pageStack.push(Qt.resolvedUrl(importQMLPageName(fileName)),
                                   { filename: folderModel.folder+"/"+fileName });

                }
            }
        }
    }

    function importQMLPageName(name)
    {
        // Checks if a DB, an XML or a CBG file has been chosen and returns appropriate QML page
        if (name.indexOf(".db",name.length-3)!== -1) {
            return "FuelpadImport.qml"
        }
        if (name.indexOf(".xml",name.length-4)!== -1) {
            return "MycarImport.qml"
        }
        if (name.indexOf(".cbg",name.length-4)!== -1) {
            return "CarBudgetImport.qml"
        }
        // We really should implement some error handling here...
        return ""
    }
}

