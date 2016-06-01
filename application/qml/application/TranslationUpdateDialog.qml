import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    id: dialog
    titleText: qsTrId("update_localisation")
    buttonTexts: [qsTrId("dialog_softkey_cancel")]

    property bool cancel: true

    onStatusChanged: {
        if(status === DialogStatus.Opening) {
            label.text = qsTrId("updating_localisation")
            indicator.visible = true
            buttonTexts = [qsTrId("dialog_softkey_cancel")]
        }
    }

    onButtonClicked: cancel ? reject() : accept()
    onAccepted: Qt.quit()
    onRejected: translationUpdater.cancel()

    Connections {
        target: translationUpdater
        onReady: {
            label.text = qsTrId("localisation_updated")
            indicator.visible = false
            cancel = false
            buttonTexts = [qsTrId("dialog_softkey_ok")]
            client.reqUpdateTranslation()
        }
        onNotAvailable: {
            label.text = qsTrId("localisation_not_available")
            indicator.visible = false
        }
    }

    content: Item {
        width: parent.width
        height: 200

        Column {
            width: parent.width - 40
            height: childrenRect.height
            anchors.centerIn: parent
            spacing: 20

            BusyIndicator {
                id: indicator
                anchors.horizontalCenter: parent.horizontalCenter
                running: visible
            }

            Label {
                id: label
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
            }
        }
    }
}

