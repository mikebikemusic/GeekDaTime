/*  Copyright (C) 2014 Alexis Rico | XDA | Pebble

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.  
*/

#include <pebble.h>

// Create Window, Time and Quote
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_quote_layer;

// Main void
int main(void) {
  init();
  app_event_loop();
  deinit();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

// Load and Unload
static void main_window_load(Window *window) {
  create_time_layer();
  create_quote_layer();
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_quote_layer);
}

static void create_time_layer() {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50)); //GRect = Rectangle where everything is shown.
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD)); //Fonts
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter); //Alignment

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void create_quote_layer() {
  // Create time TextLayer
  s_quote_layer = text_layer_create(GRect(0, 55, 144, 50)); //TODO: Fix rectangle position of quote
  text_layer_set_background_color(s_quote_layer, GColorClear);
  text_layer_set_text_color(s_quote_layer, GColorBlack);
  text_layer_set_text(s_quote_layer, "MTFBWY");

  // Improve the layout to be more like a watchface
  text_layer_set_font(s_quote_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD)); //TODO: Reduce Font
  text_layer_set_text_alignment(s_quote_layer, GTextAlignmentCenter); //Alignment

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_quote_layer)); //TODO: Root layer time?
  
  // Make sure the time is displayed from the start
  update_quote();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) { //TODO: 12/24 Persistent data + Mobile
    //Use 2h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void update_quote() {
  //TODO: Parse file and get random quote
}
