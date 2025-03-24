#include "gui.h"
#include "compression.h"
#include "decompression.h"
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QFileInfo>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QPropertyAnimation>

ImageCompressionGUI::ImageCompressionGUI(QWidget *parent) : QWidget(parent) {
    // Set window properties
    this->setWindowTitle("Image Compression");
    this->resize(500, 400);
    this->setStyleSheet("background-color: #F5F5F5; color: #333333;");
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 40, 30, 40);
    mainLayout->setSpacing(20);
    
    // Title
    QLabel *titleLabel = new QLabel("Image Compression using Huffman Image Compression", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-family: 'Segoe UI', Arial; font-size: 22px; font-weight: 300; letter-spacing: 2px; color:rgb(0, 0, 0); margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Subtitle
    QLabel *subtitleLabel = new QLabel("Compress and decompress your images", this);
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
    
    // Buttons container
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
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
        "QPushButton:pressed {"
        "   background-color:rgb(7, 24, 181);"
        "}"
    );
    
    // Decompress button
    QPushButton *decompressButton = new QPushButton("DECOMPRESS", this);
    decompressButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #F9F9F9;"
        "   color: #4A6572;"
        "   font-family: 'Segoe UI', Arial;"
        "   font-size: 13px;"
        "   font-weight: 500;"
        "   border: 1px solid #E0E0E0;"
        "   border-radius: 6px;"
        "   padding: 12px 20px;"
        "   letter-spacing: 0.5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #EEEEEE;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #E0E0E0;"
        "}"
    );
    
    buttonLayout->addWidget(compressButton);
    buttonLayout->addWidget(decompressButton);
    cardLayout->addLayout(buttonLayout);
    
    // Status label
    statusLabel = new QLabel("Select an operation to begin", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("color: #666666; font-family: 'Segoe UI', Arial; font-size: 14px; margin-top: 10px;");
    cardLayout->addWidget(statusLabel);
    
    // Add card to main layout
    mainLayout->addWidget(cardFrame);
    
    // Connect signals and slots
    connect(compressButton, &QPushButton::clicked, this, &ImageCompressionGUI::handleCompression);
    connect(decompressButton, &QPushButton::clicked, this, &ImageCompressionGUI::handleDecompression);
}

void ImageCompressionGUI::handleCompression() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select Image to Compress", "", "Images (*.png *.jpg *.bmp)");
    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.completeBaseName();  
        QString outputDir = "../output/";
        QDir().mkpath(outputDir);

        QString binFile = outputDir + baseName;
        
        // This should match the compressImage() function's naming pattern
        QString compressedImage = binFile + "_compressed.jpg";

        QFile inputFile(filePath);
        qint64 originalSize = inputFile.size();

        qDebug() << "Selected file:" << filePath;
        qDebug() << "Starting compression...";
        
        compressImage(filePath.toStdString(), binFile.toStdString());

        QFile outputFile(compressedImage);
        qint64 compressedSize = 0;
        
        if (outputFile.exists()) {
            compressedSize = outputFile.size();
        } else {
            qDebug() << "Warning: Compressed file not found at expected location: " << compressedImage;
            QString binFileLocation = binFile + ".bin";
            QFile binFileCheck(binFileLocation);
            if (binFileCheck.exists()) {
                compressedSize = binFileCheck.size();
                compressedImage = binFileLocation;
            }
        }

        qDebug() << "Compression finished.";
        qDebug() << "Original Size:" << originalSize / 1024 << "KB";
        qDebug() << "Compressed Size:" << compressedSize / 1024 << "KB";

        statusLabel->setText(
            QString("<b>Compression Complete! 🚀</b><br>Original: <b>%1 KB</b> → Compressed: <b>%2 KB</b><br>Saved as: <i>%3</i>")
            .arg(originalSize / 1024).arg(compressedSize / 1024).arg(compressedImage)
        );
        statusLabel->setStyleSheet("color: #4CAF50; font-size: 16px;");
    } else {
        qDebug() << "No file selected.";
        statusLabel->setText("❌ No file selected.");
        statusLabel->setStyleSheet("color: red;");
    }
}

void ImageCompressionGUI::handleDecompression() {
    statusLabel->setText("Selecting compressed file...");
    statusLabel->setStyleSheet("color: #666666; font-family: 'Segoe UI', Arial; font-size: 13px;");
    
    QString filePath = QFileDialog::getOpenFileName(this, "Select Compressed File", "../output/", "Binary Files (*.bin)");
    if (!filePath.isEmpty()) {
        std::cout << "Selected file: " << filePath.toStdString() << std::endl;
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.completeBaseName();
        QString decompressedImage = "../output/" + baseName + "_decompressed.png";

        statusLabel->setText("Decompressing...");
        
        // Check if corresponding .codes file exists
        QString codesFile = filePath + ".codes";
        QFile codeFileCheck(codesFile);
        if (!codeFileCheck.exists()) {
            statusLabel->setText("Error: Missing Huffman codes file (.codes)");
            statusLabel->setStyleSheet("color: #F44336; font-family: 'Segoe UI', Arial; font-size: 13px;");
            return;
        }
        
        // Redirect stderr to capture error messages
        std::stringstream errorOutput;
        std::streambuf* oldCerr = std::cerr.rdbuf(errorOutput.rdbuf());
        
        // Perform decompression
        decompressImage(filePath.toStdString(), decompressedImage.toStdString(), 512, 512);
        
        // Restore stderr
        std::cerr.rdbuf(oldCerr);
        
        // Check for errors
        std::string errorMsg = errorOutput.str();
        if (!errorMsg.empty()) {
            statusLabel->setText("Decompression failed: " + QString::fromStdString(errorMsg));
            statusLabel->setStyleSheet("color: #F44336; font-family: 'Segoe UI', Arial; font-size: 13px;");
        } else {
            // Check if output file exists and has valid size
            QFile outFile(decompressedImage);
            if (outFile.exists() && outFile.size() > 0) {
                statusLabel->setText("Decompression complete\nSaved to output folder");
                statusLabel->setStyleSheet("color: #4A6572; font-family: 'Segoe UI', Arial; font-size: 13px; font-weight: 500;");
            } else {
                statusLabel->setText("Decompression failed: Output file invalid");
                statusLabel->setStyleSheet("color: #F44336; font-family: 'Segoe UI', Arial; font-size: 13px;");
            }
        }
    } else {
        statusLabel->setText("No file selected");
        statusLabel->setStyleSheet("color: #9E9E9E; font-family: 'Segoe UI', Arial; font-size: 13px;");
    }
}