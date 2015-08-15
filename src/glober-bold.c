#include <pebble.h>

#define KEY_SHOW_WEATHER 0
#define KEY_SHOW_BATTERY 1
#define KEY_USE_CELSIUS 2

static Window *s_main_window;
static TextLayer *time_layer, *date_layer, *batt_layer;
static GFont *time_font, *date_font, *batt_font;
static Layer *weather_layer, *battery_layer;
static bool show_weather = true;
static bool show_battery = true;
static bool use_celsius = false;

static void battery_handler(BatteryChargeState new_state) {
  static char s_battery_buffer[32];
	
	bool charging = new_state.is_charging;
	bool plugged = new_state.is_plugged;
	int chargePercent = new_state.charge_percent;
	
	if(charging == true) {
		#ifdef PBL_COLOR
			text_layer_set_text_color(batt_layer, GColorCyan);
		#else
			text_layer_set_text_color(batt_layer, GColorWhite);
		#endif
		snprintf(s_battery_buffer, sizeof(s_battery_buffer), "Charging");
		text_layer_set_text(batt_layer, s_battery_buffer);
	} else if(plugged == true && chargePercent == 100) {
		#ifdef PBL_COLOR
			text_layer_set_text_color(batt_layer, GColorCyan);
		#else
			text_layer_set_text_color(batt_layer, GColorWhite);
		#endif
		snprintf(s_battery_buffer, sizeof(s_battery_buffer), "Charged");
		text_layer_set_text(batt_layer, s_battery_buffer);
	} else {
		if(chargePercent > 80) {
			#ifdef PBL_COLOR
				text_layer_set_text_color(batt_layer, GColorGreen);
			#else
				text_layer_set_text_color(batt_layer, GColorWhite);
			#endif
			
			snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  		text_layer_set_text(batt_layer, s_battery_buffer);
		} else if(chargePercent > 60) {
			#ifdef PBL_COLOR
				text_layer_set_text_color(batt_layer, GColorYellow);
			#else
				text_layer_set_text_color(batt_layer, GColorWhite);
			#endif
				
			snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  		text_layer_set_text(batt_layer, s_battery_buffer);
		} else if(chargePercent > 40) {
			#ifdef PBL_COLOR
				text_layer_set_text_color(batt_layer, GColorChromeYellow);
			#else
				text_layer_set_text_color(batt_layer, GColorWhite);
			#endif
				
			snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  		text_layer_set_text(batt_layer, s_battery_buffer);
		} else if(chargePercent > 20) {
			#ifdef PBL_COLOR
				text_layer_set_text_color(batt_layer, GColorRed);
			#else
				text_layer_set_text_color(batt_layer, GColorWhite);
			#endif
				
			snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  		text_layer_set_text(batt_layer, s_battery_buffer);
		} else if(chargePercent > 10) {
			#ifdef PBL_COLOR
				text_layer_set_text_color(batt_layer, GColorDarkCandyAppleRed);
			#else
				text_layer_set_text_color(batt_layer, GColorWhite);
			#endif
				
			snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  		text_layer_set_text(batt_layer, s_battery_buffer);
		} else if(chargePercent <= 10) {
			#ifdef PBL_COLOR
				text_layer_set_text_color(batt_layer, GColorDarkCandyAppleRed);
			#else
				text_layer_set_text_color(batt_layer, GColorWhite);
			#endif
			
			snprintf(s_battery_buffer, sizeof(s_battery_buffer), "%d%%", new_state.charge_percent);
  		text_layer_set_text(batt_layer, "Low");
		}
	}
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char time_buffer[] = "00:00";
	static char date_buffer[] = "WWW DD";
	static char week_buffer[] = "WWW";
  
  if(clock_is_24h_style() == true) {
    strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(time_buffer, sizeof("00:00"), "%l:%M", tick_time);
  }

  	strftime(week_buffer, sizeof("WWW"), "%a", tick_time);

  	if(strcmp(week_buffer, "Mon") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Mon %e", tick_time);
  	} else if(strcmp(week_buffer, "Tue") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Tue %e", tick_time);
  	} else if(strcmp(week_buffer, "Wed") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Wed %e", tick_time);
  	} else if(strcmp(week_buffer, "Thu") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Thu %e", tick_time);
  	} else if(strcmp(week_buffer, "Fri") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Fri %e", tick_time);
  	} else if(strcmp(week_buffer, "Sat") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Sat %e", tick_time);
  	} else if(strcmp(week_buffer, "Sun") == 0) {
  		strftime(date_buffer, sizeof("WWW DD"), "Sun %e", tick_time);
  	} else {
  		strftime(date_buffer, sizeof("WWW DD"), "%a %e", tick_time);
  	}
	
	text_layer_set_text(time_layer, time_buffer);
	text_layer_set_text(date_layer, date_buffer);

  	battery_handler(battery_state_service_peek());
}

