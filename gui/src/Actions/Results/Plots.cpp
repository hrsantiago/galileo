//qt
#include <QStandardItemModel>

//gui
#include "Actions/Results/Plots.h"

//ui
#include "ui_Plots.h"

namespace gui
{
	namespace results
	{
		//constructors
		Plots::Plots(std::vector<unsigned>* plots, unsigned steps, QWidget* parent) : QDialog(parent), m_ui(new Ui::Plots), m_steps(steps), m_plots(plots)
		{
			//set ui
			m_ui->setupUi(this);
			//formatter
			char formatter[200];
			sprintf(formatter, "%05d", m_plots->empty() || m_plots->back() == m_steps ? 0 : m_plots->back() + 1);
			//set edit
			m_ui->edit->setText(formatter);
			m_ui->edit->setValidator(new QIntValidator(0, m_steps));
			//set table
			m_ui->table->setColumnCount(1);
			m_ui->table->setHorizontalHeaderItem(0, new QTableWidgetItem("Plots"));
			m_ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); 
			//update
			update_table();
			m_ui->table->setCurrentCell(m_ui->table->rowCount() - 1, 0);
			//connect
			QObject::connect(m_ui->button_add, SIGNAL(clicked(bool)), this, SLOT(slot_add(void)));
			QObject::connect(m_ui->button_clear, SIGNAL(clicked(bool)), this, SLOT(slot_clear(void)));
			QObject::connect(m_ui->button_remove, SIGNAL(clicked(bool)), this, SLOT(slot_remove(void)));
		}
		
		//destructor
		Plots::~Plots(void)
		{
			delete m_ui;
		}
		
		//slots
		void Plots::slot_add(void)
		{
			const unsigned p = m_ui->edit->text().toUInt();
			if(std::find(m_plots->begin(), m_plots->end(), p) == m_plots->end())
			{
				//plots
				m_plots->push_back(p);
				//edit
				char formatter[200];
				sprintf(formatter, "%05d", p == m_steps ? 0 : p + 1);
				m_ui->edit->setText(formatter);
				//table
				update_table();
				m_ui->table->selectRow(m_ui->table->rowCount() - 1);
			}
		}
		void Plots::slot_clear(void)
		{
			//plots
			m_plots->clear();
			//table
			update_table();
			//edit
			char formatter[200];
			sprintf(formatter, "%05d", 0);
			m_ui->edit->setText(formatter);
		}
		void Plots::slot_remove(void)
		{
			//list
			QModelIndexList sr = m_ui->table->selectionModel()->selectedRows(0);
			//remove
			for(unsigned i = 0; i < (unsigned) sr.size(); i++)
			{
				const unsigned j = sr.size() - i - 1;
				m_plots->erase(m_plots->begin() + sr[j].row());
			}
			//table
			update_table();
			m_ui->table->setCurrentCell(m_ui->table->rowCount() - 1, 0);
			//formatter
			char formatter[200];
			sprintf(formatter, "%05d", m_plots->empty() || m_plots->back() == m_steps ? 0 : m_plots->back() + 1);
			//edit
			m_ui->edit->setText(formatter);
		}
		
		//update
		void Plots::update_table(void)
		{
			//set rows
			m_ui->table->setRowCount(m_plots->size());
			//set data
			char formatter[200];
			for(unsigned i = 0; i < m_plots->size(); i++)
			{
				//text
				sprintf(formatter, "%05d", (*m_plots)[i]);
				QTableWidgetItem* item = new QTableWidgetItem(formatter);
				//properties
				item->setTextAlignment(Qt::AlignCenter);
				item->setFlags(item->flags() ^ Qt::ItemIsEditable);
				//add
				m_ui->table->setItem(i, 0, item);
			}
		}
	}
}
