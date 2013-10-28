package com.packtpub.exception;

@SuppressWarnings("serial")
public class NotExistingKeyException extends Exception {
    public NotExistingKeyException(String pDetailMessage) {
        super(pDetailMessage);
    }
}
