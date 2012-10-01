import QtQuick 1.0
import org.nemomobile.grilo 0.1
import QtMultimediaKit 1.1

Item {
        width: 400
        height: 600

        ListView {
                anchors.fill: parent

                GriloRegistry {
                        id: registry

                        Component.onCompleted: {
                                console.log("Registry is ready");
                                loadAll();
                        }
                }

                model: GriloModel {
                        id: griloModel

                        source: GriloBrowse {
                                id: browser
                                source: "grl-filesystem"
                                registry: registry
                                metadataKeys: [GriloBrowse.Title]
                                typeFilter: [GriloBrowse.Audio]
                                Component.onCompleted: {
                                        console.log(browser.supportedKeys);
                                        console.log(browser.slowKeys);
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
                                        console.log(media.url);
                                        if (media.container) {
                                                browser.baseMedia = media.serialize();
                                        }
                                        else {
                                                audio.source = media.url;
                                                audio.play();
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

        Audio {
                id: audio
        }
}
