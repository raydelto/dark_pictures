// Default empty project template
#include "darkpicturesapp.hpp"
#include <QtGui/QImage>
#include <QtGui/QImageReader>

#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/multimedia/Camera>

#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

#include <bps/soundplayer.h>
#include <bb/cascades/multimedia/CameraSettings>
#include <bb/cascades/multimedia/CameraFlashMode>
using namespace bb::cascades;
using namespace bb::cascades::multimedia;
using namespace bb::system;
Camera *camera = NULL;
CameraSettings* cameraSettings = NULL;
DarkPicturesApp::DarkPicturesApp() {
	qmlRegisterType<Camera>("bb.cascades.multimedia", 1, 0, "Camera");
	QmlDocument *qml = QmlDocument::create("asset:///main.qml");

	qml->setContextProperty("darkPictures", this);

	if (!qml->hasErrors()) {
		Page *appPage = qml->createRootObject<Page>();

		if (appPage) {

			Application::instance()->setScene(appPage);
			camera = appPage->findChild<Camera*>("myCamera");
			cameraSettings = new CameraSettings();
			cameraSettings->setFlashMode(CameraFlashMode::Off);
			cameraSettings->setFocusMode(CameraFocusMode::ContinuousAuto);
			camera->applySettings(cameraSettings);

			camera->open(CameraUnit::Rear);
			rear = true;
			mirror = false;
			flashMode = 0;
		}
	}
}

DarkPicturesApp::~DarkPicturesApp() {

}

void DarkPicturesApp::switchCamera(bool flip = true) {
	if (flip) {
		rear = !rear;
	}
	if (rear) {
		camera->close();
		camera->applySettings(cameraSettings);
		camera->open(CameraUnit::Rear);
	} else {
		camera->close();
		camera->open(CameraUnit::Front);

	}
}


void DarkPicturesApp::switchMirror() {
	mirror = !mirror;
}


void DarkPicturesApp::changeFlash() {
	if (flashMode > 2) {
		flashMode = 0;
	} else {
		flashMode++;
	}
	switch (flashMode) {
	case 0:
		cameraSettings->setFlashMode(CameraFlashMode::Off);
		break;
	case 1:
		cameraSettings->setFlashMode(CameraFlashMode::On);
		break;
	case 2:
		cameraSettings->setFlashMode(CameraFlashMode::Auto);
		break;
	case 3:
		cameraSettings->setFlashMode(CameraFlashMode::Light);
		break;
	}

	if (rear) {
		camera->applySettings(cameraSettings);
		switchCamera(false);
	}

}

void DarkPicturesApp::onShutterFired() {
	soundplayer_play_sound("event_camera_shutter");
}


void DarkPicturesApp::showPhotoInCard(const QString fileName) {
	InvokeManager manager;
	InvokeRequest request;
	request.setUri(QUrl::fromLocalFile(fileName));
	request.setTarget("sys.pictures.card.previewer");
	request.setAction("bb.action.VIEW");
	InvokeTargetReply *targetReply = manager.invoke(request);
	manager.setParent(this);
	if (targetReply == NULL) {
		qDebug() << "InvokeTargetReply is NULL: targetReply = " << targetReply;
	} else {
		targetReply->setParent(this);
	}
}

void DarkPicturesApp::manipulatePhoto(const QString &fileName) {
	QImageReader reader;
	reader.setFileName(fileName);
	QImage image = reader.read();
	QSize imageSize = image.size();
	QColor color;
	if (mirror) {
		//DO NOTHING
		//image = image.mirrored(true, false);
	} else {
		// Night Vision Algorithm
		for (int i = 0; i < imageSize.width(); i++) {
			for (int ii = 0; ii < imageSize.height(); ii++) {
				color = QColor(image.pixel(i, ii));
				color.setRed(color.blue());
				color.setGreen(color.green());
				color.setBlue(color.red());
				image.setPixel(i, ii, color.rgb());
			}
		}
	}
	image.save(fileName, "JPG");
	// Show the photo by using this function that take use of the InvokeManager.
	showPhotoInCard(fileName);
}

