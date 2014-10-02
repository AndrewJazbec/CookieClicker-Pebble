#include "pebble.h"
# include "strap/strap.h"
  
#define NUM_COOKIE_PKEY 1
#define NUM_COOKIE_DEFAULT 0
#define NUM_CLICKS_PKEY 2
#define NUM_CLICKS_DEFAULT 1
  
#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 6

static Window *window;

static MenuLayer *menu_layer;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static TextLayer *cookie_text_layer;
static TextLayer *shop_text_layer;

static int num_cookie = NUM_COOKIE_DEFAULT;
static int num_clicks = NUM_CLICKS_DEFAULT;

static Layer *layer;

static GBitmap *image;

static void update_text() {
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "%u", num_cookie);
  text_layer_set_text(cookie_text_layer, body_text);
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS;

    default:
      return 0;
  }
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "Shop");
      break;
  }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title
          menu_cell_basic_draw(ctx, cell_layer, "Power Click", "5000", NULL);
          break;

        case 1:
          // This is a basic menu item with a title
          menu_cell_basic_draw(ctx, cell_layer, "Grandma", "100", NULL);
          break;

        case 2:
          // This is a basic menu item with a title
          menu_cell_basic_draw(ctx, cell_layer, "C-Robot", "300", NULL);
          break;
        
        case 3:
          // This is a basic menu item with a title
          menu_cell_basic_draw(ctx, cell_layer, "Cookie Farm", "700", NULL);
          break;
        
        case 4:
          // This is a basic menu item with a title
          menu_cell_basic_draw(ctx, cell_layer, "C-Factory", "1500", NULL);
          break;
        
        case 5:
          // This is a basic menu item with a title
          menu_cell_basic_draw(ctx, cell_layer, "Coming Soon", "Email Suggestions", NULL);
          break;
      }
  }
}

void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data)
{
    //Get which row
    int which = cell_index->row;
 
    if(which == 0) {
      if(num_cookie >=5000){
        num_clicks=num_clicks*2;
        num_cookie=num_cookie-5000;
        update_text();
      }
    }
  
    if(which == 1) {
      if(num_cookie >=100){
        num_clicks=num_clicks+1;
        num_cookie=num_cookie-100;
        update_text();
      }
    }
    if(which == 2) {
      if(num_cookie >=300){
        num_clicks=num_clicks+2;
        num_cookie=num_cookie-300;
        update_text();
      }
    }
    if(which == 3) {
      if(num_cookie >=700){
        num_clicks=num_clicks+5;
        num_cookie=num_cookie-700;
        update_text();
      }
    }
    if(which == 4) {
      if(num_cookie >=1500){
        num_clicks=num_clicks+10;
        num_cookie=num_cookie-1500;
        update_text();
      }
    }
  
}

// This initializes the menu upon window load
void window_load2(Window *window2) {
  
  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window2);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window2);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload2(Window *window2) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);
}

static void increment1_click_handler(ClickRecognizerRef recognizer, void *context) {
  num_cookie=num_cookie+num_clicks;
  if (num_cookie > 80000) {
    text_layer_set_font(cookie_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  }
  update_text();
  strap_log_event("/Clicks");
}

static void shop_click_handler(ClickRecognizerRef recognizer, void *context) {
  window = window_create();

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load2,
    .unload = window_unload2,
  });
  
  window_stack_push(window, true /* Animated */);
}

//Long click
void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  num_cookie=0;
  num_clicks=1;
  text_layer_set_font(cookie_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  update_text();
}

//Long Click Rlease
void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
}

//Long Click Press
void select_long_click_handler2(ClickRecognizerRef recognizer, void *context) {
}

//Long Click Rlease
void select_long_click_release_handler2(ClickRecognizerRef recognizer, void *context) {
}

//Click Config
static void click_config_provider(void *context) {
  
  //Button click delay 50ms
  const uint16_t repeat_interval_ms = 50;
  
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, (ClickHandler) shop_click_handler);
  
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, repeat_interval_ms, (ClickHandler) increment1_click_handler);
  
  window_long_click_subscribe(BUTTON_ID_UP, 5000, select_long_click_handler, select_long_click_release_handler);
  
  window_long_click_subscribe(BUTTON_ID_SELECT, 10000, select_long_click_handler2, select_long_click_release_handler2);
}

static void window_load(Window *me) {
  
  //Layers
  Layer *layer = window_get_root_layer(me);
  const int16_t width = layer_get_frame(layer).size.w - ACTION_BAR_WIDTH - 3;
  
  //Tetlayer details for cookie
   cookie_text_layer = text_layer_create(GRect(1, 1, width, 60));
  text_layer_set_font(cookie_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_background_color(cookie_text_layer, GColorClear);
  layer_add_child(layer, text_layer_get_layer(cookie_text_layer));
  
  //Tetlayer details for shop textlayer
  shop_text_layer = text_layer_create(GRect(10, 118, width, 60));
  text_layer_set_font(shop_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_background_color(shop_text_layer, GColorWhite);
  text_layer_set_text(shop_text_layer, "        Shop");
  layer_add_child(layer, text_layer_get_layer(shop_text_layer));
  
  //Udate text
  update_text();

}

static void window_unload(Window *window2) {
  
  text_layer_destroy(cookie_text_layer);
  text_layer_destroy(shop_text_layer);

}

static void layer_update_callback(Layer *me, GContext* ctx) {
  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.

  GRect bounds = image->bounds;

  graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { 0, -5 }, .size = bounds.size });

}

static void init(void){
  window = window_create();
  
  // Init the layer for display the image
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, layer_update_callback);
  layer_add_child(window_layer, layer);

  image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_COOKIE_CLICKER);
  
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
    
  
  });
  
  num_cookie = persist_exists(NUM_COOKIE_PKEY) ? persist_read_int(NUM_COOKIE_PKEY) : NUM_COOKIE_DEFAULT;
  num_clicks = persist_exists(NUM_CLICKS_PKEY) ? persist_read_int(NUM_CLICKS_PKEY) : NUM_CLICKS_DEFAULT;
  
  window_stack_push(window, true /* Animated */);
  
 // app_message_register_inbox_received(in_received_handler);
  int in_size = app_message_inbox_size_maximum();
  int out_size = app_message_outbox_size_maximum();
  // required for Strap to communicate! 
  app_message_open(in_size, out_size);
  
  strap_init();
  
  
}

static void deinit(void) {
  
  persist_write_int(NUM_COOKIE_PKEY, num_cookie);
  persist_write_int(NUM_CLICKS_PKEY, num_clicks);
  
  strap_deinit();

  window_destroy(window);
  
}

static void deinit2(void) {
  window_destroy(window);

}

int main(void) {
  
  init();
  app_event_loop();
  //deinit2();
  deinit();
}