SECOND = 1000;
MINUTE = SECOND * 60;
API_URL = "http://api.openweathermap.org/data/2.5/weather?lat={0}&lon={1}&mode=json";
API_CONFIG = "http://pebble-blisstime.azurewebsites.net/config.html";

var customOptions = getOptions(),
    tempInKelvin = 0,
    tempIcon = 0;

get_location_and_show_weather = function() {
  navigator.geolocation.getCurrentPosition(function(e) {
    get_weather(e.coords.longitude, e.coords.latitude);
  });

  setTimeout(get_location_and_show_weather, 5 * MINUTE);
};

get_weather = function(longitude, latitude) {
  var request = new XMLHttpRequest(),
    url = API_URL.replace("{0}", latitude).replace("{1}", longitude);

  request.open("GET", url, true);

  request.onload = function(e) {
    if(request.readyState == 4 && request.status == 200) {
      // console.log("ANSWER FROM SERVER: " + request.responseText);
      var response = JSON.parse(request.responseText);
      tempInKelvin = response.main.temp;
      tempIcon = get_icon_id(response.weather[0].icon);

      var pebble_data = {
        "target": 0,
        "icon": tempIcon,
        "degrees": getTemperature(tempInKelvin) //Number(temp)
        // "temp_kelvin": Number(Number(response.main.temp).toFixed(0)),
      };

      // send date to pebble
      Pebble.sendAppMessage(pebble_data);
      console.log("Send: " + JSON.stringify(pebble_data) + " to pebble");
    } else {
      console.log("ERROR: connection to weather api failed with code: " + request.status);
    }
  };

  request.send(null);
};

get_icon_id = function(key) {
  switch(key) {
    // clear_day
    case "01d":
      // return 0;
    // clear_night
    case "01n":
      return 1;
    // cloudy
    case "02d":
    case "02n":
    case "03d":
    case "03n":
    case "04d":
    case "04n":
      return 2;
    // shower_rain
    case "09d":
    case "09n":
      // return 3;
    // rain
    case "10d":
    case "10n":
      return 4;
    case "11d":
    case "11n":
    // thunderstorm
      return 5;
    // snow
    case "13d":
    case "13n":
      return 6;
    // mist
    case "50d":
    case "50n":
      return 7;
    // error
    default:
      return 8;
  }
};

convert_kelvin_to_celsius = function(kelvin) {
  return kelvin - 273.15;
};

convert_kelvin_to_fahrenheit = function(kelvin) {
  return (kelvin * 1.8) - 459.67;
};

function getTemperature(temp) {
  return (customOptions.temperature === 0)
          ? Number(convert_kelvin_to_fahrenheit(Number(temp)).toFixed(0))
          : Number(convert_kelvin_to_celsius(Number(temp)).toFixed(0));
}

Pebble.addEventListener("ready", function(e) {
  customOptions.target = 1;
  customOptions.icon = 0;
  customOptions.degrees = 0;
  Pebble.sendAppMessage(customOptions);
  console.log("Js ready: " + JSON.stringify(customOptions));
  setTimeout(function() {
    get_location_and_show_weather();
  }, 1000);
});

function getOptions() {
  var options = JSON.parse(localStorage.getItem('options')) ||
      {
        'temperature': 0,
        'timeformat': 0,
        'buzzfreq': 1,
        'leadtime': 2,
        'from': 9,
        'to': 17,
        'sunday': 0,
        'monday': 1,
        'tuesday': 1,
        'wednesday': 1,
        'thursday': 1,
        'friday': 1,
        'saturday': 0
      };
  return options;
}

Pebble.addEventListener("showConfiguration", function(e) {
  var json = JSON.stringify(getOptions()),
      url = API_CONFIG + "?options=" + encodeURIComponent(json);
  Pebble.openURL(url);
});

Pebble.addEventListener("webviewclosed", function(e) {
  customOptions = JSON.parse(e.response);
  localStorage.setItem("options", e.response);

  customOptions.target = 1;
  customOptions.icon = tempIcon;
  customOptions.degrees = getTemperature(tempInKelvin);

  Pebble.sendAppMessage(customOptions);
  console.log("Send options: " + JSON.stringify(customOptions) + " to pebble");
});
