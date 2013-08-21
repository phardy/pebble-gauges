#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x52, 0x39, 0x16, 0x5D, 0x4D, 0x79, 0x4D, 0xEF, 0xA5, 0x5F, 0xB7, 0x93, 0x07, 0xD7, 0x4D, 0x29 }
PBL_APP_INFO(MY_UUID,
             "Gauges", "Kids, Inc.",
             0, 1, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;


void handle_init(AppContextRef ctx) {

  window_init(&window, "Root window");
  window_stack_push(&window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
