#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "btreeplus.h"

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief MainWindow
     * \param parent
     */
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    /*!
     * \brief on_pushButtonOfInsertion_clicked
     */
    void on_pushButtonOfInsertion_clicked();

    /*!
     * \brief on_spinBox_valueChanged
     * \param arg1
     */
    void on_spinBox_valueChanged(int arg1);

    /*!
     * \brief on_pushButtonOfSearch_clicked
     */
    void on_pushButtonOfSearch_clicked();

    /*!
     * \brief on_pushButtonOfRemove_clicked
     */
    void on_pushButtonOfRemove_clicked();

private:
    Ui::MainWindow *ui;
    BTreePlus* bTreePlus = nullptr;
    int defaultDegree = 5; //quantidade de filhos

    /*!
     * \brief updateDotFile
     */
    void updateDotFile();

    /*!
     * \brief updateImage
     */
    void updateImage();

    /*!
     * \brief renderImage
     */
    void renderImage();

    template<class Container>
    void split3(const std::string &str, Container &cont, char delim = ',');
};
#endif // MAINWINDOW_H
