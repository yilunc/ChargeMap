#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_text_layer;
static TextLayer *l_text_layer;
static GBitmap* s_logo_bitmap;

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
  // Destroy output TextLayer
  text_layer_destroy(s_text_layer);
  gbitmap_destroy(s_logo_bitmap);
  //bitmap_layer_destroy(s_bitmap_layer);
}

static void init() {
  // Create main Window
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
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

