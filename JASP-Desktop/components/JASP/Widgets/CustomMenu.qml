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

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.12
import JASP.Theme 1.0

Item
{
	id					: menu
	width				: menuRectangle.width
	height				: menuRectangle.height
	property var props	: undefined
	property bool hasIcons: true

	property real _iconPad: 5 * preferencesModel.uiScale

	onPropsChanged:	hasIcons = (menu.props === undefined || "undefined" === typeof(menu.props["hasIcons"])) ? true : menu.props["hasIcons"]

	function resizeElements(newWidth)
	{
		for (var i = 0; i < repeater.count; ++i) {
			repeater.itemAt(i).width = newWidth;
		}
	}

	Rectangle
	{
		id		: menuRectangle
		z		: menuShadow.z + 1
		color	: Theme.fileMenuColorBackground
	}

	Column
	{
		id		: column
		x		: menuRectangle.x
		y		: menuRectangle.y + (Theme.menuPadding / 2)
		z		: menuRectangle.z + 1
		spacing	: Theme.menuSpacing

		Repeater
		{
			id		: repeater
			model	: menu.props === undefined ? undefined : menu.props["model"]

			onItemAdded:
			{
				if (index === 0)
				{
					menuRectangle.width = 0;
					menuRectangle.height = 0;
				}

				menuRectangle.width = Math.max(item.width, menuRectangle.width);
				menuRectangle.height += (item.height + Theme.menuSpacing)

				if (index === count - 1)
				{
					menuRectangle.height += (Theme.menuPadding - Theme.menuSpacing)
					menu.resizeElements(menuRectangle.width);
				}
			}

			delegate: Loader
			{
				sourceComponent :
				{
					if (model.isSeparator !== undefined && model.isSeparator)			return menuSeparator;
					else if (model.isGroupTitle !== undefined && model.isGroupTitle)	return menuGroupTitle;

					return menuDelegate
				}

				Component
				{
					id: menuDelegate

					Rectangle
					{
						id		: menuItem
						width	: initWidth
						height	: Theme.menuItemHeight
						color	: mouseArea.pressed ? Theme.buttonColorPressed : mouseArea.containsMouse ? Theme.buttonColorHovered : "transparent"

						property double initWidth: (menu.hasIcons ? menuItemImage.width : 0) + menuItemText.implicitWidth + (menu.hasIcons ? 5 : 4) * menu._iconPad
						// 5 * _iconPad: 2 menuItemImage.leftMargin + 2 menuItemText.rightMargin + 1 menuItemText.leftMargin
						// 4 * _iconPad: 2 menuItemText.leftMargin  + 2 menuItemText.rightMargin

						Image
						{
							id		: menuItemImage
							height	: menuItem.height - menu._iconPad
							width	: menuItem.height - menu._iconPad

							source					: menuImageSource
							smooth					: true
							sourceSize.width		: width * 2
							sourceSize.height		: height * 2
							fillMode				: Image.PreserveAspectFit

							anchors.left			: parent.left
							anchors.leftMargin		: menu._iconPad * 2
							anchors.verticalCenter	: parent.verticalCenter
						}

						Text
						{
							id					: menuItemText
							text				: displayText
							height				: menuItem.height
							font				: Theme.font
							verticalAlignment	: Text.AlignVCenter

							anchors.left		: menu.hasIcons ? menuItemImage.right : parent.left
							anchors.leftMargin	: (menu.hasIcons ? 1 : 2) * menu._iconPad
							anchors.rightMargin	: menu._iconPad * 2
						}

						MouseArea
						{
							id				: mouseArea
							hoverEnabled	: true
							anchors.fill	: parent
							onClicked		: menu.props['functionCall'](index)
						}
					}
				}
				Component
				{
					id: menuGroupTitle

					Item
					{
						id		: menuItem
						width	: initWidth
						height	: Theme.menuGroupTitleHeight

						property double initWidth: menuItemImage.width + menuItemText.implicitWidth + 5 * menu._iconPad * preferencesModel.uiScale


						Image
						{
							id		: menuItemImage
							height	: menuItem.height - menu._iconPad
							width	: menuItem.height - menu._iconPad

							source					: menuImageSource
							smooth					: true
							sourceSize.width		: width * 2
							sourceSize.height		: height * 2
							fillMode				: Image.PreserveAspectFit

							anchors.left			: parent.left
							anchors.leftMargin		: menu._iconPad * 2
							anchors.verticalCenter	: parent.verticalCenter
						}

						Text
						{
							id						: menuItemText
							text					: displayText
							font					: Theme.fontGroupTitle
							anchors.left			: menuImageSource ? menuItemImage.right : menuItem.left
							anchors.leftMargin		: menu._iconPad
							anchors.verticalCenter	: parent.verticalCenter
						}
					}
				}

				Component
				{
					id	: menuSeparator
					ToolSeparator { orientation	: Qt.Horizontal }
				}
			}
		}
	}

	RectangularGlow
	{
		id				: menuShadow
		anchors.fill	: menuRectangle

		color			: Theme.grayDarker
		spread			: 0.2
		cornerRadius	: menuRectangle.radius + glowRadius
		glowRadius		: 5
	}
}
