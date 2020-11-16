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

#ifndef COMBOBOXBASE_H
#define COMBOBOXBASE_H

#include "jasplistcontrol.h"
#include "analysis/options/boundcontrol.h"
#include "analysis/options/optionlist.h"
#include "listmodel.h"
#include "listmodellabelvalueterms.h"
#include <QMap>

class ComboBoxBase : public JASPListControl, public BoundControl
{
	Q_OBJECT
	
public:
	ComboBoxBase(QQuickItem* parent = nullptr);

	void		bindTo(Option *option)						override;
	Option*		createOption()								override;
	bool		isOptionValid(Option* option)				override;	
	bool		isJsonValid(const Json::Value& optionValue) override;
	Option*		boundTo()									override	{ return _boundTo; }
	void		setUp()										override;
	ListModel*	model()								const	override	{ return _model; }
	void		setUpModel()								override;

protected slots:
	void termsChangedHandler() override;
	void comboBoxChangeValueSlot(int index);
	void languageChangedHandler();

protected:
	OptionList*					_boundTo				= nullptr;
	int							_currentIndex			= 0;
	QString						_currentText;
	QString						_currentColumnType;
	ListModelLabelValueTerms*	_model					= nullptr;

	void _setLabelValues();
	void _resetItemWidth();
	void _resetOptions();
	void _setCurrentValue(int index, bool setComboBoxIndex = true, bool setOption = true);
};

#endif // COMBOBOXBASE_H
