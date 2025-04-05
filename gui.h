#ifndef GUI_H
#define GUI_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QListWidget>
#include <QSlider>
#include <QScrollArea>
#include <QPixmap>
#include <QImageReader>

using namespace std;

class ImagePreviewLabel : public QLabel {
    Q_OBJECT

public:
    ImagePreviewLabel(QWidget *parent = nullptr);
    void setPixmap(const QString &imagePath);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap originalPixmap;
    void updateScaledPixmap();
};

class ImageCompressionGUI : public QWidget {
    Q_OBJECT

public:
    ImageCompressionGUI(QWidget *parent = nullptr);

private:
    QMap<QString, QString> compressedFilePaths;
    QLabel *metadataLabel;
    void updateMetadata(const QString &filePath = QString());

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void handleCompression();
    void handleBatchCompression();
    void removeSelectedFiles();
    void updateCompressionQuality(int value);
    void updatePreview(QListWidgetItem *current);
    void showOriginalImage();
    void showCompressedImage();

private:
    QLabel *statusLabel;
    QListWidget *fileListWidget;
    QSlider *qualitySlider;
    QLabel *qualityLabel;
    ImagePreviewLabel *previewLabel;
    QPushButton *originalImageBtn;
    QPushButton *compressedImageBtn;
    
    int compressionQuality;
    QString lastCompressedImagePath;
};

#endif