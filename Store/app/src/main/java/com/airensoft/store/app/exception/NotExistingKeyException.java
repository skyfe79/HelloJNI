package com.airensoft.store.app.exception;

/**
 * Created by burt on 2014. 5. 14..
 */
@SuppressWarnings("serial")
public class NotExistingKeyException extends Exception {
    public NotExistingKeyException(String detailMessage) {
        super(detailMessage);
    }
}
