#ifndef UMDHWIDGET_H
#define UMDHWIDGET_H

#include "idatamodel/isettings.h"
#include "ilogic/iumdh.h"

#include <QWidget>

namespace Ui {
class UmdhWidget;
}

class QLineEdit;

class UmdhWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UmdhWidget(gui::ISettings*,
                        gui::IUmdh*,
                        QWidget *parent = nullptr);
    ~UmdhWidget();

private slots:
    void on_pathToUmdhLineEdit_editingFinished();
    void on_pathToUmdhPushButtonAdd_clicked();

    void on_snapshotOldLineEdit_editingFinished();
    void on_snapshotOldPushButtonAdd_clicked();
    void on_snapshotOldPushButtonCreate_clicked();

    void on_snapshotNewLineEdit_editingFinished();
    void on_snapshotNewPushButtonAdd_clicked();
    void on_snapshotNewPushButtonCreate_clicked();

    void on_folderForSnapshotsPushButtonAdd_clicked();
    void on_folderForSnapshotsLineEdit_editingFinished();

    void on_ntSymbolPathLineEdit_editingFinished();
    void on_targetProcessNameLineEdit_editingFinished();
    void on_pidLineEdit_editingFinished();
    void on_createReportPushButton_clicked();

private:
    bool isPathToFolderValid(QString path);
    bool isPathToFileValid(QString path);
    void setLineEditValid(QLineEdit*, bool isValid);

    bool isPathValid(QString path, bool toFile);
    void synchronizeSettingsToGui();

    void updateGuiState();
    void updateGuiState(bool isUmdhValid,
                        bool isTargetProcessRunning,
                        bool isMultipleProcessesRunning);
private:
    gui::ISettings *m_pSettings;
    gui::IUmdh *m_pUmdh;
    Ui::UmdhWidget *m_pUi;
};

#endif // UMDHWIDGET_H
