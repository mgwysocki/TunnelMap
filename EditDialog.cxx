#include "EditDialog.h"
#include "TunnelProcess.h"

#include <QPushButton.h>
#include <QCheckBox.h>
#include <QLineEdit.h>
#include <QTextEdit.h>
#include <QGridLayout.h>
#include <QLabel.h>

#include <iostream>
using namespace std;

EditDialog::EditDialog(TunnelProcess* tp, QWidget* parent) :
  QDialog::QDialog(parent),
  tunnel_proc_(tp)
{
  QGridLayout* layout = new QGridLayout;
  QLabel* name_label = new QLabel("Tunnel Name:");
  name_edit_ = new QLineEdit( tp->get_name() );
  QLabel* ssh_cmd_label = new QLabel("ssh Command:");
  ssh_cmd_edit_ = new QLineEdit( tp->get_ssh_cmd() );
  ssh_cmd_edit_->setReadOnly(true);
  QLabel* ssh_args_label = new QLabel("ssh Standard Arguments:");
  ssh_args_edit_ = new QLineEdit( tp->get_ssh_args() );
  ssh_args_edit_->setReadOnly(true);
  QLabel* args_label = new QLabel("Tunnel-specific Arguments:");
  args_edit_ = new QTextEdit( tp->get_args() );

  auto_connect_box_ = new QCheckBox("Auto-connect");
  dependent_box_ = new QCheckBox("Dependent tunnel");
  if(tp->is_auto_connect())  auto_connect_box_->setCheckState( Qt::Checked );
  if(tp->is_dependent())  dependent_box_->setCheckState( Qt::Checked );

  QPushButton* okay_btn = new QPushButton("OK");
  QPushButton* cancel_btn = new QPushButton("Cancel");
  connect(okay_btn, SIGNAL(clicked()),
	  this,       SLOT(accept()));
  connect(cancel_btn, SIGNAL(clicked()),
	  this,         SLOT(reject()));

  layout->addWidget(name_label, 0, 0, 1, 1);
  layout->addWidget(name_edit_, 0, 1, 1, 1);
  layout->addWidget(ssh_cmd_label, 1, 0, 1, 1);
  layout->addWidget(ssh_cmd_edit_, 1, 1, 1, 1);
  layout->addWidget(ssh_args_label, 2, 0, 1, 1);
  layout->addWidget(ssh_args_edit_, 2, 1, 1, 1);
  layout->addWidget(args_label, 3, 0, 1, 1);
  layout->addWidget(args_edit_, 3, 1, 3, 1);
  layout->addWidget(auto_connect_box_, 5, 0, 1, 1);
  layout->addWidget(dependent_box_, 6, 0, 1, 1);
  layout->addWidget(okay_btn, 7, 0, 1, 1);
  layout->addWidget(cancel_btn, 7, 1, 1, 1);
  setLayout(layout);
}

void EditDialog::accept()
{
  tunnel_proc_->set_name( name_edit_->text() );
  tunnel_proc_->set_args( args_edit_->toPlainText() );
  tunnel_proc_->set_auto_connect( auto_connect_box_->checkState() == Qt::Checked );
  tunnel_proc_->set_dependent( dependent_box_->checkState() == Qt::Checked );
  QDialog::accept();
  return;
}

void EditDialog::set_tunnel_process(TunnelProcess* tp)
{
  tunnel_proc_ = tp;
  name_edit_->setText( tp->get_name() );
  args_edit_->setText( tp->get_args() );
  ssh_cmd_edit_->setText( tp->get_ssh_cmd() );
  ssh_args_edit_->setText( tp->get_ssh_args() );
  auto_connect_box_->setCheckState( tp->is_auto_connect() ? Qt::Checked : Qt::Unchecked );
  dependent_box_->setCheckState( tp->is_dependent() ? Qt::Checked : Qt::Unchecked );
  return;
}
