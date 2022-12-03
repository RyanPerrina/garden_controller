package main;

import io.netty.handler.codec.mqtt.MqttQoS;
import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;

public class GardenService extends AbstractVerticle {
    enum Mode { AUTO, MANUAL, ALARM }
    enum State { IDLE, IRRIGATING, ALARM }
    
    static Mode mode = Mode.AUTO;
    static State state = State.IDLE;
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
        DataService service = new DataService(8001);
        http.deployVerticle(service);
        
        // Serial channel for Controller (Arduino)
        CommChannel channel = new SerialCommChannel("COM3", 9600);
        
        Thread.sleep(5000);
        
        while(true) {
            int temp = agent.getTemperature();
            int light = agent.getLuminosity();
            
//            if (temp == 5 && mode != Mode.ALARM && state != State.IRRIGATING) {
//                mode = Mode.ALARM;
//                agent.send("ALARMON");          // send ALARM message to sensorboard
//                channel.sendMsg("ALARMON");     // send ALARM message to controller
//                state = State.ALARM;
//                service.sendData(state.toString(), l1, l2, l3, l4, temp, light);
//            }



            
            String msg = "";
            System.out.print("temp e luce:");
            System.out.print(temp );
            System.out.println(light);
            switch(mode) {
                // receive data from sensorboard and send them to dashboard and controller
                case AUTO:
                    if (channel.isMsgAvailable()) {
                        msg = channel.receiveMsg();
                        System.out.println("arduino received:"+msg);
                        if (msg.contains("MANUALMODEON")){
                            mode = Mode.MANUAL;
                            System.out.println("Switch to manual mode.");
                            break;
                        } else if (msg.contains("ALARMON")){
                            mode = Mode.ALARM;
                            agent.send("ALARMON");          // send ALARM message to sensorboard
                            state = State.ALARM;
                            service.sendData(state.toString(), l1, l2, l3, l4, temp, light);
                            break;
                        }
                        msg = "";
                        break;
                    }
                    
                    if (light < 5){
                        msg += "LED1ON LED2ON";
                        int led34intensity = 4-(int) ((((double) light-1) / 7) * 4);
                        msg += " LED34:" + String.valueOf(led34intensity);
                        l1 = l2 = true;
                        l3 = l4 = led34intensity;
                    } else {
                        l1 = l2 = false;
                        l3 = l4 = 0;
                    }
                    
                    if(light < 2){
                        msg += " IRRIGATIONON";
                        state = State.IRRIGATING;
                    } else {
                        msg += " IRRIGATIONOFF";
                        state = State.IDLE;

                    }
                    
                    msg += " SPEED:" + String.valueOf(temp);

                    if(temp == 5){
                        msg += " TEMPERATURECHECK";
                    }
                    System.out.println("Sent:" + msg);
                    channel.sendMsg(msg);
                    msg = "";
                    
                    service.sendData(state.toString(), l1, l2, l3, l4, temp, light);
                    Thread.sleep(3000);
                    break;
                    
                // receive data from sensorboard and controller and send them to dashboard
                case MANUAL:
                    if (channel.isMsgAvailable()) {
                        msg = channel.receiveMsg();
                        System.out.println("Received:" + msg);
                        // check message
                        if (msg.contains("MANUALMODEOFF")) {
                            mode = Mode.AUTO;
                            l1 = l2 = false;
                            l3 = l4 = 0;
                            System.out.println("Switch to auto mode.");
                            break;
                        }
                        if (msg.contains("IRRIGATION: ON/OFF")) {
                            state = (state == State.IDLE) ? State.IRRIGATING : State.IDLE;
                        } else if (msg.contains("L1")) {
                            l1 = !l1;
                        } else if (msg.contains("L2")) {
                            l2 = !l2;
                        } else if (msg.contains("L3 UP")) {
                            l3++;
                        } else if (msg.contains("L3 DOWN")) {
                            l3--;
                        } else if (msg.contains("L4 UP")) {
                            l4++;
                        } else if (msg.contains("L4 DOWN")) {
                            l4--;
                        }
                        service.sendData(state.toString(), l1, l2, l3, l4, temp, light);
                    }
                    break;
                    
                // wait for ALARM OFF message from sensorboard
                case ALARM:
                    if (channel.isMsgAvailable()) {
                        msg = channel.receiveMsg();
                        if (msg.contains("ALARMOFF")) {
                            mode = Mode.AUTO;
                            state = State.IDLE;
                            agent.send("ALARMOFF");
                        }
                    }
                    service.sendData(state.toString(), l1, l2, l3, l4, temp, light);
                    Thread.sleep(3000);
                    break;
            }
        }
    }
}
