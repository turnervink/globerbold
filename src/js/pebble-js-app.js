Pebble.addEventListener('ready', function() {
	console.log('PebbleKit JS Ready!');
});

// Weather

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  // Construct URL
	var url = 'https://api.forecast.io/forecast/APIKEY/' + 
	pos.coords.latitude + ',' + pos.coords.longitude;
	
	console.log("Lat is " + pos.coords.latitude);
	console.log("Lon is " + pos.coords.longitude);

  // Send request to forecast.io
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
	  console.log(JSON.parse(responseText));

      var temperature = Math.round(json.currently.temperature);
      console.log("Temperature is " + temperature);
			
	  var temperaturec = Math.round((json.currently.temperature - 32) * 5/9);
	  console.log("Temperature in Celsius is " + temperaturec);

      // Conditions
      var conditions = json.currently.summary;      
      console.log("Conditions are " + conditions);
			
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
		    "KEY_TEMPERATURE_IN_C": temperaturec,
		    "KEY_CONDITIONS": conditions,
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready! Getting weather.');

    // Get the initial weather
    getWeather();
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received! Updating weather.');

  }                     
);

// Configuration

Pebble.addEventListener('showConfiguration', function() {
	var url = 'http://turnervink.github.io/globerbold-config/';

	console.log('Showing configuration page: ' + url);

	Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
	var configData = JSON.parse(decodeURIComponent(e.response));

	console.log('Configuration page returned: ' + JSON.stringify(configData));

	
	console.log('Sending data to Pebble.');
	Pebble.sendAppMessage({
		showBattery: configData.showBattery ? 1 : 0,
		useCelsius: configData.useCelsius ? 1 : 0,
    shakeWeather: configData.shakeWeather ? 1 : 0
	}, function() {
		console.log('Send successful!');
	}, function() {
		console.log('Send failed!');
	});
});