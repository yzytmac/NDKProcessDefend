package com.example.yzy.ndkprocessdefend;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;

import java.util.Timer;
import java.util.TimerTask;

public class ProcessService extends Service {
    private int i =0;
    public ProcessService() {
    }

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

        Timer vTimer = new Timer();
        vTimer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                Log.e("yzy", "服务活着: " + i);
                i++;
            }
        },0,3000);
    }
}
