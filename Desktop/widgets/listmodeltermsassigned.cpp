//
// Copyright (C) 2013-2018 University of Amsterdam
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//

#include "listmodeltermsassigned.h"
#include "listmodeltermsavailable.h"
#include "analysis/analysisform.h"
#include "rowcontrols.h"
#include "jasplistcontrol.h"
#include <QTimer>


using namespace std;

ListModelTermsAssigned::ListModelTermsAssigned(JASPListControl* listView, int maxRows)
	: ListModelAssignedInterface(listView)
	, _maxRows(maxRows)
{
}

void ListModelTermsAssigned::initTerms(const Terms &terms, const RowControlsOptions& allOptionsMap)
{
	ListModelAssignedInterface::initTerms(terms, allOptionsMap);

	if (source() != nullptr)
	{
		if (!_copyTermsWhenDropped)
			source()->removeTermsInAssignedList();
	}
}

void ListModelTermsAssigned::availableTermsResetHandler(Terms termsAdded, Terms termsRemoved)
{
	if (termsAdded.size() > 0 && _addNewAvailableTermsToAssignedModel)
	{
		beginResetModel();
		_addTerms(termsAdded);
		endResetModel();

		if (!_copyTermsWhenDropped)
			source()->removeTermsInAssignedList();
	}

	if (termsRemoved.size() > 0)
	{
		beginResetModel();
		_removeTerms(termsRemoved);
		endResetModel();
	}
}

Terms ListModelTermsAssigned::canAddTerms(const Terms& terms) const
{
	if (_maxRows >= 0 && int(terms.size()) > _maxRows)
		return Terms();

	return ListModelDraggable::canAddTerms(terms);
}

Terms ListModelTermsAssigned::addTerms(const Terms& termsToAdd, int dropItemIndex, JASPControl::AssignType)
{
	_tempTermsToSendBack.clear();

	beginResetModel();

	Terms newTerms = terms();
	if (dropItemIndex < 0 && _maxRows == 1)
		dropItemIndex = 0; // for single row, per default replace old item by new one.
	if (dropItemIndex >= 0 && dropItemIndex < int(terms().size()))
		newTerms.insert(dropItemIndex, termsToAdd);
	else
		newTerms.add(termsToAdd);

	size_t maxRows = size_t(_maxRows);
	if (newTerms.size() > maxRows)
	{
		for (size_t i = maxRows; i < newTerms.size(); i++)
			_tempTermsToSendBack.add(newTerms.at(i));
		newTerms.remove(maxRows, newTerms.size() - maxRows);
	}

	_setTerms(newTerms);

	endResetModel();

	return _tempTermsToSendBack;
}

Terms ListModelTermsAssigned::termsEx(const QString &what)
{
	if (_maxRows == 1 && what == "levels")
	{
		Terms result;
		if (rowCount() == 1)
		{
			QString term = data(index(0,0)).toString();
			QStringList labels = DataSetPackage::pkg()->getColumnLabelsAsStringList(term.toStdString());
			for (const QString& label : labels)
				result.add(label);
		}

		return result;
	}
	else
		return ListModelAssignedInterface::termsEx(what);
}

void ListModelTermsAssigned::removeTerm(int index)
{
	if (index < 0 || index >= int(terms().size())) return;

	beginResetModel();

	const Term& term = terms().at(size_t(index));
	const QString& termQ = term.asQString();

	if (_rowControlsMap.contains(termQ))
	{
		RowControls* controls = _rowControlsMap[termQ];
		if (controls)
			for (JASPControl* control : controls->getJASPControlsMap().values())
			{
				control->setHasError(false);
				listView()->form()->clearControlError(control);
			}

		_rowControlsMap.remove(termQ);
	}
	_removeTerm(term);

	endResetModel();
}

void ListModelTermsAssigned::changeTerm(int index, const QString& name)
{
	QString oldName = terms()[size_t(index)].asQString();
	if (oldName != name)
	{
		_rowControlsMap[name] = _rowControlsMap[oldName];
		_rowControlsOptions[name] = _rowControlsOptions[oldName];
		_rowControlsMap.remove(oldName);
		_rowControlsOptions.remove(oldName);
		_replaceTerm(index, Term(name));

		emit oneTermChanged(oldName, name);
		QModelIndex modelIndex = ListModelTermsAssigned::index(index, 0);
		emit dataChanged(modelIndex, modelIndex);
	}
}

