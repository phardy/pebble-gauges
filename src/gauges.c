#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define HAND_LENGTH 67

#define MY_UUID { 0x52, 0x39, 0x16, 0x5D, 0x4D, 0x79, 0x4D, 0xEF, 0xA5, 0x5F, 0xB7, 0x93, 0x07, 0xD7, 0x4D, 0x29 }
PBL_APP_INFO(MY_UUID,
             "Gauges", "Kids, Inc.",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
Layer time_layer;

GPoint hour_centre, minute_centre;

void time_layer_update(Layer *me, GContext *ctx) {
  PblTm now;
  get_time(&now);

  // Proof of concept follows
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, hour_centre, 3);
  graphics_fill_circle(ctx, minute_centre, 3);

  // Draw hour hand
  GPoint hour_hand;
  int32_t hour_angle = TRIG_MAX_ANGLE / 48 * now.tm_hour;
  hour_hand.y = (int16_t)(-cos_lookup(hour_angle) *
		(int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + hour_centre.y;
  hour_hand.x = (int16_t)(sin_lookup(hour_angle) *
		(int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + hour_centre.x;

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, hour_centre, hour_hand);

  // Draw minute hand
  GPoint minute_hand;
  int32_t minute_angle = TRIG_MAX_ANGLE / 120 * now.tm_min;
  minute_hand.y = (int16_t)(-cos_lookup(minute_angle) *
		  (int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + minute_centre.y;
  minute_hand.x = ((int16_t)(-sin_lookup(minute_angle) *
		  (int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + minute_centre.x);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, minute_centre, minute_hand);
}

void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
  layer_mark_dirty(&time_layer);
}

void handle_init(AppContextRef ctx) {
  hour_centre = GPoint(2, 72);
  minute_centre = GPoint(142, 96);

  window_init(&window, "Root window");
  window_stack_push(&window, true /* Animated */);

  layer_init(&time_layer, window.layer.bounds);
  time_layer.update_proc = time_layer_update;
  layer_add_child(&window.layer, &time_layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = handle_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
