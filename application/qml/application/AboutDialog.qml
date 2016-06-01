import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    titleText: qsTrId("dialog_about_title")
    privateCloseIcon: true
    onClickedOutside: close()

    onStatusChanged: if(status === DialogStatus.Opening) flickable.contentY = 0

    content: Flickable {
        id: flickable
        width: parent.width
        height: Math.min(platformContentMaximumHeight, contentHeight + 1)
        contentWidth: column.width + column.x * 2
        contentHeight: column.height + column.y * 2
        flickableDirection: Flickable.VerticalFlick
        interactive: contentHeight > height

        Column {
            id: column
            x: 20
            y: 20
            width: flickable.width - 2 * x
            spacing: 20

            Column {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Image {
                    width: 50
                    height: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/gfx/application/situations.svg"
                    smooth: true
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTrId("application_name")
                }
            }

            Column {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: versionString
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Copyright © 2013 Pastilli Labs"
                }
            }

            Column {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 5

                Label {
                    width: parent.width
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTrId("dialog_about_warnings")
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                }
            }

            Column {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 20

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: qsTrId("dialog_about_contact")
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 40

                    Image {
                        width: 48
                        height: 48
                        sourceSize.width: width
                        sourceSize.height: height
                        scale: link1MouseArea.pressed ? 0.8 : 1
                        smooth: true
                        source: "qrc:/gfx/application/f_logo.png"

                        MouseArea {
                            id: link1MouseArea
                            anchors.fill: parent
                            onClicked: {
                                Qt.openUrlExternally("http://www.facebook.com/SituationsApp")
                                privateStyle.play(Symbian.BasicButton)
                            }
                            onPressed: privateStyle.play(Symbian.BasicButton)
                        }
                    }

                    Image {
                        width: 48
                        height: 48
                        sourceSize.width: width
                        sourceSize.height: height
                        scale: link2MouseArea.pressed ? 0.8 : 1
                        smooth: true
                        source: "qrc:/gfx/application/bird_blue_48.png"

                        MouseArea {
                            id: link2MouseArea
                            anchors.fill: parent
                            onClicked: {
                                Qt.openUrlExternally("http://www.twitter.com/SituationsApp")
                                privateStyle.play(Symbian.BasicButton)
                            }
                            onPressed: privateStyle.play(Symbian.BasicButton)
                        }
                    }

                    Image {
                        width: 48
                        height: 48
                        sourceSize.width: width
                        sourceSize.height: height
                        scale: link3MouseArea.pressed ? 0.8 : 1
                        smooth: true
                        source: "qrc:/gfx/application/p_logo.png"

                        MouseArea {
                            id: link3MouseArea
                            anchors.fill: parent
                            onClicked: {
                                Qt.openUrlExternally("http://www.situationsapp.com")
                                privateStyle.play(Symbian.BasicButton)
                            }
                            onPressed: privateStyle.play(Symbian.BasicButton)
                        }
                    }
                }
            }
        }
    }

    ScrollBar {
        parent: flickable
        anchors.right: parent.right
        flickableItem: parent
    }
}

