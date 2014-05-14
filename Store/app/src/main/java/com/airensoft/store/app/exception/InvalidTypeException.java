package com.airensoft.store.app.exception;

/**
 * Created by burt on 2014. 5. 14..
 */
@SuppressWarnings("serial")
public class InvalidTypeException extends Exception {
    public InvalidTypeException(String detailMessage) {
        super(detailMessage);
    }
}
