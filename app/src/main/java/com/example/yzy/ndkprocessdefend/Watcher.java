package com.example.yzy.ndkprocessdefend;

/**
 * Created by yzy on 2017/12/12.
 */

public class Watcher {
    static {
        System.loadLibrary("native-lib");
    }
    public native void createWatcher(String userId);

    public native void connectMonitor();
}
