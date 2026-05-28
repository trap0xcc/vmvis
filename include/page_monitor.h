#ifndef _PAGE_MONITOR_H
#define _PAGE_MONITOR_H

#include "active_page_notifier.h"
#include "map.h"

void start_page_monitor(map_registry_t *reg, active_page_notifier_t *pn);

#endif
