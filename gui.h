#ifndef GUI_H
#define GUI_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>

class ImageCompressionGUI : public QWidget {
    Q_OBJECT

public:
    ImageCompressionGUI(QWidget *parent = nullptr);

private slots:
    void handleCompression();
    void handleDecompression();

private:
    QLabel *statusLabel;  
};

#endif
