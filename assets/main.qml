// Default empty project template
import bb.cascades 1.0
import bb.cascades.multimedia 1.0

Page {
    id: photoPage

    // A Container is used to gather visual items together.
    Container {        
        layout: DockLayout {
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            // This is the camera control that is defined in the cascades multimedia library.
            Camera {
                id: camera
                objectName: "myCamera"
                onTouch: {
                    if (event.isDown()) {
                        // Take photo
                        capturePhoto();
                    }
                }
                onShutterFired: {
                    darkPictures.onShutterFired();
                }
                // When the camera is opened we want to start the viewfinder
                onCameraOpened: {
                    camera.startViewfinder();
                }

                // There are loads of messages we could listen to here.
                // onPhotoSaved and onShutterFired are taken care of in the C++ code.
                onCameraOpenFailed: {
                    console.log("onCameraOpenFailed signal received with error " + error);
                }
                onViewfinderStartFailed: {
                    console.log("viewfinderStartFailed signal received with error " + error);
                }
                onViewfinderStopFailed: {
                    console.log("viewfinderStopFailed signal received with error " + error);
                }
                onPhotoCaptureFailed: {
                    console.log("photoCaptureFailed signal received with error " + error);
                }
                onPhotoSaveFailed: {
                    console.log("photoSaveFailed signal received with error " + error);
                }
                onPhotoSaved: {
                    darkPictures.manipulatePhoto(fileName);
                    // Will set the filename of the latest captured bomber photo in the ImageButton property.
                    setting.lastFileName = fileName;
                    // Makes the ImageButton visible when a photo is captured.
                    setting.visible = true;
                }
            }
        }

        //Button Container
        Container {
            background: Color.create("#c4e5c1")
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            ImageButton {
                id: btnImgSwitch
                defaultImageSource: "asset:///images/switch_camera_button.png"
                pressedImageSource: "asset:///images/switch_camera_button_pressed.png"
                onClicked: {
                    darkPictures.switchCamera(true);
                }

            }

            ImageButton {
                id: btnImgFlash
                defaultImageSource: "asset:///images/flash_off.png"
                pressedImageSource: "asset:///images/flash_button_pressed.png"
                onClicked: {
                    darkPictures.changeFlash();
                    switch (darkPictures.getFlashMode()) {
                        case 0:
                            btnImgFlash.defaultImageSource = "asset:///images/flash_off.png";
                            break;
                        case 1:
                            btnImgFlash.defaultImageSource = "asset:///images/flash_on.png";
                            break;
                        case 2:
                            btnImgFlash.defaultImageSource = "asset:///images/flash_automatic.png";
                            break;
                        case 3:
                            btnImgFlash.defaultImageSource = "asset:///images/flash_light.png";
                            break;
                    }
                }

            }

            ImageButton {
                id: btnImgNightVision
                defaultImageSource: "asset:///images/nv_on.png"
                pressedImageSource: "asset:///images/nv_pressed.png"
                onClicked: {
                    darkPictures.switchMirror();
                    if (darkPictures.isMirror()) {
                        btnImgNightVision.defaultImageSource = "asset:///images/nv_off.png";
                    } else {
                        btnImgNightVision.defaultImageSource = "asset:///images/nv_on.png";
                    }
                }

            }


        }

    } // content Container
}// Page
