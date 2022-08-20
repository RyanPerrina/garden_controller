package main;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;

public class GardenService extends AbstractVerticle {
    
    enum Mode { AUTO, MANUAL, ALARM }
    
    static Mode mode = Mode.AUTO;
    
    public static void main(String[] args) throws Exception {
        // MQTT agent for Sensorboard (ESP32)
        Vertx mqtt = Vertx.vertx();
        MQTTAgent agent = new MQTTAgent();
        mqtt.deployVerticle(agent);
        
        // HTTP agent for Dashboard (Web page)
        Vertx http = Vertx.vertx();
        DataService service = new DataService(8080);
        http.deployVerticle(service);
        
        while(true) {
            if (mode == Mode.AUTO) {
                // read data from sensorboard and send them to the dashboard 
                //service.sendData(l1, l2, l3, l4, temp, light);
            } else if (mode == Mode.MANUAL) {
                // read data from sensorboard and send them to the dashboard
                //service.sendData(l1, l2, l3, l4, temp, light);
            } else {
                
            }
        }
    }
}
