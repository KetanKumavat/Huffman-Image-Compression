#include "gui.h"
#include "compression.h"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QMimeData>
#include <QStyledItemDelegate>
#include <QMessageBox>

using namespace std;

ImagePreviewLabel::ImagePreviewLabel(QWidget *parent) : QLabel(parent) {
    setAlignment(Qt::AlignCenter);
    setStyleSheet("background-color: #F0F0F0; border: 2px dashed #CCCCCC; border-radius: 10px;");
}

void ImagePreviewLabel::setPixmap(const QString &imagePath) {
    originalPixmap = QPixmap(imagePath);
    
    updateScaledPixmap();
}

void ImagePreviewLabel::resizeEvent(QResizeEvent *event) {
    QLabel::resizeEvent(event);
    updateScaledPixmap();
}

void ImagePreviewLabel::updateScaledPixmap() {
    if (originalPixmap.isNull()) {
        setText("No Image");
        return;
    }

    QPixmap scaledPixmap = originalPixmap.scaled(
        size(), 
        Qt::KeepAspectRatio, 
        Qt::SmoothTransformation
    );
    
    QLabel::setPixmap(scaledPixmap);
}

ImageCompressionGUI::ImageCompressionGUI(QWidget *parent) : QWidget(parent), compressionQuality(50) {
    this->setWindowTitle("Image Compression");
    this->resize(800, 700);
    this->setStyleSheet("background-color: #F5F5F5; color: #333333;");
    this->setAcceptDrops(true);

    fileListWidget = new QListWidget(this);
    previewLabel = new ImagePreviewLabel(this);
    originalImageBtn = new QPushButton("Original", this);
    compressedImageBtn = new QPushButton("Compressed", this);
    qualityLabel = new QLabel("Compression Quality: 100%", this);
    qualitySlider = new QSlider(Qt::Horizontal, this);
    statusLabel = new QLabel("Drag and drop images or click 'Add Files'", this);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 40, 30, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Image Compression using Huffman Coding", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-family: 'Segoe UI', Arial; font-size: 22px; font-weight: 300; letter-spacing: 2px; color:rgb(0, 0, 0); margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Subtitle
    QLabel *subtitleLabel = new QLabel("Compress your images", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-family: 'Segoe UI', Arial; font-size: 16px; color: #666666; margin-bottom: 30px;");
    mainLayout->addWidget(subtitleLabel);
    
    // Create card container
    QFrame *cardFrame = new QFrame(this);
    cardFrame->setStyleSheet("background-color: white; border-radius: 12px;");
    
    // Add shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 30));
    shadowEffect->setOffset(0, 4);
    cardFrame->setGraphicsEffect(shadowEffect);
    
    // Card layout
    QVBoxLayout *cardLayout = new QVBoxLayout(cardFrame);
    cardLayout->setContentsMargins(25, 30, 25, 30);
    cardLayout->setSpacing(20);
    
    // Compression Quality Section
    QHBoxLayout *qualityLayout = new QHBoxLayout();
    qualitySlider->setRange(10, 50);
    qualitySlider->setValue(50);
    qualityLayout->addWidget(new QLabel("Low"));
    qualityLayout->addWidget(qualitySlider);
    qualityLayout->addWidget(new QLabel("High"));
    cardLayout->addLayout(qualityLayout);
    cardLayout->addWidget(qualityLabel);
    
    // Preview Layout with File List and Image Preview
    QHBoxLayout *previewLayout = new QHBoxLayout();
    
    // File List (left side)
    QVBoxLayout *fileListLayout = new QVBoxLayout();
    fileListWidget->setStyleSheet(
        "QListWidget { border: 2px dashed #CCCCCC; border-radius: 10px; padding: 10px; }"
        "QListWidget::item { background-color: #F9F9F9; border-radius: 5px; margin-bottom: 5px; padding: 5px; }"
        "QListWidget::item:selected { background-color: #E0E0E0; }"
    );
    fileListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileListLayout->addWidget(fileListWidget);
    
    // Preview Section (right side)
    QVBoxLayout *previewSectionLayout = new QVBoxLayout();
    previewLabel->setMinimumSize(300, 300);
    previewSectionLayout->addWidget(previewLabel);
    
    // Preview Buttons
    QHBoxLayout *previewButtonLayout = new QHBoxLayout();
    QString buttonStyle = 
        "QPushButton {"
        "   background-color: #E0E0E0;"
        "   color: #333;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #D0D0D0;"
        "}"
        "QPushButton:checked {"
        "   background-color: #1976D2;"
        "   color: white;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #F0F0F0;"
        "   color: #999;"
        "}";

    // Set buttons to be checkable
    originalImageBtn->setCheckable(true);
    compressedImageBtn->setCheckable(true);
        
    originalImageBtn->setStyleSheet(buttonStyle);
    compressedImageBtn->setStyleSheet(buttonStyle);

    // Set first button checked by default
    originalImageBtn->setChecked(true);
    compressedImageBtn->setEnabled(false);

    previewButtonLayout->addWidget(originalImageBtn);
    previewButtonLayout->addWidget(compressedImageBtn);

    previewSectionLayout->addLayout(previewButtonLayout);

    metadataLabel = new QLabel(this);
    metadataLabel->setStyleSheet(
        "QLabel {"
        "   background-color: #F8F8F8;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "   margin-top: 5px;"
        "   color: #444;"
        "   font-size: 12px;"
        "}"
    );
    metadataLabel->setText("No file selected");
    metadataLabel->setWordWrap(true);
    previewSectionLayout->addWidget(metadataLabel);
    
    // Combine layouts
    previewLayout->addLayout(fileListLayout, 1);
    previewLayout->addLayout(previewSectionLayout, 2);
    cardLayout->addLayout(previewLayout);
    
    // Action Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    // Add Files button
    QPushButton *addFilesButton = new QPushButton("ADD FILES", this);
    addFilesButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2196F3;"
        "   color: white;"
        "   font-family: 'Segoe UI', Arial;"
        "   font-size: 13px;"
        "   font-weight: 500;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px 20px;"
        "   letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover { background-color: #1976D2; }"
    );
    
    // Remove Files button
    QPushButton *removeFilesButton = new QPushButton("REMOVE", this);
    removeFilesButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F44336;"
        "   color: white;"
        "   font-family: 'Segoe UI', Arial;"
        "   font-size: 13px;"
        "   font-weight: 500;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px 20px;"
        "   letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover { background-color: #D32F2F; }"
    );
    
    // Compress button
    QPushButton *compressButton = new QPushButton("COMPRESS", this);
    compressButton->setStyleSheet(
        "QPushButton {"
        "   background-color:rgb(26, 43, 198);"
        "   color: white;"
        "   font-family: 'Segoe UI', Arial;"
        "   font-size: 13px;"
        "   font-weight: 500;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 12px 20px;"
        "   letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "   background-color:rgb(39, 17, 183);"
        "}"
    );
    
    buttonLayout->addWidget(addFilesButton);
    buttonLayout->addWidget(removeFilesButton);
    buttonLayout->addWidget(compressButton);
    cardLayout->addLayout(buttonLayout);
    
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("color: #666666; font-family: 'Segoe UI', Arial; font-size: 14px; margin-top: 10px;");
    cardLayout->addWidget(statusLabel);
    
    mainLayout->addWidget(cardFrame);
    
    connect(fileListWidget, &QListWidget::currentItemChanged, 
            this, &ImageCompressionGUI::updatePreview);
    connect(originalImageBtn, &QPushButton::clicked, 
            this, &ImageCompressionGUI::showOriginalImage);
    connect(compressedImageBtn, &QPushButton::clicked, 
            this, &ImageCompressionGUI::showCompressedImage);
    connect(addFilesButton, &QPushButton::clicked, 
            this, &ImageCompressionGUI::handleCompression);
    connect(removeFilesButton, &QPushButton::clicked, 
            this, &ImageCompressionGUI::removeSelectedFiles);
    connect(compressButton, &QPushButton::clicked, 
            this, &ImageCompressionGUI::handleBatchCompression);
    connect(qualitySlider, &QSlider::valueChanged, 
            this, &ImageCompressionGUI::updateCompressionQuality);
}