static void check_settings() {
	if (show_weather == true) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Weather!");
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "No weather!");
	}

	if (show_battery == true) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Battery!");
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "No battery!");
	}

	if (use_celsius == true) {
		APP_LOG(APP_LOG_LEVEL_INFO, "Using Celsius!");
	} else {
		APP_LOG(APP_LOG_LEVEL_INFO, "Using Fahrenheit!");
	}
} 

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *show_weather_t = dict_find(iter, KEY_SHOW_WEATHER);
  Tuple *show_battery_t = dict_find(iter, KEY_SHOW_BATTERY);
  Tuple *use_celsius_t = dict_find(iter, KEY_USE_CELSIUS);

  if (show_weather_t) {
  	persist_write_int(KEY_SHOW_WEATHER, show_weather);

  	check_settings();
  }

  if (show_battery_t) {
  	show_battery = show_battery_t->value->int8;

  	persist_write_int(KEY_SHOW_BATTERY, show_battery);

  	check_settings();
  }

  if (use_celsius_t) {
  	use_celsius = use_celsius_t->value->int8;

  	persist_write_int(KEY_USE_CELSIUS, use_celsius);

  	check_settings();
  }
}

static void main_window_load(Window *window) {
	window_set_background_color(s_main_window, GColorBlack);

	time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_BOLD_40));
	date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_LIGHTI_18));
	batt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_LIGHTI_14));
	
	time_layer = text_layer_create(GRect(0, 50, 132, 163));
	text_layer_set_text_color(time_layer, GColorWhite);
	text_layer_set_background_color(time_layer, GColorClear);
	text_layer_set_font(time_layer, time_font);
	text_layer_set_text_alignment(time_layer, GTextAlignmentRight);
	
	
	date_layer = text_layer_create(GRect(0, 90, 130, 163));
	text_layer_set_text_color(date_layer, GColorWhite);
	text_layer_set_background_color(date_layer, GColorClear);
	text_layer_set_font(date_layer, date_font);
	text_layer_set_text_alignment(date_layer, GTextAlignmentRight);
	
	
	batt_layer = text_layer_create(GRect(0, 43, 130, 163));
	text_layer_set_text_color(batt_layer, GColorWhite);
	text_layer_set_background_color(batt_layer, GColorClear);
	text_layer_set_font(batt_layer, batt_font);
	text_layer_set_text_alignment(batt_layer, GTextAlignmentRight);
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(batt_layer));

	update_time();
	battery_handler(battery_state_service_peek());

	if (persist_read_bool(KEY_SHOW_WEATHER)) {
    show_weather = persist_read_bool(KEY_SHOW_WEATHER);
  }

  if (persist_read_bool(KEY_SHOW_BATTERY)) {
    show_battery = persist_read_bool(KEY_SHOW_BATTERY);
  }

  if (persist_read_bool(KEY_USE_CELSIUS)) {
    use_celsius = persist_read_bool(KEY_USE_CELSIUS);
  }
}

static void main_window_unload(Window *window) {
	fonts_unload_custom_font(time_font);
	fonts_unload_custom_font(date_font);
	fonts_unload_custom_font(batt_font);
	
	text_layer_destroy(time_layer);
	text_layer_destroy(date_layer);
	text_layer_destroy(batt_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  s_main_window = window_create();
	
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
	
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  battery_state_service_subscribe(battery_handler);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  check_settings();
}

static void deinit() {
  window_destroy(s_main_window);
	
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}