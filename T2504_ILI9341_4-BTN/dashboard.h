#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__


void dashboard_initialize(void);

void dashboard_start_task(void);

void dashboard_set_menu_label(char *label);

void dashboard_set_menu_text(uint8_t mindx, char *label);

void dashboard_next_sensor(void);

void dashboard_previous_sensor(void);

#endif