void ImageCompressionGUI::updatePreview(QListWidgetItem *current) {
    if (!current) {
        previewLabel->clear();
        previewLabel->setText("No image selected");
        metadataLabel->setText("No file selected");
        return;
    }
    
    QString imagePath = current->text();
    
    try {
        previewLabel->setPixmap(imagePath);
        
        if (compressedFilePaths.contains(imagePath)) {
            lastCompressedImagePath = compressedFilePaths[imagePath];
            compressedImageBtn->setEnabled(true);
        } else {
            lastCompressedImagePath.clear();
            compressedImageBtn->setEnabled(false);
        }
        
        originalImageBtn->setChecked(true);
        compressedImageBtn->setChecked(false);
        
        updateMetadata(imagePath);
    } catch (const exception& e) {
        QMessageBox::warning(this, "Preview Error", 
            QString("Could not preview image: %1").arg(e.what()));
    }
}

void ImageCompressionGUI::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void ImageCompressionGUI::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        bool wasEmpty = (fileListWidget->count() == 0);
        int firstNewItemIndex = -1;
        
        for (const QUrl &url : urlList) {
            QString filePath = url.toLocalFile();
            QFileInfo fileInfo(filePath);
            
            if (fileInfo.suffix().toLower().contains(QRegExp("(png|jpg|jpeg|bmp)"))) {
                bool isDuplicate = false;
                for (int i = 0; i < fileListWidget->count(); ++i) {
                    if (fileListWidget->item(i)->text() == filePath) {
                        isDuplicate = true;
                        break;
                    }
                }
                
                if (!isDuplicate) {
                    fileListWidget->addItem(filePath);
                    
                    //for preview
                    if (firstNewItemIndex == -1) {
                        firstNewItemIndex = fileListWidget->count() - 1;
                    }
                }
            }
        }
        
        if ((wasEmpty || !fileListWidget->currentItem()) && firstNewItemIndex != -1) {
            fileListWidget->setCurrentRow(firstNewItemIndex);
        }
        
        event->acceptProposedAction();
    }
}

