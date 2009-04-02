//----------------------------------------------------------------
// Class: EditDialog
// Description: This is the dialog where tunnel properties are
// modified.  It is opened from the ConfigWindow.
// ----------------------------------------------------------------

#ifndef EDITDIALOG_H__
#define EDITDIALOG_H__

#include <QDialog.h>
class QLineEdit;
class QTextEdit;
class QCheckBox;
class TunnelProcess;

class EditDialog: public QDialog
{
Q_OBJECT

 public:
  EditDialog(TunnelProcess* tp, QWidget* parent=0);

  void set_tunnel_process(TunnelProcess* tp);
  void set_ssh_cmd(QString s) {ssh_cmd_ = s;}
  void set_ssh_args(QString s) {ssh_args_ = s;}

  virtual void accept();

 protected:
  QLineEdit* name_edit_;
  QLineEdit* ssh_cmd_edit_;
  QLineEdit* ssh_args_edit_;
  QTextEdit* args_edit_;
  QCheckBox* auto_connect_box_;
  QCheckBox* dependent_box_;

  TunnelProcess* tunnel_proc_;
  QString ssh_cmd_;
  QString ssh_args_;
};

#endif // EDITDIALOG_H__
