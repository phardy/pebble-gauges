var initialised = false;

function appMessageAck(e) {
    console.log("options sent to Pebble successfully");
}

function appMessageNack(e) {
    console.log("options not sent to Pebble: " + e.error.message);
}

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
	console.log("sending btdisco");
	Pebble.sendAppMessage({"btdisco": options["btdisco"]},
			     appMessageAck, appMessageNack);
	console.log("sending lowbat");
	Pebble.sendAppMessage({"lowbat": options["lowbat"]},
			     appMessageAck, appMessageNack);
    } else {
	console.log("no options received");
    }
});
