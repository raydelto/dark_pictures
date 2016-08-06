#ifndef __DARKPICTURESAPP_H__
#define __DARKPICTURESAPP_H__

#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/Application>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Invocation;
    }
}


class DarkPicturesApp: public QObject
{
Q_OBJECT

public:
    DarkPicturesApp();
    ~DarkPicturesApp();


    Q_INVOKABLE
	void manipulatePhoto(const QString& fileName);

    Q_INVOKABLE
    void onShutterFired();

	Q_INVOKABLE
	void switchCamera(bool flip);

	Q_INVOKABLE
	void changeFlash();

	Q_INVOKABLE
	void switchMirror();

	Q_INVOKABLE
	int getFlashMode() const {
		return flashMode;
	}
	Q_INVOKABLE
	bool isMirror() const {
		return mirror;
	}


private:

	void showPhotoInCard(const QString fileName);
    bool rear;
    int flashMode;
    bool mirror;

};

#endif // ifndef __DARKPICTURESAPP_H__
