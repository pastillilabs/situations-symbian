import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.close 1.0

CommonDialog {
    titleText: qsTrId("action_close_name")
    privateCloseIcon: true

    Component.onCompleted: if(itemIndex >= 0) {
        action.data = group.getActionData(itemIndex)
        endCheck.checked = action.situationEnd
    }

    onAccepted: {
        if(itemIndex >= 0) client.reqSetActionData(situationIndex, groupIndex, itemIndex, action.data)
        else client.reqAddAction(situationIndex, groupIndex, action.data)
    }

    onStatusChanged: if(status === DialogStatus.Opening) applications.updateApps()

    onClickedOutside: {
        privateStyle.play(Symbian.PopupClose)
        reject()
    }

    CloseAction {
        id: action
        situationEnd: endCheck.checked
    }

    ApplicationListModel {
        id: applications
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

        ListView {
            id: listView
            width: parent.width
            anchors.top: endCheck.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: 20
            clip: true

            model: applications
            delegate: MenuItem {
                text: applicationName
                onClicked: {
                    action.value = applicationUid
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

