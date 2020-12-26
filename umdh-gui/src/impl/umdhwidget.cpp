#include "umdhwidget.h"
#include "ui_umdhwidget.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QTimer>

#include <iomanip>
#include <sstream>

#define DPSAPI_VERSION (1)

#include <Windows.h>
#include <psapi.h>

namespace
{
    bool isProcess(DWORD processId, const std::wstring &processName)
    {
        TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                      PROCESS_VM_READ,
                                      FALSE, processId);
        if (NULL != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                GetModuleBaseName(hProcess, hMod, szProcessName,
                                  sizeof(szProcessName)/sizeof(TCHAR));
            }
        }
        CloseHandle(hProcess);
        return QString::fromStdWString(processName).toUpper() == QString::fromStdWString(szProcessName).toUpper();
    }

    bool isMultipleProcessRunning(std::wstring processName)
    {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        if (!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            return false;
        }
        cProcesses = cbNeeded / sizeof(DWORD);

        int cnt = 0;
        for (DWORD i = 0; i < cProcesses; ++i)
        {
            if (aProcesses[i] != 0)
            {
                if (isProcess(aProcesses[i], processName)) ++cnt;
            }
        }
        return cnt > 1;
    }

    bool isProcessRunning(std::wstring processName)
    {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        if (!EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded))
        {
            return false;
        }
        cProcesses = cbNeeded / sizeof(DWORD);

        for (DWORD i = 0; i < cProcesses; ++i)
        {
            if (aProcesses[i] != 0)
            {
                if (isProcess(aProcesses[i], processName)) return true;
            }
        }
        return false;
    }

    std::wstring getDateTimeStr()
    {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::wostringstream woss;
        woss << std::put_time(&tm, L"%d_%m_%Y_%H_%M_%S");
        auto str = woss.str();
        return str;
    }

    std::wstring buildName(gui::ISettings *pSettings, std::wstring tag)
    {
        const auto &targetProgrammName = pSettings->getTargetProgramm();

        auto extensionPos = targetProgrammName.find_last_of(L".");
        extensionPos = (extensionPos == std::wstring::npos) ? targetProgrammName.length() - 1 : extensionPos;

        auto folderDelimeterPos = targetProgrammName.find_last_of(L"\\/");
        folderDelimeterPos = (folderDelimeterPos == std::wstring::npos) ? 0 : folderDelimeterPos;

        return (pSettings->getFolderForSnapshots().empty() ? L"" : (pSettings->getFolderForSnapshots() + L"/")) + tag + L"_"
                + targetProgrammName.substr(folderDelimeterPos, extensionPos - folderDelimeterPos)
                + L"_" + getDateTimeStr() + L".txt";
    }
}

UmdhWidget::UmdhWidget(gui::ISettings *pSettings,
                       gui::IUmdh *pUmdh,
                       QWidget *parent) :
    QWidget(parent),
    m_pSettings(pSettings),
    m_pUmdh(pUmdh),
    m_pUi(new Ui::UmdhWidget)
{
    assert(m_pSettings);
    assert(m_pUmdh);

    pSettings->setUpdateNotifier([&] { updateGuiState(); });
    m_pUi->setupUi(this);

    m_pSettings->setUmdhPath(L"C:/Program Files (x86)/Windows Kits/10/Debuggers/x64/umdh.exe");

    updateGuiState();
}

UmdhWidget::~UmdhWidget()
{
    delete m_pUi;
}

void UmdhWidget::updateGuiState()
{
    synchronizeSettingsToGui();
    updateGuiState(isPathToFileValid(QString::fromStdWString(m_pSettings->getUmdhPath())) && !m_pSettings->getUmdhPath().empty(),
                   isProcessRunning(m_pSettings->getTargetProgramm()),
                   isMultipleProcessRunning(m_pSettings->getTargetProgramm()));
}

void UmdhWidget::synchronizeSettingsToGui()
{
    m_pUi->pathToUmdhLineEdit->setText(QString::fromStdWString(m_pSettings->getUmdhPath()));
    m_pUi->ntSymbolPathLineEdit->setText(QString::fromStdWString(m_pSettings->getNtSymbolPath()));
    m_pUi->folderForSnapshotsLineEdit->setText(QString::fromStdWString(m_pSettings->getFolderForSnapshots()));
    m_pUi->targetProcessNameLineEdit->setText(QString::fromStdWString(m_pSettings->getTargetProgramm()));
    m_pUi->pidLineEdit->setText(QString::fromStdWString(m_pSettings->getPID()));
}

