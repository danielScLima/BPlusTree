#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <fstream>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bTreePlus = new BTreePlus(defaultDegree);
    ui->spinBox->setValue(defaultDegree);

    updateDotFile();
    updateImage();
    renderImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDotFile()
{
    std::ofstream myFile;
    QDir::setCurrent(QCoreApplication::applicationFilePath());
    myFile.open
    (
        "file.dot"
    );

    std::string textToFile = bTreePlus->getDotFileContent();

    myFile << textToFile;
    myFile.close();
}

void MainWindow::updateImage()
{
    std::string message =
                "dot -Tpng file.dot > file.png";
    QDir::setCurrent(QCoreApplication::applicationFilePath());
    system(message.c_str());
}

void MainWindow::renderImage()
{
    QDir::setCurrent(QCoreApplication::applicationFilePath());
    QPixmap image("file.png");
    ui->labelOfImage->setPixmap(image);
    ui->labelOfImage->show();
}

#include <algorithm>
#include <iterator>

template <class Container>
void MainWindow::split3(const std::string& str, Container& cont,
              char delim)
{
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
}

void MainWindow::on_pushButtonOfInsertion_clicked()
{
    std::vector<std::string> numbersAsString;
    std::string numbers = ui->lineEditOfInsert->text().toStdString();
    split3(numbers, numbersAsString);

    bool ret = false;
    for (auto numberAsStr: numbersAsString)
    {
        int number = std::atoi(numberAsStr.c_str());
        ret = bTreePlus->insert(number);

        QMessageBox msgBox;
        if (ret)
            msgBox.setText("The number "+QString::number(number)+" was inserted");
        else
            msgBox.setText("Already exists a node with this data");
        msgBox.exec();
    }

    updateDotFile();
    updateImage();
    renderImage();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    bTreePlus = new BTreePlus(arg1);
    updateDotFile();
    updateImage();
    renderImage();
}

void MainWindow::on_pushButtonOfSearch_clicked()
{
    int number = ui->lineEditOfSearch->text().toInt();
    int ret = bTreePlus->search(number) != nullptr;

    QMessageBox msgBox;
    if (ret)
        msgBox.setText("The number exists");
    else
        msgBox.setText("This number does not exists in the tree");
    msgBox.exec();
}

void MainWindow::on_pushButtonOfRemove_clicked()
{
    int number = ui->lineEditOfRemove->text().toInt();
    bTreePlus->remove(number);

    updateDotFile();
    updateImage();
    renderImage();
}
