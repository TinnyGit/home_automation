		var mqtt;
		var reconnectTimeout = 2000;
		var host="xxx.xxx.x.xxx"; //change this
		var port=xxxx;
	
		function update(flag) {
			message = new Paho.MQTT.Message(flag);
			message.destinationName = "inTopic";
			mqtt.send(message);
		}

	 	function onConnect() {
	  // Once a connection has been made, make a subscription and send a message.
	
		console.log("Connected ");
		//mqtt.subscribe("sensor1");
		message = new Paho.MQTT.Message("0");
		message.destinationName = "inTopic";
		mqtt.send(message);
	  }

	  function MQTTconnect() {
		console.log("connecting to "+ host +" "+ port);
		mqtt = new Paho.MQTT.Client(host,port,"clientjs");
		//document.write("connecting to "+ host);
		var options = {
			timeout: 3,
			onSuccess: onConnect,
		  
		 };
		 
		mqtt.connect(options); //connect
		}
