#ifndef _ACTIVE_PAGE_NOTIFIER_H
#define _ACTIVE_PAGE_NOTIFIER_H

#include <map.h>
#include <pthread.h>
#include <stdint.h>

typedef uintptr_t page_id;

typedef struct {
  page_id *ids;
  size_t len;
  size_t cap;
} page_ids;

typedef struct {
  pthread_mutex_t mu;
  page_ids *page_ids;
} active_page_notifier;

typedef void (*active_page_notifier_visit_fn)(page_id page_id, void *userdata);

void active_page_notifier_init(active_page_notifier *apn);
void active_page_notifier_notify(active_page_notifier *apn, page_ids *page_ids);
void active_page_notifier_visit(active_page_notifier *apn,
                                active_page_notifier_visit_fn fn,
                                void *userdata);

void page_ids_add(page_ids *page_ids, page_id page_id);

page_id page_id_from_map(map *map, size_t page_num);

#endif