void ImageCompressionGUI::updateCompressionQuality(int value) {
    compressionQuality = value;
    int displayValue = value * 2;
    qualityLabel->setText(QString("Compression Quality: %1%").arg(displayValue));
}

void ImageCompressionGUI::removeSelectedFiles() {
    qDeleteAll(fileListWidget->selectedItems());
}

void ImageCompressionGUI::handleCompression() {
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Select Images to Compress", "", "Images (*.png *.jpg *.bmp)");
    if (!filePaths.isEmpty()) {
        bool wasEmpty = (fileListWidget->count() == 0);
        int firstNewItemIndex = -1;
        for (const QString &filePath : filePaths) {
            //prevent duplicates
            bool isDuplicate = false;
            for (int i = 0; i < fileListWidget->count(); ++i) {
                if (fileListWidget->item(i)->text() == filePath) {
                    isDuplicate = true;
                    break;
                }
            }
            if (!isDuplicate) {
                QListWidgetItem* item = new QListWidgetItem(filePath);
                fileListWidget->addItem(item);
                
                //for preview
                if (firstNewItemIndex == -1) {
                    firstNewItemIndex = fileListWidget->count() - 1;
                }
            }
        }
        if ((wasEmpty || !fileListWidget->currentItem()) && firstNewItemIndex != -1) {
            fileListWidget->setCurrentRow(firstNewItemIndex);
        }
    }
}

