var initialised = false;

Pebble.addEventListener("ready", function() {
    initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
    var options = JSON.parse(window.localStorage.getItem('options'));
    console.log("read options: " + JSON.stringify(options));
    console.log("showing configuration");
    var uri = 'http://hardy.dropbear.id.au/pebble/Gauges/config/2-1.html?' + 'btdisco-notif=' + encodeURIComponent(options['btdisco-notif']) + '&lowbat-notif=' + encodeURIComponent(options['lowbat-notif']);
    Pebble.openURL(uri);
});

Pebble.addEventListener("webviewclosed", function(e) {
    console.log("configuration closed");
    if (e.response != '') {
	var options = JSON.parse(decodeURIComponent(e.response));
	console.log("storing options: " + JSON.stringify(options));
	window.localStorage.setItem('options', JSON.stringify(options));
    } else {
	console.log("no options received");
    }
});
