package com.airensoft.store.app.exception;

/**
 * Created by burt on 2014. 5. 14..
 */
@SuppressWarnings("serial")
public class StoreFullException extends RuntimeException {
    public StoreFullException(String detailMessage) {
        super(detailMessage);
    }
}
