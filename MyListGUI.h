#pragma once

#include <QWidget>
#include "ui_MyListGUI.h"
#include "Servcice.h"
#include "MyListModel.h"
#include "Observer.h"

class MyListGUI : public QWidget, public Observer
{
	Q_OBJECT

private:
	Service& service;
	MyListModel* table_model;

public:
	MyListGUI(Service& service, QWidget *parent = Q_NULLPTR);
	~MyListGUI();

private:
	Ui::MyListGUI ui;
	void update() override;
};
