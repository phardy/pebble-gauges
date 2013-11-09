var initialised = false;

Pebble.addEventListener("ready", function() {
    initialized = true;
});

Pebble.addEventListener("showConfiguration", function() {
    console.log("showing configuration");
    Pebble.openURL('http://hardy.dropbear.id.au/pebble/Gauges/config/2-1.html');
});

Pebble.addEventListener("webviewclosed", function(e) {
    console.log("configuration closed");
    var options = JSON.parse(decodeURIComponent(e.response));
    console.log("Options = " + JSON.stringify(options));
});
