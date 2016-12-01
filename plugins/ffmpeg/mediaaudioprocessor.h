#ifndef MEDIAAUDIOPROCESSOR_H
#define MEDIAAUDIOPROCESSOR_H

#include <QObject>
#include <QVector>
#include <QMutex>

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

extern "C"
{
	#include <libavutil/rational.h>
	#include <libavcodec/avcodec.h>
	#include <libavformat/avformat.h>
	#include <libavutil/opt.h>
	#include <libswscale/swscale.h>
#if defined( TL_USE_LIB_AV )
	#include <libavutil/samplefmt.h>
	#include <libavutil/audioconvert.h>
#else
	#include <libswresample/swresample.h>
#endif
}

#include "audiobuffer.h"

class MediaAudioProcessor : public QObject
{
	Q_OBJECT
	Q_FLAGS( Options )

public:
	explicit MediaAudioProcessor( void );

	virtual ~MediaAudioProcessor( void );

	enum Option {
		NoOptions = 0x0,
		Preload = ( 1 << 0 ),
		Calculate = ( 1 << 1 )
	};

	Q_DECLARE_FLAGS(Options, Option)

	qreal timeToWaveformIdx( qreal pTimeStamp ) const;

	qreal wavL(qreal pTimeStamp) const;
	qreal wavR(qreal pTimeStamp) const;

	void mixAudio( qint64 pSamplePosition, qint64 pSampleCount, int pChannelOffset, int pChannelCount, void **pBuffers, float pVol ) const;

	bool preloaded( void ) const;

	inline Options options( void ) const
	{
		return( mOptions );
	}

public slots:
	bool loadMedia( const QString &pFileName );

	void process( void );

	void abort( void );

	void clearAudio();

	void setOptions( Options pOptions )
	{
		mOptions = pOptions;
	}

signals:
	void resultReady(const QString &result);

private:
	void processAudioFrame( void );

	static QString av_err( const QString &pHeader, int pErrorCode );

private:
	Options				mOptions;

	QString				 mFileName;
	AVFormatContext		*mFormatContext;
	int					 mAudioStream;
	AVCodecContext		*mAudioCodecContext;
	AVCodec				*mAudioCodec;
	AVFrame				*mFrameSrc;
	AVFrame				*mFrameDst;
	int					 mPacketSize;
	AVPacket			 mPacket;

	SwsContext			*mScaleContext;

#if defined( TL_USE_LIB_AV )
#else
    SwrContext			*mSwrContext;
#endif

	qreal 				 mAudioPTS;
	qreal				 mAudioSamplePTS;
	qreal				 mFrameReadDuration;
	qreal				 mFrameReadPTS;
	qreal				 mFrameCurrDuration;
	qreal				 mFrameCurrPTS;
	qreal				 mFrameDurationAverage;

	int					 mFrameIdx1;
	int					 mFrameIdx2;
	qreal				 mFrameMix;

	AVCodecParserContext	*AudioParserContext;
	uint8_t					*ParserData;
	int						 ParserSize;
	AVPacket				 mParserPacket;

	int						 mFrameFinished;

	bool					 mErrorState;

	uint8_t					*SrcDat;
	int						 SrcLen;

	qreal					 mDuration;

	mutable QMutex			 mAudDatLck;

	QList<AudioBuffer>		 mAudDat;

	bool					 mCalculated;
	bool					 mPreloaded;
	bool					 mDecodeI;
	bool					 mDecodeB;

	class AudPrv
	{
	public:
		AudPrv( void ) : mAudSm( 0 ), mAudMax( 0 )
		{

		}

		QVector<float>			mAudWav;
		float					mAudSm;
		float					mAudMax;
	};

	QVector<AudPrv>			mAudPrv;

	uint64_t				mChannelLayout;
	int						mChannels;
	int						mSampleRate;
	enum AVSampleFormat		mSampleFmt;

	AudioBuffer			mAD;

	QVector<QVector<float>>	mAudBuf;

	volatile bool			mProcessAborted;

	qreal					mAudPts;
	qreal					mAudNxt;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( MediaAudioProcessor::Options )

#endif // MEDIAAUDIOPROCESSOR_H
