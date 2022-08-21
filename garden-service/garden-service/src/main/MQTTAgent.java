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
    private CommChannel channel;
    static private boolean alarmOn = false;
    static private MqttClient client;
    public MQTTAgent() throws Exception {
        channel = new SerialCommChannel("COM5", 9600);
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

//			log("publishing a msg");
//			client.publish("esiot-2122",
//				  Buffer.buffer("hello"),
//				  MqttQoS.AT_LEAST_ONCE,
//				  false,
//				  false);
            new Thread(){
                @Override
                public void run() {
                    while(true){
                        if(channel.isMsgAvailable()){
                            String msg = null;
                            try {
                                msg = channel.receiveMsg();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                            if( msg != null){
                                handleMsg(msg);
                            }
                            if(alarmOn){
                                MQTTAgent.client.publish("smart-garden",
                                        Buffer.buffer("ALARMON"),
                                        MqttQoS.AT_LEAST_ONCE,
                                        false,
                                        false);
                                System.out.println("Sent alarm via MQTT");
                            }
                        }
                    }
                }
            }.start();
        });
    }

    /**
     * handle serial msg.
     * @param msg
     */
    private void handleMsg(String msg){
        if(msg.contains("ALARMON")){
            alarmOn = true;
        } else if(msg.contains("ALARMOFF")){
            System.out.println("Alarm deactivated");
            alarmOn = false;
            MQTTAgent.client.publish("smart-garden",
                    Buffer.buffer("ALARMOFF"),
                    MqttQoS.AT_LEAST_ONCE,
                    false,
                    false);
        }
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
        channel.sendMsg(msg);
        //channel.sendMsg("LED1ON LED2ON LED34:1 IRRIGATIONON SPEED:1");
    }
    
    public void send(String msg) {
        MqttClient client = MqttClient.create(vertx);
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