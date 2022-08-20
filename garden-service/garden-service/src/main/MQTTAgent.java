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
        channel = new SerialCommChannel("COM3",9600);
    }

    @Override
    public void start() {
        MQTTAgent.client = MqttClient.create(vertx);

        MQTTAgent.client.connect(1883, "broker.mqtt-dashboard.com", c -> {

            log("connected");

            log("subscribing...");
            MQTTAgent.client.publishHandler(s -> {
//                        System.out.println("There are new message in topic: " + s.topicName());
//                        System.out.println("Content(as string) of the message: " + s.payload().toString());
//                        System.out.println("QoS: " + s.qosLevel());

                        String msg = s.payload().toString();
                        if(msg.contains("ALARM")){
                            //System.out.println("ricevuto alarm infatti");
                            return;
                        }
                        Integer luminosity = Integer.parseInt(msg.substring("L: ".length(),msg.indexOf(" T:") ));
                        Integer temperature = Integer.parseInt(msg.substring(msg.indexOf("T: ")+"T: ".length(),msg.indexOf("\n")));
                        System.out.println("ok: "+luminosity + " " + temperature);
                        if(!MQTTAgent.alarmOn){
                            handleData(luminosity,temperature);
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
                            if(MQTTAgent.alarmOn){
                                MQTTAgent.client.publish("smart-garden",
                                        Buffer.buffer("ALARMON"),
                                        MqttQoS.AT_LEAST_ONCE,
                                        false,
                                        false);
                                System.out.println("inviato alarm via mqtt");
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
            MQTTAgent.alarmOn = true;
        } else if(msg.contains("ALARMOFF")){
            System.out.println("disattivato alarm");
            MQTTAgent.alarmOn = false;
            MQTTAgent.client.publish("smart-garden",
                    Buffer.buffer("ALARMOFF"),
                    MqttQoS.AT_LEAST_ONCE,
                    false,
                    false);

        }
    }

    private void log(String msg) {
        System.out.println("[DATA SERVICE] "+msg);
    }

    private void handleData(int luminosity,int temperature) {
        String msg = "";
        if (luminosity < 5){
            msg += "LED1ON LED2ON";
            int led34intensity = (int) ((((double) luminosity)/8)*4);
            msg += " LED34:"+String.valueOf(led34intensity);

        }
        if(luminosity<2){
            msg += " IRRIGATIONON";
        } else {
            msg+= " IRRIGATIONOFF";
        }
        msg += " SPEED:" + String.valueOf(temperature);

        if(temperature==5){
            msg += " TEMPERATURECHECK";
        }
        System.out.println(msg);
        channel.sendMsg(msg);
        //channel.sendMsg("LED1ON LED2ON LED34:1 IRRIGATIONON SPEED:1");
    }
}