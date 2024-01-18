#pragma once

#include <QMessageBox>

static void error(const std::string& input, QWidget* parent = nullptr) {
    QMessageBox errorBox(parent);
    //errorBox.setStyleSheet(" QLabel { color: white; } ");
    errorBox.setText(QString::fromStdString(input));
    errorBox.setIcon(QMessageBox::Critical);
    errorBox.setWindowTitle("Error");
    errorBox.setWindowIcon(QIcon(":/Resources/favicon.ico"));
    errorBox.setStandardButtons(QMessageBox::Ok);
    errorBox.exec();
    qDebug() << "Error: " << QString::fromStdString(input) << '\n';
    exit(EXIT_FAILURE);
}
