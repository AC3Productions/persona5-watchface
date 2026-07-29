var Clay = require('@rebble/clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

var SunCalc = require('suncalc');

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

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

function weatherCallback(pos) {
  var url = 'https://api.open-meteo.com/v1/forecast?' +
      'latitude=' + pos.coords.latitude +
      '&longitude=' + pos.coords.longitude +
      '&temperature_unit=fahrenheit' +
      '&current=temperature_2m,weather_code';
  
  xhrRequest(url, 'GET',
    function(responseText) {
      var json = JSON.parse(responseText);

      var temperature = Math.round(json.current.temperature_2m);
      var conditions = json.current.weather_code;

      var dictionary = {
        'TEMPERATURE': temperature,
        'CONDITIONS': conditions,
      };

      Pebble.sendAppMessage(dictionary,
        function(e) { console.log('Weather info sent!'); },
        function(e) { console.log('Error sending weather info!'); }
      );
    }
  );

  
}

function locationError(err) {
  console.log('Error requesting location!');
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    weatherCallback,
    locationError,
    { timeout: 15000, maximumAge: 60000 }
  );
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
    getWeather();
    getSunTimes();
  }
);

Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    if (e.payload['REQUEST_WEATHER']) {
      getWeather();
    }
    if (e.payload['REQUEST_SUNTIMES']) {
      getSunTimes();
    }
  }
);
