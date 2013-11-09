#include <pebble.h>

#define DIAL_RADIUS 70
#define HAND_LENGTH 70
#define INNER_RADIUS1 68
#define INNER_RADIUS2 63
#define MID_TICK_LENGTH 5
#define BIG_TICK_LENGTH 10
#define NUM_HOUR_TICKS 9
#define NUM_MINUTE_TICKS 7

Window *window;
Layer *dial_layer, *time_layer;

GPoint hour_centre, minute_centre;
GPath hour_ticks[NUM_HOUR_TICKS];
GPath minute_ticks[NUM_MINUTE_TICKS];

void dial_layer_update(Layer *me, GContext *ctx) {
  // background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, layer_get_bounds(me), 0, GCornerNone);
  // dials
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_circle(ctx, hour_centre, DIAL_RADIUS);
  graphics_fill_circle(ctx, minute_centre, DIAL_RADIUS);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_circle(ctx, hour_centre, INNER_RADIUS1);
  graphics_draw_circle(ctx, minute_centre, INNER_RADIUS1);

  // draw little ticks
  GPoint ray;
  int hour_ticks;
  if (clock_is_24h_style()) {
    hour_ticks = 24;
  } else {
    hour_ticks = 12;
  }
  // draw hour dial rays
  for (int x=0; x < (hour_ticks+1); x++) {
    // Every third line will be a big tick, so don't draw small
    if (x % 3 != 0) {
      int32_t angle = TRIG_MAX_ANGLE / (hour_ticks*2) * x;
      ray.y = (int16_t)(-cos_lookup(angle) *
	      (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.y;
      ray.x = (int16_t)(sin_lookup(angle) *
	      (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.x;
      graphics_draw_line(ctx, hour_centre, ray);
    }
  }
  // draw minute dial rays
  for (int x=0; x< 61; x++) {
    // Every fifth will be a bigger tick, so don't draw small
    if (x % 5 != 0) {
      int32_t angle = TRIG_MAX_ANGLE / 120 * x;
      ray.y = (int16_t)(-cos_lookup(angle) *
	      (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.y;
      ray.x = (int16_t)(-sin_lookup(angle) *
	       (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.x;
      graphics_draw_line(ctx, minute_centre, ray);
    }
  }
  // draw background-coloured circles over the rays, creating tick markers
  graphics_fill_circle(ctx, hour_centre, INNER_RADIUS2);
  graphics_fill_circle(ctx, minute_centre, INNER_RADIUS2);
  graphics_draw_circle(ctx, hour_centre, INNER_RADIUS2);
  graphics_draw_circle(ctx, minute_centre, INNER_RADIUS2);

  // draw big ticks
  int hour_bigticks;
  if (clock_is_24h_style()) {
    hour_bigticks = 8;
  } else {
    hour_bigticks = 4;
  }
  // draw hour dial rays
  for (int x=0; x < (hour_bigticks+1); x++) {
    int32_t angle = TRIG_MAX_ANGLE / (hour_bigticks*2) * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.y;
    ray.x = (int16_t)(sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + hour_centre.x;
    graphics_draw_line(ctx, hour_centre, ray);
  }
  graphics_fill_circle(ctx, hour_centre, INNER_RADIUS2-BIG_TICK_LENGTH);

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
  graphics_fill_circle(ctx, minute_centre, INNER_RADIUS2-MID_TICK_LENGTH);
  for (int x=0; x < 13; x+=2) {
    int32_t angle = TRIG_MAX_ANGLE / 24 * x;
    ray.y = (int16_t)(-cos_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.y;
    ray.x = (int16_t)(-sin_lookup(angle) *
	    (int32_t)INNER_RADIUS1 / TRIG_MAX_RATIO) + minute_centre.x;
    graphics_draw_line(ctx, minute_centre, ray);
  }
  graphics_fill_circle(ctx, minute_centre, INNER_RADIUS2-BIG_TICK_LENGTH);

  // centres
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, hour_centre, 4);
  graphics_fill_circle(ctx, minute_centre, 4);
}

void time_layer_update(Layer *me, GContext *ctx) {
  struct tm *now;
  time_t t = time(NULL);
  now = localtime(&t);

  // Proof of concept follows
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_circle(ctx, hour_centre, 3);
  graphics_fill_circle(ctx, minute_centre, 3);

  // Draw hour hand
  GPoint hour_hand;
  int32_t hour_angle;
  if (clock_is_24h_style()) {
    hour_angle = TRIG_MAX_ANGLE / 48 * now->tm_hour;
  } else {
    hour_angle = TRIG_MAX_ANGLE / 24 * (now->tm_hour % 12);
    // 12 hour mode looks better if we use 12 instead of 0?
    if (hour_angle == 0)
      hour_angle = 12;
  }

  hour_hand.y = (int16_t)(-cos_lookup(hour_angle) *
		(int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + hour_centre.y;
  hour_hand.x = (int16_t)(sin_lookup(hour_angle) *
		(int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + hour_centre.x;

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, hour_centre, hour_hand);

  // Draw minute hand
  GPoint minute_hand;
  int32_t minute_angle = TRIG_MAX_ANGLE / 120 * now->tm_min;
  minute_hand.y = (int16_t)(-cos_lookup(minute_angle) *
		  (int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + minute_centre.y;
  minute_hand.x = ((int16_t)(-sin_lookup(minute_angle) *
		  (int32_t)HAND_LENGTH / TRIG_MAX_RATIO) + minute_centre.x);

  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, minute_centre, minute_hand);
}

void handle_tick(struct tm *now, TimeUnits units_changed) {
  layer_mark_dirty(time_layer);
}

void in_received_handler(DictionaryIterator *received, void *context) {
  // stub!
}

void in_dropped_handler(AppMessageResult reason, void *context) {
  // stub!
}

void handle_init() {
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);

  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);

  hour_centre = GPoint(2, 72);
  minute_centre = GPoint(142, 96);

  window = window_create();
  // Getting the bounds seems to result in a 144x144 window. Bad.
  // GRect window_bounds = layer_get_bounds(window_get_root_layer(window));
  GRect window_bounds = GRect(0, 0, 144, 168);
  dial_layer = layer_create(window_bounds);
  time_layer = layer_create(window_bounds);

  layer_set_update_proc(dial_layer, dial_layer_update);
  layer_set_update_proc(time_layer, time_layer_update);

  layer_add_child(window_get_root_layer(window), dial_layer);
  layer_add_child(window_get_root_layer(window), time_layer);

  window_stack_push(window, true /* Animated */);

  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
}

void handle_deinit() {
  layer_destroy(time_layer);
  layer_destroy(dial_layer);
  window_destroy(window);
}

int main() {
  handle_init();
  app_event_loop();
  handle_deinit();
}
