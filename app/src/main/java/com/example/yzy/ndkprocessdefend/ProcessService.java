package com.example.yzy.ndkprocessdefend;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Process;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;

import java.util.Timer;
import java.util.TimerTask;

public class ProcessService extends Service {
    private int i =0;
    public ProcessService() {
    }

    private Handler mHandler = new Handler(){
        public void handleMessage(Message msg){
            switch (msg.what){
                case 1:
                    Toast.makeText(ProcessService.this,"干死我算我输",Toast.LENGTH_SHORT).show();
                    break;
                default:
            }
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Watcher vWatcher = new Watcher();
        vWatcher.createWatcher(String.valueOf(Process.myUid()));
        vWatcher.connectMonitor();

        Timer vTimer = new Timer();
        vTimer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.e("yzy", "服务活着: " + i);
                i++;
            }
        },0,3000);

        new Thread(){
            public void run(){
                while (true){
                    mHandler.sendEmptyMessage(1);
                    SystemClock.sleep(2000);
                }
            }
        }.start();
    }
}
