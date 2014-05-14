package com.airensoft.store.app;

import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.airensoft.store.app.exception.InvalidTypeException;
import com.airensoft.store.app.exception.NotExistingKeyException;
import com.airensoft.store.app.exception.StoreFullException;
import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.primitives.Ints;

import java.util.Arrays;
import java.util.List;


public class StoreActivity extends ActionBarActivity {

    private EditText mUIKeyEdit, mUIValueEdit;
    private Spinner mUITypeSpinner;
    private Button mUIGetButton, mUISetButton;
    private Store mStore;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_store);

        // Initializes text components.
        mUIKeyEdit = (EditText) findViewById(R.id.uiKeyEdit);
        mUIValueEdit = (EditText) findViewById(R.id.uiValueEdit);

        // Initializes type selector.
        ArrayAdapter<StoreType> lAdapter = new ArrayAdapter<StoreType>(
                this, android.R.layout.simple_spinner_item,
                StoreType.values());
        lAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        mUITypeSpinner = (Spinner) findViewById(R.id.uiTypeSpinner);
        mUITypeSpinner.setAdapter(lAdapter);

        // Initializes buttons.
        mUIGetButton = (Button) findViewById(R.id.uiGetValueButton);
        mUIGetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View pView) {
                onGetValue();
            }
        });
        mUISetButton = (Button) findViewById(R.id.uiSetValueButton);
        mUISetButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View pView) {
                onSetValue();
            }
        });

        mStore = new Store();
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void onGetValue() {
        // Retrieves key and type entered by the user.
        String lKey = mUIKeyEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner
                .getSelectedItem();

        // Retrieves value from the store and displays it.
        // Each data type has its own access method.
        try {
            switch (lType) {
                case Integer:
                    mUIValueEdit.setText(Integer.toString(mStore
                            .getInteger(lKey)));
                    break;
                case String:
                    mUIValueEdit.setText(mStore.getString(lKey));
                    break;
                case Color:
                    mUIValueEdit.setText(mStore.getColor(lKey).toString());
                    break;
                case IntegerArray:
                    mUIValueEdit.setText(Ints.join(";", mStore.getIntegerArray(lKey)));
                    break;
                case ColorArray:
                    mUIValueEdit.setText(Joiner.on(";").join(mStore.getColorArray(lKey)));
                    break;
            }
        } catch (NotExistingKeyException e) {
            displayError("Key does not exist in store");
        } catch (InvalidTypeException e) {
            displayError("Incorrect type.");
        }
    }

    private void onSetValue() {
        // Retrieves key and type entered by the user.
        String lKey = mUIKeyEdit.getText().toString();
        String lValue = mUIValueEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner
                .getSelectedItem();

        // Parses user entered value and saves it in the store.
        // Each data type has its own access method.
        try {
            switch (lType) {
                case Integer:
                    mStore.setInteger(lKey, Integer.parseInt(lValue));
                    break;
                case String:
                    mStore.setString(lKey, lValue);
                    break;
                case Color:
                    mStore.setColor(lKey, new Color(lValue));
                    break;
                case IntegerArray:
                    mStore.setIntegerArray(lKey, Ints.toArray(stringToList(new Function<String, Integer>(){
                        public Integer apply(String pSubValue) {
                            return Integer.parseInt(pSubValue);
                        }
                    }, lValue)));
                    break;
                case ColorArray:
                    List<Color> lIdList = stringToList(new Function<String, Color>() {
                        @Override
                        public Color apply(String pSubValue) {
                            return new Color(pSubValue);
                        }
                    }, lValue);
                    Color[] lIdArray = lIdList.toArray(new Color[lIdList.size()]);
                    mStore.setColorArray(lKey, lIdArray);
                    break;
            }
        }
        // Process any exception raised while parsing data.
        catch (NumberFormatException eNumberFormatException) {
            displayError("Incorrect value.");
        }
        catch (IllegalArgumentException eIllegalArgumentException) {
            displayError("Incorrect value.");
        }
        catch (StoreFullException e) {
            displayError("Store is Full");
        }
    }

    // Helper method to display an error message.
    private void displayError(String pError) {
        Toast.makeText(getApplicationContext(), pError,
                Toast.LENGTH_LONG).show();
    }

    private <TType> List<TType> stringToList(Function<String, TType> pConversion, String pValue) {
        String[] lSplitArray = pValue.split(";");
        List<String> lSplitList = Arrays.asList(lSplitArray);
        return Lists.transform(lSplitList, pConversion);
    }
}
