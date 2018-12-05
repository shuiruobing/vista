#ifndef PANORAMA_H
#define PANORAMA_H

#include <QList>
#include <QTimer>
#include <QObject>
#include "codec/decoder.h"
#include "codec/encoder.h"
#include "stitch/stitch_nvidia.h"
#include "stitch/stitch_cuda.h"
#include "configure.h"

class VSender;

class Panorama : public QObject
{
    Q_OBJECT
public:
    explicit Panorama(const cfg::PanoNode& pn, QObject *parent = nullptr);

    ~Panorama();

    bool open();

private slots:

    void onDecoderOpened(int id);

    void onDecoderError(int id, QString errStr);

    void onStitch();

    void onFatalError(const QString& str);

public slots:

    Q_INVOKABLE void onChangeMuxerUrl(QString outUrl);

private:

    void showDecoderInfo();

    bool libInit();

    bool createDevice(const char *deviceNo);

    bool createEncHWCtx(int width, int height);

    AVBufferRef* createHWCtx(int width, int height, AVBufferRef* pd);

    FramePtr getPanoFrame();

    bool fillhw(AVBufferRef* pCtx, FramePtr pFrame);

    void stdinThreadFunc();

    void processStdin(const QString& info);


private:
    const cfg::PanoNode param_;
    int c_inputCount_;
    const bool c_decodeHW_;
    const bool c_encodeHW_;
    const std::string c_deviceNo_;

    QTimer* pStitchTimer_;
    StreamBase inputInfo_;
    StreamBase outputInfo_;
    FramePtr pPanoFrame_{nullptr};
    std::vector<FramePtr> cameraFrames_;

    StitchCuda* pStitcher_;
    QList<Decoder*> decoders_;
    Encoder* mainEncoder_;
    std::vector<StreamInfo> streamList_;

    AVBufferRef* pDevice_{nullptr};
    AVBufferRef* pHWEncCtx_{nullptr};

    std::thread stdinThread_;
    std::atomic_bool stdinRunning_{true};

};

#endif // PANORAMA_H
