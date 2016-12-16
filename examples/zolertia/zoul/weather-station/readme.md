##SOURCES

Nous nous sommes inspirés du travail de Antonio sur Hackster.io. 
Nous avons repris son code de weather station et nous avons associé nos capteurs et actionneurs

Nous avons également implémenté un middleware qui récupère les données depuis le border router et les envoie vers Ubidots grâce à leur API REST


https://www.hackster.io/alinan/diy-amateur-weather-station-over-6lowpan-ipv6-680dba?ref=part&ref_id=16208&offset=3


##INSTALLATION


## Zolertia 1
Sur la première Zolertia, on installe le projet weather-station

	cd ./examples/zolertia/zoul/weather-station
	sudo make upd-client.upload TARGET=zoul

Ddéconnecter la RE-Mote
Brancher le relais Grove sur l'entrée ADC1
Brancher le capteur de température Grove sur l'entrée ADC3

## Zolertia 2
Sur la deuxième Zolertia, on installe le border router

	cd ../../ipv6/rpl-border-router
	sudo make border-router.upload TARGET=zoul

On lance le tunnel IPV6

	cd ../../../tools/
	sudo make tunslip6
	sudo ./tunslip6 -s /dev/ttyUSB0 -t tun aaaa::1/64

## Ubidots 

Créer un compte sur ubidots

####Dans les sources
Créer une nouvelle source de donnée
Créer une variable temperature ( range : 0 - 100 )
Créer une variable relais ( range : 0 - 1 )
Créer une variable consigne ( range : 0 - 100 )

####Sur le dashboard
Créer un nouveau widget pour visualiser la temperature (widget - chart -line chart)
Créer un nouveau widget pour visualiser la consigne (widget - controls - slider)
Créer un nouveau widget pour changer l'état du relais (widget - controls - switch)


Récupérer les identifiants de chaque variable et sa clé d'API

## Concentrateur des données

On ouvre un nouveau terminal depuis la racine de contiki

	cd ./examples/zolertia/zoul/weather-station
	npm install ubidots
	node index.js

Il faut ABSOLUMENT modifier dans index.js (lignes 27,34,35,36) les clés d'API Ubidots et les identifiants des variables
Attention a l'ECE la connexion saute parfois. Il faut donc relancer le processus si c'est le cas.


