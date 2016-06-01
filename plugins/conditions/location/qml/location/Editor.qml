import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.location 1.2
import com.pastillilabs.situations.condition.location 1.0

CommonDialog {
    titleText: qsTrId("condition_location_name")

    Component.onCompleted: {
        if(itemIndex >= 0) {
            condition.data = group.getConditionData(itemIndex)
            follow = false
        }
        currentSource.start()
        if(follow) condition.updateCurrent();
    }

    Component.onDestruction: currentSource.stop()

    onButtonClicked: index === 0 ? accept() : reject()
    onAccepted: {
        if(itemIndex >= 0) client.reqSetConditionData(situationIndex, groupIndex, itemIndex, condition.data)
        else client.reqAddCondition(situationIndex, groupIndex, condition.data)
    }

    property bool follow: true

    onFollowChanged: if(follow) condition.updateCurrent()

    onStatusChanged: {
        if(status == DialogStatus.Open) {
            mapLoader.sourceComponent = mapComponent
            buttonTexts = [qsTrId("dialog_softkey_ok"), qsTrId("dialog_softkey_cancel")]
        }
    }

    LocationCondition {
        id: condition

        function updateCurrent() {
            latitude = currentSource.latitude
            longitude = currentSource.longitude
        }
    }

    LocationSource {
        id: currentSource
        updateInterval: 1000
        gps: condition.gps

        onPositionChanged: if(follow) condition.updateCurrent()
    }

    content: Item {
        width: platformContentMaximumWidth
        height: platformContentMaximumHeight

        Loader {
            id: mapLoader
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: mapLoader.status != Loader.Ready
            visible: running
        }

        Item {
            anchors.fill: parent
            anchors.margins: 10
            visible: mapLoader.status == Loader.Ready

            CheckBox {
                anchors.top: parent.top
                anchors.left: parent.left
                checked: condition.gps
                text: qsTrId("location_checkbox_use_gps")
                platformInverted: true
                onClicked: condition.gps = !condition.gps
            }

            Button {
                property bool on: true
                width: height
                anchors.top: parent.top
                anchors.right: parent.right
                checked: follow
                iconSource: on ? "qrc:/gfx/location/position.svg" : ""
                onClicked: if(currentSource.valid) follow = true

                Timer {
                    interval: 1000
                    repeat: true
                    running: !currentSource.valid
                    onTriggered: parent.on = !parent.on
                    onRunningChanged: parent.on = !running
                }
            }

            Column {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10

                Button {
                    width: height
                    text: "+"
                    onClicked: if(mapLoader.item.zoomLevel < 17) mapLoader.item.zoomLevel += 1
                }

                Button {
                    width: height
                    text: "-"
                    onClicked: if(mapLoader.item.zoomLevel > 1) mapLoader.item.zoomLevel -= 1
                }
            }
        }
    }

    Component {
        id: mapComponent

        Map {
            id: map
            size.width: platformContentMaximumWidth
            size.height: platformContentMaximumHeight

            property alias radius: radiusCircle.radius

            plugin: Plugin {
                name: "nokia"
                parameters: [
                    PluginParameter {
                        name: "mapping.app_id";
                        value: "dBcYfF32i2r7ImPktKBR";
                    },
                    PluginParameter {
                        name: "mapping.token";
                        value: "ELulg6mbNN2GxeHOEdIhiQ";
                    },
                    PluginParameter {
                        name: "logo.position";
                        value: "bottom.left";
                    }
                ]
            }

            zoomLevel: condition.zoomLevel
            center: Coordinate {
                latitude: condition.latitude
                longitude: condition.longitude
            }

            onZoomLevelChanged: condition.zoomLevel = zoomLevel
            onCenterChanged: radiusCircle.center = center

            MapCircle {
                center: positionImage.coordinate
                radius: currentSource.accuracy
                color: "#10FF0000"
            }

            MapImage {
                id: positionImage
                offset.x: -5
                offset.y: -5
                coordinate: Coordinate {
                    latitude: currentSource.latitude
                    longitude: currentSource.longitude
                }
                source: "qrc:/gfx/location/position.svg"
                visible: currentSource.valid
            }

            MapCircle {
                id: radiusCircle
                radius: condition.radius * 5
                color: "#80009EAD"

                MapMouseArea {
                    onPositionChanged: {
                        if(mouse.button == Qt.LeftButton) {
                            var radius = Math.max(10, Math.round(radiusCircle.center.distanceTo(mouse.coordinate)))
                            radiusCircle.radius = radius
                            condition.radius = radius
                        }
                    }
                }

                Component.onCompleted: radius = condition.radius
                Behavior on radius { NumberAnimation { duration: 500; easing.type: Easing.OutBack } }
            }

            MapMouseArea {
                property int lastX: -1
                property int lastY: -1

                onPressed: {
                    lastX = mouse.x
                    lastY = mouse.y
                }
                onReleased: {
                    lastX = -1
                    lastY = -1
                }
                onPositionChanged: {
                    if(mouse.button == Qt.LeftButton) {
                        if((lastX != -1) && (lastY != -1)) {
                            var dx = mouse.x - lastX
                            var dy = mouse.y - lastY
                            map.pan(-dx, -dy)
                            follow = false
                            condition.latitude = map.center.latitude
                            condition.longitude = map.center.longitude
                        }

                        lastX = mouse.x
                        lastY = mouse.y
                    }
                }
                onDoubleClicked: {
                    follow = false
                    map.center = mouse.coordinate

                    if(map.zoomLevel < map.maximumZoomLevel) map.zoomLevel += 1
                    lastX = -1
                    lastY = -1
                }
            }
        }
    }
}

