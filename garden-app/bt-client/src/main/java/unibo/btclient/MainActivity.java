package unibo.btclient;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.util.UUID;

import unibo.btlib.BluetoothChannel;
import unibo.btlib.BluetoothUtils;
import unibo.btlib.ConnectToBluetoothServerTask;
import unibo.btlib.ConnectionTask;
import unibo.btlib.RealBluetoothChannel;
import unibo.btlib.exceptions.BluetoothDeviceNotFound;
import unibo.btclient.utils.C;

public class MainActivity extends AppCompatActivity {
    private BluetoothChannel btChannel;
    private boolean manualModeEnabled = false;
    private boolean alarmEnabled = false;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final BluetoothAdapter btAdapter = BluetoothAdapter.getDefaultAdapter();

        if(btAdapter != null && !btAdapter.isEnabled()) {
            startActivityForResult(
                new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                C.bluetooth.ENABLE_BT_REQUEST
            );
        }

        initUI();
    }

    private void initUI() {
        findViewById(R.id.disableManualModeBtn).setEnabled(false);
        findViewById(R.id.alarmBtn).setEnabled(false);
        findViewById(R.id.requireManualModeBtn).setOnClickListener(l -> {
            l.setEnabled(false);
            try {
                connectToBTServer();
            } catch (BluetoothDeviceNotFound bluetoothDeviceNotFound) {
                Toast.makeText(this, "Bluetooth device not found !", Toast.LENGTH_LONG)
                        .show();
                bluetoothDeviceNotFound.printStackTrace();
            } finally {
                l.setEnabled(true);
            }
        });

//        findViewById(R.id.sendBtn).setOnClickListener(l -> {
//            String message = ((EditText)findViewById(R.id.editText)).getText().toString();
//            btChannel.sendMessage(message);
//            ((EditText)findViewById(R.id.editText)).setText("");
//        });
    }

    @Override
    protected void onStop() {
        super.onStop();
        if(manualModeEnabled){
            sendMessage("DISABLEMANUALMODE");
        }
        btChannel.close();
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode,
                                    @Nullable final Intent data) {
        if(requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_OK) {
            Log.d(C.APP_LOG_TAG, "Bluetooth enabled!");
        }

        if(requestCode == C.bluetooth.ENABLE_BT_REQUEST && resultCode == RESULT_CANCELED) {
            Log.d(C.APP_LOG_TAG, "Bluetooth not enabled!");
        }
    }

    private void connectToBTServer() throws BluetoothDeviceNotFound {
        final BluetoothDevice serverDevice = BluetoothUtils
                .getPairedDeviceByName(C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME);
        // !!! Choose the right UUID value
        final UUID uuid = BluetoothUtils.getEmbeddedDeviceDefaultUuid();
//        final UUID uuid = BluetoothUtils.generateUuidFromString(C.bluetooth.BT_SERVER_UUID);

        new ConnectToBluetoothServerTask(serverDevice, uuid, new ConnectionTask.EventListener() {
            @Override
            public void onConnectionActive(final BluetoothChannel channel) {
                ((TextView) findViewById(R.id.statusLabel)).setText(String.format(
                    "Status : connected to server on device %s",
                    serverDevice.getName()
                ));
                findViewById(R.id.requireManualModeBtn).setEnabled(false);
                findViewById(R.id.disableManualModeBtn).setEnabled(true);
                btChannel = channel;
                btChannel.registerListener(new RealBluetoothChannel.Listener() {
                    @Override
                    public void onMessageReceived(String receivedMessage) {
                        Log.d("ricevuto" + receivedMessage,this.getClass().getSimpleName());

                        if(receivedMessage.equals("MANUAL MODE ON\r")){
                            manualModeEnabled = true;
                            Log.d("manual mode enabled",this.getClass().getSimpleName());
                            btChannel.sendMessage("ACK MANUAL MODE ON");
                        }
                        if(receivedMessage.startsWith("IRRIGATION SPEED: ")){
                            String speedStr = receivedMessage.substring("IRRIGATION SPEED: ".length(),receivedMessage.length()-1);
                            ((TextView)findViewById(R.id.irrigationTxtView)).setText(speedStr);
                        }
                        if(receivedMessage.startsWith("LED3 INTENSITY: ")){
                            String speedStr = receivedMessage.substring("LED3 INTENSITY: ".length(),receivedMessage.length()-1);
                            ((TextView)findViewById(R.id.led3TxtView)).setText(speedStr);
                        }
                        if(receivedMessage.startsWith("LED4 INTENSITY: ")){
                            String speedStr = receivedMessage.substring("LED4 INTENSITY: ".length(),receivedMessage.length()-1);
                            ((TextView)findViewById(R.id.led4TxtView)).setText(speedStr);
                        }
                        if(receivedMessage.startsWith("AUTOMODEENABLED")){
                            manualModeEnabled = false;
                            ((Button)findViewById(R.id.requireManualModeBtn)).setEnabled(true);
                            ((TextView)findViewById(R.id.statusLabel)).setText("no connection");
                            findViewById(R.id.disableManualModeBtn).setEnabled(false);
                            channel.close();
                        }
                        if(receivedMessage.startsWith("ALARM")){
                            findViewById(R.id.alarmBtn).setEnabled(true);
                            findViewById(R.id.disableManualModeBtn).setEnabled(false);
                            alarmEnabled = true;
                        }
                        if(receivedMessage.startsWith("ALARM DISABLED")){
                            findViewById(R.id.alarmBtn).setEnabled(false);
                            alarmEnabled = false;
                            ((Button)findViewById(R.id.requireManualModeBtn)).setEnabled(true);
                            btChannel.close();
                            ((TextView)findViewById(R.id.statusLabel)).setText("no connection");


                        }


                    }

                    @Override
                    public void onMessageSent(String sentMessage) {
//                        ((TextView) findViewById(R.id.chatLabel)).append(String.format(
//                                "> [SENT to %s] %s\n",
//                                btChannel.getRemoteDeviceName(),
//                                sentMessage
//                        ));
                    }
                });
                btChannel.sendMessage("MANUAL MODE REQUEST");
            }

            @Override
            public void onConnectionCanceled() {
                ((TextView) findViewById(R.id.statusLabel)).setText(String.format(
                    "Status : unable to connect, device %s not found!",
                    C.bluetooth.BT_DEVICE_ACTING_AS_SERVER_NAME
                ));
                manualModeEnabled=false;
            }


        }).execute();
    }
    private void sendMessage(String txt){
        if(manualModeEnabled){
            btChannel.sendMessage(txt);
            Log.d("sent message",this.getClass().getSimpleName());
        }
    }

    private void disableAlarm(){
        btChannel.sendMessage("DISABLEALARM");
    }

    public void led1btnHandler(View view) {
        sendMessage("LED1: ON/OFF");
    }

    public void led2btnHandler(View view) {
        sendMessage("LED2: ON/OFF");

    }

    public void irrigationOnOffHandler(View view) {
        sendMessage("IRRIGATION: ON/OFF");
    }

    public void irrigationSpeedMinusHandler(View view) {
        sendMessage("IRRIGATION: SPEED DOWN");


    }

    public void irrigationSpeedPlusHandler(View view) {
        sendMessage("IRRIGATION: SPEED UP");

    }

    public void led3intensityUpHandler(View view) {
        sendMessage("LED3: INTENSITY UP");

    }

    public void led4intensityUpHandler(View view) {
        sendMessage("LED4: INTENSITY UP");

    }

    public void led3intensityDownHandler(View view) {
        sendMessage("LED3: INTENSITY DOWN");

    }

    public void led4intensityDownHandler(View view) {
        sendMessage("LED4: INTENSITY DOWN");

    }

    public void disableManualControl(View view) {
        sendMessage("DISABLEMANUALMODE");
    }

    public void disableAlarmHandler(View view) {
        disableAlarm();
    }
}
