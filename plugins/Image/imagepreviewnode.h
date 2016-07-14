#ifndef IMAGEPREVIEWNODE_H
#define IMAGEPREVIEWNODE_H

#include <QObject>

#include <QDockWidget>
#include <QLabel>

#include <fugio/core/variant_interface.h>
#include <fugio/image/image_interface.h>

#include <fugio/nodecontrolbase.h>

class ImagePreview;

class ImagePreviewNode : public fugio::NodeControlBase
{
	Q_OBJECT

	Q_CLASSINFO( "Author", "Alex May" )
	Q_CLASSINFO( "Version", "1.0" )
	Q_CLASSINFO( "Description", "Displays an image" )
	Q_CLASSINFO( "URL", "http://wiki.bigfug.com/Image_Preview" )
	Q_CLASSINFO( "Contact", "http://www.bigfug.com/contact/" )

public:
	Q_INVOKABLE explicit ImagePreviewNode( QSharedPointer<fugio::NodeInterface> pNode );

	virtual ~ImagePreviewNode( void );

	//-------------------------------------------------------------------------
	// NodeControlInterface

	virtual QWidget *gui( void ) Q_DECL_OVERRIDE;

	virtual void inputsUpdated( qint64 pTimeStamp ) Q_DECL_OVERRIDE;

	virtual bool initialise( void ) Q_DECL_OVERRIDE;

	virtual bool deinitialise( void ) Q_DECL_OVERRIDE;

	virtual void loadSettings( QSettings &pSettings ) Q_DECL_OVERRIDE;

	virtual void saveSettings( QSettings &pSettings ) const Q_DECL_OVERRIDE;

protected slots:
	void onShowClicked();

	void onContextFrame( void );

private:
	QSharedPointer<fugio::PinInterface>			 mPinImage;

	QDockWidget								*mDockWidget;
	ImagePreview							*mGUI;

	Qt::DockWidgetArea						 mDockArea;
};

#endif // IMAGEPREVIEWNODE_H