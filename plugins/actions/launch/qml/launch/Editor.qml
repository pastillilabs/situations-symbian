import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.launch 1.0

CommonDialog {
    titleText: qsTrId("action_launch_name")
    privateCloseIcon: true

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
        endCheck.checked = action.situationEnd
    }

    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    onStatusChanged: {
        typeSwitch.checkedButton = (action.isApp ? applicationButton : bookmarkButton)
        if(status === DialogStatus.Opening) action.isApp ? applications.update() : bookmarks.update()
    }

    onClickedOutside: {
        privateStyle.play(Symbian.PopupClose)
        reject()
    }

    LaunchAction {
        id: action
        situationEnd: endCheck.checked

        property bool isApp: type == LaunchAction.TypeApplication
    }

    ApplicationListModel {
        id: applications
    }

    BookmarkListModel {
        id: bookmarks
    }

    content: Item {
        width: platformContentMaximumWidth
        height: platformContentMaximumHeight

        CheckBox {
            id: endCheck
            x: 20
            y: 10
            text: qsTrId("action_at_situation_end")
        }

        ButtonRow {
            id: typeSwitch
            width: parent.width - 60
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: endCheck.bottom
            anchors.topMargin: 20

            Button {
                id: applicationButton
                iconSource: "qrc:/gfx/launch/applications.svg"
                onClicked: {
                    applications.update()
                    action.type = LaunchAction.TypeApplication
                }
            }
            Button {
                id: bookmarkButton
                iconSource: "qrc:/gfx/launch/internet.svg"
                onClicked: {
                    bookmarks.update()
                    action.type = LaunchAction.TypeBookmark
                }
            }
        }

        ListView {
            id: listView
            width: parent.width
            anchors.top: typeSwitch.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: 20
            clip: true

            model: action.isApp ? applications : bookmarks
            delegate: MenuItem {
                text: itemName
                onClicked: {
                    action.value = itemUid
                    accept()
                }
            }

            ScrollBar {
                anchors.right: listView.right
                flickableItem: listView
            }
        }
    }
}

