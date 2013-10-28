package com.packtpub;

import java.util.Arrays;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.google.common.base.Function;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.primitives.Ints;
import com.packtpub.exception.InvalidTypeException;
import com.packtpub.exception.NotExistingKeyException;
import com.packtpub.exception.StoreFullException;

/**
 * Main application activity which allows getting and setting data in
 * a key/value store implemented on the native side.
 */
public class StoreActivity extends Activity {
    private EditText mUIKeyEdit, mUIValueEdit;
    private Spinner mUITypeSpinner;
    private Button mUIGetButton, mUISetButton;
    private Store mStore;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

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
        mUIGetButton.setOnClickListener(new OnClickListener() {
            public void onClick(View pView) {
                onGetValue();
            }
        });
        mUISetButton = (Button) findViewById(R.id.uiSetValueButton);
        mUISetButton.setOnClickListener(new OnClickListener() {
            public void onClick(View pView) {
                onSetValue();
            }
        });

        // Initializes the native side store.
        mStore = new Store();
    }

    @Override
    protected void onStart() {
        super.onStart();
        mStore.initializeStore();
        mStore.setInteger("watcherCounter", 0);
    }

    @Override
    protected void onStop() {
        super.onStop();
        mStore.finalizeStore();
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
                mUIValueEdit.setText(mStore.getColor(lKey)
                                .toString());
                break;
            case IntegerArray:
                mUIValueEdit.setText(Ints.join(";",
                                mStore.getIntegerArray(lKey)));
                break;
            case ColorArray:
                mUIValueEdit.setText(Joiner.on(";").join(
                                mStore.getColorArray(lKey)));
                break;
            }
        }
        // Process any exception raised while retrieving store data.
        catch (NotExistingKeyException eNotExistingKeyException) {
            displayError("Key does not exist in store");
        } catch (InvalidTypeException eInvalidTypeException) {
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
                mStore.setIntegerArray(lKey, Ints.toArray(stringToList(
                    new Function<String, Integer>() {
                        public Integer apply(String pSubValue) {
                            return Integer.parseInt(pSubValue);
                        }
                    }, lValue)));
                break;
            case ColorArray:
                List<Color> lIdList = stringToList(
                    new Function<String, Color>() {
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
        } catch (IllegalArgumentException eIllegalArgumentException) {
            displayError("Incorrect value.");
        } catch (StoreFullException eStoreFullException) {
            displayError("Store is full.");
        }
    }

    // Helper method to display an error message.
    private void displayError(String pError) {
        Toast.makeText(getApplicationContext(), pError,
                        Toast.LENGTH_SHORT).show();
    }

    // Helper method to convert a string to a list of values.
    private <TType> List<TType> stringToList(
                    Function<String, TType> pConversion,
                    String pValue) {
        String[] lSplitArray = pValue.split(";");
        List<String> lSplitList = Arrays.asList(lSplitArray);
        return Lists.transform(lSplitList, pConversion);
    }
}