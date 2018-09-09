//
// Created by obiwac on 21.07.18.
//

#ifndef ANDROID_EVENTS_H
#define ANDROID_EVENTS_H

#include "lib/structs.h"
#include "video.h"

int event_pointer_x = 0;
int event_pointer_y = 0;

int notification_tray_offset;
int event_pointer_click_type = 0;
int event_quit = 0;

bool has_the_event_been_updated_in_the_previous_call_to_Java_com_inobulles_obiwac_aqua_Lib_event_question_mark = false;
int event_last_release = 1;

void get_events(unsigned long long ____this) {
	event_list_t* __this = (event_list_t*) ____this;

	__this->quit = (unsigned long long) event_quit;
	__this->resize = gl_resize;

	__this->pointer_click_type = (unsigned long long) !event_last_release;//event_last_release ? 0 : (has_the_event_been_updated_in_the_previous_call_to_Java_com_inobulles_obiwac_aqua_Lib_event_question_mark ? (unsigned long long) event_pointer_click_type : 0);
	has_the_event_been_updated_in_the_previous_call_to_Java_com_inobulles_obiwac_aqua_Lib_event_question_mark = false;

	__this->pointer_x = (unsigned long long) event_pointer_x;
	__this->pointer_y = (unsigned long long) event_pointer_y - notification_tray_offset;

	gl_resize = 0;

}

static bool done_free_events_warning = false;

void free_events(unsigned long long ____this) {
	event_list_t* __this = (event_list_t*) ____this;

	if (!done_free_events_warning) {
		printf("WARNING This function is deprecated\n");
		done_free_events_warning = true;

	}

}

void init(void) {


}

#endif