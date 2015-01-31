#include <pebble.h>
#include <string.h>
  
#define NAME_KEY 0
#define ADDRESS_KEY 1
#define CITY_KEY 2
#define ZIP_KEY 3
#define HOURS_KEY 4
#define CONTYPE_KEY 5
#define ACCESS_KEY 6

static Window *s_main_window;
static TextLayer *s_text_layer;
static TextLayer *l_text_layer;
static GBitmap* s_logo_bitmap;

char name[32], addr[32], city[32], zip[7], hours[32], contype[32], access[32];
char name2[32], addr2[32], city2[32], zip2[7], hours2[32], contype2[32], access2[32];
char name3[32], addr3[32], city3[32], zip3[7], hours3[32], contype3[32], access3[32];

// AppMessage
static void send(int key, int message) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  
  dict_write_int(iter, key, &message, sizeof(int), true);
  app_message_outbox_send();
}
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  text_layer_set_text(l_text_layer, "Locating...");
  // Get the first pair
  Tuple *t = dict_read_first(iterator);

  // Process all pairs present
  while (t != NULL) {
    // Process this pair's key
    switch (t->key) {
      case NAME_KEY:
        if (strcmp(name, "") != 0)
          if (strcmp(name2, "") != 0)
            strcpy(name3, t->value->cstring);
          else
            strcpy(name2, t->value->cstring);
        else
          strcpy(name, t->value->cstring);
        break;
      case ADDRESS_KEY:
        if (strcmp(addr, "") != 0)
          if (strcmp(addr2, "") != 0)
            strcpy(addr3, t->value->cstring);
          else
            strcpy(addr2, t->value->cstring);
        else
          strcpy(addr, t->value->cstring);
        break;
      /*case CITY_KEY:
        if (city!=NULL)
          if (city2!=NULL)
            strcpy(city3, t->value->cstring);
          else
            strcpy(city2, t->value->cstring);
        else
          strcpy(city, t->value->cstring);
        break;
      case ZIP_KEY:
        if (zip!=NULL)
          if (zip2!=NULL)
            strcpy(zip3, t->value->cstring);
          else
            strcpy(zip2, t->value->cstring);
        else
          strcpy(zip, t->value->cstring);
        break;
      case HOURS_KEY:
        if (hours!=NULL)
          if (hours2!=NULL)
            strcpy(hours3, t->value->cstring);
          else
            strcpy(hours2, t->value->cstring);
        else
          strcpy(hours, t->value->cstring);
        break;
      case CONTYPE_KEY:
        if (contype!=NULL)
          if (contype2!=NULL)
            strcpy(contype3, t->value->cstring);
          else
            strcpy(contype2, t->value->cstring);
        else
          strcpy(contype, t->value->cstring);
        break;
      case ACCESS_KEY:
        if (access!=NULL)
          if (access2!=NULL)
            strcpy(access3, t->value->cstring);
          else
            strcpy(access2, t->value->cstring);
        else
          strcpy(access, t->value->cstring);
        break;*/
    }

    // Get next pair, if any
    t = dict_read_next(iterator);
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "Message received!");
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(l_text_layer, "Starting");
  send(0, 0);
}

static void click_config_provider(void *context) {
  // Assign button handlers
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_text_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, window_bounds.size.h));
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_text_layer, name);
  //text_layer_set_text(s_text_layer, addr);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_text_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
  
  // Create second text layer
  l_text_layer = text_layer_create(GRect(5, 122, window_bounds.size.w - 10, window_bounds.size.h - 125));
  text_layer_set_font(l_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(l_text_layer, "LOCATE");
  text_layer_set_text_color(l_text_layer, GColorWhite);
  text_layer_set_text_alignment(l_text_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(l_text_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_layer, text_layer_get_layer(l_text_layer));
  text_layer_set_background_color(l_text_layer, GColorBlack);
  
  // Create image layer
  s_logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LOGO_SMALL_ID);
  static BitmapLayer *s_bitmap_layer;
  s_bitmap_layer = bitmap_layer_create(GRect(50, 35, 48, 85));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_logo_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
  
}

static void main_window_unload(Window *window) {
  // Destroy output TextLayer
  text_layer_destroy(s_text_layer);
  text_layer_destroy(l_text_layer);
  gbitmap_destroy(s_logo_bitmap);
  //bitmap_layer_destroy(s_bitmap_layer);
}


static void init() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  // Create main Window
  
  s_main_window = window_create();
  //menu_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  /*window_set_window_handlers(menu_window, (WindowHandlers) {
    .load = menu_window_load,
    .unload = menu_window_unload,
  });*/
  
  window_set_click_config_provider(s_main_window, click_config_provider);
  window_stack_push(s_main_window, true);
}

static void deinit() {
  //window_destroy(menu_window);
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}  
