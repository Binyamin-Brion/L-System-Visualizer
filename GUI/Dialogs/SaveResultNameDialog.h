//
// Created by binybrion on 5/9/20.
//

#ifndef VOXEL_L_SYSTEM_SAVERESULTNAMEDIALOG_H
#define VOXEL_L_SYSTEM_SAVERESULTNAMEDIALOG_H

#include <QtWidgets/QDialog>

namespace Ui
{
    class SaveResultNameDialog;
}

namespace GUI
{
    namespace Dialogs
    {
        class SaveResultNameDialog : public QDialog
        {
                Q_OBJECT

            public:
                explicit SaveResultNameDialog(QWidget *parent = nullptr);
                int exec() override;
                QString getMostRecentName() const;
                bool noExistingNames() const;
                void removeExistingName(const QString &name);

            private slots:
                void checkUniqueName(const QString &nam);

            private:
                void setupConnections();

                Ui::SaveResultNameDialog *ui = nullptr;
                std::vector<QString> existingNames;
        };
    }
}

#endif //VOXEL_L_SYSTEM_SAVERESULTNAMEDIALOG_H
