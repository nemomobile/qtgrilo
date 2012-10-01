import QtQuick 1.0
import org.nemomobile.grilo 0.1
import QtMultimediaKit 1.1

Item {
        width: 400
        height: 600

        ListView {
                id: list
                width: parent.width
                height: parent.height * 2/3

                GriloRegistry {
                        id: registry
                        configurationFile: "youtube.ini"
                        Component.onCompleted: {
                                console.log("Registry is ready");
                                loadAll();
                        }
                }

                model: GriloModel {
                        id: griloModel

                        source: GriloBrowse {
                                id: browser
                                source: "grl-youtube"
                                registry: registry
                                metadataKeys: [GriloBrowse.Title, GriloBrowse.Url]
                                count: 10
                                Component.onCompleted: {
                                        refresh();
                                        console.log("Refreshing");
                                }

                                onAvailableChanged: console.log("Available ? " + available);
                                onBaseMediaChanged: refresh();
                        }
                }

                delegate: Rectangle {
                        color: mouse.pressed ? "steelblue" : "white"

                        width: parent.width
                        height: 40

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: {
                                        if (media.container) {
                                                browser.baseMedia = media.serialize();
                                        }
                                        else {
                                                video.source = media.url;
                                                video.play();
                                                console.log(media.url);
                                        }
                                }

                                Text {
                                        id: label
                                        width: parent.width
                                        text: media.title
                                        height: 30
					                    font.pixelSize: 36
                                        anchors.verticalCenter: parent.verticalCenter
                                }
                        }
                }
        }

        Video {
                id: video
                anchors.top: list.bottom
                height: parent.height / 3
                width: parent.width
                anchors.horizontalCenter: paremt.horizontalCenter
        }
}
