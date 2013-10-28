package com.packtpub.exception;

@SuppressWarnings("serial")
public class InvalidTypeException extends Exception {
    public InvalidTypeException(String pDetailMessage) {
        super(pDetailMessage);
    }
}
