//
// Created by binybrion on 5/2/20.
//

#ifndef VOXEL_L_SYSTEM_VARIABLEENTRY_H
#define VOXEL_L_SYSTEM_VARIABLEENTRY_H

#include <QtWidgets/QWidget>
#include "../../../L_System/DataStructures/Variable/Variable.h"

namespace Ui
{
    class VariableEntry;
}

namespace GUI
{
    namespace L_System
    {
        /**
         * Represents a variable created that can be used in an L-Script.
         */

        class VariableEntry : public QWidget
        {
                Q_OBJECT

            signals:

                /**
                 * Emitted when a new name for the entry is given.
                 *
                 * @param newName name specified by the user for the entry
                 */
                void nameChanged(VariableEntry*, const QString &newName);

                /**
                 * Emitted when a variable is selected by clicking its respective checkbox.
                 *
                 * @param newState state of the associated checkbox
                 */
                void variableSelected(VariableEntry*, int newState);

            public:

                /**
                 * Initializes the widget with the parent that has ownership over this object.
                 *
                 * @param parent that owns this object
                 */
                explicit VariableEntry(QWidget *parent = nullptr);

                /**
                 * Called when a model was loaded, and a variable can now represent that loaded model.
                 *
                 * @param modelName file location of the model that was loaded
                 */
                void addModelEntry(const QString &modelName);

                /**
                 * Get the stored associated model name for this variable.
                 *
                 * @return associated model name
                 */
                QString getAssociatedModelName() const;

                /**
                 * Get the equivalent Variable data structure for this variable for use in the script execution.
                 *
                 * @return equivalent Variable data structure
                 */
                ::L_System::DataStructures::Variable getVariableToken() const;

                /**
                 * Update the line edit field where the entry name is given visually to reflect if the given name is
                 * valid or not.
                 *
                 * @param valid true if the name specified for the entry is valid
                 */
                void nameValid(bool valid);

            private:

                /**
                  * Sets up the connections used by this object's widgets.
                  */
                void setupConnections();

                Ui::VariableEntry *ui = nullptr;
        };
    }
}

#endif //VOXEL_L_SYSTEM_VARIABLEENTRY_H
