var R_PORT = 5678;
var S_PORT = 8765;
var HOST = 'aaaa::1';

var dgram = require("dgram");
var ubidots = require('ubidots');

var server = dgram.createSocket("udp6");
var address = null;
var desiredState = null;
var cons = null;

server.on("error", function (err) {
	console.log("server error:\n" + err.stack);
	server.close();
});

server.on("listening", function () {
  	var address = server.address();
  	console.log("server listening " +
    	address.address + ":" + address.port);
});

server.bind(R_PORT);


var client = ubidots.createClient('c30b4304853b3a2dd5868d1c0df305d817eabb8f');

// Connexion établie vers Ubidots
client.auth(function () {

	var self = this;

	var temperature = this.getVariable('5852d2cf76254208c05b89d2');
	var button = this.getVariable('5852d2fa76254208c2c6c781');
	var consigne = this.getVariable('5853116276254208c05d9106');

	server.on("message", function (msg, rinfo) {
		var bt = msg.readUInt8(0) == 1 ? '1' : '0';
		var temp = msg.readUInt16LE(2);

		var temp = 65535.0/(temp)-1.0;
    	temp = 100000.0*temp;
		temp = 1.0/(Math.log(temp/100000.0)/4275+1/298.15)-273.15; //convert to temperature via datasheet ;
		console.log(temp);
		// Enregistre l'adresse du thermostat
		address = rinfo.address;

		// fait la comparaison avec la valeur de consigne
		// Règle le bouton sur la plateforme 
		
		if(cons != null) {
			if(cons > temp) {
				button.saveValue(1);
			} else if(cons+0.5 < temp) {
				button.saveValue(0);
			}
		}
		// Renvoie l'ordre si le relai n'est pas dans le bon état
		// Asservissement du relai
		if( bt != desiredState && desiredState != null) {
			sendState(desiredState);
		}
		// Asservissement de ubidots
		temperature.saveValue(temp);
	});

	var interval = setInterval(function() {
		if(!address) return;
		// récupère l'état forcé du relai
		// rétroaction de la plateforme vers le thermostat
		button.getValues(function(err, data){
			if(err) return console.log(err);
			if(data.results[0].value != data.results[1].value) {
				desiredState = data.results[0].value == 1 ? '1' : '0'; // Enregistre l'état du relai
				sendState(desiredState);	// envoie l'état
				button.saveValue(data.results[0].value);	// Debounce pour éviter d'envoyer plusieurs fois
			}
		});
		// Récupère la consigne
		consigne.getValues(function(err, data){
			if(err) return console.log(err);
			cons = data.results[0].value;
		});
	}, 1000);
  
});


function sendState(s) {
	server.send(s, S_PORT, address, function(err, done) {
		console.log(s == 1 ? 'ON' : 'OFF');
	});
}