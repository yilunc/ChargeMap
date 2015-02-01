#include <pebble.h>

#define NAME_KEY 0  
#define ADDRESS_KEY 1
#define CITY_KEY 2
#define ZIP_KEY 3
#define HOURS_KEY 4
#define CONTYPE_KEY 5
#define ACCESS_KEY 6
  
static Window *s_main_window;
static Window *menu_window;
static Window *info_window;
static TextLayer *s_text_layer;
static TextLayer *l_text_layer;
static MenuLayer *menu_layer;
static GBitmap* s_logo_bitmap;

char name[32], addr[32], city[32], zip[7], hours[32], contype[32], access[32];
char name2[32], addr2[32], city2[32], zip2[7], hours2[32], contype2[32], access2[32];
char name3[32], addr3[32], city3[32], zip3[7], hours3[32], contype3[32], access3[32];

enum {
    KEY_BUTTON_EVENT = 0,
    BUTTON_EVENT_UP = 1,
    BUTTON_EVENT_DOWN = 2,
    BUTTON_EVENT_SELECT = 3
};

void send_int(uint8_t key, uint8_t cmd){
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
      
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
      
    app_message_outbox_send();
}

static void info_window_load(Window *window){
  s_logo_bitmap = gbitmap_create_with_resource(RESOURCE_ID_LOGO_MENU_ID);
  static BitmapLayer *s_bitmap_layer;
  s_bitmap_layer = bitmap_layer_create(GRect(5, 5, 16, 28));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_logo_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bitmap_layer));
}

static void info_window_unload(Window *window){
  gbitmap_destroy(s_logo_bitmap);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create output TextLayer
  s_text_layer = text_layer_create(GRect(5, 0, window_bounds.size.w - 5, window_bounds.size.h));
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(s_text_layer, "ChargeMap");
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
  text_layer_destroy(s_text_layer);
  gbitmap_destroy(s_logo_bitmap);
}

static void draw_header_callback(GContext *ctx, const Layer *cell_layer){
  menu_cell_basic_header_draw(ctx, cell_layer, "Charging Locations:");
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context){
    //Which row is it?
    switch(cell_index->row){
    case 0:
        menu_cell_basic_draw(ctx, cell_layer, name, addr, NULL);
        break;
    case 1:
        menu_cell_basic_draw(ctx, cell_layer, name2, addr2, NULL);
        break;
    case 2:
        menu_cell_basic_draw(ctx, cell_layer, name3, addr3, NULL);
        break;
    }
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
    return 1;
}

static uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context){
    return 3;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
    return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context){
    info_window = window_create();
    window_set_window_handlers(info_window, (WindowHandlers){
      .load = info_window_load,
      .unload = info_window_unload
    });
    window_stack_push(info_window, true);  
}

static void menu_window_load(Window *window){
  menu_layer = menu_layer_create(GRect(0, 0, 144, 152));
  menu_layer_set_click_config_onto_window(menu_layer, window);
  
 //Give it its callbacks
  MenuLayerCallbacks callbacks = {
    .get_num_sections = menu_get_num_sections_callback,
    .draw_header = (MenuLayerDrawHeaderCallback) draw_header_callback,
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
    .get_header_height = (MenuLayerGetHeaderHeightCallback)menu_get_header_height_callback,
    .select_click = (MenuLayerSelectCallback) select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);

  //Add to Window
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

static void menu_window_unload(Window *window){
  menu_layer_destroy(menu_layer);
}

static void down_single_click_handler(ClickRecognizerRef recognizer, void *context){
  menu_window = window_create();
  window_set_window_handlers(menu_window, (WindowHandlers){
    .load = menu_window_load,
    .unload = menu_window_unload
  });
  window_stack_push(menu_window, true);
  text_layer_set_text(l_text_layer, "Starting");
  send_int(KEY_BUTTON_EVENT, BUTTON_EVENT_SELECT);
}

static void config_provider(Window *window){
    window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, down_single_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, down_single_click_handler);
}

static void in_received_handler(DictionaryIterator *iter, void *context){
    Tuple *t = dict_read_first(iter);
    while(t!= NULL){
      switch(t->key){
        case NAME_KEY:
          if(strcmp(name, "") != 0)
            if(strcmp(name2, "") != 0)
              strcpy(name3, t->value->cstring);
            else
              strcpy(name2, t->value->cstring);
          else
            strcpy(name, t->value->cstring);
        break;
        case ADDRESS_KEY:
          if(strcmp(addr, "") != 0)
            if(strcmp(addr2, "") != 0)
              strcpy(addr3, t->value->cstring);
            else
              strcpy(addr2, t->value->cstring);
          else
            strcpy(addr, t->value->cstring);
        break;
        case CITY_KEY:
          if(strcmp(city, "") != 0)
            if(strcmp(city2, "") != 0)
              strcpy(city3, t->value->cstring);
            else
              strcpy(city2, t->value->cstring);
          else
            strcpy(city, t->value->cstring);
        break;
        case ZIP_KEY:
          if(strcmp(zip, "") != 0)
            if(strcmp(zip2, "") != 0)
              strcpy(zip3, t->value->cstring);
            else
              strcpy(zip2, t->value->cstring);
          else
            strcpy(zip, t->value->cstring);
        break;
        case HOURS_KEY:
          if(strcmp(hours, "") != 0)
            if(strcmp(hours2, "") != 0)
              strcpy(hours3, t->value->cstring);
            else
              strcpy(hours2, t->value->cstring);
          else
            strcpy(hours, t->value->cstring);
        break;
        case CONTYPE_KEY:
          if(strcmp(contype, "") != 0)
            if(strcmp(contype2, "") != 0)
              strcpy(contype3, t->value->cstring);
            else
              strcpy(contype2, t->value->cstring);
          else
            strcpy(contype, t->value->cstring);
        break;
        case ACCESS_KEY:
          if(strcmp(access, "") != 0)
            if(strcmp(access2, "") != 0)
              strcpy(access3, t->value->cstring);
            else
              strcpy(access2, t->value->cstring);
          else
            strcpy(access, t->value->cstring);
        break;
      }
    t = dict_read_next(iter);
    }
}

static void init() {
  app_message_register_inbox_received(in_received_handler);           
  app_message_open(512, 512);
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
  window_stack_push(s_main_window, true);
}


static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}  

