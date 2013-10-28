package com.packtpub;

import android.os.Handler;

import com.packtpub.exception.InvalidTypeException;
import com.packtpub.exception.NotExistingKeyException;

public class Store implements StoreListener {
    static {
        System.loadLibrary("store");
    }

    private Handler mHandler;
    private StoreListener mDelegateListener;

    public Store(StoreListener pListener) {
        // Handler message queue is associated with the thread it was
        // created on (will be the UI thread). Any message posted on
        // a handler is magically processed on this initial thread.
        mHandler = new Handler();
        mDelegateListener = pListener;
    }

    public void onAlert(final int pValue) {
        mHandler.post(new Runnable() {
            public void run() {
                mDelegateListener.onAlert(pValue);
            }
        });
    }

    public void onAlert(final String pValue) {
        mHandler.post(new Runnable() {
            public void run() {
                mDelegateListener.onAlert(pValue);
            }
        });
    }

    public void onAlert(final Color pValue) {
        mHandler.post(new Runnable() {
            public void run() {
                mDelegateListener.onAlert(pValue);
            }
        });
    }

    public native void initializeStore();
    public native void finalizeStore();

    public native synchronized int getInteger(String pKey)
        throws NotExistingKeyException, InvalidTypeException;
    public native synchronized void setInteger(String pKey,
                    int pInt);

    public native synchronized String getString(String pKey)
        throws NotExistingKeyException, InvalidTypeException;
    public native synchronized void setString(String pKey,
                    String pString);

    public native synchronized Color getColor(String pKey)
        throws NotExistingKeyException, InvalidTypeException;
    public native synchronized void setColor(String pKey,
                    Color pColor);

    public native synchronized int[] getIntegerArray(String pKey)
        throws NotExistingKeyException;
    public native synchronized void setIntegerArray(String pKey,
                    int[] pIntArray);

    public native synchronized Color[] getColorArray(String pKey)
        throws NotExistingKeyException;
    public native synchronized void setColorArray(String pKey,
                    Color[] pColorArray);
}
