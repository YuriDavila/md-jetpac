/*
 * physics.c
 *
 *  Created on: Apr 19, 2019
 *      Author: diegomtassis
 */

#include "../inc/physics.h"

// relative position
#define IN_BETWEEN		0x01
#define OVERLAPPED		0x02
#define TO_THE_LEFT		0x04
#define TO_THE_RIGHT	0x08
#define ABOVE			0x10
#define UNDER			0x20

static u8 axisXBoxRelativePos(Box_f16, Box_f16);
static u8 axisYBoxRelativePos(Box_f16, Box_f16);
static u8 axisXPxRelativePos(fix16, Box_f16);
static u8 axisYPxRelativePos(fix16, Box_f16);

void updateBox(Object_f16* object) {

	object->box->x = object->pos.x;
	object->box->y = object->pos.y;
}

Box_f16 targetHBox(const Object_f16* object, u8 width, u8 height) {

	fix16 target_x = fix16Add(object->pos.x, object->mov.x);
	Box_f16 box = { .x = target_x, .y = object->pos.y, .w = width, .h = height };

	return box;
}

Box_f16 targetVBox(const Object_f16* object, u8 width, u8 height) {

	fix16 target_y = fix16Add(object->pos.y, object->mov.y);
	Box_f16 box = { .x = object->pos.x, .y = target_y, .w = width, .h = height };

	return box;
}

fix16 hitAbove(Box_f16 subject, Box_f16 object) {

	if (OVERLAPPED & axisXBoxRelativePos(subject, object)) {

		fix16 subject_h_f16 = FIX16(subject.h);
		fix16 subject_bottom = fix16Add(subject.y, subject_h_f16);

		if (IN_BETWEEN & axisYPxRelativePos(subject_bottom, object)) {
			return fix16Sub(object.y, subject_h_f16);
		}
	}

	return 0;
}

fix16 hitUnder(Box_f16 subject, Box_f16 object) {

	if (OVERLAPPED & axisXBoxRelativePos(subject, object)) {
		if (IN_BETWEEN & axisYPxRelativePos(subject.y, object)) {
			return fix16Add(object.y, FIX16(object.h));
		}
	}

	return 0;
}

fix16 hitLeft(Box_f16 subject, Box_f16 object) {

	if (OVERLAPPED & axisYBoxRelativePos(subject, object)) {

		fix16 subject_w_f16 = FIX16(subject.w);
		fix16 subject_right_edge = fix16Add(subject.x, subject_w_f16);

		if (IN_BETWEEN & axisXPxRelativePos(subject_right_edge, object)) {
			return fix16Sub(object.x, subject_w_f16);
		}
	}

	return 0;
}

fix16 hitRight(Box_f16 subject, Box_f16 object) {

	if (OVERLAPPED & axisYBoxRelativePos(subject, object)) {
		if (IN_BETWEEN & axisXPxRelativePos(subject.x, object)) {
			return fix16Add(object.x, FIX16(object.w));
		}
	}

	return 0;
}

static u8 axisXBoxRelativePos(Box_f16 subject, Box_f16 object) {

	if (TO_THE_RIGHT & axisXPxRelativePos(subject.x, object)) {
		return TO_THE_RIGHT;
	}

	if (TO_THE_LEFT & axisXPxRelativePos(subject.x + FIX16(subject.w), object)) {
		return TO_THE_LEFT;
	}

	return OVERLAPPED;
}

static u8 axisYBoxRelativePos(Box_f16 subject, Box_f16 object) {

	if (UNDER & axisYPxRelativePos(subject.y, object)) {
		return UNDER;
	}

	if (ABOVE & axisYPxRelativePos(subject.y + FIX16(subject.h), object)) {
		return ABOVE;
	}

	return OVERLAPPED;
}

static u8 axisXPxRelativePos(fix16 x_px, Box_f16 object) {

	if (object.x >= x_px) {
		return TO_THE_LEFT;
	}

	if (x_px >= fix16Add(object.x, FIX16(object.w))) {
		return TO_THE_RIGHT;
	}

	return IN_BETWEEN;
}

static u8 axisYPxRelativePos(fix16 y_px, Box_f16 object) {

	if (object.y >= y_px) {
		return ABOVE;
	}

	if (fix16Add(object.y, FIX16(object.h)) <= y_px) {
		return UNDER;
	}

	return IN_BETWEEN;
}
