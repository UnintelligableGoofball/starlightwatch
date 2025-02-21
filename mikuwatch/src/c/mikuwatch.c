#include <pebble.h>

#define NUM_BUFFER_SIZE 8

static Window *s_main_window;

static TextLayer *s_time_layer;
static GFont s_time_font;

static BitmapLayer *s_bitmap_layer;
static GBitmap *s_background_bitmap;
static BitmapLayer *s_hands_layer;
static GBitmap *s_hands_bitmap;

static void prv_unobstructed_will_change(GRect final_unobstructed_window, void *context) {
  GRect full_bounds = layer_get_bounds(window_get_root_layer(s_main_window));
  
  if (!grect_equal(&full_bounds, &final_unobstructed_window)) {
    // Screen is about to become obstructed

    //bitmap has height of 7, needs to be at bottom
    s_hands_layer = bitmap_layer_create(
      GRect(0, final_unobstructed_window.size.h - 7, final_unobstructed_window.size.w, 7));

    s_hands_bitmap = gbitmap_create_with_resource(RESOURCE_ID_HANDS_OVERLAY);
    bitmap_layer_set_bitmap(s_hands_layer, s_hands_bitmap);
    bitmap_layer_set_background_color(s_hands_layer, GColorClear);
    bitmap_layer_set_compositing_mode(s_hands_layer, GCompOpSet);

    layer_add_child(window_get_root_layer(s_main_window), bitmap_layer_get_layer(s_hands_layer));
  }

}

static void prv_unobstructed_did_change(void *context) {
  GRect full_bounds = layer_get_bounds(window_get_root_layer(s_main_window));
  GRect bounds = layer_get_unobstructed_bounds(window_get_root_layer(s_main_window));

  if (grect_equal(&full_bounds, &bounds)) {
    //quickview gone

    layer_remove_from_parent(bitmap_layer_get_layer(s_hands_layer));
    bitmap_layer_destroy(s_hands_layer);
    gbitmap_destroy(s_hands_bitmap);
  }
  
}


static void update_time(){
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[NUM_BUFFER_SIZE];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  //APP_LOG(APP_LOG_LEVEL_DEBUG, "%c", s_buffer[0]);

  //remove leading zero
  char *s_buffer_ptr = s_buffer;

  if (s_buffer[0] == '0' && !clock_is_24h_style()) {
    s_buffer_ptr++;
  }

  text_layer_set_text(s_time_layer, s_buffer_ptr);
}

static void tick_handler(struct tm *tick_handler, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  //WINDOW bounds + layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_bitmap_layer = bitmap_layer_create(bounds);
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);

  s_time_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(0,-15), bounds.size.w, 48));
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_VOCALOID_SANS_48));

  text_layer_set_background_color(s_time_layer, GColorClear);

  #if defined(PBL_BW)
  text_layer_set_text_color(s_time_layer, GColorWhite);
  #else
  text_layer_set_text_color(s_time_layer, GColorBlack);
  #endif

  text_layer_set_text(s_time_layer, "00:00");
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  bitmap_layer_set_bitmap(s_bitmap_layer, s_background_bitmap);

  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

}

static void main_window_unload(Window *window) {

}

static void init() {
  s_main_window = window_create();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  UnobstructedAreaHandlers unobstructed_handler = (UnobstructedAreaHandlers) {
    .will_change = prv_unobstructed_will_change,
    .did_change = prv_unobstructed_did_change
  };

  unobstructed_area_service_subscribe(unobstructed_handler, NULL);


  window_stack_push(s_main_window, true);

  update_time();

}

static void deinit() {
  window_destroy(s_main_window);
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_bitmap_layer);
  fonts_unload_custom_font(s_time_font);
  gbitmap_destroy(s_background_bitmap);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_main_window);

  app_event_loop();
  deinit();
}