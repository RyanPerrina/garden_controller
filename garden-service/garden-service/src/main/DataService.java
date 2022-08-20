package main;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.Vertx;
import io.vertx.core.buffer.Buffer;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonArray;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.RoutingContext;
import io.vertx.ext.web.handler.BodyHandler;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/*
 * Data Service as a vertx event-loop 
 */
public class DataService extends AbstractVerticle {

	private int port;
	private static final int MAX_SIZE = 10;
	private LinkedList<DataPoint> values;
	
	public DataService(int port) {
		values = new LinkedList<>();		
		this.port = port;
	}

	@Override
	public void start() {		
		Router router = Router.router(vertx);
		router.route().handler(BodyHandler.create());
		router.post("/api/data").handler(this::handleAddNewData);
		router.get("/api/data").handler(this::handleGetData);		
		vertx
			.createHttpServer()
			.requestHandler(router)
			.listen(port);

		log("Service ready.");
	}
	
	private void handleAddNewData(RoutingContext routingContext) {
		HttpServerResponse response = routingContext.response();
		JsonObject res = routingContext.body().asJsonObject();
		if (res == null) {
			sendError(400, response);
		} else {
		        boolean l1 = res.getBoolean("l1");
		        boolean l2 = res.getBoolean("l2");
		        int l3 = res.getInteger("l3");
		        int l4 = res.getInteger("l4");
		        float temp = res.getFloat("temp");
		        int light = res.getInteger("light");
			
			values.addFirst(new DataPoint(l1, l2, l3, l4, temp, light));
			if (values.size() > MAX_SIZE) {
				values.removeLast();
			}
			
			log("Added new values");
			response.setStatusCode(200).end();
		}
	}
	
	private void handleGetData(RoutingContext routingContext) {
		JsonArray arr = new JsonArray();
		for (DataPoint p: values) {
			JsonObject data = new JsonObject();
			data.put("l1", p.getL1());
			data.put("l2", p.getL2());
			data.put("l3", p.getL3());
			data.put("l4", p.getL4());
			data.put("temp", p.getTemp());
			data.put("light", p.getLight());
			arr.add(data);
		}
		routingContext.response()
			.putHeader("content-type", "application/json")
			.end(arr.encodePrettily());
	}
	
	private void sendError(int statusCode, HttpServerResponse response) {
		response.setStatusCode(statusCode).end();
	}

	private void log(String msg) {
		System.out.println("[DATA SERVICE] "+msg);
	}
	
	public void sendData(final boolean l1, final boolean l2, final int l3, final int l4, final float temp, final int light) {
	    values.addFirst(new DataPoint(l1, l2, l3, l4, temp, light));
            if (values.size() > MAX_SIZE) {
                    values.removeLast();
            }
            log("Added new values");
	}

}