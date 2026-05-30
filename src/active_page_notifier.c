#include <stdlib.h>

#include "active_page_notifier.h"
#include "draw.h"

void active_page_notifier_init(active_page_notifier *apn) {
  pthread_mutex_init(&apn->mu, nullptr);
}

void active_page_notifier_notify(active_page_notifier *apn,
                                 page_ids *page_ids) {
  pthread_mutex_lock(&apn->mu);

  if (apn->page_ids) {
    if (apn->page_ids->ids) {
      free(apn->page_ids->ids);
    }
    free(apn->page_ids);
  }

  apn->page_ids = page_ids;

  pthread_mutex_unlock(&apn->mu);
}

void active_page_notifier_visit(active_page_notifier *apn,
                                active_page_notifier_visit_fn fn,
                                void *userdata) {
  pthread_mutex_lock(&apn->mu);

  for (size_t i = 0; i < apn->page_ids->len; i++)
    fn(apn->page_ids->ids[i], userdata);

  pthread_mutex_unlock(&apn->mu);
}

const size_t page_ids_default_allocation = 1 << 8;

void page_ids_add(page_ids *page_ids, page_id page_id) {
  if (!page_ids->ids) {
    // initialize storage
    page_ids->ids = calloc(page_ids_default_allocation, sizeof(page_id));
    page_ids->len = 0;
    page_ids->cap = page_ids_default_allocation;
  } else if (page_ids->len == page_ids->cap) {
    // table double
    auto new_cap = page_ids->cap * 2;
    auto old_data = page_ids->ids;
    page_ids->ids = calloc(new_cap, sizeof(page_id));
    for (size_t i = 0; i < page_ids->len; i++)
      page_ids->ids[i] = old_data[i];
    free(old_data);
  }

  page_ids->ids[page_ids->len] = page_id;
  page_ids->len++;
}

page_id page_id_from_map(map *map, size_t page_num) {
  return map->start / PAGE_BYTE_SIZE + page_num;
}
