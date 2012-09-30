import QtQuick 1.0
import org.nemomobile.grilo 0.1
import QtMultimediaKit 1.1

Item {
        width: 400
        height: 600

        ListView {
                width: parent.width
                height: parent.height * 2/3

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
//                                count: 2
//                                skip: 1
                                metadataKeys: [GriloBrowse.Title]
                                typeFilter: [GriloBrowse.Audio]
                                Component.onCompleted: {
                                        console.log(browser.supportedKeys);
                                        console.log(browser.slowKeys);
                                        refresh();
                                        console.log("Refreshing");
                                }

                                onAvailableChanged: console.log("Available ? " + available);
                                onBaseUriChanged: refresh();
                        }
                }

                delegate: Rectangle {
                        color: mouse.pressed ? "steelblue" : "white"

                        width: parent.width
                        height: 20

                        MouseArea {
                                id: mouse
                                anchors.fill: parent
                                onClicked: {
                                        console.log(mediaUrl);
                                        if (container) {
                                                browser.baseUri = mediaUrl;
                                        }
                                        else {
                                                audio.source = mediaUrl;
                                                audio.play();
                                                //                                        player.
                                        }
                                }

                                Text {
                                        id: label
                                        width: parent.width
                                        text: mediaTitle
                                        height: 30
                                }
                        }
                }
        }

        Audio {
                id: audio
                //                id: playMusic
                //                source: "music.wav"
        }
}
