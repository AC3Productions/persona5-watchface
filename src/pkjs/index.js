var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

var SunCalc = require('suncalc');
var latestWeather = null;

function sunTimesCallback(pos) {
  const sun_times = SunCalc.getTimes(new Date(), pos.coords.latitude, pos.coords.longitude);
  
  var sunrise_hour = sun_times.sunrise.getHours();
  var sunrise_min = sun_times.sunrise.getMinutes();
  var sunset_hour = sun_times.sunset.getHours();
  var sunset_min = sun_times.sunset.getMinutes();

  var dictionary = {
    'SUNRISE_HOUR': sunrise_hour,
    'SUNRISE_MIN': sunrise_min,
    'SUNSET_HOUR': sunset_hour,
    'SUNSET_MIN': sunset_min,
  };

  Pebble.sendAppMessage(dictionary,
    function(e) { console.log('Sun Times info sent!'); },
    function(e) { console.log('Error sending Sun Times!'); }
  );
}

function sendWeather() {
  if (!latestWeather) {
    return;
  }
  Pebble.sendAppMessage(latestWeather,
    function(e) {
      console.log('Weather info sent to Pebble successfully!');
    },
    function(e) {
      console.log('Error sending weather info to Pebble!');
    }
  );
}

function toCelsius(reading) {
  if (reading.unit === '°F') {
    return (reading.value - 32) * 5 / 9;
  }
  return reading.value;
}

// Subscribe to the weather from the Pebble app's weather plugin
function subscribeToWeather() {
  Pebble.subscribeToSource({
    category: 'weather',
    item: 'location',
    properties: ['temperature', 'condition_code'],
    onData: function(update) {
      // The user's first weather location
      var here = update.instances[0];
      if (!here) {
        return;
      }

      // Any property can be missing, so check before using it
      var temperature = here.properties.temperature;
      var condition = here.properties.condition_code;
      if (!temperature || !temperature.shortText ||
          !condition || !condition.longText) {
        console.log('Missing something!')
        return;
      }

      latestWeather = {
        'TEMPERATURE': temperature.shortText.text,
        'CONDITIONS': condition.longText.text
      };
      sendWeather();
    },
    onError: function(err) {
      console.log('Weather unavailable: ' + err.code + ' ' + err.message);
    }
  });
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getSunTimes(err) {
  navigator.geolocation.getCurrentPosition(
    sunTimesCallback,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
}

Pebble.addEventListener('ready',
  function(e) {
    console.log('PebbleKit JS ready!');
    subscribeToWeather();
    getSunTimes();
  }
);

Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    if (e.payload['REQUEST_WEATHER']) {
      sendWeather();
    }
    if (e.payload['REQUEST_SUNTIMES']) {
      getSunTimes();
    }
  }
);
