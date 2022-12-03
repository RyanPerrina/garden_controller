package main;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.mqtt.MqttClient;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/*
 * MQTT Agent
 */

public class MQTTAgent extends AbstractVerticle {
    static private boolean alarmOn = false;
    static private MqttClient client;
    public MQTTAgent() throws Exception {
    }
    
    private int luminosity;
    private int temperature;

    @Override
    public void start() {
        MQTTAgent.client = MqttClient.create(vertx);

        MQTTAgent.client.connect(1883, "broker.mqtt-dashboard.com", c -> {
            log("connected");
            log("subscribing...");
            
            MQTTAgent.client.publishHandler(s -> {
                        
                        String msg = s.payload().toString();
                        if(msg.contains("ALARM")){
                            alarmOn = true;
                            return;
                        }
                        this.luminosity = Integer.parseInt(msg.substring("L: ".length(), msg.indexOf(" T:") ));
                        this.temperature = Integer.parseInt(msg.substring(msg.indexOf("T: ") + "T: ".length(), msg.indexOf("\n")));
                        System.out.println("Received: " + this.luminosity + " " + this.temperature);
                        if(!alarmOn){
                            handleData(luminosity, temperature);
                        }

                    })
                    .subscribe("smart-garden", 2);
        });
    }

    private void log(String msg) {
        System.out.println("[DATA SERVICE] " + msg);
    }

    private void handleData(int luminosity, int temperature) {
        String msg = "";
        if (luminosity < 5){
            msg += "LED1ON LED2ON";
            int led34intensity = (int) ((((double) luminosity) / 8) * 4);
            msg += " LED34:" + String.valueOf(led34intensity);

        }
        if(luminosity < 2){
            msg += " IRRIGATIONON";
        } else {
            msg+= " IRRIGATIONOFF";
        }
        msg += " SPEED:" + String.valueOf(temperature);

        if(temperature == 5){
            msg += " TEMPERATURECHECK";
        }
        System.out.println(msg);
    }
    
    public void send(String msg) {

        client.publish("smart-garden",
                       Buffer.buffer(msg),
                       MqttQoS.AT_LEAST_ONCE,
                       false,
                       false);
    }
    
    public int getLuminosity() {
        return this.luminosity;
    }
    
    public int getTemperature() {
        return this.temperature;
    }
}