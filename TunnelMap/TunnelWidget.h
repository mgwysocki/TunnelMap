#ifndef __TUNNELWIDGET_H
#define __TUNNELWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QLineEdit>
#include <QCheckBox>
#include <QTime>
#include <QAction>
class QPushButton;
class QLabel;

class TunnelWidget : public QWidget
{
  Q_OBJECT

 public:
  TunnelWidget(QWidget* parent=0);
  ~TunnelWidget();

  const QSize sizeHint();
  QAction* get_action() {return action_;}
  
  QString get_name() {return name_line_edit_->text();}
  QString get_args() {return args_line_edit_->text();}
  bool get_enabled() {return (auto_connect_->checkState() == Qt::Checked);}

  void set_name(QString s) {name_line_edit_->setText(s); action_->setText(s);}
  void set_args(QString s) {args_line_edit_->setText(s);}
  void set_enabled(bool b) {auto_connect_->setCheckState( b ? Qt::Checked : Qt::Unchecked ); if(b) open();}

 signals:
  void connected(QString, QString);
  void disconnected(QString);

 public slots:
  void open();
  void close();

 protected slots:
  void when_connected();
  void when_disconnected();
  void toggle_state();
  void change_name(QString);

 protected:
  QString build_cmd_();

  QLabel* name_label_;
  QLineEdit* name_line_edit_;
  QLabel* args_label_;
  QLineEdit* args_line_edit_;

  QPushButton* connect_button_;
  QPushButton* disconnect_button_;

  QCheckBox* auto_connect_;
  QAction* action_;

  QProcess process_;
  QTime timer_;
  bool close_initiated_;

  static QIcon red_icon_;
  static QIcon green_icon_;
};

#endif
