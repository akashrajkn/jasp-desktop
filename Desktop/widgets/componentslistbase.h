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

#ifndef COMPONENTSLIST_H
#define COMPONENTSLIST_H

#include "jasplistcontrol.h"
#include "analysis/options/boundcontrol.h"
#include "analysis/options/optionstable.h"
#include "listmodeltermsassigned.h"

class ComponentsListBase : public JASPListControl, public BoundControl
{
	Q_OBJECT
public:
	ComponentsListBase(QQuickItem* item = nullptr);

	ListModel*	model()								const	override { return _termsModel; }
	void		setUpModel()								override;
	Option*		boundTo()									override { return _boundTo; }
	void		bindTo(Option *option)						override;
	Option*		createOption()								override;
	bool		isOptionValid(Option* option)				override;
	bool		isJsonValid(const Json::Value& optionValue)	override;

protected slots:
	void		termsChangedHandler()						override;
	void		addItemHandler();
	void		removeItemHandler(int index);
	void		nameChangedHandler(int index, QString name);

private:
	ListModelTermsAssigned*		_termsModel		= nullptr;
	OptionsTable*				_boundTo		= nullptr;

protected:
	QString				_makeUnique(const QString& val, int index = -1);
	QString				_makeUnique(const QString& val, const QList<QString>& values, int index = -1);
	QString				_changeLastNumber(const QString& val);


};

#endif // COMPONENTSLIST_H
