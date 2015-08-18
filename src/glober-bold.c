#include <pebble.h>

// #define KEY_SHOW_WEATHER 0
#define KEY_SHOW_BATTERY 1
#define KEY_USE_CELSIUS 2
#define KEY_TEMPERATURE 3
#define KEY_TEMPERATURE_IN_C 4
#define KEY_CONDITIONS 5
#define KEY_SHAKE_FOR_WEATHER 6

static Window *s_main_window;
static TextLayer *time_layer, *date_layer, *batt_layer, *temp_layer, *conditions_layer, *temp_layer_unanimated, *conditions_layer_unanimated;
static GFont *time_font, *date_font, *batt_font, *temp_font;
static Layer *weather_layer, *battery_layer, *weather_layer_unanimated;
// static bool show_weather = 1;
static bool show_battery = 1;
static bool use_celsius = 0;
static bool shake_for_weather = 1;

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

void on_animation_stopped(Animation *anim, bool finished, void *context) {
    //Free the memory used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay) {
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 
    //Set stopped handler to free memory
    AnimationHandlers handlers = {
        //The reference to the stopped handler is the only one in the array
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
 
    //Start animation!
    animation_schedule((Animation*) anim);
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

static void update_layers() {
  if (show_battery == 0) {
	layer_set_hidden(battery_layer, true);
  } else {
	layer_set_hidden(battery_layer, false);
  }

  if (shake_for_weather == 0) {
  	layer_set_hidden(weather_layer, true);
  	layer_set_hidden(weather_layer_unanimated, false);
  } else {
  	layer_set_hidden(weather_layer, false);
  	layer_set_hidden(weather_layer_unanimated, true);
  }
}

static void animate_layers() {
	// Temperature layer

	GRect tins = GRect(0, 178, 130, 163);
	GRect tinf = GRect(0, 130, 130, 163);
	animate_layer(text_layer_get_layer(temp_layer), &tins, &tinf, 1000, 0);

	GRect touts = GRect(0, 130, 130, 163);
	GRect toutf = GRect(0, 178, 130, 163);
	animate_layer(text_layer_get_layer(temp_layer), &touts, &toutf, 1000, 5000);

	// Conditions layer

	GRect wins = GRect(0, 187, 130, 163);
	GRect winf = GRect(0, 145, 130, 163);
	animate_layer(text_layer_get_layer(conditions_layer), &wins, &winf, 1000, 0);

	GRect wouts = GRect(0, 145, 130, 163);
	GRect woutf = GRect(0, 187, 130, 163);
	animate_layer(text_layer_get_layer(conditions_layer), &wouts, &woutf, 1000, 5000);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  static char temp_buffer[15];
  static char temp_c_buffer[15];
  static char conditions_buffer[100];

  Tuple *show_battery_t = dict_find(iter, KEY_SHOW_BATTERY);
  Tuple *use_celsius_t = dict_find(iter, KEY_USE_CELSIUS);
  Tuple *temperature_t = dict_find(iter, KEY_TEMPERATURE);
  Tuple *temperature_in_c_t = dict_find(iter, KEY_TEMPERATURE_IN_C);
  Tuple *conditions_t = dict_find(iter, KEY_CONDITIONS);
  Tuple *shake_for_weather_t = dict_find(iter, KEY_SHAKE_FOR_WEATHER);

  if (show_battery_t) {
  	APP_LOG(APP_LOG_LEVEL_INFO, "KEY_SHOW_BATTERY received!");

  	show_battery = show_battery_t->value->int8;

  	persist_write_int(KEY_SHOW_BATTERY, show_battery);

  }

  if (use_celsius_t) {
  	APP_LOG(APP_LOG_LEVEL_INFO, "KEY_USE_CELSIUS received!");

  	use_celsius = use_celsius_t->value->int8;

  	persist_write_int(KEY_USE_CELSIUS, use_celsius);
  }

  if (shake_for_weather_t) {
  	APP_LOG(APP_LOG_LEVEL_INFO, "KEY_SHAKE_FOR_WEATHER received!");

  	shake_for_weather = shake_for_weather_t->value->int8;

  	persist_write_int(KEY_SHAKE_FOR_WEATHER, shake_for_weather);
  }

  if (temperature_t) {
  	APP_LOG(APP_LOG_LEVEL_INFO, "KEY_TEMPERATURE received!");

  	snprintf(temp_buffer, sizeof(temp_buffer), "%d degrees", (int)temperature_t->value->int32);
  }

  if (temperature_in_c_t) {
  	APP_LOG(APP_LOG_LEVEL_INFO, "KEY_TEMPERATURE_IN_C received!");

  	snprintf(temp_c_buffer, sizeof(temp_c_buffer), "%d degrees", (int)temperature_in_c_t->value->int32);
  }

  if (conditions_t) {
  	APP_LOG(APP_LOG_LEVEL_INFO, "KEY_CONDITIONS received!");

  	snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_t->value->cstring);
  	text_layer_set_text(conditions_layer, conditions_buffer);
  	text_layer_set_text(conditions_layer_unanimated, conditions_buffer);
  }

  if (show_battery == 0) {
	layer_set_hidden(battery_layer, true);
  } else {
	layer_set_hidden(battery_layer, false);
  } 

  if (shake_for_weather == 0) {
  	layer_set_hidden(weather_layer, true);
  	layer_set_hidden(weather_layer_unanimated, false);
  } else {
  	layer_set_hidden(weather_layer, false);
  	layer_set_hidden(weather_layer_unanimated, true);
  }

  if (use_celsius == 1) {
  	text_layer_set_text(temp_layer, temp_c_buffer);
  	text_layer_set_text(temp_layer_unanimated, temp_c_buffer);
  } else {
  	text_layer_set_text(temp_layer, temp_buffer);
  	text_layer_set_text(temp_layer_unanimated, temp_buffer);
  }
}

static void main_window_load(Window *window) {
	window_set_background_color(s_main_window, GColorBlack);

	time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_BOLD_40));
	date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_LIGHTI_18));
	batt_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_LIGHTI_14));
	temp_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_GLOBER_LIGHTI_12));

	weather_layer = layer_create(GRect(0, 0, 144, 168));
	battery_layer = layer_create(GRect(0, 0, 144, 168));
	weather_layer_unanimated = layer_create(GRect(0, 0, 144, 168));
	
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

	temp_layer = text_layer_create(GRect(0, 178, 130, 163));
	text_layer_set_text_color(temp_layer, GColorWhite);
	text_layer_set_background_color(temp_layer, GColorClear);
	text_layer_set_font(temp_layer, temp_font);
	text_layer_set_text_alignment(temp_layer, GTextAlignmentRight);
	text_layer_set_text(temp_layer, "Updating");

	conditions_layer = text_layer_create(GRect(0, 187, 130, 163));
	text_layer_set_text_color(conditions_layer, GColorWhite);
	text_layer_set_background_color(conditions_layer, GColorClear);
	text_layer_set_font(conditions_layer, batt_font);
	text_layer_set_text_alignment(conditions_layer, GTextAlignmentRight);
	text_layer_set_text(conditions_layer, "Weather");

	temp_layer_unanimated = text_layer_create(GRect(0, 130, 130, 163));
	text_layer_set_text_color(temp_layer_unanimated, GColorWhite);
	text_layer_set_background_color(temp_layer_unanimated, GColorClear);
	text_layer_set_font(temp_layer_unanimated, temp_font);
	text_layer_set_text_alignment(temp_layer_unanimated, GTextAlignmentRight);
	text_layer_set_text(temp_layer_unanimated, "Updating");

	conditions_layer_unanimated = text_layer_create(GRect(0, 145, 130, 163));
	text_layer_set_text_color(conditions_layer_unanimated, GColorWhite);
	text_layer_set_background_color(conditions_layer_unanimated, GColorClear);
	text_layer_set_font(conditions_layer_unanimated, batt_font);
	text_layer_set_text_alignment(conditions_layer_unanimated, GTextAlignmentRight);
	text_layer_set_text(conditions_layer_unanimated, "Weather");
	
	layer_add_child(window_get_root_layer(window), weather_layer);
	layer_add_child(window_get_root_layer(window), weather_layer_unanimated);
	layer_add_child(window_get_root_layer(window), battery_layer);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
	layer_add_child(weather_layer, text_layer_get_layer(temp_layer));
	layer_add_child(weather_layer, text_layer_get_layer(conditions_layer));
	layer_add_child(weather_layer_unanimated, text_layer_get_layer(temp_layer_unanimated));
	layer_add_child(weather_layer_unanimated, text_layer_get_layer(conditions_layer_unanimated));
	layer_add_child(battery_layer, text_layer_get_layer(batt_layer));

  	if (persist_exists(KEY_SHOW_BATTERY)) {
  	  show_battery = persist_read_int(KEY_SHOW_BATTERY);
  	}

  	if (persist_exists(KEY_USE_CELSIUS)) {
  	  use_celsius = persist_read_int(KEY_USE_CELSIUS);
  	}

  	if (persist_exists(KEY_SHAKE_FOR_WEATHER)) {
  	  shake_for_weather = persist_read_int(KEY_SHAKE_FOR_WEATHER);
  	}

  	update_layers();
  	update_time();
	battery_handler(battery_state_service_peek());
}

static void main_window_unload(Window *window) {
	fonts_unload_custom_font(time_font);
	fonts_unload_custom_font(date_font);
	fonts_unload_custom_font(batt_font);
	fonts_unload_custom_font(temp_font);
	
	text_layer_destroy(time_layer);
	text_layer_destroy(date_layer);
	text_layer_destroy(batt_layer);
	text_layer_destroy(temp_layer);
	text_layer_destroy(temp_layer_unanimated);
	text_layer_destroy(conditions_layer);
	text_layer_destroy(conditions_layer_unanimated);
	layer_destroy(weather_layer);
	layer_destroy(weather_layer_unanimated);
	layer_destroy(battery_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();

  // Update weather every 30 minutes
  if(tick_time->tm_min % 30 == 0) {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);

  // Send the message!
  app_message_outbox_send();
}
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
	if (shake_for_weather == 0) {
		// Do not animate
	} else {
		animate_layers();
	}
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
  accel_tap_service_subscribe(tap_handler);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  // check_settings();
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