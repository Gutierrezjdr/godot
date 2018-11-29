/*************************************************************************/
/*  button.cpp                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "button.h"
#include "core/translation.h"
#include "servers/visual_server.h"

Size2 Button::get_minimum_size() const {

	Size2 minsize = label->get_minimum_size();

	Ref<Texture> _icon;
	if (icon.is_null() && has_icon("icon"))
		_icon = Control::get_icon("icon");
	else
		_icon = icon;

	if (!_icon.is_null()) {

		minsize.height = MAX(minsize.height, _icon->get_height());
		minsize.width += _icon->get_width();

	}

	return get_stylebox("normal")->get_minimum_size() + minsize;
}

void Button::_set_internal_margin(Margin p_margin, float p_value) {
	_internal_margin[p_margin] = p_value;
}

void Button::_notification(int p_what) {

	if (p_what == NOTIFICATION_DRAW) {

		RID ci = get_canvas_item();
		Size2 size = get_size();
		Color color;
		Color color_icon(1, 1, 1, 1);

		Ref<StyleBox> style = get_stylebox("normal");

		switch (get_draw_mode()) {

			case DRAW_NORMAL: {

				style = get_stylebox("normal");
				if (!flat)
					style->draw(ci, Rect2(Point2(0, 0), size));
				color = get_color("font_color");
				if (has_color("icon_color_normal"))
					color_icon = get_color("icon_color_normal");
			} break;
			case DRAW_HOVER_PRESSED: {
				if (has_stylebox("hover_pressed") && has_stylebox_override("hover_pressed")) {
					style = get_stylebox("hover_pressed");
					if (!flat)
						style->draw(ci, Rect2(Point2(0, 0), size));
					if (has_color("font_color_hover_pressed"))
						color = get_color("font_color_hover_pressed");
					else
						color = get_color("font_color");
					if (has_color("icon_color_hover_pressed"))
						color_icon = get_color("icon_color_hover_pressed");

					break;
				}
			}
			case DRAW_PRESSED: {

				style = get_stylebox("pressed");
				if (!flat)
					style->draw(ci, Rect2(Point2(0, 0), size));
				if (has_color("font_color_pressed"))
					color = get_color("font_color_pressed");
				else
					color = get_color("font_color");
				if (has_color("icon_color_pressed"))
					color_icon = get_color("icon_color_pressed");

			} break;
			case DRAW_HOVER: {

				style = get_stylebox("hover");
				if (!flat)
					style->draw(ci, Rect2(Point2(0, 0), size));
				color = get_color("font_color_hover");
				if (has_color("icon_color_hover"))
					color_icon = get_color("icon_color_hover");

			} break;
			case DRAW_DISABLED: {

				style = get_stylebox("disabled");
				if (!flat)
					style->draw(ci, Rect2(Point2(0, 0), size));
				color = get_color("font_color_disabled");
				if (has_color("icon_color_disabled"))
					color_icon = get_color("icon_color_disabled");

			} break;
		}

		if (has_focus()) {

			Ref<StyleBox> style = get_stylebox("focus");
			style->draw(ci, Rect2(Point2(), size));
		}

		Ref<Font> font = get_font("font");
		Ref<Texture> _icon;
		if (icon.is_null() && has_icon("icon"))
			_icon = Control::get_icon("icon");
		else
			_icon = icon;
		
		if (!_icon.is_null()) {

			int valign = size.height - style->get_minimum_size().y;
			if (is_disabled())
				color_icon.a = 0.4;
			if (_internal_margin[MARGIN_LEFT] > 0) {
				_icon->draw(ci, style->get_offset() + Point2(_internal_margin[MARGIN_LEFT] + get_constant("hseparation"), Math::floor((valign - _icon->get_height()) / 2.0)), color_icon);
			} else {
				_icon->draw(ci, style->get_offset() + Point2(0, Math::floor((valign - _icon->get_height()) / 2.0)), color_icon);
			}
			label->set_margin(MARGIN_LEFT, _icon->get_width() + get_constant("hseparation"));
		}
	}
}

void Button::set_text(const String &p_text) {

	if (get_text() == p_text)
		return;
	label->set_text(p_text);
	update();
	_change_notify("text");
	minimum_size_changed();
}
String Button::get_text() const {

	return label->get_text();
}

void Button::set_icon(const Ref<Texture> &p_icon) {

	if (icon == p_icon)
		return;
	icon = p_icon;
	update();
	_change_notify("icon");
	minimum_size_changed();
}

Ref<Texture> Button::get_icon() const {

	return icon;
}

void Button::set_flat(bool p_flat) {

	flat = p_flat;
	update();
	_change_notify("flat");
}

bool Button::is_flat() const {

	return flat;
}

void Button::set_clip_text(bool p_clip_text) {

	label->set_clip_text(p_clip_text);
	update();
	minimum_size_changed();
}

bool Button::get_clip_text() const {

	return label->is_clipping_text();
}

void Button::set_text_align(Label::Align p_align) {

	label->set_align(p_align);
	update();
}

Label::Align Button::get_text_align() const {

	return label->get_align();
}

void Button::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_text", "text"), &Button::set_text);
	ClassDB::bind_method(D_METHOD("get_text"), &Button::get_text);
	ClassDB::bind_method(D_METHOD("set_button_icon", "texture"), &Button::set_icon);
	ClassDB::bind_method(D_METHOD("get_button_icon"), &Button::get_icon);
	ClassDB::bind_method(D_METHOD("set_flat", "enabled"), &Button::set_flat);
	ClassDB::bind_method(D_METHOD("set_clip_text", "enabled"), &Button::set_clip_text);
	ClassDB::bind_method(D_METHOD("get_clip_text"), &Button::get_clip_text);
	ClassDB::bind_method(D_METHOD("set_text_align", "align"), &Button::set_text_align);
	ClassDB::bind_method(D_METHOD("get_text_align"), &Button::get_text_align);
	ClassDB::bind_method(D_METHOD("is_flat"), &Button::is_flat);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "text", PROPERTY_HINT_MULTILINE_TEXT, "", PROPERTY_USAGE_DEFAULT_INTL), "set_text", "get_text");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture"), "set_button_icon", "get_button_icon");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "flat"), "set_flat", "is_flat");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "clip_text"), "set_clip_text", "get_clip_text");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "align", PROPERTY_HINT_ENUM, "Left,Center,Right"), "set_text_align", "get_text_align");
}

Button::Button(const String &p_text) {

	flat = false;
	set_mouse_filter(MOUSE_FILTER_STOP);

	for (int i = 0; i < 4; i++) {
		_internal_margin[i] = 0;
	}

	label = memnew(Label(p_text));
	label->set_mouse_filter(MOUSE_FILTER_IGNORE);
	label->set_align(Label::ALIGN_CENTER);
	label->set_valign(Label::VALIGN_CENTER);
	add_child(label);
	label->set_anchors_and_margins_preset(PRESET_WIDE, PRESET_MODE_MINSIZE);
}

Button::~Button() {
}
