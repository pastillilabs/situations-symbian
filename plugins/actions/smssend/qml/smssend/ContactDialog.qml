import QtQuick 1.1
import com.nokia.symbian 1.1
import com.pastillilabs.situations.action.smssend 1.0

CommonDialog {
    id: contactDialog
    property int selectedIndex: -1
    property int selectedId: -1
    property string selectedNumber: ""
    property variant contactModel
    titleText: qsTrId("action_sms_send_select_recipient")
    privateCloseIcon: true

    onStatusChanged: {
        if(!contactModel && status === DialogStatus.Open) {
            contactModel = contactModelComponent.createObject(contactDialog)
            selectedIndex = contactModel.indexById(action.contactId)
        }
    }

    onAccepted: {
        action.contactId = selectedId
        action.contactNumber = selectedNumber
    }
    onClickedOutside: {
        privateStyle.play(Symbian.PopupClose)
        reject()
    }

    content: ListView {
        width: platformContentMaximumWidth
        height: platformContentMaximumHeight

        model: contactModel

        delegate: MenuItem {
            text: contactName
            onClicked: {
                selectedIndex = index
                selectedId = contactId

                numberDialog.model = contactModel.numbers(index)
                if(numberDialog.model[1]) numberDialog.open()
                else accept()
            }
        }

        ScrollBar {
            anchors.right: parent.right
            flickableItem: parent
        }
    }

    Component {
        id: contactModelComponent
        ContactListModel {}
    }

    SelectionDialog {
        id: numberDialog
        titleText: qsTrId("action_sms_send_select_number")
        onAccepted: contactDialog.accept()
        delegate: MenuItem {
            text: modelData
            onClicked: {
                selectedNumber = text
                numberDialog.accept()
            }

            Keys.onPressed: {
                if (event.key == Qt.Key_Up || event.key == Qt.Key_Down) scrollBar.flash()
            }
        }
    }
}