bool UmdhWidget::isPathToFolderValid(QString path)
{
    return isPathValid(path, false);
}

bool UmdhWidget::isPathToFileValid(QString path)
{
    return isPathValid(path, true);
}

bool UmdhWidget::isPathValid(QString path, bool toFile)
{
    const bool pathIsNotEmpty = !path.isEmpty();
    const bool pathExists = QFileInfo::exists(path);
    const bool isFile = (toFile ? QFileInfo(path).isFile() : true);
    return pathIsNotEmpty && pathExists && isFile;
}

void UmdhWidget::setLineEditValid(QLineEdit *pTarget, bool isValid)
{
    if (isValid)
    {
        pTarget->setStyleSheet("");
    }
    else
    {
        pTarget->setStyleSheet("border: 1px solid red");
    }
}

void UmdhWidget::updateGuiState(bool isUmdhValid, bool isTargetProcessRunning, bool isMultipleProcessesRunning)
{
    m_pUi->pathToUmdhLabel->setEnabled(true);
    m_pUi->pathToUmdhLineEdit->setEnabled(true);
    m_pUi->pathToUmdhPushButtonAdd->setEnabled(true);
    setLineEditValid(m_pUi->pathToUmdhLineEdit,
                     isPathToFileValid(m_pUi->pathToUmdhLineEdit->text()) &&
                     !m_pUi->pathToUmdhLineEdit->text().isEmpty());

    m_pUi->ntSymbolPathLabel->setEnabled(isUmdhValid);
    m_pUi->ntSymbolPathLineEdit->setEnabled(isUmdhValid);

    m_pUi->folderForSnapshotsLabel->setEnabled(isUmdhValid);
    m_pUi->folderForSnapshotsLineEdit->setEnabled(isUmdhValid);
    m_pUi->folderForSnapshotsPushButtonAdd->setEnabled(isUmdhValid);
    setLineEditValid(m_pUi->folderForSnapshotsLineEdit,
                     isPathToFolderValid(m_pUi->folderForSnapshotsLineEdit->text()) ||
                     m_pUi->folderForSnapshotsLineEdit->text().isEmpty());

    m_pUi->targetProcessNameLabel->setEnabled(isUmdhValid);
    m_pUi->targetProcessNameLineEdit->setEnabled(isUmdhValid);

    m_pUi->pidLabel->setEnabled(isUmdhValid && isTargetProcessRunning && isMultipleProcessesRunning);
    m_pUi->pidLineEdit->setEnabled(isUmdhValid && isTargetProcessRunning && isMultipleProcessesRunning);

    const bool isPidValid = m_pUi->pidLineEdit->isEnabled() ? !m_pUi->pidLineEdit->text().isEmpty() : true;
    m_pUi->snapshotOldLabel->setEnabled(isUmdhValid && isPidValid);
    m_pUi->snapshotOldLineEdit->setEnabled(isUmdhValid && isPidValid);
    m_pUi->snapshotOldPushButtonCreate->setEnabled(isUmdhValid && isPidValid && isTargetProcessRunning);
    m_pUi->snapshotOldPushButtonAdd->setEnabled(isUmdhValid);
    setLineEditValid(m_pUi->snapshotOldLineEdit,
                     isPathToFileValid(m_pUi->snapshotOldLineEdit->text()) ||
                     m_pUi->snapshotOldLineEdit->text().isEmpty());

    const bool isSnapshotOldValid = isPathValid(m_pUi->snapshotOldLineEdit->text(), true) &&
                                    !m_pUi->snapshotOldLineEdit->text().isEmpty();
    m_pUi->snapshotNewLabel->setEnabled(isUmdhValid && isPidValid && isSnapshotOldValid);
    m_pUi->snapshotNewLineEdit->setEnabled(isUmdhValid && isPidValid && isSnapshotOldValid);
    m_pUi->snapshotNewPushButtonCreate->setEnabled(isUmdhValid &&
                                                   isPidValid &&
                                                   isTargetProcessRunning &&
                                                   isSnapshotOldValid);
    m_pUi->snapshotNewPushButtonAdd->setEnabled(isUmdhValid && isSnapshotOldValid);
    setLineEditValid(m_pUi->snapshotNewLineEdit,
                     isPathToFileValid(m_pUi->snapshotNewLineEdit->text()) ||
                     m_pUi->snapshotNewLineEdit->text().isEmpty());

    m_pUi->createReportPushButton->setEnabled(isUmdhValid
                                              && isPathValid(m_pUi->snapshotNewLineEdit->text(), true)
                                              && !m_pUi->snapshotNewLineEdit->text().isEmpty()
                                              && isPathValid(m_pUi->snapshotOldLineEdit->text(), true)
                                              && !m_pUi->snapshotOldLineEdit->text().isEmpty());
}

