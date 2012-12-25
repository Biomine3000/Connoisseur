import QtQuick 2.0


Item {
    id: delegate

    function isImage() {
        return type.ofContentType("image");
    }

    function delegateHeight(imageSize) {
        if (isImage()) {
            return imageSize;
        } else
            return basicMargin * 4;
    }

    width: parent.width
    height: delegateHeight(parent.width / 3)

    Rectangle {
        id: wrapper

        height: parent.height - basicMargin
        width: parent.width

        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        color: "white"
        radius: 2
        border.width: 2
        border.color: "gray"

        Image {
            id: image
            fillMode: Image.PreserveAspectFit

            onStatusChanged: {
                if (image.status != Image.Ready) {
                    delegate.height = delegateHeight(delegate.parent.width / 3);
                }
            }

            function assignSource() {
                if (isImage())
                    return "image://objects/" + metadata.id
                else
                    return "image://objects/" + "no_image"

            }

            source: assignSource()

            height: wrapper.height - basicMargin * 2
            width: height

            anchors.left: parent.left
            anchors.leftMargin: basicMargin // height > width ? 0 : basicMargin
            anchors.top: parent.top
            anchors.topMargin: width > height ? basicMargin : 0

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onClicked: {
                    if (mouse.button === Qt.LeftButton) {
                        var width = delegate.width - 2 * basicMargin;
                        delegate.height = delegateHeight(width);
                        // image.height = image.sourceSize.height;
                        image.width = width;
                    } else if (mouse.button === Qt.RightButton) {
                        var width = delegate.parent.width / 3;
                        delegate.height = delegateHeight(width);
                        image.width = width;
                    }
                }


            }
        }

        Text {
            id: text

            anchors.left: parent.left
            anchors.leftMargin: basicMargin
            anchors.top: image.bottom
            // anchors.topMargin: basicMargin

            function renderChannelAndSender(channel, sender) {
                return sender + "@" + channel;
            }

            function renderTextual() {
                if (metadata.event !== undefined &&
                        metadata.event.indexOf("routing/") === 0) {
                    return "Routing object " + metadata.event;
                } else if (type.ofContentType("text")) {
                    if (metadata.channel !== undefined &&
                            metadata.sender !== undefined) {
                        return renderChannelAndSender(metadata.channel,
                                                      metadata.sender) +
                                "> " + payload;
                    } else
                        return payload;
                } else if (type.ofContentType("image")) {
                    return "Image of type " + type.subType();
                } else if (type.ofType("biomine", "url")) {
                    if (metadata.channel !== undefined &&
                            metadata.sender !== undefined) {
                        return renderChannelAndSender(metadata.channel,
                                                      metadata.sender) + "> " +
                                "<a href=\"" + payload + "\">" + payload + "</a>";
                    } else {
                        return "<a href=\"" + payload + "\">" + payload + "</a>";
                    }

                }

                return id;
            }

            text: renderTextual()
        }

    }
}
