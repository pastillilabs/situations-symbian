import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: settingsPage
    tools: settingsTools

    property int situationIndex: 0
    property variant situation
    property variant situationConditions
    property variant situationActions

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: heading.height + tabGroup.contentHeight

        SettingsHeading {
            id: heading
            width: parent.width
            height: 100
        }

        TabGroup {
            id: tabGroup

            y: heading.height
            width: parent.width
            height: settingsPage.height - heading.height

            property int contentHeight: currentTab.contentHeight

            ConditionsPage {
                id: conditionsPage
            }

            ActionsPage {
                id: actionsPage
            }

            states: [
                State {
                    when: heading.index == 0
                    PropertyChanges {
                        target: tabGroup
                        currentTab: conditionsPage
                    }
                },
                State {
                    when: heading.index == 1
                    PropertyChanges {
                        target: tabGroup
                        currentTab: actionsPage
                    }
                }
            ]
        }
    }

    ScrollBar {
        anchors.right: flickable.right
        flickableItem: flickable
    }

    ToolBarLayout {
        id: settingsTools

        ToolButton {
            iconSource: "toolbar-back"
            onClicked: pageStack.pop()
        }
        ToolButton {
            iconSource: "toolbar-settings"
            onClicked: settingsDialog.open()
        }
    }

    Loader {
        id: settingsDialog

        function open() {
            source = "qrc:/qml/application/SituationSettingsDialog.qml"
            item.open()
        }
    }
}

