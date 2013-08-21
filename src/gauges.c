#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "gauges.h"

#define MY_UUID { 0x52, 0x39, 0x16, 0x5D, 0x4D, 0x79, 0x4D, 0xEF, 0xA5, 0x5F, 0xB7, 0x93, 0x07, 0xD7, 0x4D, 0x29 }
PBL_APP_INFO(MY_UUID,
             "Gauges", "Kids, Inc.",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
Layer dial_layer, time_layer;

GPoint hour_centre, minute_centre;
GPath hour_ticks[NUM_HOUR_TICKS];
GPath minute_ticks[NUM_MINUTE_TICKS];

void dial_layer_update(Layer *me, GContext *ctx) {
  // background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, me->bounds, 0, GCornerNone);
  // dials
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, hour_centre, DIAL_RADIUS);
  graphics_fill_circle(ctx, minute_centre, DIAL_RADIUS);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_circle(ctx, hour_centre, INNER_RADIUS1);
  graphics_draw_circle(ctx, minute_centre, INNER_RADIUS1);

  // draw little ticks
  GPoint ray;
  for (int x=0; x < 25; x++) {
    int32_t angle = TRIG_MAX_ANGLE / 48 * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.y;
    ray.x = (int16_t)(sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.x;
    graphics_draw_line(ctx, hour_centre, ray);
  }
  for (int x=0; x< 61; x++) {
    int32_t angle = TRIG_MAX_ANGLE / 120 * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.y;
    ray.x = (int16_t)(-sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.x;
    graphics_draw_line(ctx, minute_centre, ray);
  }
  graphics_fill_circle(ctx, hour_centre, INNER_RADIUS2);
  graphics_fill_circle(ctx, minute_centre, INNER_RADIUS2);
  graphics_draw_circle(ctx, hour_centre, INNER_RADIUS2);
  graphics_draw_circle(ctx, minute_centre, INNER_RADIUS2);

  // draw big ticks
  for (int x=0; x < 9; x++) {
    int32_t angle = TRIG_MAX_ANGLE / 16 * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.y;
    ray.x = (int16_t)(sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.x;
    graphics_draw_line(ctx, hour_centre, ray);
  }
  graphics_fill_circle(ctx, hour_centre, INNER_RADIUS2-8);

  // minutes are done in two passes - small ticks for
  // 5-min intervals, larger for 10.
  for (int x=1; x < 13; x+=2) {
    int32_t angle = TRIG_MAX_ANGLE / 24 * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.y;
    ray.x = (int16_t)(-sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.x;
    graphics_draw_line(ctx, minute_centre, ray);
  }
  graphics_fill_circle(ctx, minute_centre, INNER_RADIUS2-4);
  for (int x=0; x < 13; x+=2) {
    int32_t angle = TRIG_MAX_ANGLE / 24 * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.y;
    ray.x = (int16_t)(-sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.x;
    graphics_draw_line(ctx, minute_centre, ray);
  }
  graphics_fill_circle(ctx, minute_centre, INNER_RADIUS2-8);

  // centres
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, hour_centre, 4);
  graphics_fill_circle(ctx, minute_centre, 4);
}

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

  layer_init(&dial_layer, window.layer.bounds);
  dial_layer.update_proc = dial_layer_update;
  layer_add_child(&window.layer, &dial_layer);
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
