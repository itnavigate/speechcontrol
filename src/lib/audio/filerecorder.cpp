/*
    Copyright (c) 2012, Synthetic Intellect Institute <sii@thesii.org>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Synthetic Intellect Institute nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Synthetic Intellect Institute <sii@thesii.org> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Synthetic Intellect Institute <sii@thesii.org> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "filerecorder.hpp"
#include "devicemanager.hpp"

#include <QMessageBox>
#include <QDebug>

#include <QGlib/Error>
#include <QGlib/Connect>

using namespace SpeechControl::Audio;

QMap<QString, QString> FileRecorder::muxers;

void FileRecorder::setup()
{
    FileRecorder::muxers.insert("Ogg", "oggmux");
    FileRecorder::muxers.insert("Avi", "avimux");
    FileRecorder::muxers.insert("Wav", "wavenc");
}

void FileRecorder::findDevices()
{
    const char* srcElementName = "autoaudiosrc";
    QGst::ElementPtr src = QGst::ElementFactory::make(srcElementName);
    
    if (!src) {
        QMessageBox::critical(NULL, tr("Error"),
                              tr("Failed to create element \"%1\". Make sure you have "
                              "gstreamer-plugins-good installed").arg(srcElementName));
                              return;
    }
    
    QGst::PropertyProbePtr deviceProbe;
    
    src->setState(QGst::StateReady);
    QGst::ChildProxyPtr childProxy = src.dynamicCast<QGst::ChildProxy>();
    if (childProxy && childProxy->childrenCount() > 0) {
        deviceProbe = childProxy->childByIndex(0).dynamicCast<QGst::PropertyProbe>();
    }
    src->setState(QGst::StateNull);
    
    if (deviceProbe && deviceProbe->propertySupportsProbe("device")) {
        audioProbe = deviceProbe;
        probeForDevices(deviceProbe);
        
        // this signal will notify us when devices change
        QGlib::connect(deviceProbe, "probe-needed",
                       this, &FileRecorder::probeForDevices, QGlib::PassSender);
    } else {
        DeviceManager::add("Default");
    }
}

void FileRecorder::probeForDevices ( const QGst::PropertyProbePtr& propertyProbe )
{
    QList<QGlib::Value> devices = propertyProbe->probeAndGetValues("device");
    
    Q_FOREACH(const QGlib::Value & device, devices) {
        propertyProbe->setProperty("device", device);
        QString deviceName = propertyProbe->property("device-name").toString();
        
        DeviceManager::add(deviceName);
    }
}

QGst::BinPtr FileRecorder::createAudioSrcBin()
{
    QGst::BinPtr audioBin;
    
    try {
        audioBin = QGst::Bin::fromDescription("autoaudiosrc name=\"audiosrc\" ! audioconvert ! "
        "audioresample ! audiorate ! speexenc ! queue");
    } catch (const QGlib::Error & error) {
        qCritical() << "Failed to create audio source bin:" << error;
        return QGst::BinPtr();
    }
    
    //set the source's properties
    QString device = DeviceManager::currentChoice();
    if (!device.isEmpty()) {
        QGst::ElementPtr src = audioBin->getElementByName("audiosrc");
        
        //autoaudiosrc creates the actual source in the READY state
        src->setState(QGst::StateReady);
        
        QGst::ChildProxyPtr childProxy = src.dynamicCast<QGst::ChildProxy>();
        if (childProxy && childProxy->childrenCount() > 0) {
            QGst::ObjectPtr realSrc = childProxy->childByIndex(0);
            realSrc->setProperty("device", device);
        }
    }
    
    return audioBin;
}

void FileRecorder::onBusMessage ( const QGst::MessagePtr& message )
{

}

FileRecorder::FileRecorder ( QObject* parent ) : QObject ( parent )
{

}

FileRecorder::FileRecorder ( QString _outFile, QObject* parent ) : QObject ( parent ),
    outFile(_outFile)
{

}

FileRecorder::FileRecorder ( QFile& _outFile, QObject* parent ) : QObject ( parent )
{
    outFile = _outFile.fileName();
}

FileRecorder::~FileRecorder()
{

}

void FileRecorder::setFile ( QString path )
{
    outFile = path;
}

void FileRecorder::setFile ( QFile& file )
{
    outFile = file.fileName();
}

void FileRecorder::setEncoding ( QString _encoding )
{
    encoding = _encoding;
}

void FileRecorder::start()
{

}

void FileRecorder::stop()
{

}

#include "filerecorder.moc"
