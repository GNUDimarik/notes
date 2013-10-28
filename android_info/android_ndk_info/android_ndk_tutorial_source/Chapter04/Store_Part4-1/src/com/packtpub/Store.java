package com.packtpub;

import com.packtpub.exception.InvalidTypeException;
import com.packtpub.exception.NotExistingKeyException;

public class Store {
    static {
        System.loadLibrary("store");
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
