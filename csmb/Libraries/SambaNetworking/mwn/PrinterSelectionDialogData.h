/* Name: PrinterSelectiondialogData.h

   Description: This file is a part of the libmwn library.

   Author:	Oleg Noskov (olegn@corel.com)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.


*/


#ifndef CPrinterSelectionDialogData_included
#define CPrinterSelectionDialogData_included

#include <qdialog.h>
#include <qlabel.h>

#include "listview.h"
#define QListView CListView
#define QListViewItem CListViewItem

#include <qpushbutton.h>
#include <qlineedit.h>

class CPrinterSelectionDialogData : public QDialog
{
    Q_OBJECT

public:

    CPrinterSelectionDialogData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CPrinterSelectionDialogData();

public slots:


protected slots:

    virtual void OnDoubleClicked(QListViewItem*);

protected:
    QLabel* m_pPrinterLabel;
    QLineEdit* m_pPath;
    QPushButton* m_pOKButton;
    QPushButton* m_pCancelButton;
    QListView* m_pTree;
    QLabel* m_pConnectAsLabel;
    QLineEdit* m_pConnectAs;

};

#endif // CPrinterSelectionDialogData_included
