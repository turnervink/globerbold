console.log('main.js loaded!');

(function () {
	loadOptions();
	submitHandler();
})();

function submitHandler() {
	var $submitButton = $('#submitButton');

	$submitButton.on('click', function() {
		console.log('Submit');

		var return_to = getQueryParam('return_to', 'pebblejs://close#');
		document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
	});
}

function loadOptions() {
	var $showWeather = $('#showWeather');
	var $showBattery = $('#showBattery');
	var $useCelsius = $('#useCelsius');

	if (localStorage.backgroundColor) {
		$showWeather[0].checked = localStorage.showWeather === 'true';
		$showBattery[0].checked = localStorage.showBattery === 'true';
		$useCelsius[0].checked = localStorage.useCelsius === 'true';
	}
}

function getAndStoreConfigData() {
	var $showWeather = $('#showWeather');
	var $showBattery = $('#showBattery');
	var $useCelsius = $('#useCelsius');

	var options = {
		showWeather: $showWeather[0].checked,
		showBattery: $showBattery[0].checked,
		useCelsius: $useCelsius[0].checked,
	};

	localStorage.showWeather = options.showWeather;
	localStorage.showBattery = options.showBattery;
	localStorage.useCelsius = options.useCelsius;

	console.log('Got options: ' + JSON.stringify(options));
	return options;
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}