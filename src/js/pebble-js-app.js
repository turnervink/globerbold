Pebble.addEventListener('ready', function() {
	console.log('PebbleKit JS Ready!');
});

Pebble.addEventListener('showConfiguration', function() {
	var url = 'http://346283b4.ngrok.com/';

	console.log('Showing configuration page: ' + url);

	Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
	var configData = JSON.parse(decodeURIComponent(e.response));

	console.log('Configuration page returned: ' + JSON.stringify(configData));

	if (configData.backgroundColor) {
		Pebble.sendAppMessage({
			showWeather: configData.showWeather,
			showBattery: configData.showBattery,
			useCelsius: configData.useCelsius
		}, function() {
			console.log('Send successful!');
		}, function() {
			console.log('Send failed!');
		});
	}
});