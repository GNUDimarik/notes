package com.packtpub;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

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

        // Initializes the native side store and recreates it if it
        // has already been created by a previous activity instance.
        mStore = new Store();
    }

    private void onGetValue() {
        // Retrieves key and type entered by the user.
        String lKey = mUIKeyEdit.getText().toString();
        StoreType lType = (StoreType) mUITypeSpinner
                        .getSelectedItem();

        // Retrieves value from the store and displays it.
        // Each data type has its own access method.
        switch (lType) {
        case Integer:
            mUIValueEdit.setText(Integer.toString(mStore
                            .getInteger(lKey)));
            break;
        case String:
            mUIValueEdit.setText(mStore.getString(lKey));
            break;
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
            }
        }
        // Process any exception raised while parsing data.
        catch (NumberFormatException eNumberFormatException) {
            displayError("Incorrect value.");
        }
    }

    // Helper method to display an error message.
    private void displayError(String pError) {
        Toast.makeText(getApplicationContext(), pError,
                        Toast.LENGTH_LONG).show();
    }
}