void UmdhWidget::on_pathToUmdhPushButtonAdd_clicked()
{
    const auto &currentPath = QString::fromStdWString(m_pSettings->getUmdhPath());
    const auto &path = QFileDialog::getOpenFileName(this, tr("Choose path to UMDH"), currentPath, "UMDH executable (*.exe)");
    if (path.isEmpty())
        return;

    m_pSettings->setUmdhPath(path.toStdWString());
}

void UmdhWidget::on_folderForSnapshotsPushButtonAdd_clicked()
{
    const auto &currentPath = QString::fromStdWString(m_pSettings->getFolderForSnapshots());
    const auto &path = QFileDialog::getExistingDirectory(this, tr("Choose directory"),
                                                         currentPath,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if (path.isEmpty())
        return;

    m_pSettings->setFolderForSnapshots(path.toStdWString());
}

void UmdhWidget::on_snapshotOldPushButtonAdd_clicked()
{
    const auto &path = QFileDialog::getOpenFileName(this, tr("Choose first snapshot"), m_pUi->snapshotOldLineEdit->text(), "UMDH snapshot (*.*)");
    if (path.isEmpty())
        return;

    m_pUi->snapshotOldLineEdit->setText(path);
    updateGuiState();
}

void UmdhWidget::on_snapshotNewPushButtonAdd_clicked()
{
    const auto &path = QFileDialog::getOpenFileName(this, tr("Choose second snapshot"), m_pUi->snapshotNewLineEdit->text(), "UMDH snapshot (*.*)");
    if (path.isEmpty())
        return;

    m_pUi->snapshotNewLineEdit->setText(path);
    updateGuiState();
}

void UmdhWidget::on_snapshotOldPushButtonCreate_clicked()
{
    m_pUi->snapshotOldLineEdit->setText(QString::fromStdWString(buildName(m_pSettings, L"first_snapshot")));
    m_pUmdh->createSnapshot(m_pUi->snapshotOldLineEdit->text().toStdWString());
    updateGuiState();
}

void UmdhWidget::on_snapshotNewPushButtonCreate_clicked()
{
    m_pUi->snapshotNewLineEdit->setText(QString::fromStdWString(buildName(m_pSettings, L"second_snapshot")));
    m_pUmdh->createSnapshot(m_pUi->snapshotNewLineEdit->text().toStdWString());
    updateGuiState();
}

void UmdhWidget::on_pathToUmdhLineEdit_editingFinished()
{
    m_pSettings->setUmdhPath(m_pUi->pathToUmdhLineEdit->text().toStdWString());
}

void UmdhWidget::on_ntSymbolPathLineEdit_editingFinished()
{
    m_pSettings->setNtSymbolPath(m_pUi->ntSymbolPathLineEdit->text().toStdWString());

    if (m_pUi->ntSymbolPathLineEdit->text().isEmpty())
    {
        SetEnvironmentVariable(L"_NT_SYMBOL_PATH", NULL);
    }
    else
    {
        SetEnvironmentVariable(L"_NT_SYMBOL_PATH", (m_pUi->ntSymbolPathLineEdit->text().toStdWString().c_str()));
    }
}

void UmdhWidget::on_folderForSnapshotsLineEdit_editingFinished()
{
    m_pSettings->setFolderForSnapshots(m_pUi->folderForSnapshotsLineEdit->text().toStdWString());
}

void UmdhWidget::on_snapshotOldLineEdit_editingFinished()
{
    updateGuiState();
}

void UmdhWidget::on_snapshotNewLineEdit_editingFinished()
{
    updateGuiState();
}

void UmdhWidget::on_createReportPushButton_clicked()
{
    m_pUmdh->createReport(buildName(m_pSettings, L"report"),
                          m_pUi->snapshotOldLineEdit->text().toStdWString(),
                          m_pUi->snapshotNewLineEdit->text().toStdWString());
}

void UmdhWidget::on_targetProcessNameLineEdit_editingFinished()
{
    m_pSettings->setTargetProgramm(m_pUi->targetProcessNameLineEdit->text().toStdWString());
}

void UmdhWidget::on_pidLineEdit_editingFinished()
{
    m_pSettings->setPID(m_pUi->pidLineEdit->text().toStdWString());
}
