package main;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;

public class GardenService extends AbstractVerticle {
    
    enum Mode { AUTO, MANUAL, ALARM }
    
    static Mode mode = Mode.AUTO;
    static boolean l1 = false;
    static boolean l2 = false;
    static int l3 = 0;
    static int l4 = 0;
    
    public static void main(String[] args) throws Exception {
        // MQTT agent for Sensorboard (ESP32)
        Vertx mqtt = Vertx.vertx();
        MQTTAgent agent = new MQTTAgent();
        mqtt.deployVerticle(agent);
        
        // HTTP agent for Dashboard (Web page)
        Vertx http = Vertx.vertx();
        DataService service = new DataService(8080);
        http.deployVerticle(service);
        
        // Serial channel for Controller (Arduino)
        CommChannel channel = new SerialCommChannel("COM5", 9600);
        
        while(true) {
            int temp = agent.getTemperature();
            int light = agent.getLuminosity();
            
            if (temp == 5 && mode != Mode.ALARM) {
                mode = Mode.ALARM;
                agent.send("ALARMON");          // send ALARM message to sensorboard
                channel.sendMsg("ALARMON");     // send ALARM message to controller
            }
            
            String msg = "";
            switch(mode) {
                // receive data from sensorboard and send them to dashboard and controller
                case AUTO:
                    if (light < 5){
                        msg += "LED1ON LED2ON";
                        int led34intensity = (int) ((((double) light) / 8) * 4);
                        msg += " LED34:" + String.valueOf(led34intensity);
                        l1 = l2 = true;
                        l3 = l4 = led34intensity;
                    }
                    
                    if(light < 2){
                        msg += " IRRIGATIONON";
                    } else {
                        msg += " IRRIGATIONOFF";
                    }
                    msg += " SPEED:" + String.valueOf(temp);

                    if(temp == 5){
                        msg += " TEMPERATURECHECK";
                    }
                    System.out.println("Sent: " + msg);
                    channel.sendMsg(msg);
                    
                    service.sendData(l1, l2, l3, l4, temp, light);
                    break;
                    
                // receive data from sensorboard and controller and send them to dashboard
                case MANUAL:
                    if (channel.isMsgAvailable()) {
                        msg = channel.receiveMsg();
                        // handle message (l1, l2, l3, l4, irrigation)
                        service.sendData(l1, l2, l3, l4, temp, light);
                    }
                    break;
                    
                // wait for ALARM OFF message from sensorboard
                case ALARM:
                    if (channel.isMsgAvailable()) {
                        msg = channel.receiveMsg();
                        if (msg.contains("ALARMOFF")) {
                            mode = Mode.AUTO;
                            agent.send("ALARMOFF");
                        }
                    }
                    break;
            }
        }
    }
}