void ImageCompressionGUI::handleBatchCompression() {
    if (fileListWidget->count() == 0) {
        statusLabel->setText("❌ No files selected for compression");
        statusLabel->setStyleSheet("color: red;");
        return;
    }

    QString outputDir = "../output/";
    QDir().mkpath(outputDir);

    int totalFiles = fileListWidget->count();
    int successCount = 0;
    int failCount = 0;

    for (int i = 0; i < fileListWidget->count(); ++i) {
        QString filePath = fileListWidget->item(i)->text();
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.completeBaseName();
        QString binFile = outputDir + baseName;

        QFile inputFile(filePath);
        qint64 originalSize = inputFile.size();

        vector<int> compressionParams = {cv::IMWRITE_JPEG_QUALITY, compressionQuality};
        QString compressedImage = binFile + "_compressed.jpg";

        try {
            compressImage(filePath.toStdString(), binFile.toStdString(), compressionParams);
            
            QFile outputFile(compressedImage);
            qint64 compressedSize = 0;
            
            if (outputFile.exists()) {
                compressedSize = outputFile.size();
                
                compressedFilePaths[filePath] = compressedImage;
                
                if (fileListWidget->currentItem() && fileListWidget->currentItem()->text() == filePath) {
                    lastCompressedImagePath = compressedImage;
                    compressedImageBtn->setEnabled(true);
                    updateMetadata(compressedImage);
                }
            }
            
            statusLabel->setText(
                QString("Compressed: %1\nOriginal: %2 KB → Compressed: %3 KB")
                .arg(fileInfo.fileName())
                .arg(originalSize / 1024)
                .arg(compressedSize / 1024)
            );

            successCount++;
        } catch (const exception& e) {
            statusLabel->setText(QString("Error compressing %1: %2")
                .arg(fileInfo.fileName())
                .arg(e.what()));
            failCount++;
        }

        QApplication::processEvents();
    }

    statusLabel->setText(
        QString("Batch Compression Complete\n ✅ Successful: %1 | ❌ Failed: %2")
        .arg(successCount).arg(failCount)
    );
    statusLabel->setStyleSheet("color: #4CAF50; font-size: 16px;");
}

void ImageCompressionGUI::showOriginalImage() {
    QListWidgetItem *current = fileListWidget->currentItem();
    if (current) {
        previewLabel->setPixmap(current->text());
        originalImageBtn->setChecked(true);
        compressedImageBtn->setChecked(false);
        updateMetadata();
    }
}

void ImageCompressionGUI::showCompressedImage() {
    if (!lastCompressedImagePath.isEmpty() && QFile::exists(lastCompressedImagePath)) {
        previewLabel->setPixmap(lastCompressedImagePath);
        originalImageBtn->setChecked(false);
        compressedImageBtn->setChecked(true);
        updateMetadata(lastCompressedImagePath);
    } else {
        QMessageBox::warning(this, "Preview Error", "No compressed image available");
    }
}

void ImageCompressionGUI::updateMetadata(const QString &filePath) {
    if (filePath.isEmpty() && fileListWidget->currentItem() == nullptr) {
        metadataLabel->setText("No file selected");
        return;
    }
    
    QString path = filePath.isEmpty() ? fileListWidget->currentItem()->text() : filePath;
    QFileInfo fileInfo(path);
    
    if (!fileInfo.exists()) {
        metadataLabel->setText("File not found");
        return;
    }
    
    QString fileName = fileInfo.fileName();
    qint64 sizeInBytes = fileInfo.size();
    QString lastModified = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
    
    QString dimensions = "Unknown";
    try {
        QImage img(path);
        if (!img.isNull()) {
            dimensions = QString("%1 x %2").arg(img.width()).arg(img.height());
        }
    } catch (...) {
    }
    
    QString sizeStr;
    if (sizeInBytes < 1024) {
        sizeStr = QString("%1 bytes").arg(sizeInBytes);
    } else if (sizeInBytes < 1024 * 1024) {
        sizeStr = QString("%1 KB").arg(sizeInBytes / 1024.0, 0, 'f', 2);
    } else {
        sizeStr = QString("%1 MB").arg(sizeInBytes / (1024.0 * 1024.0), 0, 'f', 2);
    }
    
    metadataLabel->setText(QString(
        "<table style='margin-top:5px;'>"
        "<tr><td><b>File:</b></td><td>%1</td></tr>"
        "<tr><td><b>Size:</b></td><td>%2</td></tr>"
        "<tr><td><b>Dimensions:</b></td><td>%3</td></tr>"
        "<tr><td><b>Modified:</b></td><td>%4</td></tr>"
        "</table>"
    ).arg(fileName).arg(sizeStr).arg(dimensions).arg(lastModified));
}
