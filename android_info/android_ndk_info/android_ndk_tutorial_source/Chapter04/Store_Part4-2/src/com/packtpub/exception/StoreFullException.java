package com.packtpub.exception;

@SuppressWarnings("serial")
public class StoreFullException extends RuntimeException {
    public StoreFullException(String pDetailMessage) {
        super(pDetailMessage);
    }
}
